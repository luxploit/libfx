#pragma once

//! Available macro options for libfx. remove the "//&" comment to use or define manually

//& #define FX_DISABLE_BUILTIN_LOG
/// Disables the default logging implementation, leaving only the manual sinks

//& #define FX_DISABLE_LOGGING
/// Disables fxPrintf and variants (replaced with an empty macro)

//& #define FX_DISABLE_CRYPTO
/// Disables the fxCrypto module