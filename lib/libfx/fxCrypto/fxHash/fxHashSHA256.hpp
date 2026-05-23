#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// SHA256 Hash - 256-bit
struct fxHashSHA256 : fxHash<32> {
	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint8_t hash[32];

		hash_state state;
		sha256_init(&state);
		sha256_process(&state, data, len);
		sha256_done(&state, hash);

		std::string str(FX_PTR_AS(const char*, hash), sizeof(hash));
		return fxCryptoUtils::get()->dump_hex(str);
	}
};

#endif