#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// Tiger Hash - 192-bit
struct fxHashTiger : fxHash<24> {
	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint8_t hash[24];

		hash_state state;
		tiger_init(&state);
		tiger_process(&state, data, len);
		tiger_done(&state, hash);

		std::string str(FX_PTR_AS(const char*, hash), sizeof(hash));
		return fxCryptoUtils::get()->dump_hex(str);
	}
};

#endif