import os
import sys

f_src = open("src.mk","w")
f_path = open("cpppath.mk","w")

# 包含头文件的文件夹路径列表
ycm_flags = []

f_src.write('C_SOURCES = \\\n')
f_path.write('C_INCLUDES = \\\n')

# 不使用的文件夹路径列表
# 前5个路径是需要手动处理的路径
remove_dir = ['.','HALLIB/Src','HALLIB/Src/Legacy','build', \
            'board/ports','board/linker_scripts','rt-thread/components/cplusplus', \
            'rt-thread/components/drivers/audio','rt-thread/components/drivers/can', \
            'rt-thread/components/drivers/cputime','rt-thread/components/drivers/hwcrypto', \
            'rt-thread/components/drivers/hwtimer','rt-thread/components/drivers/i2c', \
            'rt-thread/components/drivers/misc','rt-thread/components/drivers/mtd', \
            'rt-thread/components/drivers/pm','rt-thread/components/drivers/rtc', \
            'rt-thread/components/drivers/sdio','rt-thread/components/drivers/sensors', \
            'rt-thread/components/drivers/serial','rt-thread/components/drivers/spi', \
            'rt-thread/components/drivers/src','rt-thread/components/drivers/touch', \
            'rt-thread/components/drivers/usb','rt-thread/components/drivers/watchdog', \
            'rt-thread/components/drivers/wlan', \
            'rt-thread/components/dfs','rt-thread/components/dfs', \
            'rt-thread/components/lwp','rt-thread/components/net',\
            'rt-thread/components/utilities','rt-thread/components/vbus',\
            'rt-thread/components/vmm','rt-thread/libcpu/arm/cortex-a',\
            'rt-thread/libcpu/arm/cortex-m0','rt-thread/libcpu/arm/cortex-m3',\
            'rt-thread/libcpu/arm/cortex-m7', 'hardware/HAL_Drivers']

# 从第四个元素开始将所有包含的子目录路径加入到不使用的文件夹路径列表中
for m in remove_dir[4:] :
    for a,b,c in os.walk(m) :
        remove_dir.append(a)

# 遍历所有目录和子目录，并判断出包含头文件的文件夹路径和'.c'文件的全路径加文件名
for root,dirs,files in os.walk(os.getcwd()):
    root = os.path.relpath(root,os.getcwd())        # 从当前路径开始计算路径
    if not root in remove_dir :                     # 判断不在不使用文件夹路径中的文件，即正常使用中的文件位置
        for file in files :                         # 遍历所有文件
            if os.path.splitext(file)[1] == '.c' :  # 如果文件是'.c'文件，则将文件路径加文件名写入到src.mk文件中
                f_src.writelines(os.path.join(root, file) + ' \\\n')
            elif os.path.splitext(file)[1] == '.h' :#如果文件是头文件，则将所在路径加入到头文件所在文件夹列表中
                if not root in ycm_flags :          #头文件路径在ycm_flags列表中没有才加入ycm_flags，避免重复加入头文件夹列表
                    ycm_flags.append(root)

#根据需要增加 的C文件和 HAL库的 C文件
ycm_flags.append('hardware/HAL_Drivers')
add_sources = '''\
hardware/HAL_Drivers/drv_gpio.c \\
rt-thread/components/drivers/misc/pin.c \\

include hal_src.mk

'''
f_src.write(add_sources)

#启动文件路径和其它汇编文件（线程切换文件）
startup_path = 'rt-thread/libcpu/arm/cortex-m4/context_gcc.s \\\n'
asm_sources = 'CORE/startup_stm32f429xx.s'
f_src.write('\n\nASM_SOURCES =  \\\n' + startup_path + asm_sources)

#GCC编译器的路径（可以通过此选项进行指定版本GCC进行编译，一般用于系统中存在多个版本的GCC）
gcc_path = 'GCC_PATH = /opt/gcc-arm-none-eabi-6-2017-q2-update/bin'
f_src.write('\n\n\n' + gcc_path + '\n\n\n')

#C定义
c_define = '''

# C defines
C_DEFS =  \\
-DUSE_HAL_DRIVER  \\
-DSTM32F429xx \\
-DHAVE_CCONFIG_H \\
-DRT_USING_MINILIBC  \\
-DRT_USING_NEWLIB


# AS incluedes
AS_INCLUDES =


#以下是YCM配置文件需要复制的FLAG内容
'''
for a in ycm_flags :
    f_path.write('-I' + a + ' \\\n')
f_path.write(c_define)
for a in ycm_flags :
    f_path.write("#'-I',\n#'" + a + "',\n")

f_src.close()
f_path.close()

#在当前目录下生成下载脚本
shell_file = '''\
#!/bin/bash 
JLinkExe -CommandFile jlink.conf
'''
with open('download.sh','w') as sh :
    sh.writelines(shell_file)

#JLink下载配置文件内容前半部分    
jlink_conf_cache = '''\
si SWD 
Device STM32F429IG
Speed 4000
loadbin build/\
'''
#JLink下载配置文件后半部分
jlink_conf_cache1 = '''\
.bin,0x8000000
r
q

'''

#读取Makefile文件
with open('Makefile','r') as make_file :
    makefile_cache = make_file.readlines()

#提取读出的Makefile文件中的TARGET以拼接成需要烧录的文件全名
with open('jlink.conf','w') as jlink_conf :
    jlink_conf.writelines(jlink_conf_cache +  makefile_cache[4].strip() + jlink_conf_cache1 )


