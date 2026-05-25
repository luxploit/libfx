#include <libfx/fxCrypto/fxCipher/fxCipherBlowfish.hpp>

bool fxCipherBlowfish::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen < 4 || keylen > 56) {
		fxPrintf(fxLogError, "Blowfish setup failed! keylen out of range");
		return false;
	}

	if (this->initialized_) {
		cbc_done(&this->cbc_);
		this->initialized_ = false;
	}

	std::uint8_t dummy_iv[8] = {0};
	auto errc = cbc_start(find_cipher("blowfish"), dummy_iv, key, keylen, 0, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "Blowfish-CBC start failed!", {})

	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherBlowfish::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
		fxPrintf(fxLogError, "Blowfish-CBC encrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> ciphertext(pt_len);

	auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "Blowfish-CBC encrypt failed!", {})
	return ciphertext;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherBlowfish::decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
	if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
		fxPrintf(fxLogError, "Blowfish-CBC decrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> plaintext(ct_len);

	auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "Blowfish-CBC decrypt failed!", {})
	return plaintext;
}