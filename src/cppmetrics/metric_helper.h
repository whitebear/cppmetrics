/**
 *  Created on: Aug 2, 2019
 *      Author: sunchao
 * */

#include <boost/lexical_cast.hpp>
#include <ifaddrs.h>
#include "cppmetrics/graphite/graphite_reporter.h"
#include "cppmetrics/graphite/graphite_sender_tcp.h"


#define GRAPHITE_ADDR "10.39.12.24"
#define GRAPHITE_PORT 2003

namespace cppmetrics {
namespace graphite {

class MetricHelper {
 public:
  static MetricHelper& GetInstance();

  virtual ~MetricHelper();

  void start(const std::string& module, const std::string& clusterid="0");

  void stop();

  void counter(const std::string& name, boost::uint64_t count_value);


  cppmetrics::core::TimerContextPtr timer(const std::string& name);


  core::MetricRegistryPtr metric_registry;
 private:
  MetricHelper();
  GraphiteSenderPtr graphite_sender;

  boost::shared_ptr<GraphiteReporter> graphite_reporter;

  std::string& replaceStr(std::string& str, const std::string& to_replaced, const std::string& newchars);
  std::string GetHostIp();


};

}
}
