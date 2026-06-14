#include "util/hash.hpp"

uint64_t hash_string(const std::string& s, uint64_t seed)
{
    return fnv1a_64(s.data(), s.size(), seed);
}