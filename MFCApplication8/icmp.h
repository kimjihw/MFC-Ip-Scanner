#pragma once
#include <winsock2.h>
#include "mac.h"
#include "ip.h"
#include "pch.h"
#include <pcap.h>

#pragma pack(push,1)
struct ethernet_header {
    Mac      dmac_;
    Mac      smac_;
    uint16_t type_;

    Mac      dmac() { return dmac_; }
    Mac      smac() { return smac_; }
    uint16_t type() { return ntohs(type_); }

    typedef enum {
        Ip4 = 0x0800,
        Arp = 0x0806,
        Ip6 = 0x86DD
    } Type;
};
#pragma pack(pop)

#pragma pack(push,1)
struct ip_header {
    u_int8_t ip_hl;
    u_int8_t ip_tos;
    u_int16_t ip_len;
    u_int16_t ip_id;
    uint16_t ip_off;
    uint8_t ip_ttl;
    uint8_t ip_pro;
    uint16_t ip_chk;
    Ip sip;
    Ip dip;
};
#pragma pack(pop)

#pragma pack(push,1)
struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t id;
    uint16_t seq;
};
#pragma pack(pop)


#pragma pack(push,1)
struct ping_ {
    ethernet_header eth_;
    ip_header iph_;
    icmp_header icmp_;
};
#pragma pack(pop)