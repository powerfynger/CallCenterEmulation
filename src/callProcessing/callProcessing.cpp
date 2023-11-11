#include "callProcessing.h"

// std::queue<HttpRequest> callQueue;
// std::vector<Operator> operators;
// QueueConfig conf;

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

void CallCenter::handleHttpRequest(const HttpRequest &request)
{
    if (callQueue.size() < conf.queueLength)
    {
        // Добавляем запрос в очередь
        callQueue.push(request);

        // Отправляем HTTP ответ с CallID
        std::cout << "HTTP response sent for CallID " << request.callId << std::endl;

        // Пытаемся распределить вызов на свободного оператора
        int freeOperatorIndex = findFreeOperator();
        if (freeOperatorIndex != -1)
        {
            operators[freeOperatorIndex].processCall(request);
            return;
        }

        // Если все операторы заняты, удерживаем заявку в очереди в указанном диапазоне времени
        std::srand(std::time(0));
        int holdTime = std::rand() % (conf.maxProcessingTime - conf.minProcessingTime + 1) + conf.minProcessingTime;

        std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start_time < std::chrono::seconds(holdTime))
        {
            // Hold call in the queue without blocking the thread
            struct pollfd poll_fd;
            poll_fd.fd = -1;  // No file descriptor, just using poll as a timer
            poll_fd.events = 0;
            poll_fd.revents = 0;
            poll(&poll_fd, 1, 100);  // Poll with a timeout of 100 milliseconds
        }

        // Выход по timeout
        std::cout << "Call with CallID " << request.callId << " ended with timeout" << std::endl;
    }
    else
    {
        // Отправляем HTTP ответ с отказом, так как очередь переполнена
        std::cout << "HTTP response sent for CallID " << request.callId << " (Queue is full)" << std::endl;
    }
}