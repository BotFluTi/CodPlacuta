################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_enet.c \
../drivers/fsl_gpio.c \
../drivers/fsl_lpadc.c \
../drivers/fsl_lpflexcomm.c \
../drivers/fsl_lpuart.c \
../drivers/fsl_reset.c \
../drivers/fsl_spc.c 

C_DEPS += \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_enet.d \
./drivers/fsl_gpio.d \
./drivers/fsl_lpadc.d \
./drivers/fsl_lpflexcomm.d \
./drivers/fsl_lpuart.d \
./drivers/fsl_reset.d \
./drivers/fsl_spc.d 

OBJS += \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_enet.o \
./drivers/fsl_gpio.o \
./drivers/fsl_lpadc.o \
./drivers/fsl_lpflexcomm.o \
./drivers/fsl_lpuart.o \
./drivers/fsl_reset.o \
./drivers/fsl_spc.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DPRINTF_ADVANCED_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers/flash" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS/m-profile" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device/periph" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/str" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/silicon_id" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/debug_console_lite" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/gpio" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/uart" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/include" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port/sys_arch/dynamic" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/apps/http" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcux_config.h" -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcuxsdk_version.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-drivers

clean-drivers:
	-$(RM) ./drivers/fsl_clock.d ./drivers/fsl_clock.o ./drivers/fsl_common.d ./drivers/fsl_common.o ./drivers/fsl_common_arm.d ./drivers/fsl_common_arm.o ./drivers/fsl_enet.d ./drivers/fsl_enet.o ./drivers/fsl_gpio.d ./drivers/fsl_gpio.o ./drivers/fsl_lpadc.d ./drivers/fsl_lpadc.o ./drivers/fsl_lpflexcomm.d ./drivers/fsl_lpflexcomm.o ./drivers/fsl_lpuart.d ./drivers/fsl_lpuart.o ./drivers/fsl_reset.d ./drivers/fsl_reset.o ./drivers/fsl_spc.d ./drivers/fsl_spc.o

.PHONY: clean-drivers

