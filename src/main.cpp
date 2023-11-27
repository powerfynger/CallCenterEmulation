#include <iostream>
#include "HTTPServ/HTTPServ.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
  bool enableLogs;
  if (argc >= 2 && strcmp(argv[1], "-l") == 0) {
    enableLogs = true;
  }
  el::Configurations defaultConf;
  defaultConf.setToDefault();
  defaultConf.setGlobally(el::ConfigurationType::Filename, "logs/CallCenter.log");
  el::Loggers::reconfigureLogger("default", defaultConf);

  QueueConfig config = readConfigFromFile(CONFIG_PATH);
  runHTTPServ(config, enableLogs);

  return 0;
}
