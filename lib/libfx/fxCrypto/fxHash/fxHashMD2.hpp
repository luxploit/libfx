#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// MD2 Hash - 128-bit
struct fxHashMD2 : fxHash<16> {
	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint8_t hash[16];

		hash_state state;
		md2_init(&state);
		md2_process(&state, data, len);
		md2_done(&state, hash);

		std::string str(FX_PTR_AS(const char*, hash), sizeof(hash));
		return fxCryptoUtils::get()->dump_hex(str);
	}
};

#endif