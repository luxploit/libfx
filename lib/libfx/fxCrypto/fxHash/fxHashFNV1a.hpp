#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// FNV1a Hash - 64-bit
struct fxHashFNV1a : public fxHash64 {
	[[nodiscard]] virtual std::uint64_t hash(const std::uint8_t* data, const std::size_t len) const noexcept override {
        std::uint64_t hash = 0xCBF29CE484222325ULL;  // FNV1a 64-bit offset
        const std::uint64_t prime = 0x100000001B3ULL;  // FNV1a 64-bit prime

        for (size_t i = 0; i < len; ++i) {
            hash ^= data[i];
            hash *= prime;
        }

        return hash;
    }
};

#endif