#include <libfx/fxCrypto/fxCipher/fxCipherChaCha20.hpp>

bool fxCipherChaCha20::init(const std::uint8_t* key, const std::size_t keylen) noexcept {
	if (keylen != 32) {
		fxPrintf(fxLogError, "ChaCha20 setup failed! keylen %zu is not 32", keylen);
		return false;
	}

	if (this->initialized_) {
		chacha_done(&this->state_);
		this->initialized_ = false;
	}

	auto errc = chacha_setup(&this->state_, key, keylen, 20);
	FX_TOMCRYPT_ASSERT(errc, "ChaCha20 setup failed!", {})

	this->initialized_ = true;
	return true;
}

[[nodiscard]] std::vector<std::uint8_t> fxCipherChaCha20::encrypt(const std::uint8_t* iv, const std::uint8_t* plaintext, const std::size_t pt_len) noexcept {
	if (!this->initialized_ || !plaintext || !iv) {
		fxPrintf(fxLogError, "ChaCha20 crypt failed! requirements not met");
		return {};
	}

	// Set IV and initial counter (0). Assumes standard 12-byte IETF nonce.
	auto errc = chacha_ivctr32(&this->state_, iv, 12, 0);
	FX_TOMCRYPT_ASSERT(errc, "ChaCha20 IV setup failed!", {})

	std::vector<std::uint8_t> ciphertext(pt_len);
	errc = chacha_crypt(&this->state_, plaintext, pt_len, ciphertext.data());
	FX_TOMCRYPT_ASSERT(errc, "ChaCha20 encrypt failed!", {})

	return ciphertext;
}