/*
 * $Id$
 */
#ifndef _BENCH_H
#define _BENCH_H

/*#ifdef WIN32
#include <windows.h>
typedef unsigned char bool_t;
#endif
*/


// #include	<assert.h>
// #include        <ctype.h>
#include        <stdio.h>
#include <string.h> 

// #ifndef WIN32
// #include        <unistd.h>
// #endif

#include        <stdlib.h>
// #include        <fcntl.h>
// #include        <signal.h>
// #include        <errno.h>

// #ifndef WIN32
// #include        <strings.h>
// #endif

#include	<sys/types.h>

// #ifndef WIN32
// #include        <sys/mman.h>
// #endif

// #include        <sys/stat.h>

// #ifndef WIN32
// #include        <sys/wait.h>
// #include	<time.h>
// #include        <sys/time.h>
// #include	<time.h>
// #include        <sys/socket.h>
// #include        <sys/un.h>
// #include        <sys/resource.h>
// #define PORTMAP
// #include	<rpc/rpc.h>
// #endif

// #include	<rpc/types.h>

// #include 	<stdarg.h>
#ifndef HAVE_uint
typedef unsigned int uint;
#endif

#ifndef HAVE_uint64
#ifdef HAVE_uint64_t
typedef uint64_t uint64;
#else /* HAVE_uint64_t */
typedef unsigned long long uint64;
#endif /* HAVE_uint64_t */
#endif /* HAVE_uint64 */

#ifndef HAVE_int64
#ifdef HAVE_int64_t
typedef int64_t int64;
#else /* HAVE_int64_t */
typedef long long int64;
#endif /* HAVE_int64_t */
#endif /* HAVE_int64 */

// #define NO_PORTMAPPER




#include "Enclave_t.h"
// #include	"stats.h"
#include	"timing.h"
//#include	"lib_debug.h"
// #include	"lib_tcp.h"
// #include	"lib_udp.h"
// #include	"lib_unix.h"


#ifdef	DEBUG
#	define		debug(x) fprintf x
#else
#	define		debug(x)
#endif

#define BUFSIZE	4096

/*#ifndef	XFERSIZE
#define	XFERSIZE	(64*1024)	/* all bandwidth I/O should use this 
#endif
*/
//#if defined(SYS5) || defined(WIN32)
//#define	bzero(b, len)	memset(b, 0, len)
//#define	bcopy(s, d, l)	memcpy(d, s, l)
//#define	rindex(s, c)	strrchr(s, c)
//#endif
// #define	gettime		usecs_spent
#define	streq		!strcmp
#define	ulong		unsigned long


/*#ifdef WIN32
#include <process.h>
#define getpid _getpid
int	gettimeofday(struct timeval *tv, struct timezone *tz);
#endif

//#define	SMALLEST_LINE	32		/* smallest cache line size */
// #define	TIME_OPEN2CLOSE

// #define	GO_AWAY	signal(SIGALRM, exit); alarm(60 * 60);
#define	REAL_SHORT	   50000
#define	SHORT	 	 1000000
#define	MEDIUM	 	 2000000
#define	LONGER		 7500000	/* for networking data transfers */
#define	ENOUGH		REAL_SHORT

#define	TRIES		11



typedef struct {
	uint64 u;
	uint64 n;
} value_t;

typedef struct {
	int	N;
	value_t	v[TRIES];
} result_t;

int	sizeof_result(int N);
void    insertinit(result_t *r);
void    insertsort(uint64, uint64, result_t *);
void	save_median();
void	save_minimum();
void	set_results(result_t *r);
result_t* get_results();



	
#define	BENCH_INNER(loop_body, enough) { 				\
	static iter_t	__iterations = 1;				\
	int		__enough = get_enough(enough);			\
	iter_t		__n;						\
	double		__result = 0.;					\
									\
	while(__result < 0.95 * __enough) {				\
		start(0);						\
		for (__n = __iterations; __n > 0; __n--) {		\
			loop_body;					\
		}							\
		__result = stop(0,0);					\
		if (__result < 0.99 * __enough 				\
		    || __result > 1.2 * __enough) {			\
			if (__result > 150.) {				\
				double	tmp = __iterations / __result;	\
				tmp *= 1.1 * __enough;			\
				__iterations = (iter_t)(tmp + 1);	\
			} else {					\
				if (__iterations > (iter_t)1<<27) {	\
					__result = 0.;			\
					break;				\
				}					\
				__iterations <<= 3;			\
			}						\
		}							\
	} /* while */							\
	save_n((uint64)__iterations); settime((uint64)__result);	\
}


//typedef unsigned long iter_t;
typedef void (*benchmp_f)(iter_t iterations, void* cookie);

extern void benchmp(benchmp_f initialize, 
		    benchmp_f benchmark,
		    benchmp_f cleanup,
		    int enough, 
		    int parallel,
		    int warmup,
		    int repetitions,
		    void* cookie
	);

/* 
 * These are used by weird benchmarks which cannot return, such as page
 * protection fault handling.  See lat_sig.c for sample usage.
 */
extern void* benchmp_getstate();
extern iter_t benchmp_interval(void* _state);


/*
 *  Debug stuff
 */

#endif /* _BENCH_H */
