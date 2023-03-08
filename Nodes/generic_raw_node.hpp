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
            std::cout << "A socket id: " << socked_id << std::endl;
        };
    }

protected:

    /** 
     * Create the socket for the node.
    */
    int CreateSocket() {
        if ((this->socked_id = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL)) == -1) {
            perror("Error could not open socket!");
            return -1;
        }
        return 0;
    }

    int socked_id; // The id of the socket interface that will be created
    char if_name[IFNAMSIZ]; // The name of the interface that this node will send to

};

#endif // __GENERAL_NODE_HPP