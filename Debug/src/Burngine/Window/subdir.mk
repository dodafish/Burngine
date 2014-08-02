################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Burngine/Window/Window.cpp \
../src/Burngine/Window/WindowImpl.cpp 

OBJS += \
./src/Burngine/Window/Window.o \
./src/Burngine/Window/WindowImpl.o 

CPP_DEPS += \
./src/Burngine/Window/Window.d \
./src/Burngine/Window/WindowImpl.d 


# Each subdirectory must supply rules for building sources it contributes
src/Burngine/Window/%.o: ../src/Burngine/Window/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/dominik/workspace/Burngine/include" -I"/home/dominik/workspace/Burngine/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


