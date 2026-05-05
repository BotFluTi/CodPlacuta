################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/altcp.c \
../lwip/src/core/altcp_alloc.c \
../lwip/src/core/altcp_tcp.c \
../lwip/src/core/def.c \
../lwip/src/core/dns.c \
../lwip/src/core/inet_chksum.c \
../lwip/src/core/init.c \
../lwip/src/core/ip.c \
../lwip/src/core/mem.c \
../lwip/src/core/memp.c \
../lwip/src/core/netif.c \
../lwip/src/core/pbuf.c \
../lwip/src/core/raw.c \
../lwip/src/core/stats.c \
../lwip/src/core/sys.c \
../lwip/src/core/tcp.c \
../lwip/src/core/tcp_in.c \
../lwip/src/core/tcp_out.c \
../lwip/src/core/timeouts.c \
../lwip/src/core/udp.c 

C_DEPS += \
./lwip/src/core/altcp.d \
./lwip/src/core/altcp_alloc.d \
./lwip/src/core/altcp_tcp.d \
./lwip/src/core/def.d \
./lwip/src/core/dns.d \
./lwip/src/core/inet_chksum.d \
./lwip/src/core/init.d \
./lwip/src/core/ip.d \
./lwip/src/core/mem.d \
./lwip/src/core/memp.d \
./lwip/src/core/netif.d \
./lwip/src/core/pbuf.d \
./lwip/src/core/raw.d \
./lwip/src/core/stats.d \
./lwip/src/core/sys.d \
./lwip/src/core/tcp.d \
./lwip/src/core/tcp_in.d \
./lwip/src/core/tcp_out.d \
./lwip/src/core/timeouts.d \
./lwip/src/core/udp.d 

OBJS += \
./lwip/src/core/altcp.o \
./lwip/src/core/altcp_alloc.o \
./lwip/src/core/altcp_tcp.o \
./lwip/src/core/def.o \
./lwip/src/core/dns.o \
./lwip/src/core/inet_chksum.o \
./lwip/src/core/init.o \
./lwip/src/core/ip.o \
./lwip/src/core/mem.o \
./lwip/src/core/memp.o \
./lwip/src/core/netif.o \
./lwip/src/core/pbuf.o \
./lwip/src/core/raw.o \
./lwip/src/core/stats.o \
./lwip/src/core/sys.o \
./lwip/src/core/tcp.o \
./lwip/src/core/tcp_in.o \
./lwip/src/core/tcp_out.o \
./lwip/src/core/timeouts.o \
./lwip/src/core/udp.o 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/%.o: ../lwip/src/core/%.c lwip/src/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MCXN947VDF -DCPU_MCXN947VDF_cm33 -DCPU_MCXN947VDF_cm33_core0 -DMCUXPRESSO_SDK -DPRINTF_ADVANCED_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DMCXN947_cm33_core0_SERIES -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/drivers/flash" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/CMSIS/m-profile" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/device/periph" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/str" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/silicon_id" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/utilities/debug_console_lite" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/gpio" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/uart" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/component/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/phy" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/include" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/port/sys_arch/dynamic" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/lwip/src/apps/http" -I"/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/board" -O0 -fno-common -g3 -gdwarf-4 -mcpu=cortex-m33 -c -ffunction-sections -fdata-sections -fno-builtin -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcux_config.h" -imacros "/home/freak/Documents/MCUXpresso_25.6.136/workspace/frdmmcxn947_lwip_httpsrv_bm_cm33_core0/source/mcuxsdk_version.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lwip-2f-src-2f-core

clean-lwip-2f-src-2f-core:
	-$(RM) ./lwip/src/core/altcp.d ./lwip/src/core/altcp.o ./lwip/src/core/altcp_alloc.d ./lwip/src/core/altcp_alloc.o ./lwip/src/core/altcp_tcp.d ./lwip/src/core/altcp_tcp.o ./lwip/src/core/def.d ./lwip/src/core/def.o ./lwip/src/core/dns.d ./lwip/src/core/dns.o ./lwip/src/core/inet_chksum.d ./lwip/src/core/inet_chksum.o ./lwip/src/core/init.d ./lwip/src/core/init.o ./lwip/src/core/ip.d ./lwip/src/core/ip.o ./lwip/src/core/mem.d ./lwip/src/core/mem.o ./lwip/src/core/memp.d ./lwip/src/core/memp.o ./lwip/src/core/netif.d ./lwip/src/core/netif.o ./lwip/src/core/pbuf.d ./lwip/src/core/pbuf.o ./lwip/src/core/raw.d ./lwip/src/core/raw.o ./lwip/src/core/stats.d ./lwip/src/core/stats.o ./lwip/src/core/sys.d ./lwip/src/core/sys.o ./lwip/src/core/tcp.d ./lwip/src/core/tcp.o ./lwip/src/core/tcp_in.d ./lwip/src/core/tcp_in.o ./lwip/src/core/tcp_out.d ./lwip/src/core/tcp_out.o ./lwip/src/core/timeouts.d ./lwip/src/core/timeouts.o ./lwip/src/core/udp.d ./lwip/src/core/udp.o

.PHONY: clean-lwip-2f-src-2f-core

