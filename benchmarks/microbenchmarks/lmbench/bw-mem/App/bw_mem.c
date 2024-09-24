/*
 * |Aisha Hasan|
 * 
 * The |App/bw_mem.c| file. 
 *
 */


#include "EnclaveBridge.h"
#include "bw_mem.h"
#include "bench_app.h"


int main(int ac, char *av[]){

	int parallel = 1;
    int warmup = 0;
    int repetitions = TRIES;
	size_t nbytes;
	state_t state;    

	int c;
  	char    *usage = "[-P <parallelism>] [-W <warmup>] [-N <repetitions>] <size> what [conflict]\nwhat: rd wr rdwr cp fwr frd fcp bzero bcopy\n<size> must be larger than 512";


	state.overhead = 0;

	while (( c = getopt(ac, av, "P:W:N:")) != EOF) {
        switch(c) {
        case 'P':
            parallel = atoi(optarg);
            if (parallel <= 0) lmbench_usage(ac, av, usage);
            break;
        case 'W':
            warmup = atoi(optarg);
            break;
        case 'N':
            repetitions = atoi(optarg);
            break;
        default:
            lmbench_usage(ac, av, usage);
            break;
        }
    }


	/* should have two, possibly three [indicates align] arguments left */
	state.aligned = state.need_buf2 = 0;
	if (optind + 3 == ac) {
		state.aligned = 1;
	} else if (optind + 2 != ac) {
		lmbench_usage(ac, av, usage);
	}

	nbytes = state.nbytes = bytes(av[optind]);

	if (state.nbytes < 512) { /* this is the number of bytes in the loop */
		lmbench_usage(ac, av, usage);
	}


	if (streq(av[optind+1], "cp") ||
	    streq(av[optind+1], "fcp") || streq(av[optind+1], "bcopy")) {
		state.need_buf2 = 1;
	}


	if (streq(av[optind+1], "rd") 	||
		streq(av[optind+1], "wr") 	||
		streq(av[optind+1], "rdwr") ||
		streq(av[optind+1], "cp") 	||
		streq(av[optind+1], "frd") 	||
		streq(av[optind+1], "fwr") 	||
		streq(av[optind+1], "fcp") 	||
		streq(av[optind+1], "bzero")||
		streq(av[optind+1], "bcopy")) 
	{

		send_params(parallel,
					warmup,
					repetitions,
					av[optind+1],
					strlen(av[optind+1]) + 1,
					state);
	}
	else {
		lmbench_usage(ac, av, usage);
	}

	printf("Info: SampleEnclave successfully returned.\n");
	return (0);
}


void
lmbench_usage(int argc, char *argv[], char* usage)
{
	fprintf(stderr,"Usage: %s %s\n", argv[0], usage);
	exit(-1);
}
