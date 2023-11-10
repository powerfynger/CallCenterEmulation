#include <iostream>
#include "HTTPServ/HTTPServ.h"



int main(int, char**){
    QueueConfig config = readConfigFromFile(CONFIG_PATH);
    std::cout << "Queue Length: " << config.queueLength << std::endl;
    std::cout << "Time Range: " << config.minProcessingTime << " - " << config.maxProcessingTime << std::endl;
    std::cout << "Processing Duration: " << config.processingDuration << std::endl;
}

