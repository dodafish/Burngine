################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Burngine/Window/Unix/WindowImplX11.cpp 

OBJS += \
./src/Burngine/Window/Unix/WindowImplX11.o 

CPP_DEPS += \
./src/Burngine/Window/Unix/WindowImplX11.d 


# Each subdirectory must supply rules for building sources it contributes
src/Burngine/Window/Unix/%.o: ../src/Burngine/Window/Unix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/dominik/workspace/Burngine/include" -I"/home/dominik/workspace/Burngine/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


