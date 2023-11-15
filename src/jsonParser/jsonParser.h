#pragma once

#define CONFIG_PATH "../config/conf.json"
#include <fstream>

#include "../../include/json.hpp"
#include "../../include/easylogging++.h"

using json = nlohmann::json;

struct QueueConfig {
    int queueLength;
    int minProcessingTime;
    int maxProcessingTime;
    int processingDuration;
    int maxOperatorsNum;
};

QueueConfig readConfigFromFile(const std::string& filename);
