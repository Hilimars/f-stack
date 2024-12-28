export RTE_SDK=/data/f-stack/dpdk
export RTE_TARGET=x86_64-native-linuxapp-gcc

git clone https://github.com/ansyun/dpdk-ans.git
git clone https://github.com/ansyun/dpdk-redis.git
export RTE_ANS=/data/f-stack/dpdk-ans
cd /data/f-stack/dpdk-ans
./install_deps.sh
cd ans
make
./build/ans -c 0x2 -n 1  -- -p 0x1 --config="(0,0,1)"

cd ../dpdk-redis
make
./src/redis-server  redis.conf
