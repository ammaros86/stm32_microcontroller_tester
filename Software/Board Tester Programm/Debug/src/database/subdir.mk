################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/database/DatabaseCommandListener.cpp \
../src/database/DatabaseController.cpp 

CPP_DEPS += \
./src/database/DatabaseCommandListener.d \
./src/database/DatabaseController.d 

OBJS += \
./src/database/DatabaseCommandListener.o \
./src/database/DatabaseController.o 


# Each subdirectory must supply rules for building sources it contributes
src/database/%.o: ../src/database/%.cpp src/database/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-database

clean-src-2f-database:
	-$(RM) ./src/database/DatabaseCommandListener.d ./src/database/DatabaseCommandListener.o ./src/database/DatabaseController.d ./src/database/DatabaseController.o

.PHONY: clean-src-2f-database

