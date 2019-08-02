/*
 * Copyright 2000-2014 NeuStar, Inc. All rights reserved.
 * NeuStar, the Neustar logo and related names and logos are registered
 * trademarks, service marks or tradenames of NeuStar, Inc. All other
 * product names, company names, marks, logos and symbols may be trademarks
 * of their respective owners.
 */

/*
 * test_graphite_reporter.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: vpoliboy
 */

#include <boost/lexical_cast.hpp>

#include "cppmetrics/metric_helper.h"



using namespace std;
using namespace cppmetrics;
using namespace graphite;

int main(int argc, char* argv[]) {
  cout << "hello" << endl;
  MetricHelper::GetInstance().start("moduleA");

  int i = 0;
  do {
    cout << i << endl;
    MetricHelper::GetInstance().counter("testC", 1);
    {
    cppmetrics::core::TimerContextPtr timer = MetricHelper::GetInstance().timer("timerC");
    sleep(1);
    //timer->stop() | {zone} ;
    }
    i++;
  } while (i < 1000);
  MetricHelper::GetInstance().stop();
  return 0;
}

