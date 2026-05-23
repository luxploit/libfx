#pragma once

#include <string>

#include <libfx/fxCore/fxMacros.hpp>
#include <libfx/fxCrypto/fxCryptoUtils.hpp>

#ifndef FX_DISABLE_CRYPTO

template <std::size_t N>
struct fxHash {
	virtual ~fxHash() = default;

	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept = 0;
	[[nodiscard]] virtual std::string hash_str(const std::string& data) const noexcept {
		return this->hash_str(FX_PTR_AS(const std::uint8_t*, data.data()), data.size());
	}

	[[nodiscard]] virtual constexpr std::size_t digest_size() const noexcept {
		return N;
	}
};

struct fxHash16 : public fxHash<2> {
	virtual ~fxHash16() = default;

	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept {
		auto ret = this->hash(data, len);
		return fxCryptoUtils::get()->to_hex(ret);
	}

	[[nodiscard]] virtual std::uint16_t hash(const std::uint8_t* data, const std::size_t len) const noexcept = 0;
	[[nodiscard]] virtual std::uint16_t hash(const std::string& data) const noexcept {
		return this->hash(FX_PTR_AS(const std::uint8_t*, data.data()), data.size());
	}
};

struct fxHash32 : public fxHash<4> {
	virtual ~fxHash32() = default;

	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept {
		auto ret = this->hash(data, len);
		return fxCryptoUtils::get()->to_hex(ret);
	}

	[[nodiscard]] virtual std::uint32_t hash(const std::uint8_t* data, const std::size_t len) const noexcept = 0;
	[[nodiscard]] virtual std::uint32_t hash(const std::string& data) const noexcept {
		return this->hash(FX_PTR_AS(const std::uint8_t*, data.data()), data.size());
	}
};

struct fxHash64 : public fxHash<8> {
	virtual ~fxHash64() = default;

	[[nodiscard]] virtual std::string hash_str(const std::uint8_t* data, const std::size_t len) const noexcept {
		auto ret = this->hash(data, len);
		return fxCryptoUtils::get()->to_hex(ret);
	}

	[[nodiscard]] virtual std::uint64_t hash(const std::uint8_t* data, const std::size_t len) const noexcept = 0;
	[[nodiscard]] virtual std::uint64_t hash(const std::string& data) const noexcept {
		return this->hash(FX_PTR_AS(const std::uint8_t*, data.data()), data.size());
	}
};

#endif