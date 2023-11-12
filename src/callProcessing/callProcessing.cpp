#include "callProcessing.h"



int Operator::getOperatorId(){
    return this->operatorId;
}
OperatorStatus Operator::getOperatorStatus(){
    return this->status;
}


void Operator::processCall(const HttpRequest &request)
{
    status = BUSY;
    std::cout << "Operator " << operatorId << " is processing call with CallID " << request.callId << std::endl;

    // Создаем поток для асинхронной обработки вызова
    std::thread([this, request]() {
        std::this_thread::sleep_for(std::chrono::seconds(processingTime));
        std::cout << "Operator " << operatorId << " finished processing call with CallID " << request.callId << std::endl;
        status = FREE;
    }).detach();

}

HttpRequest CallCenter::findHttpRequestByCallId(const std::string& targetCallId) {
    for (const auto& request : callQueue) {
        if (request.callId == targetCallId) {
            return request; // Найдено совпадение, возвращаем структуру HttpRequest
        }
    }
    return HttpRequest{};
}

int CallCenter::findFreeOperator()
{
    for (size_t i = 0; i < operators.size(); ++i)
    {
        if (operators[i].getOperatorStatus() == FREE)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void CallCenter::addToQueue(HttpRequest &request){
    std::srand(std::time(0));
    auto now = std::chrono::steady_clock::now();
    int holdTime = std::rand() % (conf.maxProcessingTime - conf.minProcessingTime + 1) + conf.minProcessingTime;
    auto endTime = now + std::chrono::seconds(holdTime);

    request.timeOut = endTime;
    while(true){
        request.callId = generateRandomString();
        if (findHttpRequestByCallId(request.callId).callId.empty()){
            break;
        }
    }
    callQueue.push_back(request);
}

void CallCenter::processQueue(){
    auto now = std::chrono::steady_clock::now();
    bool canProcessCall = false;
    for (auto& request : callQueue) {
        if (now > request.timeOut){
            // CDR
            auto it = std::find(callQueue.begin(), callQueue.end(), request); 
            callQueue.erase(it);

            std::cout << "Call with CallID " << request.callId << " ended with timeout" << std::endl;
            continue;
        }
        for (auto& op : operators) {
            if (op.getOperatorStatus() == BUSY) continue;
            canProcessCall = true;
            auto it = std::find(callQueue.begin(), callQueue.end(), request); 
            callQueue.erase(it);
            op.processCall(request);
        }
        if (canProcessCall == false) break;
    }
}

bool CallCenter::handleHttpRequest(HttpRequest &request)
{
    if (callQueue.size() < conf.queueLength)
    {

        // Отправляем HTTP ответ с CallID
        std::cout << "HTTP response sent for CallID " << request.callId << std::endl;

        // Пытаемся распределить вызов на свободного оператора
        int freeOperatorIndex = findFreeOperator();
        if (freeOperatorIndex != -1)
        {
            if (callQueue.empty()){
                operators[freeOperatorIndex].processCall(request);
                return true;
            }
            addToQueue(request);
            processQueue();
            return true;
        }
        addToQueue(request);
        return true;
    }
    else
    {
        // Отправляем HTTP ответ с отказом, так как очередь переполнена
        std::cout << "HTTP response sent for CallID " << request.callId << " (Queue is full)" << std::endl;
        return false;
    }
}