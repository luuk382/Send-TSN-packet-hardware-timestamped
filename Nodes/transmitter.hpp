#ifndef __TRANSMITTER_HPP
#define __TRANSMITTER_HPP

#include "generic_raw_node.hpp"

#include <iostream>
#include "sys/socket.h"

/**
 * A node that can be used to transmit packages of packet_type over the network
*/
template<typename packet_type = generic_packet_header_t>
class Transmitter : public GenericRAWNode<packet_type> {

public:
    Transmitter(char if_name[]) : GenericRAWNode<packet_type>(if_name) {
        std::cout << "A transmitter was created to send packets of type: " << typeid(packet_type).name() << std::endl;
        initialize_eth_header();
        uint8_t initial_payload[64];
        for (size_t i = 0; i < 64; i++) {
            initial_payload[i] = 0xff;
        }
        change_payload(initial_payload, 64);
    }    

    void set_dst_mac_address(int dest[ETH_ALEN]) {
        std::cout << "Dst mac: ";
        for(size_t i = 0; i < ETH_ALEN; i++) {
            printf("%X:", dest[i]);
            this->socket_address.sll_addr[i] = dest[i];
            packet_header->dst_mac[i] = dest[i];
        }
        std::cout << std::endl;
    }    

    void send_packet() {
        if (sendto(this->socked_id, send_buffer, (payload_size + sizeof(packet_type)), 0, (sockaddr *)&this->socket_address, sizeof(sockaddr_ll)) < 0) {
            perror("Error: failed to send the packet!");
        }
    }

    void change_payload(uint8_t payload[], int size) {
        if (size > MAXPAYLOAD) {
            throw std::runtime_error("The payload size was to large");
        }
        int start_payload = sizeof(packet_type);
        for (size_t i = 0; i < size; i++) {
            send_buffer[i + start_payload] = payload[i];
        }
        if (size < 64) {
            payload_size = 64;
            for (size_t i = size; i < 64; i++) {
                send_buffer[i + start_payload] = 0x00;
            }
        } else {
            payload_size = size;
        }
    }

private: 

    void initialize_eth_header() {
        this->initialize_eth_src_mac_address(packet_header->src_mac);
        int dst_mac[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        set_dst_mac_address(dst_mac);
        packet_header->ethertype = htons(0x0800);
    }

protected:
    char send_buffer[MAXPAYLOAD+sizeof(packet_type)]; // The buffer in which a message will be stored before it is send
    int payload_size; // The size of the payload;
    packet_type * const packet_header = (packet_type *) send_buffer; // The header for the message to be send
};

/**
 * A node that can be used to transmit packages of vlan_packet_header_t over the network
*/
template<>
class Transmitter<vlan_packet_header_t> : public GenericRAWNode<vlan_packet_header_t> {

public:
    Transmitter(char if_name[]) : GenericRAWNode<vlan_packet_header_t>(if_name) {
        std::cout << "A transmitter was created to send packets of type: " << typeid(vlan_packet_header_t).name() << std::endl;
        initialize_eth_header();
        uint8_t initial_payload[64];
        for (size_t i = 0; i < 64; i++) {
            initial_payload[i] = 0xff;
        }
        change_payload(initial_payload, 64);
    }    

    void set_dst_mac_address(int dest[ETH_ALEN]) {
        std::cout << "Dst mac: ";
        for(size_t i = 0; i < ETH_ALEN; i++) {
            printf("%X:", dest[i]);
            this->socket_address.sll_addr[i] = dest[i];
            packet_header->dst_mac[i] = dest[i];
        }
        std::cout << std::endl;
    }    

    void send_packet() {
        if (sendto(this->socked_id, send_buffer, (payload_size + sizeof(vlan_packet_header_t)), 0, (sockaddr *)&this->socket_address, sizeof(sockaddr_ll)) < 0) {
            perror("Error: failed to send the packet!");
        }
    }

    void change_payload(uint8_t payload[], int size) {
        if (size > MAXPAYLOAD) {
            throw std::runtime_error("The payload size was to large");
        }
        int start_payload = sizeof(vlan_packet_header_t);
        for (size_t i = 0; i < size; i++) {
            send_buffer[i + start_payload] = payload[i];
        }
        if (size < 64) {
            payload_size = 64;
            for (size_t i = size; i < 64; i++) {
                send_buffer[i + start_payload] = 0xff;
            }
        } else {
            payload_size = size;
        }
    }

    void change_vlan_field(int vlan_id, int dei, int pcp) {
        std::cout << "VLAN_id: " << vlan_id << ", DEI: " << dei << ", PCP: " << pcp << std::endl;
        packet_header->vlan_field.VID = vlan_id;
        packet_header->vlan_field.DEI = dei; 
        packet_header->vlan_field.PCP = pcp;
        uint16_t network_vlan_field = htons(*reinterpret_cast<uint16_t*>(&packet_header->vlan_field));
        packet_header->vlan_field = * reinterpret_cast<vlan_field_t*>(&network_vlan_field);  
        packet_header->size = htons(0x0000);
    }

private: 

    void initialize_eth_header() {
        initialize_eth_src_mac_address(packet_header->src_mac);
        int dst_mac[ETH_ALEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
        set_dst_mac_address(dst_mac);
        packet_header->TPID = htons(0x8100);
        packet_header->size = htons(0);
        change_vlan_field(0, 0, 0);
    }

protected:
    char send_buffer[MAXPAYLOAD+sizeof(vlan_packet_header_t)]; // The buffer in which a message will be stored before it is send
    int payload_size; // The size of the payload;
    vlan_packet_header_t * const packet_header = (vlan_packet_header_t *) send_buffer; // The header for the message to be send
};

#endif // __TRANSMITTER_HPP
