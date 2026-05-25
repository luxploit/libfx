#pragma once

#include <libfx/fxCore/fxLogging.hpp>
#include <libfx/fxCore/fxMacros.hpp>

#include <vector>
#include <cstdint>
#include <cstring>

#ifndef FX_DISABLE_LZ4_COMPRESSION

#include <lz4.h>

struct fxCompression {

    [[nodiscard]] std::vector<std::uint8_t> compress(const std::uint8_t* data, const std::size_t len) noexcept;
    [[nodiscard]] std::vector<std::uint8_t> compress(const std::vector<std::uint8_t>& data) noexcept {
        return this->compress(data.data(), data.size());
    }

    [[nodiscard]] std::vector<std::uint8_t> decompress(const std::uint8_t* compressed, const std::size_t cp_len, const std::size_t og_len) noexcept;
    [[nodiscard]] std::vector<std::uint8_t> decompress(const std::vector<std::uint8_t>& compressed, const std::size_t og_len) noexcept {
        return this->decompress(compressed.data(), compressed.size(), og_len);
    }
};

#endif