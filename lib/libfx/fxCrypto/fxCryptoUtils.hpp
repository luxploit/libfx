#pragma once

#include <libfx/fxConfig.hpp>
#include <libfx/fxCore/fxSingleton.hpp>
#include <libfx/fxCore/fxMacros.hpp>

#include <istream>
#include <random>
#include <sstream>
#include <iomanip>

#ifndef FX_DISABLE_CRYPTO

#include "tomcrypt.h"

struct fxCryptoEntropyParams {
	std::string prng_name_;
	prng_state* prng_state_;
	std::function<void(const void*, const std::size_t)> prng_entropy_fn_;
};

struct fxCryptoUtils : fxSingleton<fxCryptoUtils> {
	void init_ltc() {
		ltc_mp = ltm_desc;
		register_all_ciphers();
		register_all_hashes();
		register_all_prngs();
	}

	std::string dump_hex(const std::string& data, const std::string& separator = "") {
		if (data.empty()) {
			return {};
		}

		const std::size_t hex_size = data.size() * 2;
		const std::size_t sep_size = separator.size() * (data.size() - 1);

		std::string result;
		result.reserve(hex_size + sep_size);

		for (std::size_t i = 0; i < data.size(); ++i) {
			if (i != 0 && !separator.empty()) {
				result.append(separator);
			}

			fmt::format_to(std::back_inserter(result), "{:02X}", FX_VAL_AS(unsigned char, data[i]));
		}

		return result;
	}

	template <typename T>
	std::string to_hex(T i) {
		static_assert(std::is_integral_v<T> || std::is_enum_v<T>, "to_hex requires an integral or enum type");
		using U = std::make_unsigned_t<std::underlying_type_t<T>>;

		return fmt::format("0x{:0{}x}", FX_VAL_AS(U, i), sizeof(T) * 2);
	}

	void generate_entropy(fxCryptoEntropyParams params) {
		auto id = find_prng(params.prng_name_.c_str());
		rng_make_prng(128, id, params.prng_state_, nullptr);

		int i[4]; // uninitialized data
		auto* i_ptr = &i;

		params.prng_entropy_fn_(&i, sizeof(i));
		params.prng_entropy_fn_(&i_ptr, sizeof(int*));

		auto t = time(nullptr);
		params.prng_entropy_fn_(&t, sizeof(t));

		std::random_device rd{};
		for (auto j = 0; j < 4; ++j) {
			const auto x = rd();
			params.prng_entropy_fn_(&x, sizeof(x));
		}

	}

	std::string get_tomcrypt_ec_string(int ec) {
		switch (ec) {
			case CRYPT_OK:                   return "CRYPT_OK";
			case CRYPT_ERROR:                return "CRYPT_ERROR";
			case CRYPT_NOP:                  return "CRYPT_NOP";
			case CRYPT_INVALID_KEYSIZE:      return "CRYPT_INVALID_KEYSIZE";
			case CRYPT_INVALID_ROUNDS:       return "CRYPT_INVALID_ROUNDS";
			case CRYPT_FAIL_TESTVECTOR:      return "CRYPT_FAIL_TESTVECTOR";
			case CRYPT_BUFFER_OVERFLOW:      return "CRYPT_BUFFER_OVERFLOW";
			case CRYPT_INVALID_PACKET:       return "CRYPT_INVALID_PACKET";
			case CRYPT_INVALID_PRNGSIZE:     return "CRYPT_INVALID_PRNGSIZE";
			case CRYPT_ERROR_READPRNG:       return "CRYPT_ERROR_READPRNG";
			case CRYPT_INVALID_CIPHER:       return "CRYPT_INVALID_CIPHER";
			case CRYPT_INVALID_HASH:         return "CRYPT_INVALID_HASH";
			case CRYPT_INVALID_PRNG:         return "CRYPT_INVALID_PRNG";
			case CRYPT_MEM:                  return "CRYPT_MEM";
			case CRYPT_PK_TYPE_MISMATCH:     return "CRYPT_PK_TYPE_MISMATCH";
			case CRYPT_PK_NOT_PRIVATE:       return "CRYPT_PK_NOT_PRIVATE";
			case CRYPT_INVALID_ARG:          return "CRYPT_INVALID_ARG";
			case CRYPT_FILE_NOTFOUND:        return "CRYPT_FILE_NOTFOUND";
			case CRYPT_PK_INVALID_TYPE:      return "CRYPT_PK_INVALID_TYPE";
			case CRYPT_OVERFLOW:             return "CRYPT_OVERFLOW";
			case CRYPT_PK_ASN1_ERROR:        return "CRYPT_PK_ASN1_ERROR";
			case CRYPT_INPUT_TOO_LONG:       return "CRYPT_INPUT_TOO_LONG";
			case CRYPT_PK_INVALID_SIZE:      return "CRYPT_PK_INVALID_SIZE";
			case CRYPT_INVALID_PRIME_SIZE:   return "CRYPT_INVALID_PRIME_SIZE";
			case CRYPT_PK_INVALID_PADDING:   return "CRYPT_PK_INVALID_PADDING";
			case CRYPT_HASH_OVERFLOW:        return "CRYPT_HASH_OVERFLOW";
			case CRYPT_PW_CTX_MISSING:       return "CRYPT_PW_CTX_MISSING";
			case CRYPT_UNKNOWN_PEM:          return "CRYPT_UNKNOWN_PEM";
			case CRYPT_ERR_NUM:              return "CRYPT_ERR_NUM";
			default:                         return "UNKNOWN_CRYPT_ERROR";
    	}
	}
};

#define FX_TOMCRYPT_ASSERT(errc, msg, def)                        \
	if (errc != CRYPT_OK) {                                    \
		fxPrintf(fxLogError, msg " error code: %s",            \
			fxCryptoUtils::get()->get_tomcrypt_ec_string(errc) \
		);                                                     \
		return def;                                            \
	}

#endif