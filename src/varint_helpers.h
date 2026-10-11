#pragma once

#include <godot_cpp/core/defs.hpp>

#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/packed_int32_array.hpp>
#include <godot_cpp/variant/packed_int64_array.hpp>
#include <godot_cpp/variant/string.hpp>

#include <godot_cpp/templates/pair.hpp>

godot::Pair<uint64_t, int64_t> uvarint(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
godot::Pair<int64_t, int64_t> svarint(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
godot::Pair<uint32_t, int64_t> uvarint32(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
void append_uvarint(godot::PackedByteArray &p_buffer, uint64_t p_value);
void append_svarint(godot::PackedByteArray &p_buffer, int64_t p_value);
_ALWAYS_INLINE_ void append_uvarint32(godot::PackedByteArray &p_buffer, uint32_t p_value) {
	append_uvarint(p_buffer, p_value);
}

godot::Pair<godot::PackedByteArray, int64_t> uvarbytes(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
godot::Pair<godot::String, int64_t> uvarstring(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
void append_uvarbytes(godot::PackedByteArray &p_buffer, const godot::PackedByteArray &p_value);
void append_uvarstring(godot::PackedByteArray &p_buffer, const godot::String &p_value);

godot::Pair<godot::PackedInt64Array, int64_t> uvarintarray(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
godot::Pair<godot::PackedInt32Array, int64_t> uvarint32array(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
godot::Pair<godot::PackedStringArray, int64_t> uvarstringarray(const godot::PackedByteArray &p_buffer, int64_t p_offset = 0);
void append_uvarintarray(godot::PackedByteArray &p_buffer, const godot::PackedInt64Array &p_value);
void append_uvarint32array(godot::PackedByteArray &p_buffer, const godot::PackedInt32Array &p_value);
void append_uvarstringarray(godot::PackedByteArray &p_buffer, const godot::PackedStringArray &p_value);

void append_float(godot::PackedByteArray &p_buffer, float p_value);
void append_double(godot::PackedByteArray &p_buffer, double p_value);
