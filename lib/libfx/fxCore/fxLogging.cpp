#include "fxLogging.hpp"
#include "fxAnsi.hpp"

#include <ctime>
#include <format>
#include <iostream>
#include <iomanip>
#include <istream>
#include <sstream>

#ifndef FX_DISABLE_LOGGING

fxLoggingChannelsHandler fxCb;
void fxSetLoggingChannelsHandler(fxLoggingChannelsHandler cb) noexcept {
	fxCb = cb;
}

[[nodiscard]] std::string fxGetChannelColor(fxLoggingChannels ch) noexcept {
	static std::unordered_map<fxLoggingChannels, std::string> channelColorsMap = {
		{ fxLogInfo,	FX_ANSI_FG_RGB(92, 150, 44) },
		{ fxLogWarn,	FX_ANSI_FG_RGB(166, 138, 13) },
		{ fxLogError,	FX_ANSI_FG_RGB(240, 82, 79) },
		{ fxLogDebug,	FX_ANSI_FG_STYLE_256(214) },
		{ fxLogTrace,	FX_ANSI_FG_STYLE_256(162) },
		{ fxLogFatal, 	FX_ANSI_FG_STYLE_256(124) },
		{ fxLogNotice, 	FX_ANSI_FG_CYAN },
		{ fxLogSystem, 	FX_ANSI_FG_STYLE_256(243) },
	};

	return channelColorsMap[ch];
}

[[nodiscard]] std::string fxGetChannelName(fxLoggingChannels ch) noexcept {
	static std::unordered_map<fxLoggingChannels, std::string> channelNamesMap = {
		{ fxLogInfo,	  "INFO" },
		{ fxLogWarn,	  "WARN" },
		{ fxLogError,	 "ERROR" },
		{ fxLogDebug,	 "DEBUG" },
		{ fxLogTrace,	 "TRACE" },
		{ fxLogFatal, 	 "FATAL" },
		{ fxLogNotice, 	"NOTICE" },
		{ fxLogSystem, 	"SYSTEM" },
	};

	return channelNamesMap[ch];
}

void fxBuiltinLog(fxLoggingChannels ch, const std::string& message, const std::source_location& loc) noexcept {
	tm local_time;
	const time_t time_since_epoch = std::time(nullptr);
	localtime_s(&local_time, &time_since_epoch);

	const auto console_timestamp = std::format("[{:0>2}:{:0>2}:{:0>2}]", local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
	const auto channel_color = fxGetChannelColor(ch);
	const auto channel_name = fxGetChannelName(ch);

	const auto prefix = std::format("{}{:>6}{} {} {} in {} (line:{})",
		channel_color, channel_name, FX_ANSI_RESET, console_timestamp,
		loc.function_name(), loc.file_name(), loc.line()
	);

	std::cout << prefix << FX_ANSI_RESET << "\n";

	std::istringstream stream(message);
	std::string line;
	while (std::getline(stream, line)) {
		std::cout << std::setw(7) << "" << line << FX_ANSI_RESET << "\n";
	}

	if (ch == fxLogFatal) {
		exit(1);
	}
}

void fxPrintfImpl(fxLoggingChannels ch, const std::string& message, const std::source_location& loc) noexcept {
	if (fxCb) {
		return fxCb(ch, message, loc);
	}

#ifndef FX_DISABLE_BUILTIN_LOG
	fxBuiltinLog(ch, message, loc);
#endif
}

#endif