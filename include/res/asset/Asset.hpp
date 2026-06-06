#ifndef ASSET_HPP
#define ASSET_HPP

#include <string>
#include <cstdint>
#include <iosfwd>

#define ASSETS_CONSTRUCTORS(Asset)           \
Asset(std::ifstream &file);                  \
Asset() = delete;                            \
Asset(Asset &) = delete;                     \
Asset &operator=(const Asset &) = delete;    \
Asset(Asset &&other) noexcept;               \
Asset &operator=(Asset &&other) noexcept;    \
~Asset();                                    \
static std::string get_type_name() noexcept; \
static uint32_t get_type_size() noexcept

#endif
