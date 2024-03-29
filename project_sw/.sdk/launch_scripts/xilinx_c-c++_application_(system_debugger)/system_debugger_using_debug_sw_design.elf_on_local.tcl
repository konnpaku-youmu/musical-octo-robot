connect -url tcp:127.0.0.1:3121
source /users/students/r0822930/Downloads/sw_package_2021/sw_package_pynq_2/musical-octo-robot/project_sw/hw_platform/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Xilinx TUL 1234-tulA"} -index 0
loadhw -hw /users/students/r0822930/Downloads/sw_package_2021/sw_package_pynq_2/musical-octo-robot/project_sw/hw_platform/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Xilinx TUL 1234-tulA"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Xilinx TUL 1234-tulA"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Xilinx TUL 1234-tulA"} -index 0
dow /users/students/r0822930/Downloads/sw_package_2021/sw_package_pynq_2/musical-octo-robot/project_sw/sw_design/Debug/sw_design.elf
configparams force-mem-access 0
bpadd -addr &main
