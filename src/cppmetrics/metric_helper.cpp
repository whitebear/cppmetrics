

#include <boost/lexical_cast.hpp>
#include <ifaddrs.h>
#include "cppmetrics/graphite/graphite_reporter.h"
#include "cppmetrics/graphite/graphite_sender_tcp.h"
#include "metric_helper.h"

#define GRAPHITE_ADDR "10.39.12.24"
#define GRAPHITE_PORT 2003

namespace cppmetrics {
namespace graphite {

std::string& MetricHelper::replaceStr(std::string& str, const std::string& to_replaced, const std::string& newchars)
{
    for(std::string::size_type pos(0); pos != std::string::npos; pos += newchars.length())
    {
        pos = str.find(to_replaced,pos);
        if(pos!=std::string::npos)
           str.replace(pos,to_replaced.length(),newchars);
        else
            break;
    }
    return   str;
}

std::string MetricHelper::GetHostIp() {
  struct ifaddrs *ifaddr, *ifa;
  int family, s;
  char host[NI_MAXHOST];
  if (getifaddrs(&ifaddr) == -1) {
    return "";
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    if (family == AF_INET) {
      s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
                      NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      if (s != 0) {
        continue;
      }
      // skip 127.0.0.1
      if (inet_addr(host) != 16777343) {
        std::string hostStr(host);
        replaceStr(hostStr,std::string("."),std::string("_"));
        return hostStr;
      }
    }
  }
  return "";
}




  MetricHelper& MetricHelper::GetInstance() {
    static MetricHelper metricHelper;
    return metricHelper;
  }
  ;


  MetricHelper::MetricHelper() {
  }
  ;

  MetricHelper::~MetricHelper() {
  }
  ;

  void MetricHelper::start(const std::string& module, const std::string& clusterid) {
    metric_registry.reset(new core::MetricRegistry());

    graphite_sender.reset(new GraphiteSenderTCP(GRAPHITE_ADDR, GRAPHITE_PORT));

    std::ostringstream ostr;
    ostr << module << '.' << clusterid << '.' << GetHostIp();
    std::string prefix(ostr.str());

    graphite_reporter.reset(
        new GraphiteReporter(metric_registry, graphite_sender, prefix,
                             boost::chrono::seconds(1)));

    graphite_reporter->start(boost::chrono::milliseconds(1000 * 60));
  }

  void MetricHelper::stop() {
    graphite_reporter->stopNow();
  }


  core::MetricRegistryPtr metric_registry;
  GraphiteSenderPtr graphite_sender;
  boost::shared_ptr<GraphiteReporter> graphite_reporter;


  void MetricHelper::counter(const std::string& name, boost::uint64_t count_value) {
    core::CounterPtr counter_ptr(metric_registry->counter(name));
    counter_ptr->increment(count_value);
  }

  cppmetrics::core::TimerContextPtr MetricHelper::timer(const std::string& name) {
    return metric_registry->timer(name)->timerContextPtr();
  }

};

}

