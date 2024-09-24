/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave_lib_timing.c| file.
 *
 *
 *
 */

#include "bench.h"
#include "debug_print.h"

static struct mytimeval start_tv, stop_tv;
// FILE                 *ftiming;
static volatile uint64 use_result_dummy;
static uint64 iterations;
static void init_timing(void);

extern char *buf;

int sizeof_result(int repetitions);


typedef enum { warmup, timing_interval, cooldown } benchmp_state;

typedef struct {
    benchmp_state state;
    benchmp_f initialize;
    benchmp_f benchmark;
    benchmp_f cleanup;
    int childid;		//-not really useful. keeping it here so compiler doesn't yell.
    int enough;
    iter_t iterations;
//  int     parallel;
    int repetitions;
    void *cookie;
    iter_t iterations_batch;
    int need_warmup;
    long i;
    int r_size;
    result_t *r;
} benchmp_child_state;

static benchmp_child_state _benchmp_child_state;


int benchmp_childid()
{
    return _benchmp_child_state.childid;
}



void *benchmp_getstate()
{
    return ((void *) &_benchmp_child_state);
}


#define DONE    1
#define NOT_DONE    0
int done = NOT_DONE;



void benchmp(benchmp_f initialize, benchmp_f benchmark, benchmp_f cleanup, int enough, int parallel,	//not removed this yet so it doesn't break functions that call it
	     int warmuptime, int repetitions, void *cookie)
{
    iter_t iterations = 1;

#ifdef _DEBUG_TIM
    DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		"benchmp(%p, %p, %p, %d, %d, %d, %d, %p): entering\n",
		__FILE__, __LINE__, initialize, benchmark, cleanup,
		enough, parallel, warmuptime, repetitions, cookie);
#endif

    enough = get_enough(enough);

    DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d \tenough=%d",
		__FILE__, __LINE__, enough);

    /* initialize results */
    settime(0);
    save_n(1);

    /* let the children run for warmup microseconds */
    // if (warmuptime > 0) {
    //      struct mytimeval delay;
    //      delay.tv_sec = warmuptime / 1000000;
    //      delay.tv_usec = warmuptime % 1000000;

    //      select(0, NULL, NULL, NULL, &delay);
    // }


    /* collect results */
    //do child stuff

    double result = 0.;
    double usecs;
    long i = 0;
    int need_warmup;

    _benchmp_child_state.state = warmup;
    _benchmp_child_state.initialize = initialize;
    _benchmp_child_state.benchmark = benchmark;
    _benchmp_child_state.cleanup = cleanup;
    _benchmp_child_state.enough = enough;
    _benchmp_child_state.iterations = iterations;
    _benchmp_child_state.iterations_batch = 1;
    _benchmp_child_state.repetitions = repetitions;
    _benchmp_child_state.cookie = cookie;
    _benchmp_child_state.need_warmup = 1;
    _benchmp_child_state.i = 0;
    _benchmp_child_state.r_size = sizeof_result(repetitions);


    if (_benchmp_child_state.r) {
	DEBUG_PRINT("%s:%d benchmp: r already allocated.\n",
		    __FILE__, __LINE__);
	free(_benchmp_child_state.r);
    } else {
	DEBUG_PRINT("%s:%d benchmp: r not allocated.\n",
		    __FILE__, __LINE__);
    }

    _benchmp_child_state.r =
	(result_t *) malloc(_benchmp_child_state.r_size);

    if (!_benchmp_child_state.r)
	return;
    insertinit(_benchmp_child_state.r);
    set_results(_benchmp_child_state.r);

    if (initialize)
	(*initialize) (0, cookie);

    /* start experiments, collecting results */
    insertinit(_benchmp_child_state.r);

    done = NOT_DONE;

    while (!done) {
		int val = benchmp_interval(&_benchmp_child_state);
		if (val != -1) {
	    	(*benchmark) (val, cookie);
		}
    }


    //end child stuff


    /* tell one child to report its results */
    /* Compute median time; iterations is constant! */
    set_results(_benchmp_child_state.r);
}




iter_t benchmp_interval(void *_state)
{
    char c;
    iter_t iterations;
    double result;
    //struct timeval        timeout;
    benchmp_child_state *state = (benchmp_child_state *) _state;

    iterations =
	(state->state ==
	 timing_interval ? state->iterations : state->iterations_batch);

    if (!state->need_warmup) {
	result = stop(0, 0);
	if (state->cleanup) {
	    (*state->cleanup) (iterations, state->cookie);
	}
	save_n(state->iterations);
	result -= t_overhead() + get_n() * l_overhead();
	settime(result >= 0. ? (uint64) result : 0.);
    }



    switch (state->state) {
    case warmup:

	iterations = state->iterations_batch;
	state->state = timing_interval;
	iterations = state->iterations;


	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		    "benchmp_interval(warmup iterations: %lu).\n",
		    __FILE__, __LINE__, iterations);

	if (state->need_warmup) {
	    state->need_warmup = 0;
	    /* send 'ready' */
	}

	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		    "benchmp_interval(warmup iterations: %lu).",
		    __FILE__, __LINE__, iterations);

	break;
    case timing_interval:
	iterations = state->iterations;
	if (result > 0.95 * state->enough) {
	    insertsort(gettime(), get_n(), get_results());
	    state->i++;
	    /* we completed all the experiments, return results */
	    if (state->i >= state->repetitions) {
		state->state = cooldown;
	    }
	}
	if (result < 0.99 * state->enough || result > 1.2 * state->enough) {
	    if (result > 150.) {
		double tmp = iterations / result;
		tmp *= 1.1 * state->enough;
		iterations = (iter_t) (tmp + 1);
	    } else {
		iterations <<= 3;
		if (iterations > 1 << 27
		    || result < 0. && iterations > 1 << 20) {
		    state->state = cooldown;
		}
	    }
	}
	state->iterations = iterations;
	if (state->state == cooldown) {
	    iterations = state->iterations_batch;
	}

	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		    "benchmp_interval(interval iterations: %lu).",
		    __FILE__, __LINE__, iterations);

	break;
    case cooldown:
	iterations = state->iterations_batch;

//              if (FD_ISSET(state->result_signal, &fds)) {
	/* 
	 * At this point all children have stopped their
	 * measurement loops, so we can block waiting for
	 * the parent to tell us to send our results back.
	 * From this point on, we will do no more "work".
	 */
	//write(state->response, (void*)get_results(), state->r_size);  ////////////////


	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		    "benchmp_interval(cooldown iterations: %lu).",
		    __FILE__, __LINE__, iterations);

	if (state->cleanup) {

	    DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
			"benchmp_interval(state->cleanup iterations: %lu).",
			__FILE__, __LINE__, iterations);

	    (*state->cleanup) (0, state->cookie);
	}

	/* Now wait for signal to exit */
	//      exit(0);
	done = DONE;

	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		    "benchmp_interval(DONE: state->cleanup iterations: %lu).",
		    __FILE__, __LINE__, iterations);

	return -1;
    };


    if (state->initialize) {
	(*state->initialize) (iterations, state->cookie);
    }
    start(0);
    //printf("iterations: %d\n", iterations);


    DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
		"benchmp_interval(return iterations: %lu).",
		__FILE__, __LINE__, iterations);

    return (iterations);
}



/*
 * Start timing now.
 */
void start(struct mytimeval *tv)
{
    if (tv == NULL) {
	tv = &start_tv;
    }
// #ifdef       RUSAGE
    // getrusage(RUSAGE_SELF, &ru_start);
// #endif

    // (void) gettimeofday(tv, (struct timezone *) 0);
    ocall_gettimeofday(tv);

}

/*
 * Stop timing and return real time in microseconds.
 */
uint64 stop(struct mytimeval *begin, struct mytimeval *end)
{
    if (end == NULL) {
	end = &stop_tv;
	// (void) gettimeofday(end, (struct timezone *) 0);
	ocall_gettimeofday(end);
    }
    // (void) gettimeofday(end, (struct timezone *) 0);
    ocall_gettimeofday(end);

// #ifdef       RUSAGE
//      getrusage(RUSAGE_SELF, &ru_stop);
// #endif

    if (begin == NULL) {
	begin = &start_tv;
    }
    return (tvdelta(begin, end));
}



void save_n(uint64 n)
{
    iterations = n;
}

uint64 get_n(void)
{
    return (iterations);
}

/*
 * Make the time spend be usecs.
 */
void settime(uint64 usecs)
{
    bzero((void *) &start_tv, sizeof(start_tv));
    stop_tv.tv_sec = usecs / 1000000;
    stop_tv.tv_usec = usecs % 1000000;
}




void micro(char *s, uint64 n)
{
    struct mytimeval td;
    double micro;

    tvsub(&td, &stop_tv, &start_tv);
    micro = td.tv_sec * 1000000 + td.tv_usec;
    micro /= n;
    if (micro == 0.0)
	return;
//      if (!ftiming) ftiming = stderr;
//      fprintf(ftiming, "%s: %.4f microseconds\n", s, micro);

    DEBUG_PRINT("\tFILE: %s:%d \n\t %s: %.4f microseconds\n",
		__FILE__, __LINE__, s, micro);

#if 0
    if (micro >= 100) {
	fprintf(ftiming, "%s: %.1f microseconds\n", s, micro);
    } else if (micro >= 10) {
	fprintf(ftiming, "%s: %.3f microseconds\n", s, micro);
    } else {
	fprintf(ftiming, "%s: %.4f microseconds\n", s, micro);
    }
#endif
}



void nano(char *s, uint64 n)
{
    struct mytimeval td;
    double micro;

    tvsub(&td, &stop_tv, &start_tv);
    micro = td.tv_sec * 1000000 + td.tv_usec;
    micro *= 1000;
    if (micro == 0.0)
	return;
    // if (!ftiming) ftiming = stderr;
    // fprintf(ftiming, "%s: %.2f nanoseconds\n", s, micro / n);


    DEBUG_PRINT("\tFILE: %s:%d \n\t %s: %.2f nanoseconds\n",
		__FILE__, __LINE__, s, micro / n);
}



// void
// nano(char *s, uint64 n){
//      FILE *ftiming = NULL;

//     struct timeval td;
//     double  micro;

//     tvsub(&td, &stop_tv, &start_tv);
//     micro = td.tv_sec * 1000000 + td.tv_usec;
//     micro *= 1000;
//     if (micro == 0.0) return;
//     if (!ftiming) ftiming = stdout;
//     fprintf(ftiming, "%s: %.2f nanoseconds\n", s, micro / n);
// }



uint64 tvdelta(struct mytimeval * start, struct mytimeval * stop)
{
    struct mytimeval td;
    uint64 usecs;

    tvsub(&td, stop, start);
    usecs = td.tv_sec;
    usecs *= 1000000;
    usecs += td.tv_usec;
    return (usecs);
}


void tvsub(struct mytimeval *tdiff, struct mytimeval *t1,
	   struct mytimeval *t0)
{
    tdiff->tv_sec = t1->tv_sec - t0->tv_sec;
    tdiff->tv_usec = t1->tv_usec - t0->tv_usec;
    if (tdiff->tv_usec < 0 && tdiff->tv_sec > 0) {
	tdiff->tv_sec--;
	tdiff->tv_usec += 1000000;

	//assert(tdiff->tv_usec >= 0);
#ifndef  NDEBUG
	if (tdiff->tv_usec < 0) {
	    DEBUG_PRINT("\tFILE: %s LINE:%d \t negative time!",
			__FILE__, __LINE__);
	    //exit(); - sgx doesn't support exit
	}
#endif				/* end assert() */

    }

    /* time shouldn't go backwards!!! */
    if (tdiff->tv_usec < 0 || t1->tv_sec < t0->tv_sec) {
	tdiff->tv_sec = 0;
	tdiff->tv_usec = 0;
    }
}



uint64 gettime(void)
{
    // DEBUG_PRINT("\tFILE: %s:%d \t REACHED GETTIME",
    //              __FILE__, __LINE__);

    return (tvdelta(&start_tv, &stop_tv));
}


void use_int(int result)
{
    use_result_dummy += result;
}

void use_pointer(void *result)
{
    use_result_dummy += (long) result;
}

int sizeof_result(int repetitions)
{
    if (repetitions <= TRIES)
	return (sizeof(result_t));
    return (sizeof(result_t) + (repetitions - TRIES) * sizeof(value_t));
}

void insertinit(result_t * r)
{
    int i;

    r->N = 0;
}

// /* biggest to smallest */
void insertsort(uint64 u, uint64 n, result_t * r)
{
    int i, j;

    if (u == 0)
	return;

    for (i = 0; i < r->N; ++i) {
	if (u / (double) n > r->v[i].u / (double) r->v[i].n) {
	    for (j = r->N; j > i; --j) {
		r->v[j] = r->v[j - 1];
	    }
	    break;
	}
    }
    r->v[i].u = u;
    r->v[i].n = n;
    r->N++;
}

static result_t _results;
static result_t *results = &_results;

result_t *get_results()
{
    return (results);
}

void set_results(result_t * r)
{
    results = r;
    save_median();
}

void save_minimum()
{
    if (results->N == 0) {
	save_n(1);
	settime(0);
    } else {
	save_n(results->v[results->N - 1].n);
	settime(results->v[results->N - 1].u);
    }
}

void save_median()
{
    int i = results->N / 2;
    uint64 u, n;

    if (results->N == 0) {
	n = 1;
	u = 0;
    } else if (results->N % 2) {
	n = results->v[i].n;
	u = results->v[i].u;
    } else {
	n = (results->v[i].n + results->v[i - 1].n) / 2;
	u = (results->v[i].u + results->v[i - 1].u) / 2;
    }

    save_n(n);
    settime(u);
}



static long *one_op(register long *p)
{
    BENCH_INNER(p = (long *) *p;, 0);
    return (p);
}




static long *two_op(register long *p)
{
    BENCH_INNER(p = (long *) *p; p = (long *) *p;, 0);
    return (p);
}



static long *p = (long *) &p;
static long *q = (long *) &q;



double l_overhead(void)
{
    int i;
    uint64 N_save, u_save;
    static double overhead;
    static int initialized = 0;
    result_t one, two, *r_save;

    init_timing();
    if (initialized)
	return (overhead);

    initialized = 1;

    // if (getenv("LOOP_O")) {
    //      overhead = atof(getenv("LOOP_O"));
    // } else {
    r_save = get_results();
    N_save = get_n();
    u_save = gettime();
    insertinit(&one);
    insertinit(&two);
    for (i = 0; i < TRIES; ++i) {
	use_pointer((void *) one_op(p));
	if (gettime() > t_overhead())
	    insertsort(gettime() - t_overhead(), get_n(), &one);
	use_pointer((void *) two_op(p));
	if (gettime() > t_overhead())
	    insertsort(gettime() - t_overhead(), get_n(), &two);
    }
    /*
     * u1 = (n1 * (overhead + work))
     * u2 = (n2 * (overhead + 2 * work))
     * ==> overhead = 2. * u1 / n1 - u2 / n2
     */
    set_results(&one);
    save_minimum();
    overhead = 2. * gettime() / (double) get_n();

    set_results(&two);
    save_minimum();
    overhead -= gettime() / (double) get_n();

    if (overhead < 0.)
	overhead = 0.;		/* Gag */

    set_results(r_save);
    save_n(N_save);
    settime(u_save);
    // }
    return (overhead);
}

/*
 * Figure out the timing overhead.  This has to track bench.h
 */
uint64 t_overhead(void)
{
    uint64 N_save, u_save;
    static int initialized = 0;
    static uint64 overhead = 0;
    struct mytimeval tv;
    result_t *r_save;

    init_timing();
    if (initialized)
	return (overhead);

    initialized = 1;

    // if (getenv("TIMING_O")) {
    //      overhead = atof(getenv("TIMING_O"));
    // }
    // else
    if (get_enough(0) <= 50000) {
	/* it is not in the noise, so compute it */
	int i;
	result_t r;

	r_save = get_results();
	N_save = get_n();
	u_save = gettime();
	insertinit(&r);
	for (i = 0; i < TRIES; ++i) {
	    BENCH_INNER(ocall_gettimeofday(&tv), 0);
	    insertsort(gettime(), get_n(), &r);
	}
	set_results(&r);
	save_minimum();
	overhead = gettime() / get_n();

	set_results(r_save);
	save_n(N_save);
	settime(u_save);
    }
    return (overhead);
}


/*
 * Figure out how long to run it.
 * If enough == 0, then they want us to figure it out.
 * If enough is !0 then return it unless we think it is too short.
 */
static int long_enough;
static int compute_enough();

int get_enough(int e)
{
    init_timing();
    return (long_enough > e ? long_enough : e);
}


static void init_timing(void)
{
    static int done = 0;

    if (done)
	return;
    done = 1;
    long_enough = compute_enough();
    t_overhead();
    l_overhead();
}




typedef long TYPE;

static TYPE **enough_duration(register long N, register TYPE ** p)
{
#define	ENOUGH_DURATION_TEN(one)	one one one one one one one one one one
    while (N-- > 0) {
	ENOUGH_DURATION_TEN(p = (TYPE **) * p;);
    }
    return (p);
}







static uint64 duration(long N)
{
    uint64 usecs;
    TYPE *x = (TYPE *) & x;
    TYPE **p = (TYPE **) & x;

    start(0);
    p = enough_duration(N, p);
    usecs = stop(0, 0);
    use_pointer((void *) p);
    return (usecs);
}

/*
 * find the minimum time that work "N" takes in "tries" tests
 */

static uint64 time_N(iter_t N)
{
    int i;
    uint64 usecs;
    result_t r, *r_save;

    r_save = get_results();
    insertinit(&r);
    for (i = 1; i < TRIES; ++i) {
	usecs = duration(N);
	insertsort(usecs, N, &r);
    }
    set_results(&r);
    save_minimum();
    usecs = gettime();
    set_results(r_save);
    return (usecs);
}


/*
 * return the amount of work needed to run "enough" microseconds
 */
static iter_t find_N(int enough)
{
    int tries;
    static iter_t N = 10000;
    static uint64 usecs = 0;

    if (!usecs)
	usecs = time_N(N);

    for (tries = 0; tries < 10; ++tries) {
	if (0.98 * enough < usecs && usecs < 1.02 * enough)
	    return (N);
	if (usecs < 1000)
	    N *= 10;
	else {
	    double n = N;

	    n /= usecs;
	    n *= enough;
	    N = n + 1;
	}
	usecs = time_N(N);
    }
    return (0);
}

/*
 * We want to verify that small modifications proportionally affect the runtime
 */
static double test_points[] = { 1.015, 1.02, 1.035 };

static int test_time(int enough)
{
    int i;
    iter_t N;
    uint64 usecs, expected, baseline, diff;

    if ((N = find_N(enough)) == 0)
	return (0);

    baseline = time_N(N);

    for (i = 0; i < sizeof(test_points) / sizeof(double); ++i) {
	usecs = time_N((int) ((double) N * test_points[i]));
	expected = (uint64) ((double) baseline * test_points[i]);
	diff = expected > usecs ? expected - usecs : usecs - expected;
	if (diff / (double) expected > 0.0025)
	    return (0);
    }
    return (1);
}


/*
 * We want to find the smallest timing interval that has accurate timing
 */
static int possibilities[] = { 5000, 10000, 50000, 100000 };

static int compute_enough()
{
    int i;

    // if (getenv("ENOUGH")) {
    //      return (atoi(("ENOUGH")));
    // }
    for (i = 0; i < sizeof(possibilities) / sizeof(int); ++i) {
	if (test_time(possibilities[i]))
	    return (possibilities[i]);
    }

    /* 
     * if we can't find a timing interval that is sufficient, 
     * then use SHORT as a default.
     */
    return (SHORT);
}
