C_INCLUDES = \
-Iboard \
-ICORE \
-IHALLIB/Inc \
-IHALLIB/Inc/Legacy \
-Ihardware/beep \
-Ihardware/button \
-Ihardware/decode \
-Ihardware/key \
-Ihardware/LCD \
-Ihardware/LED \
-Ihardware/pcf8574 \
-Ihardware/player \
-Irt-thread/components/drivers/include \
-Irt-thread/components/drivers/include/drivers \
-Irt-thread/components/drivers/include/ipc \
-Irt-thread/components/finsh \
-Irt-thread/include \
-Irt-thread/include/libc \
-ISYSTEM/delay \
-ISYSTEM/sys \
-IUSER \
-Ihardware/HAL_Drivers \
-Ihardware/HAL_Drivers/config/f4 \


# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER  \
-DSTM32F429xx \
-DHAVE_CCONFIG_H \
-DRT_USING_MINILIBC  \
-DRT_USING_NEWLIB


# AS incluedes
AS_INCLUDES =


#以下是YCM配置文件需要复制的FLAG内容
#'-I',
#'board',
#'-I',
#'CORE',
#'-I',
#'HALLIB/Inc',
#'-I',
#'HALLIB/Inc/Legacy',
#'-I',
#'hardware/beep',
#'-I',
#'hardware/button',
#'-I',
#'hardware/decode',
#'-I',
#'hardware/key',
#'-I',
#'hardware/LCD',
#'-I',
#'hardware/LED',
#'-I',
#'hardware/pcf8574',
#'-I',
#'hardware/player',
#'-I',
#'rt-thread/components/drivers/include',
#'-I',
#'rt-thread/components/drivers/include/drivers',
#'-I',
#'rt-thread/components/drivers/include/ipc',
#'-I',
#'rt-thread/components/finsh',
#'-I',
#'rt-thread/include',
#'-I',
#'rt-thread/include/libc',
#'-I',
#'SYSTEM/delay',
#'-I',
#'SYSTEM/sys',
#'-I',
#'USER',
#'-I',
#'hardware/HAL_Drivers',
#'-I',
#'hardware/HAL_Drivers/config/f4',
