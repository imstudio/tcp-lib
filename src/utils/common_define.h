#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H

// #define BOOST_SYSTEM_NO_DEPRECATED
// #define BOOST_ERROR_CODE_HEADER_ONLY

#include <string>
#include <memory>
#include <thread>
#include <mutex>

#define DF_SHARED_PTR(T) typedef std::shared_ptr<T> T##Ptr;
#define BEGIN_NAMESPACE(x) namespace x{
#define END_NAMESPACE  }

#define MUTEX_GUARD(x) std::lock_guard<std::mutex> mutex_guard(x);

#endif
