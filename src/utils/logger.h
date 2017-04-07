#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>

#include <boost/log/trivial.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

#define FLOG(level) BOOST_LOG_TRIVIAL(level)

inline void init_fnet_logger(const std::string &prefix)
{
  logging::add_common_attributes();
  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
  logging::add_file_log(
      keywords::file_name = prefix + "_%Y%m%d_%H-%M-%S.%N.log",
      keywords::rotation_size = 10 * 1024 * 1024,
      keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%] [%Severity%]: %Message%",
      keywords::min_free_space=3 * 1024 * 1024  );
  logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::debug);
}

#endif
