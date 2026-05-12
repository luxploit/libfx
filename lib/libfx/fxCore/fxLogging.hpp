#pragma once

#include <libfx/fxConfig.hpp>

#include <functional>
#include <source_location>
#include <string>
#include <format>

enum fxLoggingChannels {
	fxLogInfo,   fxLogWarn,
	fxLogError,  fxLogDebug,
	fxLogTrace,  fxLogFatal,
	fxLogNotice, fxLogSystem
};

#ifndef FX_DISABLE_LOGGING
using fxLoggingChannelsHandler = std::function<void(fxLoggingChannels, const std::string&, const std::source_location& loc)>;

void fxSetLoggingChannelsHandler(fxLoggingChannelsHandler cb) noexcept;
[[nodiscard]] std::string fxGetChannelColor(fxLoggingChannels ch) noexcept;
[[nodiscard]] std::string fxGetChannelName(fxLoggingChannels ch) noexcept;

void fxPrintfImpl(fxLoggingChannels ch, const std::string& message, const std::source_location& loc) noexcept;

template <typename... Args>
void fxPrintf(fxLoggingChannels ch, const std::string& format, Args const &...args,
	const std::source_location& loc = std::source_location::current()) noexcept
{
	auto message = std::vformat(format, std::make_format_args(args...));
	return fxPrintfImpl(ch, message, loc);
}
#else
#define fxPrintf(...) ((void)0)
#endif
