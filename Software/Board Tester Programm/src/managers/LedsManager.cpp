/*
 * LedsManager.cpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */





#include "LedsManager.hpp"


LedsManager::~LedsManager() {
	stopLedsThread();
}

/* start LEDs Thread
 */
void LedsManager::startLedsThread(){
	ledsThread = std::thread(&LedsManager::checkLedNewCommand, this);
}

/* stop LEDs Thread
 */
void LedsManager::stopLedsThread(){
	if (ledsThread.joinable()) {
		ledsThread.join();
	}
}

/* LEDs Thread
 */
void LedsManager::checkLedNewCommand(){
	while(1){
		setLeds();
		const auto threadPeriod = std::chrono::milliseconds(LEDS_THREAD_PERIOD);
		auto startTime = std::chrono::steady_clock::now();
		auto endTime = std::chrono::steady_clock::now();
		auto elapsedTime = endTime - startTime;
		auto waitTime = threadPeriod - elapsedTime;
		if (waitTime > std::chrono::milliseconds::zero()) {
			std::this_thread::sleep_for(waitTime);
		}
	}
}

/* Set new LEDs State
 */
void LedsManager::setLeds(){
	LEDS_COMMAND newLedsCommand = controller->getLedsCommand();
	if(newLedsCommand == LEDS_COMMAND::setGreenOn){
		digitalWrite(LED_GREEN_PI_PIN, HIGH);
		digitalWrite(LED_RED_PI_PIN, LOW);
	}else if(newLedsCommand == LEDS_COMMAND::setRedOn){
		digitalWrite(LED_RED_PI_PIN, HIGH);
		digitalWrite(LED_GREEN_PI_PIN, LOW);
	}else if(newLedsCommand == LEDS_COMMAND::setAllOn){
		digitalWrite(LED_GREEN_PI_PIN, HIGH);
		digitalWrite(LED_RED_PI_PIN, HIGH);
	}else if(newLedsCommand == LEDS_COMMAND::blinkAll || newLedsCommand == LEDS_COMMAND::blinkGreen || newLedsCommand == LEDS_COMMAND::blinkRed){
		if(newLedsCommand == LEDS_COMMAND::blinkGreen){
			if(blinkStateOn == false){
				digitalWrite(LED_GREEN_PI_PIN, HIGH);
				digitalWrite(LED_RED_PI_PIN, LOW);
				blinkStateOn = true;
			}else{
				digitalWrite(LED_GREEN_PI_PIN, LOW);
				digitalWrite(LED_RED_PI_PIN, LOW);
				blinkStateOn = false;
			}
		}else if(newLedsCommand == LEDS_COMMAND::blinkRed){
			if(blinkStateOn == false){
				digitalWrite(LED_RED_PI_PIN, HIGH);
				digitalWrite(LED_GREEN_PI_PIN, LOW);
				blinkStateOn = true;
			}else{
				digitalWrite(LED_GREEN_PI_PIN, LOW);
				digitalWrite(LED_RED_PI_PIN, LOW);
				blinkStateOn = false;
			}
		}else if(newLedsCommand == LEDS_COMMAND::blinkAll){
			if(blinkStateOn == false){
				digitalWrite(LED_GREEN_PI_PIN, HIGH);
				digitalWrite(LED_RED_PI_PIN, HIGH);
				blinkStateOn = true;
			}else{
				digitalWrite(LED_GREEN_PI_PIN, LOW);
				digitalWrite(LED_RED_PI_PIN, LOW);
				blinkStateOn = false;
			}
		}
	}
}


