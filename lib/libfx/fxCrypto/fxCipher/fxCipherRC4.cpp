#include <libfx/fxCrypto/fxCipher/fxCipherRC4.hpp>

bool fxCipherRC4::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen < 1 || keylen > 256) {
		fxPrintf(fxLogError, "RC4 setup failed! keylen out of range");
		return false;
	}

	this->key_storage.assign(key, key + keylen);
	rc4_stream_setup(&this->state_, this->key_storage.data(), this->key_storage.size());
	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherRC4::encrypt(const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext) {
		fxPrintf(fxLogError, "RC4 crypt failed! requirements not met");
		return {};
	}

	std::vector<std::uint8_t> ciphertext(pt_len);
	rc4_stream_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
	return ciphertext;
}

