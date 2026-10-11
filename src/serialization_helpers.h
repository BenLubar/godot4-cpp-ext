#pragma once

#include "packed_array.h"

#ifdef SQUIRREL_ENABLED
#define SERIALIZE_SCRIPT(m_name) \
	if ((m_name).is_valid() && !(m_name)->get_source().is_empty()) { \
		append_uvarstring(p_data, (m_name)->get_source()); \
		append_uvarstring(p_data, m_name##_template); \
	} else { \
		p_data.append(0); \
	}

#define PARSE_SCRIPT_RETURN(m_object, m_name, m_identifier, m_return) \
	const Pair<String, int64_t> m_name = uvarstring(p_data, p_offset); \
	ERR_FAIL_COND_V_MSG((m_name).second <= 0, (m_return), "Reached end of data while decoding script " #m_name "."); \
	p_offset += (m_name).second; \
	if (!(m_name).first.is_empty()) { \
		Ref<SquirrelScript> m_name##_script; \
		m_name##_script.instantiate(); \
		m_name##_script->set_source((m_name).first); \
		m_name##_script->set_name(vformat("mode://%s/%s/%s.nut", (m_object)->get_class(), (m_identifier), #m_name)); \
		(void)m_name##_script->compile(); \
		(m_object)->set_##m_name(m_name##_script); \
		const Pair<String, int64_t> m_name##_template = uvarstring(p_data, p_offset); \
		ERR_FAIL_COND_V_MSG(m_name##_template.second <= 0, (m_return), "Reached end of data while decoding script " #m_name "."); \
		p_offset += m_name##_template.second; \
		(m_object)->set_##m_name##_template(m_name##_template.first); \
	}

#define PARSE_SCRIPT(m_object, m_name) PARSE_SCRIPT_RETURN((m_object), m_name, (m_object)->get_script_identifier(), nullptr)
#endif

#define SERIALIZE_REF_ARRAY(m_array, m_recursion_limit) \
	ERR_FAIL_COND_V_MSG((m_recursion_limit) <= 0, false, "Data is too deeply nested."); \
	append_uvarint(p_data, (m_array).size()); \
	for (const auto &item : (m_array)) { \
		ERR_FAIL_COND_V(!item->serialize(p_data, (m_recursion_limit) - 1), false); \
	}

#define SERIALIZE_REF_ARRAY_UNLIMITED(m_array) \
	append_uvarint(p_data, (m_array).size()); \
	for (const auto &item : (m_array)) { \
		ERR_FAIL_COND_V(!item->serialize(p_data), false); \
	}

#define SERIALIZE_SCRIPT_VARIABLE_ARRAY(m_array, m_recursion_limit) \
	ERR_FAIL_COND_V_MSG((m_recursion_limit) <= 0, false, "Data is too deeply nested."); \
	append_uvarint(p_data, (m_array).size()); \
	for (const auto &item : (m_array)) { \
		ERR_FAIL_COND_V(!item->_serialize(p_data, (m_recursion_limit) - 1), false); \
	}

#define SERIALIZE_SCRIPT_VARIABLE_ARRAY_UNLIMITED(m_array) \
	append_uvarint(p_data, (m_array).size()); \
	for (const auto &item : (m_array)) { \
		ERR_FAIL_COND_V(!item->_serialize(p_data), false); \
	}

template <typename T>
struct ParseRefArrayHelper {};

template <typename T>
struct ParseRefArrayHelper<PackedArray<godot::Ref<T>>> {
	using type = T;
};

#define PARSE_REF_ARRAY(m_object, m_array, m_return, m_recursion_limit) \
	{ \
		ERR_FAIL_COND_V_MSG((m_recursion_limit) <= 0, (m_return), "Data is too deeply nested."); \
		const Pair<uint64_t, int64_t> num_##m_array = uvarint(p_data, p_offset); \
		ERR_FAIL_COND_V_MSG(num_##m_array.second <= 0, (m_return), "Reached end of data while decoding " #m_array " array."); \
		p_offset += num_##m_array.second; \
		(m_object)->m_array.resize(num_##m_array.first); \
		for (uint64_t i = 0; i < num_##m_array.first; i++) { \
			(m_object)->m_array.write(i) = ParseRefArrayHelper<decltype((m_object)->m_array)>::type::parse(p_data, p_offset, (m_recursion_limit) - 1); \
			ERR_FAIL_COND_V((m_object)->m_array[i].is_null(), (m_return)); \
		} \
	}

#define PARSE_REF_ARRAY_UNLIMITED(m_object, m_array, m_return) \
	{ \
		const Pair<uint64_t, int64_t> num_##m_array = uvarint(p_data, p_offset); \
		ERR_FAIL_COND_V_MSG(num_##m_array.second <= 0, (m_return), "Reached end of data while decoding " #m_array " array."); \
		p_offset += num_##m_array.second; \
		(m_object)->m_array.resize(num_##m_array.first); \
		for (uint64_t i = 0; i < num_##m_array.first; i++) { \
			(m_object)->m_array.write(i) = ParseRefArrayHelper<decltype((m_object)->m_array)>::type::parse(p_data, p_offset); \
			ERR_FAIL_COND_V((m_object)->m_array[i].is_null(), (m_return)); \
		} \
	}

#define PARSE_SCRIPT_VARIABLE_ARRAY(m_object, m_array, m_return, m_recursion_limit) \
	{ \
		ERR_FAIL_COND_V_MSG((m_recursion_limit) <= 0, (m_return), "Data is too deeply nested."); \
		const Pair<uint64_t, int64_t> num_##m_array = uvarint(p_data, p_offset); \
		ERR_FAIL_COND_V_MSG(num_##m_array.second <= 0, (m_return), "Reached end of data while decoding " #m_array " array."); \
		p_offset += num_##m_array.second; \
		(m_object)->m_array.resize(num_##m_array.first); \
		for (uint64_t i = 0; i < num_##m_array.first; i++) { \
			(m_object)->m_array.write(i) = ParseRefArrayHelper<decltype((m_object)->m_array)>::type::_parse(p_data, p_offset, (m_recursion_limit) - 1); \
			ERR_FAIL_COND_V((m_object)->m_array[i].is_null(), (m_return)); \
		} \
	}

#define PARSE_SCRIPT_VARIABLE_ARRAY_UNLIMITED(m_object, m_array, m_return) \
	{ \
		const Pair<uint64_t, int64_t> num_##m_array = uvarint(p_data, p_offset); \
		ERR_FAIL_COND_V_MSG(num_##m_array.second <= 0, (m_return), "Reached end of data while decoding " #m_array " array."); \
		p_offset += num_##m_array.second; \
		(m_object)->m_array.resize(num_##m_array.first); \
		for (uint64_t i = 0; i < num_##m_array.first; i++) { \
			(m_object)->m_array.write(i) = ParseRefArrayHelper<decltype((m_object)->m_array)>::type::_parse(p_data, p_offset); \
			ERR_FAIL_COND_V((m_object)->m_array[i].is_null(), (m_return)); \
		} \
	}
