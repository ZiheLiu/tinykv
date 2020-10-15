#include <iostream>
#include <ctime>
#include <thread>
#include <vector>

#include "db/db.h"
#include "db/hash_db.h"
#include "util/zipf_query_generator.h"
#include "constants.h"

namespace tinykv {

  void QueryWorker(db::DB *db, ZipfQueryGenerator *generator) {
    Slice key;
    Slice real_value;
    Slice value;
    clock_t query_cost = 0;
    char *buf = new char[16 + 1024 * 1024 + 1024 + 8];
    clock_t *costs = new clock_t[kQueryTimes];
    for (int i = 0; i < kQueryTimes; i++) {
      generator->Next(&key, &real_value);

      clock_t begin_t = clock();
      Status s = db->Get(key, &value, buf);
      clock_t end_t = clock();
      query_cost += end_t - begin_t;
      costs[i] = end_t - begin_t;

      if (s.IsNotFound()) {
        printf("Not found. key [%s]\n", key.ToString().c_str());
      } else if (real_value.compare(value) != 0) {
        printf("Get value error. key [%s]\n", key.ToString().c_str());
      }
    }

    std::sort(costs, costs + kQueryTimes);
    printf("Per thread Query cost. Avg: %.4fms, p99: %.4fms, p95: %.4fms, p90: %.4fms, p80: %.4fms, p70: %.4fms, p50: %.4fms\n",
           query_cost * 1.0 / CLOCKS_PER_SEC * 1000 / kQueryTimes,
           costs[static_cast<int>(0.99 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000,
           costs[static_cast<int>(0.95 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000,
           costs[static_cast<int>(0.90 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000,
           costs[static_cast<int>(0.80 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000,
           costs[static_cast<int>(0.70 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000,
           costs[static_cast<int>(0.50 * kQueryTimes)] * 1.0 / CLOCKS_PER_SEC * 1000);

    delete[] buf;
    delete[] costs;
  }

  Status TestDB(Options& options) {
    db::DB* db;

    clock_t begin_t = clock();
    Status s = db::Open(options, &db);
    RETURN_IFN_OK(s)
    clock_t end_t = clock();
    printf("Build index cost: %.4fms\n", (end_t - begin_t) * 1.0 / CLOCKS_PER_SEC * 1000);

    ZipfQueryGenerator *generator;
    s = ZipfQueryGenerator::NewZipfQueryGenerator(options.query_filename,
                                                  kQueryTimes, kFirstQueryTimes,
                                                  &generator);
    RETURN_IFN_OK(s)

    std::vector<std::thread> threads;
    threads.reserve(kQueryThreadsNum);
    for (int i = 0; i < kQueryThreadsNum; i++) {
      threads.emplace_back(QueryWorker, db, generator);
    }

    for (auto &t : threads) {
      t.join();
    }
    delete db;
    delete generator;

    return s;
  }
}

int main() {
  tinykv::Options options;
  options.raw_filename = "raw_input_kv.bin";
  options.index_filename = "index_input_kv.bin";
  options.query_filename = "query_kv.bin";
  options.open_db_func = tinykv::db::HashDB::OpenDB;

  tinykv::Status s = tinykv::TestDB(options);
  if (!s.ok()) {
    std::cout << "Error: " << s.ToString() << std::endl;
  }

  return 0;
}
