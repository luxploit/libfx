#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// Jenkins on at a Time Hash - 32-bit
struct fxHashJenkins : fxHash32 {
	[[nodiscard]] virtual std::uint32_t hash(const std::uint8_t* data, const std::size_t len) const noexcept override {
        std::uint32_t hash = 0;

        for (size_t i = 0; i < len; ++i) {
            hash += data[i];
            hash += (hash << 10);
            hash ^= (hash >> 6);
        }

        hash += (hash << 3);
        hash ^= (hash >> 11);
        hash += (hash << 15);
        return hash;
    }
};

#endif