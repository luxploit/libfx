#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// SHA512 Hash - 512-bit
struct fxHashSHA512 : fxHash<64> {
	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint8_t hash[64];

		hash_state state;
		sha512_init(&state);
		sha512_process(&state, data, len);
		sha512_done(&state, hash);

		std::string str(FX_PTR_AS(const char*, hash), sizeof(hash));
		return fxCryptoUtils::get()->dump_hex(str);
	}
};

#endif