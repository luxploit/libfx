#pragma once

#define FX_PTR_AS(type, ptr) reinterpret_cast<type>((ptr))
#define FX_VAL_AS(type, val) static_cast<type>((val))
#define FX_DEREF_PTR_AS(type, ptr) *FX_PTR_AS(type*, ptr)
#define FX_ENUM_UNDER(val) static_cast<std::underlying_type_t<decltype(val)>>(val)