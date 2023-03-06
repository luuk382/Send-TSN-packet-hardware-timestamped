#include "Nodes/general_node.hpp"

#include <iostream>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <unistd.h>

#include "Transmitter/transmitter.hpp"

#define PAYLOAD_SIZE 64

#define VLAN

#ifdef VLAN
    #define PACKET_TYPE vlan_packet_header_t
#else
    #define PACKET_TYPE packet_header_t
#endif


int main(int argc, char* argv[]) {

    int raw_socket; // the type of socket to be used
    char sendbuffer[PAYLOAD_SIZE + sizeof(PACKET_TYPE)]; // the buffer to store the to be send packet in
    PACKET_TYPE * packet_header = (PACKET_TYPE *) sendbuffer;
    char if_name[IFNAMSIZ]; // The interface name
    sockaddr_ll socket_address; // The address of the socket when sending the data

    if ((raw_socket = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL)) == -1) {
        perror("Error could not open socket!");
        return -1;
    }

    if (argc > 1) {
        strcpy(if_name, argv[1]);
    } else {
        perror("Error: no interface was specified!");
        return -1;
    }

    Transmitter<> test = Transmitter<>(if_name);
    test.test();


    ifreq ifr={0};
    size_t if_name_len=strlen(if_name);
    if (if_name_len<sizeof(ifr.ifr_name)) {
        memcpy(ifr.ifr_name,if_name,if_name_len);
        ifr.ifr_name[if_name_len]=0;
    } else {
        perror("interface name is too long");
        return -1;
    }

    printf("Interface name: %s\n", (char*) ifr.ifr_name);

    // receive the socket index
    if (ioctl(raw_socket,SIOCGIFINDEX,&ifr) == -1) {
        perror(strerror(errno));
        return -1;
    }

    printf("ifr_index: %d \n", ifr.ifr_ifindex);
    socket_address.sll_ifindex = ifr.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;

    // receive the socket address4
    memset(&ifr, 0, sizeof(ifreq));
    strncpy(ifr.ifr_name, if_name, IFNAMSIZ-1);
    if (ioctl(raw_socket, SIOCGIFHWADDR, &ifr) == -1) {
        perror(strerror(errno));
        return -1;
    }

    printf("MAC_addres: ");

    // Write the correct headers
    for (size_t i = 0; i < 6; i++){
        printf("%X:",((uint8_t *) &ifr.ifr_ifru.ifru_hwaddr.sa_data)[i]);
        packet_header->src_mac[i] = ((uint8_t *) &ifr.ifr_ifru.ifru_hwaddr.sa_data)[i];
        packet_header->dst_mac[i] = 0xFF;
        socket_address.sll_addr[i] = 0xFF;
    }
    printf("\n");

    #ifdef VLAN
        packet_header->TPID = htons(0x8100);
        packet_header->vlan_field = {};
        packet_header->vlan_field.VID = 7;
        printf("VLAN_ID: %d \n", packet_header->vlan_field.VID);
        uint16_t network_vlan_field = htons(*reinterpret_cast<uint16_t*>(&packet_header->vlan_field));
        packet_header->vlan_field = * reinterpret_cast<vlan_field_t*>(&network_vlan_field);    
        printf("Packet type %d\n", packet_header->TPID);
    #else
        packet_header->ethertype = htons(0x0800);
    #endif

    for (size_t i = sizeof(vlan_packet_header_t); i < (PAYLOAD_SIZE+sizeof(vlan_packet_header_t)); i++) {
        sendbuffer[i] = 0xff;
    }    

    if (sendto(raw_socket, sendbuffer, sizeof(sendbuffer), 0, (sockaddr *)&socket_address, sizeof(sockaddr_ll)) < 0) {
        perror("Error: failed to send the packet!");
        return -1;
    }

    close(raw_socket);
    return 0;
}