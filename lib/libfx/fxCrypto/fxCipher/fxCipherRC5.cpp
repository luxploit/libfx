#include <libfx/fxCrypto/fxCipher/fxCipherRC5.hpp>

bool fxCipherRC5::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen < 1 || keylen > 255) {
		fxPrintf(fxLogError, "RC5 setup failed! keylen out of range");
		return false;
	}

	if (this->initialized_) {
		cbc_done(&this->cbc_);
		this->initialized_ = false;
	}

	std::uint8_t dummy_iv[8] = {0};
	auto errc = cbc_start(find_cipher("rc5"), dummy_iv, key, keylen, this->rounds_, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "RC5-CBC start failed!", {})

	this->initialized_ = true;
	return true;
}


[[nodiscard]] std::vector<std::uint8_t> fxCipherRC5::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
		fxPrintf(fxLogError, "RC5-CBC encrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> ciphertext(pt_len);

	auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "RC5-CBC encrypt failed!", {})
	return ciphertext;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherRC5::decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
	if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
		fxPrintf(fxLogError, "RC5-CBC decrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> plaintext(ct_len);

	auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "RC5-CBC decrypt failed!", {})
	return plaintext;
}