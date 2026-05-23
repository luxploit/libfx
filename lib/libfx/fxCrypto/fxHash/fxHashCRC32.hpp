#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// CRC32 Hash - 32-bit
struct fxHashCRC32 : fxHash32 {
	[[nodiscard]] virtual std::uint32_t hash(const std::uint8_t* data, const std::size_t len) const noexcept override {
		crc32_state md;
		unsigned char out[4];
		std::uint32_t crc = 0;

		crc32_init(&md);
		crc32_update(&md, data, FX_VAL_AS(unsigned long, len));
		crc32_finish(&md, out, sizeof(uint32_t));

		// Convert result from 4 bytes (big endian) to uint32_t
		crc =
			(FX_VAL_AS(std::uint32_t, out[0]) << 24) |
			(FX_VAL_AS(std::uint32_t, out[1]) << 16) |
			(FX_VAL_AS(std::uint32_t, out[2]) << 8)  |
			(FX_VAL_AS(std::uint32_t, out[3]))
		;

		return crc;
	}
};

#endif