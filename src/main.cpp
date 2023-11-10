#include <iostream>
#include <fstream>
#include "HTTPServ/HTTPServ.h"
#pragma once

using json = nlohmann::json;


int main(int, char**){
    QueueConfig config = readConfigFromFile("config.json");
    std::cout << "Queue Length: " << config.queueLength << std::endl;
    std::cout << "Time Range: " << config.minProcessingTime << " - " << config.maxProcessingTime << std::endl;
    std::cout << "Processing Duration: " << config.processingDuration << std::endl;
}

