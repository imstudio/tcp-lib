#ifndef GLOBALSTATISTICS_H
#define GLOBALSTATISTICS_H

#include <mutex>
#include "common_define.h"
#include <atomic>

BEGIN_NAMESPACE(utils)

class GlobalStatistics{
public:
  static GlobalStatistics* instance(){
    return inst;
  }

  ~GlobalStatistics() {
    delete inst;
  }

  std::atomic<int64_t> his_conn_count{0};
  std::atomic<int64_t> total_data_recieved{0};
  std::atomic<int64_t> total_data_send{0};
  std::chrono::time_point<std::chrono::system_clock> server_start_time;
  std::chrono::time_point<std::chrono::system_clock> server_end_time;
private:
  GlobalStatistics() {
  }

  static GlobalStatistics* inst;
};


END_NAMESPACE
#endif
