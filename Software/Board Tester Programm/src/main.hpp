/*
 * main.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include "controllers/PeripheralsInterfacesController.hpp"
#include "database/DatabaseCommandListener.hpp"
#include "helpers/Initializer.hpp"
#include "managers/TestProcessManager.hpp"
#include "managers/LedsManager.hpp"
#include "managers/DisplayManager.hpp"
#include "managers/ButtonsManager.hpp"
#include "controllers/BoardConnectionController.hpp"
#include "helpers/Logger.hpp"

Logger logger;
PeripheralsInterfacesController controller(&logger);
Initializer initializer(&controller);
DisplayManager displayManager(&controller);
TestProcessManager testProcessManager(&controller, &displayManager);
DatabaseCommandListener databaseCommandListener(&controller, &testProcessManager);
LedsManager ledsManager(&controller);
BoardConnectionController boardConnectionController(&controller, &displayManager);
ButtonsManager buttonsManager(&controller, &testProcessManager, &displayManager, &boardConnectionController);

void initialize();
void exitFunction();
string logFileName;

#endif /* MAIN_HPP_ */
