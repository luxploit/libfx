#pragma once

#include <libfx/fxConfig.hpp>

#include <functional>
#include <source_location>
#include <string>

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>

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
void fxPrintfEx(const std::source_location& loc, fxLoggingChannels ch, const char* format, Args&&... args) noexcept
{
	auto message = fmt::sprintf(format, std::forward<Args>(args)...);
	return fxPrintfImpl(ch, message, loc);
}

#define fxPrintf(ch, msg, ...) fxPrintfEx(std::source_location::current(), ch, msg, __VA_ARGS__)
#else
#define fxPrintf(...) ((void)0)
#endif