#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    // Lock
    std::lock_guard<std::mutex> uLock(_mutex);

    // Add new message to queue
    _queue.push_back(std::move(msg));

    // Send a notification
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    
    // Initialize previous and current timestamp
    auto currTime = std::chrono::steady_clock::now();
    auto prevTime = currTime;

    // // Generate cycle duration with random value
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(4000, 6000);
    long cycleTime = dis(gen);

    while (true) {
        // Record timestamp
        currTime = std::chrono::steady_clock::now();

        // Calculate elapsed time
        long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currTime - prevTime).count();
        
        // If elapsed time is larger then cycle time
        if (elapsedTime > cycleTime) {
            // Toggle current phase of the traffic light
            if (_currentPhase == TrafficLightPhase::green) {
                _currentPhase = TrafficLightPhase::red;
            } else if (_currentPhase == TrafficLightPhase::red) {
                _currentPhase = TrafficLightPhase::green;
            } else {
                std::cout << "Undefined case in TrafficLight::cycleThroughPhases()" << std::endl;
            }

            // Send update method to message queue with using move semantics
            _queue.send(std::move(_currentPhase));

            // Update prevTime and cycleTime
            prevTime = currTime;
            cycleTime = dis(gen);
        }

        // Wait 1 ms between two loos
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
