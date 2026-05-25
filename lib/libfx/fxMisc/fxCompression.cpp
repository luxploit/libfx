#include <libfx/fxMisc/fxCompression.hpp>

[[nodiscard]] std::vector<std::uint8_t> fxCompression::compress(const std::uint8_t* data, const std::size_t len) noexcept{
	if (!data || len == 0) {
		fxPrintf(fxLogError, "LZ4 compress failed! requirements not met");
		return {};
	}

	auto max_dest_size = LZ4_compressBound(len);
	if (max_dest_size <= 0) {
		fxPrintf(fxLogError, "LZ4 compress failed! invalid size");
		return {};
	}

	std::vector<std::uint8_t> output(max_dest_size);
	auto cp_len = LZ4_compress_default(FX_PTR_AS(const char*, data), FX_PTR_AS(char*, output.data()), len, max_dest_size);
	if (cp_len <= 0) {
		fxPrintf(fxLogError, "LZ4 compress failed!");
		return {};
	}

	output.resize(cp_len);
	return output;
}

[[nodiscard]] std::vector<std::uint8_t> fxCompression::decompress(const std::uint8_t* compressed, const std::size_t cp_len, const std::size_t og_len) noexcept {
	if (!compressed || cp_len == 0 || og_len == 0) {
		fxPrintf(fxLogError, "LZ4 decompress failed! requirements not met");
		return {};
	}

	std::vector<std::uint8_t> output(og_len);
	auto dp_size = LZ4_decompress_safe(FX_PTR_AS(const char*, compressed), FX_PTR_AS(char*, output.data()), cp_len, og_len );
	if (dp_size < 0) {
		fxPrintf(fxLogError, "LZ4 decompress failed! error: %d", dp_size);
		return {};
	}

	output.resize(dp_size);
	return output;
}