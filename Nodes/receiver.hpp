#ifndef __RECEIVER_HPP
#define __RECEIVER_HPP

#include "generic_raw_node.hpp"

/**
 * A node that can be used to receiver raw packets from 
*/

template<typename packet_type = generic_packet_header_t>
class Receiver : public GenericRAWNode<packet_type> {
public:

    Receiver (char IF_name[]) : GenericRAWNode<packet_type>(IF_name) {
        while(true) {
            receive_msg();
        }
    }

    void receive_msg() {
        std::cout << "Waiting to receive a msg." << std::endl;
        ssize_t rec_size;
        if ((rec_size = recvfrom(this->socked_id, receive_buffer, (MAXPAYLOAD + sizeof(packet_type)),0,
                     NULL, NULL)) < 0) 
                     {
                        perror("Error: could not receive packet!");
                     } else {
                        std::cout << "Msg size: " << rec_size << std::endl;
                        std::cout << "Msg content: " << std::endl;
                        for (size_t i = 0; i < rec_size; i++) {
                            printf("%X ", (uint8_t) receive_buffer[i]);
                        }
                        std::cout << std::endl;
                     };
        std::cout << "Finished receiving msg." << std::endl;
    }

    void set_rcv_pckt_type(int pckt_type) {
        std::cout << "Rcv packet type: " << pckt_type << std::endl;
        this->socket_address.sll_pkttype = pckt_type;
        bind(this->socked_id, (sockaddr *) &this->socket_address, sizeof(sockaddr_ll));
    }   

protected:
    char receive_buffer[MAXPAYLOAD + sizeof(packet_type)];
    packet_type * const packet_header = (packet_type *) receive_buffer;
    const int start_payload = sizeof(packet_type);
    unsigned int sockaddr_length = sizeof(sockaddr_ll);

};

#endif // __RECEIVER_HPP