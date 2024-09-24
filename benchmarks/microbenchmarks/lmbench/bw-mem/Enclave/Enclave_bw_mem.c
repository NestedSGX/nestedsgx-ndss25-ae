/*
 * |Aisha Hasan|
 *
 * The |Enclave/Enclave.c| file.
 *
 *
 *
 */


#include "Enclave_bw_mem.h"
#include "debug_print.h"

char buf[BUFSIZE];

void	rd(iter_t iterations, void *cookie);
void	wr(iter_t iterations, void *cookie);
void	rdwr(iter_t iterations, void *cookie);
void	mcp(iter_t iterations, void *cookie);
void	fwr(iter_t iterations, void *cookie);
void	frd(iter_t iterations, void *cookie);
void	fcp(iter_t iterations, void *cookie);
void	loop_bzero(iter_t iterations, void *cookie);
void	loop_bcopy(iter_t iterations, void *cookie);

void	init_loop(iter_t iterations, void *cookie);
void	cleanup(iter_t iterations, void *cookie);
void	adjusted_bandwidth(uint64 t, uint64 b, uint64 iter, double ovrhd);



void ecall_send_params(int parallel, int warmup, int repetitions,
					char *benchmark, size_t blen,
					state_t state){
//					void *cookie, size_t clen){



/*	DEBUG_PRINT("\tAISHAPRINT FILE: %s LINE:%d "
						"parallel: %d, warmup: %d, reps: %d, "
						"benchmark:[%s], overhead:[%f], nbytes:[%lu], "
						"need_buf2:[%d], aligned:[%d]",

                        __FILE__, __LINE__, parallel, warmup, repetitions,
						benchmark, state.overhead, state.nbytes,
						state.need_buf2, state.aligned);
*/

	char buf[BUFSIZE] = {'\0'};

	snprintf(buf, BUFSIZE, "\tAISHAPRINT FILE: %s LINE:%d "
						"parallel: %d, warmup: %d, reps: %d, "
						"benchmark:[%s], overhead:[%f], nbytes:[%lu], "
						"need_buf2:[%d], aligned:[%d]",

                        __FILE__, __LINE__, parallel, warmup, repetitions,
						benchmark, state.overhead, state.nbytes,
						state.need_buf2, state.aligned);


    ocall_print_string(buf);
    memset(buf, '\0', BUFSIZE);

 	if (!strcmp("rd", benchmark)) {
		benchmp(init_loop, rd, cleanup, 0, parallel, 
			warmup, repetitions, &state);
    } else if (!strcmp("wr", benchmark)) {
		benchmp(init_loop, wr, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("rdwr", benchmark)) {
		benchmp(init_loop, rdwr, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("cp", benchmark)) {
		benchmp(init_loop, mcp, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("frd", benchmark)) {
		benchmp(init_loop, frd, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("fwr", benchmark)) {
		benchmp(init_loop, fwr, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("fcp", benchmark)) {
		benchmp(init_loop, fcp, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("bzero", benchmark)) {
		benchmp(init_loop, loop_bzero, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	} else if (!strcmp("bcopy", benchmark)) {
		benchmp(init_loop, loop_bcopy, cleanup, 0, parallel, 
			warmup, repetitions, &state);
	}

	DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "sending to adjusted_bandwidth\n",
                            __FILE__, __LINE__);

	adjusted_bandwidth(gettime(), state.nbytes, 
			   get_n() * parallel, state.overhead);
}





void
rd(iter_t iterations, void *cookie)
{	
	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;
	register int sum = 0;


    DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "inside rd, iterations: %lu.",
                            __FILE__, __LINE__, iterations);

/* 	

	DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
							"lastone: %d, lastone+4: %d.\n",
							__FILE__, __LINE__, *(lastone), *(lastone+4));
*/	
	

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    while (p <= lastone) {

/*			DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d p[0]: %d.\n", __FILE__, __LINE__, p[0]);
*/
			sum += 
#define	DOIT(i)	p[i]+
			DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
			DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
			DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
			DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
			DOIT(104	) DOIT(108) DOIT(112) DOIT(116) DOIT(120) 
			p[124];

/*			DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d p[124]: %d.\n", __FILE__, __LINE__, p[124]);

	 		DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d sum: %d.\n", __FILE__, __LINE__, sum);
*/
			p +=  128;
	    }
	}

/*    DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d BENCHMARK SUM SUM SUM SUM SUM SUM: [%d].\n", __FILE__, __LINE__, sum);
*/

	use_int(sum);
}
#undef	DOIT





void
wr(iter_t iterations, void *cookie)
{	
	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    while (p <= lastone) {
#define	DOIT(i)	p[i] = 1;
		DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
		DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
		DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
		DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
		DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
		p +=  128;
	    }
	}
}
#undef	DOIT




void
rdwr(iter_t iterations, void *cookie)
{	
	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;
	register int sum = 0;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    while (p <= lastone) {
#define	DOIT(i)	sum += p[i]; p[i] = 1;
		DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
		DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
		DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
		DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
		DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
		p +=  128;
	    }
	}
	use_int(sum);
}
#undef	DOIT



void
mcp(iter_t iterations, void *cookie)
{

    DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "inside mcp, iterations: %lu.",
                            __FILE__, __LINE__, iterations);


	
	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;
	TYPE* p_save = NULL;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    register TYPE *dst = state->buf2;
	    while (p <= lastone) {
#define	DOIT(i)	dst[i] = p[i];
		DOIT(0) DOIT(4) DOIT(8) DOIT(12) DOIT(16) DOIT(20) DOIT(24)
		DOIT(28) DOIT(32) DOIT(36) DOIT(40) DOIT(44) DOIT(48) DOIT(52)
		DOIT(56) DOIT(60) DOIT(64) DOIT(68) DOIT(72) DOIT(76)
		DOIT(80) DOIT(84) DOIT(88) DOIT(92) DOIT(96) DOIT(100)
		DOIT(104) DOIT(108) DOIT(112) DOIT(116) DOIT(120) DOIT(124);
		p += 128;
		dst += 128;
	    }
	    p_save = p;

	}

	use_pointer(p_save);
}
#undef	DOIT




void
fwr(iter_t iterations, void *cookie)
{	


	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;
	TYPE* p_save = NULL;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    while (p <= lastone) {
#define	DOIT(i)	p[i]=
		DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6)
		DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12)
		DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18)
		DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
		DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30)
		DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35) DOIT(36)
		DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
		DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48)
		DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54)
		DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
		DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66)
		DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72)
		DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78)
		DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84)
		DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
		DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96)
		DOIT(97) DOIT(98) DOIT(99) DOIT(100) DOIT(101) DOIT(102)
		DOIT(103) DOIT(104) DOIT(105) DOIT(106) DOIT(107)
		DOIT(108) DOIT(109) DOIT(110) DOIT(111) DOIT(112)
		DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
		DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
		DOIT(123) DOIT(124) DOIT(125) DOIT(126) DOIT(127) 1;
		p += 128;
	    }
	    p_save = p;

	//	DEBUG_PRINT("%lu ", iterations);
		
	}
	use_pointer(p_save);
}
#undef	DOIT




void
frd(iter_t iterations, void *cookie)
{	
	state_t *state = (state_t *) cookie;
	register int sum = 0;
	register TYPE *lastone = state->lastone;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    while (p <= lastone) {
		sum +=
#define	DOIT(i)	p[i]+
		DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6)
		DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12)
		DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18)
		DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
		DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30)
		DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35) DOIT(36)
		DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
		DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48)
		DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54)
		DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
		DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66)
		DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72)
		DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78)
		DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84)
		DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
		DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96)
		DOIT(97) DOIT(98) DOIT(99) DOIT(100) DOIT(101) DOIT(102)
		DOIT(103) DOIT(104) DOIT(105) DOIT(106) DOIT(107)
		DOIT(108) DOIT(109) DOIT(110) DOIT(111) DOIT(112)
		DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
		DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
		DOIT(123) DOIT(124) DOIT(125) DOIT(126) p[127];
		p += 128;
	    }
	}
	use_int(sum);
}
#undef	DOIT




void
fcp(iter_t iterations, void *cookie)
{

    DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "inside fcp, iterations: %lu\n",
                            __FILE__, __LINE__, iterations);

	
	state_t *state = (state_t *) cookie;
	register TYPE *lastone = state->lastone;

	while (iterations-- > 0) {
	    register TYPE *p = state->buf;
	    register TYPE *dst = state->buf2;
	    while (p <= lastone) {
#define	DOIT(i)	dst[i]=p[i];
		DOIT(0) DOIT(1) DOIT(2) DOIT(3) DOIT(4) DOIT(5) DOIT(6)
		DOIT(7) DOIT(8) DOIT(9) DOIT(10) DOIT(11) DOIT(12)
		DOIT(13) DOIT(14) DOIT(15) DOIT(16) DOIT(17) DOIT(18)
		DOIT(19) DOIT(20) DOIT(21) DOIT(22) DOIT(23) DOIT(24)
		DOIT(25) DOIT(26) DOIT(27) DOIT(28) DOIT(29) DOIT(30)
		DOIT(31) DOIT(32) DOIT(33) DOIT(34) DOIT(35) DOIT(36)
		DOIT(37) DOIT(38) DOIT(39) DOIT(40) DOIT(41) DOIT(42)
		DOIT(43) DOIT(44) DOIT(45) DOIT(46) DOIT(47) DOIT(48)
		DOIT(49) DOIT(50) DOIT(51) DOIT(52) DOIT(53) DOIT(54)
		DOIT(55) DOIT(56) DOIT(57) DOIT(58) DOIT(59) DOIT(60)
		DOIT(61) DOIT(62) DOIT(63) DOIT(64) DOIT(65) DOIT(66)
		DOIT(67) DOIT(68) DOIT(69) DOIT(70) DOIT(71) DOIT(72)
		DOIT(73) DOIT(74) DOIT(75) DOIT(76) DOIT(77) DOIT(78)
		DOIT(79) DOIT(80) DOIT(81) DOIT(82) DOIT(83) DOIT(84)
		DOIT(85) DOIT(86) DOIT(87) DOIT(88) DOIT(89) DOIT(90)
		DOIT(91) DOIT(92) DOIT(93) DOIT(94) DOIT(95) DOIT(96)
		DOIT(97) DOIT(98) DOIT(99) DOIT(100) DOIT(101) DOIT(102)
		DOIT(103) DOIT(104) DOIT(105) DOIT(106) DOIT(107)
		DOIT(108) DOIT(109) DOIT(110) DOIT(111) DOIT(112)
		DOIT(113) DOIT(114) DOIT(115) DOIT(116) DOIT(117)
		DOIT(118) DOIT(119) DOIT(120) DOIT(121) DOIT(122)
		DOIT(123) DOIT(124) DOIT(125) DOIT(126) DOIT(127)
		p += 128;
		dst += 128;
	    }
	}

	DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "done with iter loop\n",
                            __FILE__, __LINE__);
}





void
loop_bzero(iter_t iterations, void *cookie)
{	
	state_t *state = (state_t *) cookie;
	register TYPE *p = state->buf;
	register TYPE *dst = state->buf2;
	register size_t  N = state->N;

	while (iterations-- > 0) {
		bzero(p, N);
	}
}




void
loop_bcopy(iter_t iterations, void *cookie)
{	

    DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "inside fcp, iterations: %lu\n",
                            __FILE__, __LINE__, iterations);
	

	state_t *state = (state_t *) cookie;
	register TYPE *p = state->buf;
	register TYPE *dst = state->buf2;
	register size_t  N = state->N;

	while (iterations-- > 0) {
		bcopy(p,dst,N);
	}

	DEBUG_PRINT("AISHAPRINT FILE: %s LINE:%d "
                            "done with iter loop\n",
                            __FILE__, __LINE__);

}



void
init_loop(iter_t iterations, void *cookie)
{
	state_t *state = (state_t *) cookie;

	if (iterations) return;


    state->buf = (TYPE *)malloc(state->nbytes); //valloc is obsolete
	state->buf2_orig = NULL;
	state->lastone = (TYPE*)state->buf - 1;
	state->lastone = (TYPE*)((char *)state->buf + state->nbytes - 512);
	state->N = state->nbytes;

	if (!state->buf) {
		snprintf(buf, BUFSIZE,  "AISHAPRINT FILE: %s LINE:%d\n"
                            "\t malloc error.\n", __FILE__, __LINE__);

		//perror("malloc");
		//exit(1);
	}


	bzero((void*)state->buf, state->nbytes);

	if (state->need_buf2 == 1) {
		
		state->buf2_orig = state->buf2 = (TYPE *)malloc(state->nbytes + 2048); //valloc obsolete
		
		if (!state->buf2) {

			//perror("malloc");
			//exit(1);

			snprintf(buf, BUFSIZE,  "AISHAPRINT FILE: %s LINE:%d\n"
                            "\t malloc error.\n", __FILE__, __LINE__);
		}

		/* default is to have stuff unaligned wrt each other */
		/* XXX - this is not well tested or thought out */
		if (state->aligned) {
			char	*tmp = (char *)state->buf2;

			tmp += 2048 - 128;
			state->buf2 = (TYPE *)tmp;
		}
	}

	//START TODO AISHA - filling with 1s
/*    for (int i = 0; i < ((state->nbytes)/sizeof(TYPE)); i++){
        state->buf[i] = 1;
    }
*/
    //END TODO AISHA
}


void
cleanup(iter_t iterations, void *cookie)
{
	state_t *state = (state_t *) cookie;

	if (iterations) return;

	free(state->buf);
	if (state->buf2_orig) free(state->buf2_orig);
}



char tempbuf[BUFSIZE];
/*
 * Almost like bandwidth() in lib_timing.c, but we need to adjust
 * bandwidth based upon loop overhead.
 */
void adjusted_bandwidth(uint64 time, uint64 bytes, uint64 iter, double overhd)
{
#define MB	(1000. * 1000.)

//	extern FILE *ftiming;
	double secs = ((double)time / (double)iter - overhd) / 1000000.0;
	double mb;
	
	mb = bytes / MB;

	if (secs <= 0.)
		return;

    //if (!ftiming) ftiming = stderr;
	if (mb < 1.) {
		//(void) fprintf(ftiming, "%.6f ", mb);
		snprintf(tempbuf, BUFSIZE, "\tFILE: %s:%d \n\t %.6f MB",
                __FILE__, __LINE__, mb);

	} else {
		//(void) fprintf(ftiming, "%.2f ", mb);
		snprintf(tempbuf, BUFSIZE, "\tFILE: %s:%d \n\t %.2f MB",
                __FILE__, __LINE__, mb);
	}
	if (mb / secs < 1.) {
		//(void) fprintf(ftiming, "%.6f\n", mb/secs);
		snprintf(buf, BUFSIZE, "%s %.6f MB/SEC\n", tempbuf, mb/secs);
		ocall_print_string(buf);
		memset(buf, '\0', BUFSIZE);
	} else {
		//(void) fprintf(ftiming, "%.2f\n", mb/secs);
		snprintf(buf, BUFSIZE, "%s %.2f MB/SEC\n", tempbuf, mb/secs);
		ocall_print_string(buf);
		memset(buf, '\0', BUFSIZE);
	}
}
