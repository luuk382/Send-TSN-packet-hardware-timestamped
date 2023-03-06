#include "transmitter.hpp"

template<typename packet_type>
Transmitter<packet_type>::Transmitter(char if_name[]) : GeneralRAWNode<packet_type>(if_name) {}