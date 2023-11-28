#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <string>
#include <mutex>
#include "../../include/easylogging++.h"

const std::string cdrLogFileName = "../logs/cdrLog.txt";


class CallDetailRecord
{
public:
    CallDetailRecord();

    void setStatus(int setStatusCode);

    void setOperatorResponse(int operatorIdResponse);

    void writeCDRToFile();
    std::chrono::system_clock::time_point arrivalTime;
    std::string callId;
    std::string number;
    std::chrono::system_clock::time_point endTime;
    int status;
    std::chrono::system_clock::time_point operatorResponseTime = std::chrono::system_clock::time_point::min();
    int operatorId = -1;
    int duration = -1;

private:
    std::string _formatDateTime(const std::chrono::system_clock::time_point& time);
};
// #endif