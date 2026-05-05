################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/netif/ppp/polarssl/arc4.c \
../lwip/src/netif/ppp/polarssl/des.c \
../lwip/src/netif/ppp/polarssl/md4.c \
../lwip/src/netif/ppp/polarssl/md5.c \
../lwip/src/netif/ppp/polarssl/sha1.c 

C_DEPS += \
./lwip/src/netif/ppp/polarssl/arc4.d \
./lwip/src/netif/ppp/polarssl/des.d \
./lwip/src/netif/ppp/polarssl/md4.d \
./lwip/src/netif/ppp/polarssl/md5.d \
./lwip/src/netif/ppp/polarssl/sha1.d 

OBJS += \
./lwip/src/netif/ppp/polarssl/arc4.o \
./lwip/src/netif/ppp/polarssl/des.o \
./lwip/src/netif/ppp/polarssl/md4.o \
./lwip/src/netif/ppp/polarssl/md5.o \
./lwip/src/netif/ppp/polarssl/sha1.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/netif/ppp/polarssl/%.o: ../lwip/src/netif/ppp/polarssl/%.c lwip/src/netif/ppp/polarssl/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DPRINTF_ADVANCED_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers/flash" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS/m-profile" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device/periph" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/str" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/silicon_id" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/debug_console_lite" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/gpio" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/uart" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/include" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port/sys_arch/dynamic" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/apps/http" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcux_config.h" -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcuxsdk_version.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl:
	-$(RM) ./lwip/src/netif/ppp/polarssl/arc4.d ./lwip/src/netif/ppp/polarssl/arc4.o ./lwip/src/netif/ppp/polarssl/des.d ./lwip/src/netif/ppp/polarssl/des.o ./lwip/src/netif/ppp/polarssl/md4.d ./lwip/src/netif/ppp/polarssl/md4.o ./lwip/src/netif/ppp/polarssl/md5.d ./lwip/src/netif/ppp/polarssl/md5.o ./lwip/src/netif/ppp/polarssl/sha1.d ./lwip/src/netif/ppp/polarssl/sha1.o

.PHONY: clean-lwip-2f-src-2f-netif-2f-ppp-2f-polarssl

