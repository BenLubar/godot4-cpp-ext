#pragma once

// IWYU pragma: private, include "dry.h"

#include <godot_cpp/core/class_db.hpp> // IWYU pragma: export

godot::String enum_property_hint(const godot::String &type_name, bool is_bitfield);

#define DECLARE_PROPERTY(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
\
public: \
	[[nodiscard]] m_type get_##m_name() const; \
	void set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name)

#define DECLARE_PROPERTY_IMPLGET(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
\
public: \
	[[nodiscard]] m_type get_##m_name() const override; \
	void set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name)

#define DECLARE_PROPERTY_IS(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
\
public: \
	[[nodiscard]] m_type is_##m_name() const; \
	void set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name)

#define DECLARE_PROPERTY_OBJECTID(m_type, m_name, ...) \
public: /* allow direct access to the ObjectID without going through ObjectDB twice */ \
	ObjectID _##m_name __VA_ARGS__; \
\
public: \
	[[nodiscard]] m_type *get_##m_name() const; \
	void set_##m_name(m_type *new_##m_name)

#ifdef SQUIRREL_ENABLED
#define DECLARE_PROPERTY_SCRIPT(m_name) \
	DECLARE_PROPERTY(Ref<SquirrelScript>, m_name); \
	DECLARE_PROPERTY(String, m_name##_template); \
	Ref<SquirrelFunction> _##m_name##_import_cache
#endif

#define BIND_PROPERTY(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name __VA_OPT__(, ) __VA_ARGS__), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_BITFIELD(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_FLAGS, enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), true), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_BITFIELD, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_ENUM, enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), false), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_NODE(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_NODE_TYPE, #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_MULTILINE_TEXT(m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::STRING, #m_name, PROPERTY_HINT_MULTILINE_TEXT), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_VARIANT_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(m_type) + ":"), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::INT) + "/" + itos(PROPERTY_HINT_ENUM) + ":" + _mm_enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), false)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::OBJECT) + "/" + itos(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_ARRAY_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::OBJECT) + "/" + itos(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_IS(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("is_" #m_name), &self_type::is_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name __VA_OPT__(, ) __VA_ARGS__), "set_" #m_name, "is_" #m_name)

#define BIND_PROPERTY_OBJECTID_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_OBJECTID_NODE_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_NODE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#ifdef SQUIRREL_ENABLED
#define BIND_PROPERTY_SCRIPT(m_name) \
	BIND_PROPERTY_RESOURCE(SquirrelScript, m_name); \
	BIND_PROPERTY(Variant::STRING, m_name##_template)
#endif

#define IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			_##m_name = new_##m_name; \
			m_onchange; \
		} \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, m_onchange) \
	m_type m_class::is_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			_##m_name = new_##m_name; \
			m_onchange; \
		} \
	}
#define IMPLEMENT_PROPERTY(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, emit_changed())
#define IMPLEMENT_PROPERTY_IS(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, emit_changed())
#define IMPLEMENT_PROPERTY_SIMPLE(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_SIMPLE_IS(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_OBJECTID_ONCHANGE(m_class, m_type, m_name, m_onchange) \
	m_type *m_class::get_##m_name() const { \
		return Object::cast_to<m_type>(ObjectDB::get_instance(_##m_name)); \
	} \
	void m_class::set_##m_name(m_type *new_##m_name) { /* NOLINT(bugprone-macro-parentheses) */ \
		_##m_name = new_##m_name ? new_##m_name->get_instance_id() : ObjectID(); \
		m_onchange; /* always calls onchange for now; decide whether this needs to check the old value eventually */ \
	}
#define IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_OBJECTID_ONCHANGE(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_ONSET(m_class, m_type, m_name, m_onset) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		_##m_name = new_##m_name; \
		m_onset; \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_SIGNAL(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			if (_##m_name.is_valid()) { \
				_##m_name->disconnect(CHANGED, callable_mp(this, &m_class::m_onchange)); \
			} \
			_##m_name = new_##m_name; \
			if (_##m_name.is_valid()) { \
				_##m_name->connect(CHANGED, callable_mp(this, &m_class::m_onchange)); \
			} \
			m_onchange(); \
		} \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_SIGNAL_DEFERRED(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			if (_##m_name.is_valid()) { \
				_##m_name->disconnect(CHANGED, callable_mp(this, &m_class::m_onchange)); \
			} \
			_##m_name = new_##m_name; \
			if (_##m_name.is_valid()) { \
				_##m_name->connect(CHANGED, callable_mp(this, &m_class::m_onchange), CONNECT_DEFERRED); \
			} \
			m_onchange(); \
		} \
	}
#define IMPLEMENT_PROPERTY_SIGNAL(m_class, m_type, m_name, m_signal, m_callback) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			if (_##m_name.is_valid()) { \
				_##m_name->disconnect(m_signal, callable_mp(this, m_callback)); \
			} \
			_##m_name = new_##m_name; \
			if (_##m_name.is_valid()) { \
				_##m_name->connect(m_signal, callable_mp(this, m_callback)); \
			} \
		} \
	}
#define IMPLEMENT_PROPERTY_ARRAY_CHANGED(m_class, m_type, m_name, m_callback) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i].ptr(); \
			if (item) { \
				item->disconnect(CHANGED, callable_mp(this, m_callback)); \
			} \
		} \
		_##m_name = new_##m_name; \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i].ptr(); \
			if (item) { \
				item->connect(CHANGED, callable_mp(this, m_callback)); \
			} \
		} \
		callable_mp(this, m_callback).call(); \
	}
#define IMPLEMENT_PROPERTY_ARRAY_CHANGED_ITEM(m_class, m_type, m_name, m_callback) \
	m_type m_class::get_##m_name() const { \
		return _##m_name; \
	} \
	void m_class::set_##m_name(std::conditional_t<std::is_trivially_copy_constructible_v<m_type>, m_type, const m_type &> new_##m_name) { \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->disconnect(CHANGED, callable_mp(this, m_callback).bind(item)); \
			} \
		} \
		_##m_name = new_##m_name; \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->connect(CHANGED, callable_mp(this, m_callback).bind(item)); \
				callable_mp(this, m_callback).call(item); \
			} \
		} \
	}

#ifdef SQUIRREL_ENABLED
#define IMPLEMENT_PROPERTY_SCRIPT(m_class, m_name) \
	IMPLEMENT_PROPERTY(m_class, Ref<SquirrelScript>, m_name); \
	IMPLEMENT_PROPERTY(m_class, String, m_name##_template)
#endif
