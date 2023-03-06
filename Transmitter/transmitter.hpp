#ifndef __TRANSMITTER_HPP
#define __TRANSMITTER_HPP

#include "../Nodes/general_node.hpp"

#include "sys/socket.h"

/**
 * A node that can be used to transmit packages over the network
*/
template<typename packet_type = generic_packet_header_t>
class Transmitter : public GeneralRAWNode<packet_type> {

public:
    Transmitter(char []);

    void test() {
        printf("%d", testing);
    }

protected:
    char send_buffer[MAXPAYLOAD+sizeof(packet_type)]; // The buffer in which a message will be stored before it is send
    packet_type * const header = (packet_type *) send_buffer; // The header for the message to be send
    int testing;

};

#endif // __TRANSMITTER_HPP
