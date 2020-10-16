#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <sys/time.h>

#include "db/db.h"
#include "util/zipf_query_generator.h"
#include "constants.h"

namespace tinykv {

  double GetDurationMs(const timeval &begin_t, const timeval &end_t) {
    return (end_t.tv_sec - begin_t.tv_sec) * 1000 + (end_t.tv_usec - begin_t.tv_usec) * 1.0 / 1000;
  }

  void QueryWorker(db::DB *db, ZipfQueryGenerator *generator) {
    Slice key;
    Slice real_value;
    Slice value;
    double query_cost = 0;
    timeval begin_t;
    timeval end_t;
    char *buf = new char[16 + 1024 * 1024 + 1024 + 8];
    double *costs = new double[kQueryTimes];
    for (int i = 0; i < kQueryTimes; i++) {
      generator->Next(&key, &real_value);

      gettimeofday(&begin_t, nullptr);
      Status s = db->Get(key, &value, buf);
      gettimeofday(&end_t, nullptr);
      double duration = GetDurationMs(begin_t, end_t);
      query_cost += duration;
      costs[i] = duration;

      if (s.IsNotFound()) {
        printf("Not found. key [%s]\n", key.ToString().c_str());
      } else if (real_value.compare(value) != 0) {
        printf("Get value error. key [%s]\n", key.ToString().c_str());
      }
    }

    std::sort(costs, costs + kQueryTimes);
    printf("Per thread Query cost. Avg: %.4fms, p99: %.4fms, p95: %.4fms, p90: %.4fms, p80: %.4fms, p70: %.4fms, p50: %.4fms\n",
           query_cost / kQueryTimes,
           costs[static_cast<int>(0.99 * kQueryTimes)],
           costs[static_cast<int>(0.95 * kQueryTimes)],
           costs[static_cast<int>(0.90 * kQueryTimes)],
           costs[static_cast<int>(0.80 * kQueryTimes)],
           costs[static_cast<int>(0.70 * kQueryTimes)],
           costs[static_cast<int>(0.50 * kQueryTimes)]);

    delete[] buf;
    delete[] costs;
  }

  Status TestDB() {
    db::DB* db;

    timeval begin_t;
    timeval end_t;
    gettimeofday(&begin_t, nullptr);
    Status s = db::Open(kRawFilename, kIndexFilename, kDbPolicy, &db);
    RETURN_IFN_OK(s)
    gettimeofday(&end_t, nullptr);
    printf("Build index costs: %.4fms\n", GetDurationMs(begin_t, end_t));

    ZipfQueryGenerator *generator;
    s = ZipfQueryGenerator::NewZipfQueryGenerator(kQueryFilename,
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
  tinykv::Status s = tinykv::TestDB();
  if (!s.ok()) {
    std::cout << "Error: " << s.ToString() << std::endl;
  }

  return 0;
}
