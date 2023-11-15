#include <iostream>
// #include "callProcessing/callProcessing.h"
#include "HTTPServ/HTTPServ.h"

INITIALIZE_EASYLOGGINGPP

int main(int, char **)
{
  el::Configurations defaultConf;

  defaultConf.setToDefault();
  defaultConf.setGlobally(el::ConfigurationType::Filename, "logs/CallCenter.log");
  el::Loggers::reconfigureLogger("default", defaultConf);

  QueueConfig config = readConfigFromFile(CONFIG_PATH);
  runHTTPServ(config);

  return 0;
}
