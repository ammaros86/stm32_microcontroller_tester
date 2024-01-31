################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/controllers/BoardConnectionController.cpp \
../src/controllers/PeripheralsInterfacesController.cpp 

CPP_DEPS += \
./src/controllers/BoardConnectionController.d \
./src/controllers/PeripheralsInterfacesController.d 

OBJS += \
./src/controllers/BoardConnectionController.o \
./src/controllers/PeripheralsInterfacesController.o 


# Each subdirectory must supply rules for building sources it contributes
src/controllers/%.o: ../src/controllers/%.cpp src/controllers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-controllers

clean-src-2f-controllers:
	-$(RM) ./src/controllers/BoardConnectionController.d ./src/controllers/BoardConnectionController.o ./src/controllers/PeripheralsInterfacesController.d ./src/controllers/PeripheralsInterfacesController.o

.PHONY: clean-src-2f-controllers

