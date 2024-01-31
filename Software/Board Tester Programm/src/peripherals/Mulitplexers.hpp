/*
 * Mulitplexers.hpp
 *
 *  Created on: 29.05.2023
 *      Author: Astra
 */

#ifndef PERIPHERALS_MULITPLEXERS_HPP_
#define PERIPHERALS_MULITPLEXERS_HPP_


#include "../helpers/Logger.hpp"

#define Mux5Free 5
#define Mux6Free 6

class Multiplexers{

public :

	Multiplexers();
	virtual ~Multiplexers();

 	bool initialControllMultiplexers(Logger *_logger);
	int setMultiplexer7Conenction(Mulitplexer7_Config  mux7Command);
	int setMultiplexer5Conenction(uint8_t pinNumber);
	int setMultiplexer6Conenction(int muxNumber);

 private :

	Logger * logger;

};

#endif /* PERIPHERALS_MULITPLEXERS_HPP_ */
