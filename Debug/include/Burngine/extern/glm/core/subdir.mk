################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/Burngine/extern/glm/core/dummy.cpp 

OBJS += \
./include/Burngine/extern/glm/core/dummy.o 

CPP_DEPS += \
./include/Burngine/extern/glm/core/dummy.d 


# Each subdirectory must supply rules for building sources it contributes
include/Burngine/extern/glm/core/%.o: ../include/Burngine/extern/glm/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/dominik/workspace/Burngine/include" -I"/home/dominik/workspace/Burngine/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


