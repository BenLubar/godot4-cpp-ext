#pragma once

// IWYU pragma: private, include "dry.h"

#include "godot_squirrel_defs.h" // IWYU pragma: export

#include <godot_cpp/core/defs.hpp>

#include <godot_cpp/variant/string.hpp>

#define SQUSEDOUBLE
#define _SQ64 /* NOLINT(bugprone-reserved-identifier) */
#define KEEP_SLOT_ORDER
#include "../../godot4-squirrel/squirrel/include/squirrel.h" // IWYU pragma: export

// wrappers for godot::String
[[nodiscard]] _FORCE_INLINE_ SQRESULT sq_throwerror(HSQUIRRELVM p_vm, const godot::String &p_message) {
	return sq_throwerror(p_vm, p_message.utf8().get_data());
}

_FORCE_INLINE_ void sq_pushstring(HSQUIRRELVM p_vm, const godot::String &p_string) {
	const godot::CharString chars = p_string.utf8();
	sq_pushstring(p_vm, chars.get_data(), chars.length());
}
// Don't allow accidentally using the String constructor (use length overload of this function)
void sq_pushstring(HSQUIRRELVM p_vm, const char *p_string) = delete;

[[nodiscard]] _FORCE_INLINE_ SQRESULT sq_getstring(HSQUIRRELVM p_vm, SQInteger p_index, godot::String *r_string) {
	const SQChar *c = nullptr;
	SQInteger size = 0;
	const SQRESULT result = sq_getstringandsize(p_vm, p_index, &c, &size);
	if (SQ_SUCCEEDED(result)) {
		*r_string = godot::String::utf8(c, size);
	}
	return result;
}

// wrappers for bool
_FORCE_INLINE_ void sq_pushbool(HSQUIRRELVM p_vm, bool p_value) {
	sq_pushbool(p_vm, static_cast<SQBool>(p_value ? SQTrue : SQFalse));
}

[[nodiscard]] _FORCE_INLINE_ SQRESULT sq_getbool(HSQUIRRELVM p_vm, SQInteger p_index, bool *r_value) {
	SQBool value = SQFalse;
	const SQRESULT result = sq_getbool(p_vm, p_index, &value);
	if (SQ_SUCCEEDED(result)) {
		*r_value = value != SQFalse;
	}
	return result;
}

_FORCE_INLINE_ SQRESULT sq_newslot(HSQUIRRELVM p_vm, SQInteger p_index, bool p_static = false) {
	return sq_newslot(p_vm, p_index, static_cast<SQBool>(p_static ? SQTrue : SQFalse));
}

_FORCE_INLINE_ SQRESULT sq_rawdeleteslot(HSQUIRRELVM p_vm, SQInteger p_index, bool p_push_value = false) {
	return sq_rawdeleteslot(p_vm, p_index, static_cast<SQBool>(p_push_value ? SQTrue : SQFalse));
}

[[nodiscard]] _FORCE_INLINE_ SQRESULT sq_call(HSQUIRRELVM p_vm, SQInteger p_nparams, bool p_retval, bool p_raiseerror) {
	return sq_call(p_vm, p_nparams, static_cast<SQBool>(p_retval ? SQTrue : SQFalse), static_cast<SQBool>(p_raiseerror ? SQTrue : SQFalse));
}

// convert these from macros to functions for better assertion failure messages
#undef SQ_FAILED
[[nodiscard]] _ALWAYS_INLINE_ constexpr bool SQ_FAILED(SQRESULT res) noexcept {
	return res < 0;
}
#undef SQ_SUCCEEDED
[[nodiscard]] _ALWAYS_INLINE_ constexpr bool SQ_SUCCEEDED(SQRESULT res) noexcept {
	return res >= 0;
}
