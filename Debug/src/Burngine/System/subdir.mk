################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Burngine/System/NonCopyable.cpp \
../src/Burngine/System/NonInstancable.cpp 

OBJS += \
./src/Burngine/System/NonCopyable.o \
./src/Burngine/System/NonInstancable.o 

CPP_DEPS += \
./src/Burngine/System/NonCopyable.d \
./src/Burngine/System/NonInstancable.d 


# Each subdirectory must supply rules for building sources it contributes
src/Burngine/System/%.o: ../src/Burngine/System/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/dominik/workspace/Burngine/include" -I"/home/dominik/workspace/Burngine/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


