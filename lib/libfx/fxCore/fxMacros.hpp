#pragma once

#define FX_PTR_AS(type, ptr) reinterpret_cast<type>((ptr))
#define FX_VAL_AS(type, val) static_cast<type>((val))
#define FX_DEREF_PTR_AS(type, ptr) *FX_PTR_AS(type*, ptr)
#define FX_ENUM_UNDER(val) static_cast<std::underlying_type_t<decltype(val)>>(val)

#ifdef _WIN32
	#define FX_IS_WINDOWS
#elifdef __APPLE__
	#define FX_IS_MACOSX
#elifdef __linux__
	#define FX_IS_LINUX
#endif
