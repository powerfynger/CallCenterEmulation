#include "CDRJournal.h"

void CallDetailRecord::setFromHttpRequest(const HttpRequest& request)
{
    arrivalTime = std::chrono::system_clock::now();
    callId = request.callId;
}

void CallDetailRecord::setEndTimeAndStatus(const std::string& callStatus)
{
    endTime = std::chrono::system_clock::now();
    status = callStatus;
}

void CallDetailRecord::setOperatorResponse(const std::string& operatorIdResponse)
{
    operatorResponseTime = std::chrono::system_clock::now();
    operatorId = operatorIdResponse;
}

void CallDetailRecord::writeCDRToFile()
{
    std::ofstream file(cdrLogFileName, std::ios_base::app);
    if (file.is_open())
    {
        duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - arrivalTime);
        file << formatDateTime(arrivalTime) << ';'
                << callId << ';'
                << formatDateTime(endTime) << ';'
                << status << ';'
                << formatDateTime(operatorResponseTime) << ';'
                << operatorId << ';'
                << duration.count() << '\n';
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file for writing CDR.\n";
    }
}

std::string CallDetailRecord::formatDateTime(const std::chrono::system_clock::time_point& time)
{
    auto time_t = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::localtime(&time_t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}