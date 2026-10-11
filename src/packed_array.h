// Bridge between godot::Vector and godot::TypedArray for faster intra-extension array handling.

#pragma once

// IWYU pragma: begin_exports

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/variant.hpp>

// IWYU pragma: end_exports

template <typename T>
class PackedArray {
	godot::Vector<T> _vector;

public:
	_FORCE_INLINE_ PackedArray() = default;
	PackedArray(const godot::TypedArray<T> &p_array) {
		_vector.resize(p_array.size());
		for (int64_t i = 0; i < p_array.size(); i++) {
			_vector.write[i] = godot::VariantCaster<T>::cast(p_array[i]);
		}
	}
	_FORCE_INLINE_ PackedArray(std::initializer_list<T> p_values) : _vector{ p_values } {}
	_FORCE_INLINE_ PackedArray(const PackedArray &p_array) = default;
	_FORCE_INLINE_ PackedArray(PackedArray &&p_array) noexcept : _vector{ std::move(p_array._vector) } {}

	[[nodiscard]] _FORCE_INLINE_ const T &operator[](int64_t p_index) const {
		return _vector[p_index];
	}
	[[nodiscard]] _FORCE_INLINE_ T &write(int64_t p_index) {
		return _vector.write[p_index];
	}
	[[nodiscard]] _FORCE_INLINE_ godot::VectorWriteProxy<T> &write() {
		return _vector.write;
	}
	_FORCE_INLINE_ bool append(const T &p_value) {
		return _vector.append(p_value);
	}
	_FORCE_INLINE_ void append_array(const PackedArray &p_array) {
		_vector.append_array(p_array._vector);
	}
	_FORCE_INLINE_ void insert(int64_t p_index, const T &p_value) {
		_vector.insert(p_index, p_value);
	}
	_FORCE_INLINE_ void fill(const T &p_value) {
		_vector.fill(p_value);
	}
	_FORCE_INLINE_ bool erase(const T &p_value) {
		return _vector.erase(p_value);
	}
	_FORCE_INLINE_ void remove_at(int64_t p_index) {
		_vector.remove_at(p_index);
	}
	[[nodiscard]] _FORCE_INLINE_ PackedArray<T> slice(int64_t p_begin, int64_t p_end = INT64_MAX) {
		PackedArray<T> array;
		array._vector = _vector.slice(p_begin, p_end);
		return array;
	}

	[[nodiscard]] _FORCE_INLINE_ typename godot::Vector<T>::ConstIterator begin() const {
		return _vector.begin();
	}
	[[nodiscard]] _FORCE_INLINE_ typename godot::Vector<T>::ConstIterator end() const {
		return _vector.end();
	}
	[[nodiscard]] _FORCE_INLINE_ const T &get(int64_t p_index) const {
		return _vector.get(p_index);
	}
	_FORCE_INLINE_ void set(int64_t p_index, const T &p_value) const {
		_vector.set(p_index, p_value);
	}

	[[nodiscard]] _FORCE_INLINE_ T *ptrw() {
		return _vector.ptrw();
	}
	[[nodiscard]] _FORCE_INLINE_ const T *ptr() const {
		return _vector.ptr();
	}
	[[nodiscard]] _FORCE_INLINE_ operator godot::Span<T>() const {
		return _vector.span();
	}
	[[nodiscard]] _FORCE_INLINE_ godot::Span<T> span() const {
		return _vector.span();
	}

	[[nodiscard]] _FORCE_INLINE_ bool is_empty() const {
		return _vector.is_empty();
	}
	_FORCE_INLINE_ void clear() {
		_vector.clear();
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t size() const {
		return _vector.size();
	}
	_FORCE_INLINE_ godot::Error resize(int64_t p_size) {
		return _vector.resize(p_size);
	}

	[[nodiscard]] _FORCE_INLINE_ int64_t find(const T &p_value, int64_t p_from = 0) const {
		return _vector.find(p_value, p_from);
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t rfind(const T &p_value, int64_t p_from = -1) const {
		return _vector.rfind(p_value, p_from);
	}
	[[nodiscard]] _FORCE_INLINE_ bool has(const T &p_value) const {
		return _vector.has(p_value);
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t count(const T &p_value) const {
		return _vector.count(p_value);
	}
	[[nodiscard]] bool any(const godot::Callable &p_condition) const {
		for (const T &item : _vector) {
			if (p_condition.call(item)) {
				return true;
			}
		}

		return false;
	}
	[[nodiscard]] bool all(const godot::Callable &p_condition) const {
		for (const T &item : _vector) {
			if (!p_condition.call(item)) {
				return false;
			}
		}

		return true;
	}

	template <typename Comparator, bool Validate = SORT_ARRAY_VALIDATE_ENABLED, typename... Args>
	_FORCE_INLINE_ void sort_custom(Args &&...args) {
		_vector.template sort_custom<Comparator, Validate, Args...>(std::forward<Args>(args)...);
	}

	[[nodiscard]] _FORCE_INLINE_ int64_t bsearch(const T &p_value, bool p_before) const {
		return _vector.bsearch(p_value, p_before);
	}

	template <typename Comparator, typename Value, typename... Args>
	[[nodiscard]] _FORCE_INLINE_ int64_t bsearch_custom(const Value &p_value, bool p_before, Args &&...args) const {
		return _vector.template bsearch_custom<Comparator, Value, Args...>(p_value, p_before, std::forward<Args>(args)...);
	}

	[[nodiscard]] _FORCE_INLINE_ bool operator==(const PackedArray &p_array) const {
		return _vector == p_array._vector;
	}
	[[nodiscard]] _FORCE_INLINE_ bool operator!=(const PackedArray &p_array) const {
		return _vector != p_array._vector;
	}
	void operator=(const PackedArray &p_from) { _vector = p_from._vector; }
	void operator=(PackedArray &&p_from) noexcept { _vector = std::move(p_from._vector); }
	[[nodiscard]] operator godot::TypedArray<T>() const {
		godot::TypedArray<T> array;
		array.resize(_vector.size());
		for (int64_t i = 0; i < _vector.size(); i++) {
			array[i] = _vector[i];
		}
		return array;
	}
	[[nodiscard]] _FORCE_INLINE_ operator godot::Variant() const {
		return operator godot::TypedArray<T>();
	}
};

template <typename T>
class PackedArray<godot::Ref<T>> {
	godot::Vector<godot::Ref<T>> _vector;

public:
	_FORCE_INLINE_ PackedArray() = default;
	PackedArray(const godot::TypedArray<T> &p_array) {
		_vector.resize(p_array.size());
		for (int64_t i = 0; i < p_array.size(); i++) {
			_vector.write[i] = p_array[i];
		}
	}
	PackedArray(const godot::TypedArray<godot::Ref<T>> &p_array) {
		_vector.resize(p_array.size());
		for (int64_t i = 0; i < p_array.size(); i++) {
			_vector.write[i] = p_array[i];
		}
	}
	_FORCE_INLINE_ PackedArray(std::initializer_list<godot::Ref<T>> p_values) : _vector{ p_values } {}
	_FORCE_INLINE_ PackedArray(const PackedArray &p_array) = default;
	_FORCE_INLINE_ PackedArray(PackedArray &&p_array) noexcept : _vector{ std::move(p_array._vector) } {}

	[[nodiscard]] _FORCE_INLINE_ const godot::Ref<T> &operator[](int64_t p_index) const {
		return _vector[p_index];
	}
	[[nodiscard]] _FORCE_INLINE_ godot::Ref<T> &write(int64_t p_index) {
		return _vector.write[p_index];
	}
	[[nodiscard]] _FORCE_INLINE_ godot::VectorWriteProxy<godot::Ref<T>> &write() {
		return _vector.write;
	}
	_FORCE_INLINE_ bool append(const godot::Ref<T> &p_value) {
		return _vector.append(p_value);
	}
	_FORCE_INLINE_ void append_array(const PackedArray &p_array) {
		_vector.append_array(p_array._vector);
	}
	_FORCE_INLINE_ void insert(int64_t p_index, const godot::Ref<T> &p_value) {
		_vector.insert(p_index, p_value);
	}
	_FORCE_INLINE_ void fill(const godot::Ref<T> &p_value) {
		_vector.fill(p_value);
	}
	_FORCE_INLINE_ bool erase(const godot::Ref<T> &p_value) {
		return _vector.erase(p_value);
	}
	_FORCE_INLINE_ void remove_at(int64_t p_index) {
		_vector.remove_at(p_index);
	}
	[[nodiscard]] _FORCE_INLINE_ PackedArray<godot::Ref<T>> slice(int64_t p_begin, int64_t p_end = INT64_MAX) {
		PackedArray<godot::Ref<T>> array;
		array._vector = _vector.slice(p_begin, p_end);
		return array;
	}

	[[nodiscard]] _FORCE_INLINE_ typename godot::Vector<godot::Ref<T>>::ConstIterator begin() const {
		return _vector.begin();
	}
	[[nodiscard]] _FORCE_INLINE_ typename godot::Vector<godot::Ref<T>>::ConstIterator end() const {
		return _vector.end();
	}
	[[nodiscard]] _FORCE_INLINE_ const godot::Ref<T> &get(int64_t p_index) const {
		return _vector.get(p_index);
	}
	_FORCE_INLINE_ void set(int64_t p_index, const godot::Ref<T> &p_value) const {
		_vector.set(p_index, p_value);
	}

	[[nodiscard]] _FORCE_INLINE_ godot::Ref<T> *ptrw() {
		return _vector.ptrw();
	}
	[[nodiscard]] _FORCE_INLINE_ const godot::Ref<T> *ptr() const {
		return _vector.ptr();
	}
	[[nodiscard]] _FORCE_INLINE_ operator godot::Span<godot::Ref<T>>() const {
		return _vector.span();
	}
	[[nodiscard]] _FORCE_INLINE_ godot::Span<godot::Ref<T>> span() const {
		return _vector.span();
	}

	[[nodiscard]] _FORCE_INLINE_ bool is_empty() const {
		return _vector.is_empty();
	}
	_FORCE_INLINE_ void clear() {
		_vector.clear();
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t size() const {
		return _vector.size();
	}
	_FORCE_INLINE_ void resize(int64_t p_size) {
		_vector.resize(p_size);
	}

	[[nodiscard]] _FORCE_INLINE_ int64_t find(const godot::Ref<T> &p_value, int64_t p_from = 0) const {
		return _vector.find(p_value, p_from);
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t rfind(const godot::Ref<T> &p_value, int64_t p_from = -1) const {
		return _vector.rfind(p_value, p_from);
	}
	[[nodiscard]] _FORCE_INLINE_ bool has(const godot::Ref<T> &p_value) const {
		return _vector.has(p_value);
	}
	[[nodiscard]] _FORCE_INLINE_ int64_t count(const godot::Ref<T> &p_value) const {
		return _vector.count(p_value);
	}
	[[nodiscard]] bool any(const godot::Callable &p_condition) const {
		for (const godot::Ref<T> &item : _vector) {
			if (p_condition.call(item)) {
				return true;
			}
		}

		return false;
	}
	[[nodiscard]] bool all(const godot::Callable &p_condition) const {
		for (const T &item : _vector) {
			if (!p_condition.call(item)) {
				return false;
			}
		}

		return true;
	}

	template <typename Comparator, bool Validate = SORT_ARRAY_VALIDATE_ENABLED, typename... Args>
	_FORCE_INLINE_ void sort_custom(Args &&...args) {
		_vector.template sort_custom<Comparator, Validate, Args...>(std::forward<Args>(args)...);
	}

	[[nodiscard]] _FORCE_INLINE_ int64_t bsearch(const godot::Ref<T> &p_value, bool p_before) const {
		return _vector.bsearch(p_value, p_before);
	}

	template <typename Comparator, typename Value, typename... Args>
	[[nodiscard]] _FORCE_INLINE_ int64_t bsearch_custom(const Value &p_value, bool p_before, Args &&...args) const {
		return _vector.template bsearch_custom<Comparator, Value, Args...>(p_value, p_before, std::forward<Args>(args)...);
	}

	[[nodiscard]] _FORCE_INLINE_ bool operator==(const PackedArray &p_array) const {
		return _vector == p_array._vector;
	}
	[[nodiscard]] _FORCE_INLINE_ bool operator!=(const PackedArray &p_array) const {
		return _vector != p_array._vector;
	}
	void operator=(const PackedArray &p_from) { _vector = p_from._vector; }
	void operator=(PackedArray &&p_from) noexcept { _vector = std::move(p_from._vector); }
	[[nodiscard]] operator godot::TypedArray<T>() const {
		godot::TypedArray<T> array;
		array.resize(_vector.size());
		for (int64_t i = 0; i < _vector.size(); i++) {
			array[i] = _vector[i];
		}
		return array;
	}
	[[nodiscard]] operator godot::TypedArray<godot::Ref<T>>() const {
		godot::TypedArray<godot::Ref<T>> array;
		array.resize(_vector.size());
		for (int64_t i = 0; i < _vector.size(); i++) {
			array[i] = _vector[i];
		}
		return array;
	}
	[[nodiscard]] _FORCE_INLINE_ operator godot::Variant() const {
		return operator godot::TypedArray<godot::Ref<T>>();
	}
};

namespace godot {
template <typename T>
struct VariantCaster<PackedArray<T>> {
	[[nodiscard]] static _FORCE_INLINE_ PackedArray<T> cast(const Variant &p_variant) {
		return TypedArray<T>(p_variant.operator Array());
	}
};

template <typename T>
struct VariantCaster<const PackedArray<T> &> {
	[[nodiscard]] static _FORCE_INLINE_ PackedArray<T> cast(const Variant &p_variant) {
		return TypedArray<T>(p_variant.operator Array());
	}
};

template <typename T>
struct PtrToArg<PackedArray<T>> {
	[[nodiscard]] _FORCE_INLINE_ static PackedArray<T> convert(const void *p_ptr) {
		return TypedArray<T>(*reinterpret_cast<const Array *>(p_ptr));
	}
	typedef Array EncodeT;
	_FORCE_INLINE_ static void encode(const PackedArray<T> &p_val, void *p_ptr) {
		*reinterpret_cast<Array *>(p_ptr) = p_val.operator TypedArray<T>();
	}
};

template <typename T>
struct PtrToArg<const PackedArray<T> &> {
	typedef Array EncodeT;
	[[nodiscard]] _FORCE_INLINE_ static PackedArray<T> convert(const void *p_ptr) {
		return TypedArray<T>(*reinterpret_cast<const Array *>(p_ptr));
	}
};

template <typename T>
struct GetTypeInfo<PackedArray<T>> {
	static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;
	static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
	[[nodiscard]] static inline PropertyInfo get_class_info() {
		return GetTypeInfo<TypedArray<T>>::get_class_info();
	}
};

template <typename T>
struct GetTypeInfo<const PackedArray<T> &> {
	static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;
	static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
	[[nodiscard]] static inline PropertyInfo get_class_info() {
		return GetTypeInfo<const TypedArray<T> &>::get_class_info();
	}
};

template <typename T>
struct VariantCaster<PackedArray<Ref<T>>> {
	[[nodiscard]] static _FORCE_INLINE_ PackedArray<Ref<T>> cast(const Variant &p_variant) {
		return TypedArray<T>(p_variant.operator Array());
	}
};

template <typename T>
struct VariantCaster<const PackedArray<Ref<T>> &> {
	[[nodiscard]] static _FORCE_INLINE_ PackedArray<Ref<T>> cast(const Variant &p_variant) {
		return TypedArray<T>(p_variant.operator Array());
	}
};

template <typename T>
struct PtrToArg<PackedArray<Ref<T>>> {
	[[nodiscard]] _FORCE_INLINE_ static PackedArray<Ref<T>> convert(const void *p_ptr) {
		return TypedArray<T>(*reinterpret_cast<const Array *>(p_ptr));
	}
	typedef Array EncodeT;
	_FORCE_INLINE_ static void encode(const PackedArray<Ref<T>> &p_val, void *p_ptr) {
		*reinterpret_cast<Array *>(p_ptr) = p_val.operator TypedArray<T>();
	}
};

template <typename T>
struct PtrToArg<const PackedArray<Ref<T>> &> {
	typedef Array EncodeT;
	[[nodiscard]] _FORCE_INLINE_ static PackedArray<Ref<T>> convert(const void *p_ptr) {
		return TypedArray<T>(*reinterpret_cast<const Array *>(p_ptr));
	}
};

template <typename T>
struct GetTypeInfo<PackedArray<Ref<T>>> {
	static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;
	static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
	[[nodiscard]] static inline PropertyInfo get_class_info() {
		return make_property_info(Variant::Type::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE, T::get_class_static());
	}
};

template <typename T>
struct GetTypeInfo<const PackedArray<Ref<T>> &> {
	static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY;
	static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
	[[nodiscard]] static inline PropertyInfo get_class_info() {
		return make_property_info(Variant::Type::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE, T::get_class_static());
	}
};
} //namespace godot
