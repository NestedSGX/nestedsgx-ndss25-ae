//
//  ycsbc.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/19/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include "core/client.h"
#include "db/db_factory.h"
#include "ycsbc_urts.h"

using namespace std;

ycsbc::DB *g_db;
ycsbc::CoreWorkload g_wl;

int DelegateClient(const int num_ops, bool is_loading) {
  g_db->Init();
  ycsbc::Client client(*g_db, g_wl);
  int oks = 0;
  for (int i = 0; i < num_ops; ++i) {
    if (is_loading) {
      oks += client.DoInsert();
    } else {
      oks += client.DoTransaction();
    }
  }
  g_db->Close();
  return oks;
}

int main(const int argc, const char *argv[]) {
  utils::Properties props;
  string file_name = ParseCommandLine(argc, argv, props);

  g_db = ycsbc::DBFactory::CreateDB(props);
  if (!g_db) {
    cout << "Unknown database name " << props["dbname"] << endl;
    exit(0);
  }

  g_wl.Init(props);

  run_benchmark(props, file_name);
}

