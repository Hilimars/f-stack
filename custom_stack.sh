# initialize dpdk
# 绑定网卡
$ ip link set ens33 down
$ dpdk-devbind.py -b igb_uio ens33
$ ./usertools/dpdk-devbind.py –status	//查看网卡是否绑定
# 编译运行
$ cd dpdk-22.07/examples/custom_stack
$ make
$ ./build/dpdk_custom_stack
