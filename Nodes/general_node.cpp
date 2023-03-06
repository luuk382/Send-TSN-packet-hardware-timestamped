#include "general_node.hpp"

template<typename packet_type>
GeneralRAWNode<packet_type>::GeneralRAWNode(char IF_name[]) : if_name{IF_name}{
    if (this->CreateSocket() == -1) {
        return -1;
    };
}

template<typename packet_type>
int GeneralRAWNode<packet_type>::CreateSocket() {
    if ((this->socked_id = socket(AF_PACKET, SOCK_RAW, ETH_P_ALL)) == -1) {
        perror("Error could not open socket!");
        return -1;
    }
}