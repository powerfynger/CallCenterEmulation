# Call Center System
This system simulates a call center where customers make requests and operators process them. The system has several components that work together to manage calls and ensure efficient handling of customer requests. 

Here is a request example:
```
http://192.168.0.65:8000/call?number=91123
```

## Components
The following are the key components of the system:

* **Call Center**: This is the central component that manages all incoming requests and assigns them to available operators. It also keeps track of the status of each call and ensures that no two operators work on the same call simultaneously.
* **Operator**: An operator is responsible for processing customer requests. Each operator has an ID and a status (available or busy). When an operator becomes available, they check the call queue for any pending requests and pick up the first one. They then process the request until completion and update their status accordingly.
* **Call Request**: A call request represents a customer's request for assistance. It includes information such as the caller's phone number and the current status of the request.
* **Call Record**: A call record contains detailed information about a specific call, including the arrival time, the duration of the call, the operator who handled the call, and the final disposition of the call (e.g., completed, timed out, etc.).

## How it Works
When a new call comes into the system, it is added to the back of the call queue. If there are no available operators, the call remains in the queue until an operator becomes available or the waiting time will expire. Once an operator becomes available, they check the front of the call queue for any pending requests and pick up the first one. They then process the request until completion and update their status accordingly.

If the call queue reaches its maximum capacity, subsequent calls will be rejected with a message indicating that the server is too busy. In addition, the system logs various events, such as when a call is received, assigned to an operator, processed by an operator, and completed. These log messages include details such as the caller's phone number, the operator's ID, and the duration of the call.

## Configuration
The behavior of the system can be configured using a JSON configuration file. The file should contain the following parameters:

* `queueLength`: The maximum size of the call queue.
* `timeRange`: The minimum and maximum amount of time in seconds required to process a call.
* `processingDuration`: The average amount of time in seconds required to process a call.
* `maxOperators`: The maximum number of operators allowed in the system.

Here is an example configuration file:
```json
{
  "queueLength": 10,
  "timeRange": {
    "min": 60,
    "max": 120
  },
  "processingDuration": 90,
  "maxOperators": 4
}
```
## Testing
This project includes several unit tests via gtest for the `CallCenter` class and its related classes. The tests cover various scenarios such as adding operators, handling HTTP requests, and finding free operators.

Another way to test Call Center is using scripts to imitate the real life experience. 

Here is an example:
```code
$numberOfRequests = 15

for ($i=1; $i -le $numberOfRequests; $i++) {
    $randomNumber = Get-Random -Minimum 10000 -Maximum 99999
    $url = "http://192.168.0.65:8000/call?number=$randomNumber"
    Invoke-WebRequest -Uri $url
}
```
