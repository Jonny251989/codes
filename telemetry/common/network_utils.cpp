#include "network_utils.hpp"

uint64_t host_to_network_uint64(uint64_t value) {
    return __builtin_bswap64(value);
}

uint64_t network_to_host_uint64(uint64_t value) {
    return __builtin_bswap64(value);
}