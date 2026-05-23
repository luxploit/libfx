#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// DJB2 Hash - 32-bit
struct fxHashDJB2 : fxHash32 {
	[[nodiscard]] virtual std::uint32_t hash(const std::uint8_t* data, const std::size_t len) const noexcept override {
        std::uint32_t hash = 5381;
        for (size_t i = 0; i < len; ++i) {
            hash = ((hash << 5) + hash) + data[i];  // hash * 33 + c
        }
        return hash;
    }
};

#endif