#ifndef __GENERAL_NODE_HPP
#define __GENERAL_NODE_HPP

#include <net/if.h>

#include "packet_headers.hpp"

#define MAXPAYLOAD 1500

/**
 * A general node inside the network, this is used as a template for
 * the transmitter and receiver classes. This class is meant for nodes
 * that need to send and receive RAW packets.
*/
template<typename packet_type = generic_packet_header_t>
class GeneralRAWNode {
public:
    /**
     * Initialize the class
    */
    GeneralRAWNode(char []);

protected:

    /** 
     * Create the socket for the node.
    */
    int CreateSocket();

    int socked_id; // The id of the socket interface that will be created
    char if_name[IFNAMSIZ]; // The name of the interface that this node will send to

};

#endif // __GENERAL_NODE_HPP