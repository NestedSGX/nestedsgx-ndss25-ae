#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <future>
#include <sys/time.h>
#include "core/utils.h"
#include "core/timer.h"
#include "core/core_workload.h"

using namespace std;

void UsageMessage(const char *command);
bool StrStartWith(const char *str, const char *pre);
string ParseCommandLine(int argc, const char *argv[], utils::Properties &props);

extern "C" int DelegateClient(const int num_ops, bool is_loading);

void run_benchmark(const utils::Properties& props, const std::string& file_name) {
	struct timeval tval_before, tval_after, tval_result;
	int record_count = stoi(props["recordcount"]);
	int op_count = stoi(props["operationcount"]);

	// Loads data
	gettimeofday(&tval_before, NULL);
	int load_success = DelegateClient(record_count, true);
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("# Loading records: %d/%d (%ld.%06ld s)\n", load_success, record_count,
		(long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

	// Peforms transactions
	gettimeofday(&tval_before, NULL);
	int trans_success = DelegateClient(op_count, false);
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("# successful records: %d/%d (%ld.%06ld s)\n", trans_success, op_count,
		(long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	
	printf("# Transaction throughput (KTPS):\n");
	printf("%.4lf\n", op_count / 1000.0 / (tval_result.tv_sec + tval_result.tv_usec / 1e6));
}

string ParseCommandLine(int argc, const char *argv[], utils::Properties &props) {
	int argindex = 1;
	string filename;
	while (argindex < argc && StrStartWith(argv[argindex], "-")) {
		if (strcmp(argv[argindex], "-threads") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("threadcount", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-db") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("dbname", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-host") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("host", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-port") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("port", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-slaves") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("slaves", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-n") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("recordcount", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-t") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			props.SetProperty("operationcount", argv[argindex]);
			argindex++;
		} else if (strcmp(argv[argindex], "-P") == 0) {
			argindex++;
			if (argindex >= argc) {
				UsageMessage(argv[0]);
				exit(0);
			}
			filename.assign(argv[argindex]);
			ifstream input(argv[argindex]);
			try {
				if (!input.is_open()) throw utils::Exception("File not open!");
				while (!input.eof() && !input.bad()) {
					std::string line;
					std::getline(input, line);
					if (line[0] == '#') continue;
					size_t pos = line.find_first_of('=');
					if (pos == std::string::npos) continue;
					props.SetProperty(utils::Trim(line.substr(0, pos)), utils::Trim(line.substr(pos + 1)));
				}
			} catch (const string &message) {
				cout << message << endl;
				exit(0);
			}
			input.close();
			argindex++;
		} else {
		//   printf("Unknown option '%s'\n", argv[argindex]);
			cout << "Unknown option '" << argv[argindex] << "'" << endl;
			exit(0);
		}
	}

	if (argindex == 1 || argindex != argc) {
		UsageMessage(argv[0]);
		exit(0);
	}

	return filename;
}

void UsageMessage(const char *command) {
	printf("Usage: %s [options]\n", command);
	printf("Options:\n");
	printf("  -threads n: execute using n threads (default: 1)\n");
	printf("  -db dbname: specify the name of the DB to use (default: basic)\n");
	printf("  -P propertyfile: load properties from the given file. Multiple files can\n");
	printf("                   be specified, and will be processed in the order specified\n");
}

inline bool StrStartWith(const char *str, const char *pre) {
	return strncmp(str, pre, strlen(pre)) == 0;
}
