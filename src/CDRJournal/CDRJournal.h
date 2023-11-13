#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <string>

#include "../HTTPServ/HTTPServ.h"

const std::string cdrLogFileName = "../../cdrLog.txt";

class CallDetailRecord
{
public:
    std::chrono::system_clock::time_point arrivalTime;
    std::string callId;
    std::chrono::system_clock::time_point endTime;
    std::string status;
    std::chrono::system_clock::time_point operatorResponseTime;
    std::string operatorId;
    std::chrono::seconds duration;

    void setFromHttpRequest(const HttpRequest& request);

    void setEndTimeAndStatus(const std::string& callStatus);

    void setOperatorResponse(const std::string& operatorIdResponse);

    void writeCDRToFile();

private:
    std::string formatDateTime(const std::chrono::system_clock::time_point& time);
};
