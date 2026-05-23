#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// CRC16 Hash - 16-bit
struct fxHashCRC16 : fxHash16 {
	[[nodiscard]] virtual std::uint16_t hash(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint16_t crc = 0xFFFF;

        for (size_t i = 0; i < len; ++i) {
            crc ^= (std::uint16_t)data[i] << 8;

            for (int j = 0; j < 8; ++j) {
                if (crc & 0x8000) {
					crc = (crc << 1) ^ 0x1021;
				} else {
					crc <<= 1;
				}
            }
        }

        return crc;
	}
};

#endif