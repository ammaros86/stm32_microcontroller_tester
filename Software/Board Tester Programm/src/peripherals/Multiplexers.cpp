/*
 * Multiplexers.cpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#include "../peripherals/Mulitplexers.hpp"

Multiplexers::Multiplexers(){
	logger = NULL;
}

Multiplexers::~Multiplexers() {

}

/* initialize Multiplexer 5, 6, 7
 */
bool Multiplexers::initialControllMultiplexers(Logger *_logger){
	logger = _logger;
	bool isInitialized = true;
	int isMux5Initialised = setMultiplexer5Conenction(Mux5Free);
	int isMux6Initialised = setMultiplexer6Conenction(Mux6Free);
	int isMux7Initialised = setMultiplexer7Conenction(Mulitplexer7_Config::free);
	if(isMux5Initialised < 0){
 		logger->printToFileLogger("Error:Multiplexers initialControllMultiplexers failed to initialize Multiplexer 5", false);
 		isInitialized = false;
	}
	if(isMux6Initialised < 0){
 		logger->printToFileLogger("Error:Multiplexers initialControllMultiplexers failed to initialize Multiplexer 6", false);
 		isInitialized = false;
	}
	if(isMux7Initialised < 0){
 		logger->printToFileLogger("Error:Multiplexers initialControllMultiplexers failed to initialize Multiplexer 7", false);
 		isInitialized = false;
	}
	return isInitialized;
}

/* connect Multiplexer 5 Input/Output Z to Pin Number (Y0 - Y4)
 * Parameter:
 * pinNumber: Pin Number  (Y0 - Y4)
 */
int Multiplexers::setMultiplexer5Conenction(uint8_t pinNumber){
	int Mux5State = MUX5_CONTROLL_ERROR;
	switch(pinNumber){
		case MuxY0 :
			digitalWrite(S0_MUX5, LOW);
			digitalWrite(S1_MUX5, LOW);
			digitalWrite(S2_MUX5, LOW);
			Mux5State = MuxY0;
			break;
		case MuxY1 :
			digitalWrite(S0_MUX5, HIGH);
			digitalWrite(S1_MUX5, LOW);
			digitalWrite(S2_MUX5, LOW);
			Mux5State = MuxY1;
			break;
		case MuxY2 :
			digitalWrite(S0_MUX5, LOW);
			digitalWrite(S1_MUX5, HIGH);
			digitalWrite(S2_MUX5, LOW);
			Mux5State = MuxY2;
			break;
		case MuxY3 :
			digitalWrite(S0_MUX5, HIGH);
			digitalWrite(S1_MUX5, HIGH);
			digitalWrite(S2_MUX5, LOW);
			Mux5State = MuxY3;
			break;
		case MuxY4 :
			digitalWrite(S0_MUX5, LOW);
			digitalWrite(S1_MUX5, LOW);
			digitalWrite(S2_MUX5, HIGH);
			Mux5State = MuxY4;
			break;
		case Mux5Free :
			digitalWrite(S0_MUX5, HIGH);
			digitalWrite(S1_MUX5, LOW);
			digitalWrite(S2_MUX5, HIGH);
			Mux5State = Mux5Free;
			break;
		default:
			Mux5State = MUX5_CONTROLL_ERROR;
			break;
	}
	delay(_5MS_DELAY);
 	if(Mux5State >= MuxY0 && Mux5State <= Mux5Free){
		return 1;
	}else{
 		logger->printToFileLogger("Error:Multiplexers setMultiplexer5Conenction set Multiplexer 5 to Pin " + to_string(pinNumber) + " failed", false);
		return Mux5State;
	}
}

/* connect Multiplexer 6 Input/Output Z to Multiplexer (1-4) Z Pin
 * Parameter:
 * muxNumber: Multiplexer (1-4) Z
 */
int Multiplexers::setMultiplexer6Conenction(int muxNumber){
	int Mux6State = MUX6_CONTROLL_ERROR;
 	switch(muxNumber){
		case Mux1:
			digitalWrite(S0_MUX6, LOW);
			digitalWrite(S1_MUX6, LOW);
			digitalWrite(S2_MUX6, LOW);
			Mux6State = Mux1;
			break;
		case Mux2:
			digitalWrite(S0_MUX6, HIGH);
			digitalWrite(S1_MUX6, LOW);
			digitalWrite(S2_MUX6, LOW);
			Mux6State = Mux2;
			break;
		case Mux3:
			digitalWrite(S0_MUX6, LOW);
			digitalWrite(S1_MUX6, HIGH);
			digitalWrite(S2_MUX6, LOW);
			Mux6State = Mux3;
			break;
		case Mux4:
			digitalWrite(S0_MUX6, HIGH);
			digitalWrite(S1_MUX6, HIGH);
			digitalWrite(S2_MUX6, LOW);
			Mux6State = Mux4;
			break;
		case Mux5:
			digitalWrite(S0_MUX6, LOW);
			digitalWrite(S1_MUX6, LOW);
			digitalWrite(S2_MUX6, HIGH);
			Mux6State = Mux5;
			break;
		case Mux6Free:
			digitalWrite(S0_MUX6, LOW);
			digitalWrite(S1_MUX6, HIGH);
			digitalWrite(S2_MUX6, HIGH);
			Mux6State = Mux6Free;
			break;
		default:
			Mux6State = MUX6_CONTROLL_ERROR;
			break;
	}
	delay(_5MS_DELAY);
 	if(Mux6State >= Mux1 && Mux6State <= Mux6Free){
		return 1;
	}else{
 		logger->printToFileLogger("Error:Multiplexers setMultiplexer6Conenction set Multiplexer 6 to Multiplexer " + to_string(muxNumber) + " failed", false);
		return MUX6_CONTROLL_ERROR;
	}
 }

/* Set Multiplexer 7
 * Parameter:
 * mux7Command: Multiplexer 7 Command
 */
int Multiplexers::setMultiplexer7Conenction(Mulitplexer7_Config  mux7Command){
	Mulitplexer7_Config Mux7State = Mulitplexer7_Config::none;
 	switch(mux7Command){
		case Mulitplexer7_Config::free:
			digitalWrite(S1_MUX7, LOW);
			digitalWrite(S2_MUX7, LOW);
			digitalWrite(S3_MUX7, LOW);
			Mux7State = Mulitplexer7_Config::free;
			break;
		case Mulitplexer7_Config::AdcConnection:
			digitalWrite(S1_MUX7, LOW);
			digitalWrite(S2_MUX7, HIGH);
			digitalWrite(S3_MUX7, LOW);
			Mux7State = Mulitplexer7_Config::AdcConnection;
			break;
		case Mulitplexer7_Config::DacConnection:
			digitalWrite(S1_MUX7, HIGH);
			digitalWrite(S2_MUX7, LOW);
			digitalWrite(S3_MUX7, LOW);
			Mux7State = Mulitplexer7_Config::DacConnection;
			break;
		case Mulitplexer7_Config::AdcDacConnection:
			digitalWrite(S1_MUX7, HIGH);
			digitalWrite(S2_MUX7, HIGH);
			digitalWrite(S3_MUX7, LOW);
			Mux7State = Mulitplexer7_Config::AdcDacConnection;
			break;
		case Mulitplexer7_Config::CurrentSourceConnection:
			digitalWrite(S1_MUX7, LOW);
			digitalWrite(S2_MUX7, HIGH);
			digitalWrite(S3_MUX7, HIGH);
			Mux7State = Mulitplexer7_Config::CurrentSourceConnection;
			break;
		default:
			Mux7State = Mulitplexer7_Config::none;
			break;
	}
	delay(_5MS_DELAY);
  	if(Mux7State != Mulitplexer7_Config::none){
 		return static_cast<int>(Mux7State);
 	}else{
 		logger->printToFileLogger("Error:Multiplexers setMultiplexer7Conenction failed to set Multiplexer 7", false);
  		return MUX7_CONTROLL_ERROR;
 	}
 }
