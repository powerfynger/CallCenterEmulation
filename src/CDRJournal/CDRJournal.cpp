#include "CDRJournal.h"

CallDetailRecord::CallDetailRecord()
{
}


void CallDetailRecord::setStatus(int setStatusCode)
{
    status = setStatusCode;
}

void CallDetailRecord::setOperatorResponse(int operatorIdResponse)
{
    operatorId = operatorIdResponse;
}

void CallDetailRecord::writeCDRToFile()
{
    CLOG(INFO, "cdrLogger")
                << _formatDateTime(arrivalTime) << ';'
                << callId << ';'
                << _formatDateTime(endTime) << ';'
                << status << ';'
                << (operatorResponseTime != std::chrono::system_clock::time_point::min() 
                ? _formatDateTime(operatorResponseTime) : "") << ';'
                << (operatorId != -1 ? std::to_string(operatorId) : "") << ';'
                << (duration != -1 ? std::to_string(duration) : "") << ";";
}

std::string CallDetailRecord::_formatDateTime(const std::chrono::system_clock::time_point& time)
{
    auto time_t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&time_t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}