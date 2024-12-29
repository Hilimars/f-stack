#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_mbuf.h>

#define BURST_SIZE 32
#define OUTPUT_FILE "packet_data.txt"

// 解析数据包
static void parse_packet(struct rte_mbuf *mbuf, FILE *output_file) {
    struct rte_ether_hdr *eth_hdr;
    struct rte_ipv4_hdr *ip_hdr;

    // 确保数据包长度足够
    if (mbuf->pkt_len < sizeof(struct rte_ether_hdr)) {
        fprintf(stderr, "Packet too short for Ethernet header\n");
        return;
    }

    eth_hdr = rte_pktmbuf_mtod(mbuf, struct rte_ether_hdr *);
    fprintf(output_file, "Ethernet Header:\n");
    fprintf(output_file, "  Src MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth_hdr->src_addr.addr_bytes[0], eth_hdr->src_addr.addr_bytes[1],
            eth_hdr->src_addr.addr_bytes[2], eth_hdr->src_addr.addr_bytes[3],
            eth_hdr->src_addr.addr_bytes[4], eth_hdr->src_addr.addr_bytes[5]);
    fprintf(output_file, "  Dst MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
            eth_hdr->dst_addr.addr_bytes[0], eth_hdr->dst_addr.addr_bytes[1],
            eth_hdr->dst_addr.addr_bytes[2], eth_hdr->dst_addr.addr_bytes[3],
            eth_hdr->dst_addr.addr_bytes[4], eth_hdr->dst_addr.addr_bytes[5]);

    // 解析 IPv4 数据包
    if (mbuf->pkt_len >= sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr)) {
        ip_hdr = (struct rte_ipv4_hdr *)(eth_hdr + 1);
        fprintf(output_file, "IPv4 Header:\n");
        fprintf(output_file, "  Src IP: %u.%u.%u.%u\n",
                (ip_hdr->src_addr) & 0xFF, (ip_hdr->src_addr >> 8) & 0xFF,
                (ip_hdr->src_addr >> 16) & 0xFF, (ip_hdr->src_addr >> 24) & 0xFF);
        fprintf(output_file, "  Dst IP: %u.%u.%u.%u\n",
                (ip_hdr->dst_addr) & 0xFF, (ip_hdr->dst_addr >> 8) & 0xFF,
                (ip_hdr->dst_addr >> 16) & 0xFF, (ip_hdr->dst_addr >> 24) & 0xFF);
    }
}

int main(int argc, char **argv) {
    struct rte_mempool *mbuf_pool;
    uint16_t portid = 0;

    // 初始化 DPDK 环境
    if (rte_eal_init(argc, argv) < 0)
        rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

    // 创建内存池
    mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", 8192, 256, 0,
                                        RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());
    if (mbuf_pool == NULL)
        rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

    // 启动网卡
    if (rte_eth_dev_start(portid) != 0)
        rte_exit(EXIT_FAILURE, "Error starting Ethernet device\n");

    struct rte_mbuf *bufs[BURST_SIZE];
    FILE *output_file = fopen(OUTPUT_FILE, "w");

    if (!output_file)
        rte_exit(EXIT_FAILURE, "Error opening output file\n");

    // 数据包处理循环
    while (1) {
        const uint16_t nb_rx = rte_eth_rx_burst(portid, 0, bufs, BURST_SIZE);
        for (int i = 0; i < nb_rx; i++) {
            parse_packet(bufs[i], output_file);
            rte_pktmbuf_free(bufs[i]);
        }
        fflush(output_file);
    }

    fclose(output_file);
    rte_eth_dev_stop(portid);
    return 0;
}


