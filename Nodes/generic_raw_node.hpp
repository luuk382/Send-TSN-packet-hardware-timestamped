#ifndef __GENERAL_NODE_HPP
#define __GENERAL_NODE_HPP

#include <net/if.h>
#include <stdexcept>
#include <net/ethernet.h>
#include <string.h>

#include "packet_headers.hpp"

#define MAXPAYLOAD 1500

/**
 * A general node inside the network, this is used as a template for
 * the transmitter and receiver classes. This class is meant for nodes
 * that need to send and receive RAW packets.
*/
template<typename packet_type = generic_packet_header_t>
class GenericRAWNode {
public:
    /**
     * Initialize the class
    */
    GenericRAWNode(char IF_name[]) {
        strcpy(if_name, IF_name);
        std::cout << "Interface name: " << if_name << std::endl;
        if (this->CreateSocket() == -1) {
            throw std::runtime_error("Failed to create the socket identifier.");
        } else {
            initialize_socket_address();
        };
    }

protected:

    /** 
     * Create the socket for the node.
    */
    int CreateSocket() {
        if ((socked_id = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
            perror("Error could not open socket!");
            return -1;
        }
        return 0;
    }

    /**
     * Bind the interface specified by the ifindex of the interface name, to
     * the socket created.
    */
    void initialize_socket_address() {
        socket_address.sll_ifindex = get_ifr_index();
        std::cout << "Binding socket with id: " << socked_id << " to interface id: " << socket_address.sll_ifindex << std::endl;
        socket_address.sll_halen = ETH_ALEN;
        bind(socked_id, (sockaddr *) &socket_address, sizeof(sockaddr_ll));
    }

    ifreq initialize_ifr() {
        ifreq ifr={0};
        size_t if_name_len=strlen(this->if_name);

        // check if the interface name isn't to long
        if (if_name_len<sizeof(ifr.ifr_name)) {
            memcpy(ifr.ifr_name,this->if_name,if_name_len);
            ifr.ifr_name[if_name_len]=0;
        } else {
            throw std::runtime_error("The interface name is too long");
        }
        return ifr;
    }

    int get_ifr_index() {
        ifreq ifr= initialize_ifr();

        // receive the socket index
        if (ioctl(this->socked_id,SIOCGIFINDEX,&ifr) == -1) {
            perror(strerror(errno));
        }
        return ifr.ifr_ifindex;
    }

    void initialize_eth_src_mac_address(uint8_t location[ETH_ALEN]) {
        ifreq ifr = this->initialize_ifr();

        // receive mac address of the interface
        if (ioctl(this->socked_id, SIOCGIFHWADDR, &ifr) == -1) {
            perror(strerror(errno));
        }
        std::cout << "Src MAC: ";
        for (size_t i = 0; i < ETH_ALEN; i++) {
            location[i] = ((uint8_t *) &ifr.ifr_ifru.ifru_hwaddr.sa_data)[i];
            printf("%X:", ((uint8_t *) &ifr.ifr_ifru.ifru_hwaddr.sa_data)[i]);
        }
        std::cout << std::endl;
    }

    int socked_id; // The id of the socket interface that will be created
    char if_name[IFNAMSIZ]; // The name of the interface that this node will send to
    sockaddr_ll socket_address; // The address of the socket

};

#endif // __GENERAL_NODE_HPP