#include "dry.h"

// These four varint-related functions and the comment
// following this one are derived from Go 1.26.1.
//
// Copyright 2011 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This file implements "varint" encoding of 64-bit integers.
// The encoding is:
// - unsigned integers are serialized 7 bits at a time, starting with the
//   least significant bits
// - the most significant bit (msb) in each output byte indicates if there
//   is a continuation byte (msb = 1)
// - signed integers are mapped to unsigned integers using "zig-zag"
//   encoding: Positive values x are written as 2*x + 0, negative values
//   are written as 2*(^x) + 1; that is, negative numbers are complemented
//   and whether to complement is encoded in bit 0.
//
// Design note:
// At most 10 bytes are needed for 64-bit values. The encoding could
// be more dense: a full 64-bit value needs an extra byte just to hold bit 63.
// Instead, the msb of the previous byte could be used to hold bit 63 since we
// know there can't be more than 64 bits. This is a trivial improvement and
// would reduce the maximum encoding length to 9 bytes. However, it breaks the
// invariant that the msb is always the "continuation bit" and thus makes the
// format incompatible with a varint encoding for larger numbers (say 128-bit).

Pair<uint64_t, int64_t> uvarint(const PackedByteArray &p_buffer, int64_t p_offset) {
	static constexpr int64_t MaxVarintLen64 = 10;

	Pair<uint64_t, int64_t> result(0, 0);

	uint_fast8_t shift = 0;
	for (int64_t i = 0; i < MaxVarintLen64; i++) {
		if (p_offset + i >= p_buffer.size()) {
			// buffer too short
			result.first = 0;
			result.second = 0;
			return result;
		}

		const uint8_t b = p_buffer[p_offset + i];
		if (b < 0x80) {
			if (i == MaxVarintLen64 - 1 && b > 1) {
				// overflow
				result.first = 0;
				result.second = -(i + 1);
				return result;
			}

			result.first |= static_cast<uint64_t>(b) << shift;
			result.second = i + 1;
			return result;
		}

		result.first |= static_cast<uint64_t>(b & 0x7f) << shift;
		shift += 7;
	}

	// overflow
	result.first = 0;
	result.second = -(MaxVarintLen64 + 1);
	return result;
}

Pair<int64_t, int64_t> svarint(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> result = uvarint(p_buffer, p_offset); // ok to continue in presence of error
	Pair<int64_t, int64_t> signed_result(result.first >> 1, result.second);
	if ((result.first & 1) != 0) {
		signed_result.first = ~signed_result.first;
	}
	return signed_result;
}

void append_uvarint(PackedByteArray &p_buffer, uint64_t p_value) {
	while (p_value >= 0x80) {
		p_buffer.append(static_cast<uint8_t>(p_value) | 0x80);
		p_value >>= 7;
	}

	p_buffer.append(p_value);
}

void append_svarint(PackedByteArray &p_buffer, int64_t p_value) {
	uint64_t value = static_cast<uint64_t>(p_value) << 1;
	if (p_value < 0) {
		value = ~value;
	}

	append_uvarint(p_buffer, value);
}

// These are convenience functions for handling uvarint-prefixed data.

Pair<PackedByteArray, int64_t> uvarbytes(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> length = uvarint(p_buffer, p_offset);
	if (length.second <= 0 || static_cast<int64_t>(length.first) < 0 || p_buffer.size() < p_offset + length.second + static_cast<int64_t>(length.first)) {
		return { {}, 0 };
	}
	return { p_buffer.slice(p_offset + length.second, p_offset + length.second + length.first), length.second + static_cast<int64_t>(length.first) };
}

Pair<String, int64_t> uvarstring(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<PackedByteArray, int64_t> bytes = uvarbytes(p_buffer, p_offset);
	return { bytes.first.get_string_from_utf8(), bytes.second };
}

Pair<PackedInt64Array, int64_t> uvarintarray(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> length = uvarint(p_buffer, p_offset);
	if (length.second <= 0) {
		return { {}, 0 };
	}

	int64_t offset = p_offset + length.second;

	PackedInt64Array array;
	array.resize(length.first);
	for (uint64_t i = 0; i < length.first; i++) {
		const Pair<uint64_t, int64_t> item = uvarint(p_buffer, offset);
		if (item.second <= 0) {
			return { {}, 0 };
		}

		offset += item.second;
		array[i] = item.first;
	}

	return { array, offset - p_offset };
}

Pair<PackedStringArray, int64_t> uvarstringarray(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> length = uvarint(p_buffer, p_offset);
	if (length.second <= 0) {
		return { {}, 0 };
	}

	int64_t offset = p_offset + length.second;

	PackedStringArray array;
	array.resize(length.first);
	for (uint64_t i = 0; i < length.first; i++) {
		const Pair<String, int64_t> item = uvarstring(p_buffer, offset);
		if (item.second <= 0) {
			return { {}, 0 };
		}

		offset += item.second;
		array[i] = item.first;
	}

	return { array, offset - p_offset };
}

void append_uvarbytes(PackedByteArray &p_buffer, const PackedByteArray &p_value) {
	append_uvarint(p_buffer, p_value.size());
	p_buffer.append_array(p_value);
}

void append_uvarstring(PackedByteArray &p_buffer, const String &p_value) {
	append_uvarbytes(p_buffer, p_value.to_utf8_buffer());
}

void append_uvarintarray(PackedByteArray &p_buffer, const PackedInt64Array &p_value) {
	append_uvarint(p_buffer, p_value.size());
	for (int64_t i : p_value) {
		append_uvarint(p_buffer, static_cast<uint64_t>(i));
	}
}

void append_uvarstringarray(PackedByteArray &p_buffer, const PackedStringArray &p_value) {
	append_uvarint(p_buffer, p_value.size());
	for (const String &s : p_value) {
		append_uvarstring(p_buffer, s);
	}
}

// 32-bit versions

Pair<uint32_t, int64_t> uvarint32(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> i = uvarint(p_buffer, p_offset);
	if (i.second <= 0 || i.first > UINT32_MAX) {
		return { 0, 0 };
	}

	return { static_cast<uint32_t>(i.first), i.second };
}

Pair<PackedInt32Array, int64_t> uvarint32array(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> length = uvarint(p_buffer, p_offset);
	if (length.second <= 0) {
		return { {}, 0 };
	}

	int64_t offset = p_offset + length.second;

	PackedInt32Array array;
	array.resize(length.first);
	for (uint64_t i = 0; i < length.first; i++) {
		const Pair<uint32_t, int64_t> item = uvarint32(p_buffer, offset);
		if (item.second <= 0) {
			return { {}, 0 };
		}

		offset += item.second;
		array[i] = item.first;
	}

	return { array, offset - p_offset };
}

void append_uvarint32array(PackedByteArray &p_buffer, const PackedInt32Array &p_value) {
	append_uvarint(p_buffer, p_value.size());
	for (int32_t i : p_value) {
		append_uvarint(p_buffer, static_cast<uint32_t>(i));
	}
}

// ...and some convenience functions for statically-sized data:

void append_float(PackedByteArray &p_buffer, float p_value) {
	const int64_t offset = p_buffer.size();
	p_buffer.resize(offset + 4);
	p_buffer.encode_float(offset, p_value);
}

void append_double(PackedByteArray &p_buffer, double p_value) {
	const int64_t offset = p_buffer.size();
	p_buffer.resize(offset + 8);
	p_buffer.encode_double(offset, p_value);
}
