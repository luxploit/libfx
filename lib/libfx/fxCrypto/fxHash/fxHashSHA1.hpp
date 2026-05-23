#pragma once

#include <libfx/fxCrypto/fxHash.hpp>

#ifndef FX_DISABLE_CRYPTO

// SHA1 Hash - 160-bit
struct fxHashSHA1 : fxHash<20> {
	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept override {
		std::uint8_t hash[20];

		hash_state state;
		sha1_init(&state);
		sha1_process(&state, data, len);
		sha1_done(&state, hash);

		std::string str(FX_PTR_AS(const char*, hash), sizeof(hash));
		return fxCryptoUtils::get()->dump_hex(str);
	}
};

#endif