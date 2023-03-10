#include <iostream>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <unistd.h>

#include "Nodes/transmitter.hpp"
#include "Nodes/receiver.hpp"

#define PAYLOAD_SIZE 64

#define VLAN

#ifdef VLAN
    #define PACKET_TYPE vlan_packet_header_t
#else
    #define PACKET_TYPE packet_header_t
#endif


int main(int argc, char* argv[]) {

    char if_name[IFNAMSIZ]; // The interface name

    if (argc > 1) {
        strcpy(if_name, argv[1]);
    } else {
        perror("Error: no interface was specified!");
        return -1;
    }

    // Receiver<vlan_packet_header_t> test = Receiver<vlan_packet_header_t>(if_name);

    Transmitter<vlan_packet_header_t> test = Transmitter<vlan_packet_header_t>(if_name);
    // test.change_vlan_field(5, 0, 0);
    //char test_payload[] = "Some test message";
    //test.change_payload((uint8_t*) test_payload, sizeof(test_payload));

    while(true) {
        sleep(1);
        test.send_packet();
    }

    return 0;
}