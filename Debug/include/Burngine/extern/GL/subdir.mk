################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../include/Burngine/extern/GL/glew.c 

OBJS += \
./include/Burngine/extern/GL/glew.o 

C_DEPS += \
./include/Burngine/extern/GL/glew.d 


# Each subdirectory must supply rules for building sources it contributes
include/Burngine/extern/GL/%.o: ../include/Burngine/extern/GL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


