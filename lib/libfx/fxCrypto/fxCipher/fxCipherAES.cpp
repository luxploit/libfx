#include <libfx/fxCrypto/fxCipher/fxCipherAES.hpp>

bool fxCipherAES::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen != 16 && keylen != 24 && keylen != 32) {
		fxPrintf(fxLogError, "AES setup failed! keylen %zu is not 16, 24 or 32", keylen);
		return false;
	}

	if (this->initialized_) {
		cbc_done(&this->cbc_);
		this->initialized_ = false;
	}

	std::uint8_t dummy_iv[16] = {0};
	auto errc = cbc_start(find_cipher("aes"), dummy_iv, key, keylen, 0, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "AES-CBC start failed!", {})

	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherAES::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv || pt_len % 16 != 0) {
		fxPrintf(fxLogError, "AES-CBC encrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 16);
	std::vector<std::uint8_t> ciphertext(pt_len);

	auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "AES-CBC encrypt failed!", {})
	return ciphertext;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherAES::decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
	if (!this->initialized_ || !ciphertext || !iv || ct_len % 16 != 0) {
		fxPrintf(fxLogError, "AES-CBC decrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 16);
	std::vector<std::uint8_t> plaintext(ct_len);

	auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "AES-CBC decrypt failed!", {})
	return plaintext;
}