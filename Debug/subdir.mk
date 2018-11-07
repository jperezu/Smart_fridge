################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fsm.c \
../inicializacion.c \
../main.c \
../mfrc522.c \
../mfrc522_hal_linux.c \
../nevera_control.c \
../rfid.c \
../socket.c \
../temperatura.c \
../temporizador.c \
../usuario.c 

OBJS += \
./fsm.o \
./inicializacion.o \
./main.o \
./mfrc522.o \
./mfrc522_hal_linux.o \
./nevera_control.o \
./rfid.o \
./socket.o \
./temperatura.o \
./temporizador.o \
./usuario.o 

C_DEPS += \
./fsm.d \
./inicializacion.d \
./main.d \
./mfrc522.d \
./mfrc522_hal_linux.d \
./nevera_control.d \
./rfid.d \
./socket.d \
./temperatura.d \
./temporizador.d \
./usuario.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"C:\SysGCC\Raspberry\include" -I"C:\SysGCC\Raspberry\include\wiringPi\wiringPi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


