################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/managers/ButtonsManager.cpp \
../src/managers/DisplayManager.cpp \
../src/managers/LedsManager.cpp \
../src/managers/TestProcessManager.cpp 

CPP_DEPS += \
./src/managers/ButtonsManager.d \
./src/managers/DisplayManager.d \
./src/managers/LedsManager.d \
./src/managers/TestProcessManager.d 

OBJS += \
./src/managers/ButtonsManager.o \
./src/managers/DisplayManager.o \
./src/managers/LedsManager.o \
./src/managers/TestProcessManager.o 


# Each subdirectory must supply rules for building sources it contributes
src/managers/%.o: ../src/managers/%.cpp src/managers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-managers

clean-src-2f-managers:
	-$(RM) ./src/managers/ButtonsManager.d ./src/managers/ButtonsManager.o ./src/managers/DisplayManager.d ./src/managers/DisplayManager.o ./src/managers/LedsManager.d ./src/managers/LedsManager.o ./src/managers/TestProcessManager.d ./src/managers/TestProcessManager.o

.PHONY: clean-src-2f-managers

