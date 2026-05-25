#include <libfx/fxCrypto/fxCipher/fxCipherXSalsa20.hpp>

bool fxCipherXSalsa20::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen != 32) {
		fxPrintf(fxLogError, "XSalsa20 setup failed! keylen %zu is not 32", keylen);
		return false;
	}

	this->key_storage.assign(key, key + keylen);
	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherXSalsa20::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv) {
		fxPrintf(fxLogError, "XSalsa20 crypt failed! requirements not met");
		return {};
	}

	// XSalsa20 fundamentally requires the IV during setup because it uses
	// the first 16 bytes of the 24-byte nonce to derive a subkey via HSalsa20.
	auto errc = xsalsa20_setup(&this->state_, this->key_storage.data(), 32, iv, 24, this->rounds_);
	FX_TOMCRYPT_ASSERT(errc, "XSalsa20 late setup failed!", {})

	std::vector<std::uint8_t> ciphertext(pt_len);
	errc = salsa20_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
	FX_TOMCRYPT_ASSERT(errc, "XSalsa20 crypt failed!", {})

	return ciphertext;
}
