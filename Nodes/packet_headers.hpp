#ifndef __PACKET_HEADERS_HPP
#define __PACKET_HEADERS_HPP
/**
 * This file will contain structures for the supported packet types
*/

#include <stdint.h>

/**
 * The header for a vlan packet based on 
 * https://en.wikipedia.org/wiki/IEEE_802.1Q
*/
struct generic_packet_header_t {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
};

struct vlan_field_t {
    uint16_t VID : 12;
    uint16_t DEI : 1;
    uint16_t PCP : 3; 
};

/**
 * The header for a vlan packet based on 
 * https://en.wikipedia.org/wiki/IEEE_802.1Q
*/
struct vlan_packet_header_t {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t TPID;
    vlan_field_t vlan_field;
    uint16_t size;
};

#endif // __PACKET_HEADERS_HPP