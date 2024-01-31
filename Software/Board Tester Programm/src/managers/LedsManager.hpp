/*
 * LedsManager.hpp
 *
 *  Created on: 30.08.2023
 *      Author: Astra
 */

#ifndef MANAGERS_LEDSMANAGER_HPP_
#define MANAGERS_LEDSMANAGER_HPP_


#define LEDS_THREAD_PERIOD 50

#include "../controllers/PeripheralsInterfacesController.hpp"

class LedsManager{

public:

	LedsManager(PeripheralsInterfacesController *_controller): controller(_controller){
		blinkStateOn = false;
		startLedsThread();
 	};

 	virtual ~LedsManager();
  	void startLedsThread();
 	void stopLedsThread();
 	void checkLedNewCommand();
 	void setLeds();

private:

 	PeripheralsInterfacesController *controller;
   	std::thread ledsThread;
	bool blinkStateOn;

};


#endif /* MANAGERS_LEDSMANAGER_HPP_ */
