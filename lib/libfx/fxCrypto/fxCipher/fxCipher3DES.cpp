#include <libfx/fxCrypto/fxCipher/fxCipher3DES.hpp>

bool fxCipher3DES::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen != 16 && keylen != 24) {
		fxPrintf(fxLogError, "3DES setup failed! keylen %zu is not 16 (2-Key) or 24 (3-Key)", keylen);
		return false;
	}

	if (this->initialized_) {
		cbc_done(&this->cbc_);
		this->initialized_ = false;
	}

	std::uint8_t dummy_iv[8] = {0};
	auto errc = cbc_start(find_cipher("3des"), dummy_iv, key, keylen, 0, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "3DES-CBC start failed!", false)

	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipher3DES::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv || pt_len % 8 != 0) {
		fxPrintf(fxLogError, "3DES-CBC encrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> ciphertext(pt_len);

	auto errc = cbc_encrypt(plaintext, ciphertext.data(), pt_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "3DES-CBC encrypt failed!", {})
	return ciphertext;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipher3DES::decrypt(const std::uint8_t* iv, const std::uint8_t* ciphertext, const std::size_t ct_len) noexcept {
	if (!this->initialized_ || !ciphertext || !iv || ct_len % 8 != 0) {
		fxPrintf(fxLogError, "3DES-CBC decrypt failed! requirements not met");
		return {};
	}

	std::memcpy(this->cbc_.IV, iv, 8);
	std::vector<std::uint8_t> plaintext(ct_len);

	auto errc = cbc_decrypt(ciphertext, plaintext.data(), ct_len, &this->cbc_);
	FX_TOMCRYPT_ASSERT(errc, "3DES-CBC decrypt failed!", {})
	return plaintext;
}