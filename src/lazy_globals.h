#pragma once

// IWYU pragma: begin_exports

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/templates/local_vector.hpp>

#include <functional>
#include <vector>

// IWYU pragma: end_exports

// the weird mixed types here are due to the order of initialization (we can't use the godot allocator until static init has finished, etc)
extern std::vector<std::function<void()>> &register_singletons() noexcept;
extern std::vector<std::function<void()>> &pre_init_eager_globals() noexcept;
extern std::vector<std::function<void()>> &init_eager_globals() noexcept;
extern godot::LocalVector<std::function<void()>> &free_lazy_globals() noexcept;
extern godot::LocalVector<std::function<void()>> &free_singletons() noexcept;
#ifdef DEBUG_ENABLED
extern thread_local bool lazy_globals_must_be_first_used_from_the_main_thread;
#endif

template <typename T>
class LazyGlobal {
	std::function<godot::Ref<T>()> _init;
	mutable godot::Ref<T> _ref;
	mutable bool _initialized = false;

protected:
	void _maybe_init() const {
		if (unlikely(!_initialized)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_initialized = true;
			_ref = _init();
			free_lazy_globals().push_back([this]() -> void {
				_ref.unref();
				_initialized = false;
			});
		}
	}

public:
	explicit LazyGlobal(const std::function<godot::Ref<T>()> &init, bool eager = false) noexcept : _init(init) {
		if (eager) {
			init_eager_globals().emplace_back([this]() -> void {
				_maybe_init();
			});
		}
	}

	const godot::Ref<T> &ref() const {
		_maybe_init();
		return _ref;
	}
	operator godot::Ref<T>() const {
		_maybe_init();
		return _ref;
	}
	T *operator*() const {
		_maybe_init();
		return _ref.ptr();
	}
	T *operator->() const {
		_maybe_init();
		return _ref.ptr();
	}
	void operator=(const godot::Ref<T> &value) {
		_maybe_init();
		_ref = value;
	}
};

template <typename T>
class LazyResource : public LazyGlobal<T> {
public:
	explicit LazyResource(const char32_t *path, bool eager = false) noexcept : LazyGlobal<T>{
				[path, eager]() -> godot::Ref<T> {
					const godot::String path_string{ path };
					if (eager && likely(godot::ResourceLoader::get_singleton()->load_threaded_get_status(path_string) != godot::ResourceLoader::THREAD_LOAD_INVALID_RESOURCE)) {
						return godot::ResourceLoader::get_singleton()->load_threaded_get(path_string);
					}

					return godot::ResourceLoader::get_singleton()->load(path_string, T::get_class_static());
				},
				eager,
			} {
		if (eager) {
			pre_init_eager_globals().emplace_back([path]() -> void {
				godot::ResourceLoader::get_singleton()->load_threaded_request(path, T::get_class_static());
			});
		}
	}
};

template <typename T, typename M>
class LazyVariant {
	const M _init;
	mutable T *_variant = nullptr;

	void _maybe_init() const {
		if (unlikely(!_variant)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_variant = memnew(T(_init));
			free_lazy_globals().push_back([this]() -> void {
				if (_variant) {
					memdelete(_variant);
					_variant = nullptr;
				}
			});
		}
	}

public:
	LazyVariant(const M &init) noexcept : _init(init) {
		init_eager_globals().emplace_back([this]() -> void {
			_maybe_init();
		});
	}

	operator T &() const {
		_maybe_init();
		return *_variant;
	}
	T &operator*() const {
		_maybe_init();
		return *_variant;
	}
	T *operator->() const {
		_maybe_init();
		return _variant;
	}
};

template <typename T>
class LazyVariant<T, void> {
	mutable T *_variant = nullptr;

	void _maybe_init() const {
		if (unlikely(!_variant)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_variant = memnew(T);
			free_lazy_globals().push_back([this]() -> void {
				if (_variant) {
					memdelete(_variant);
					_variant = nullptr;
				}
			});
		}
	}

public:
	LazyVariant() noexcept {
		init_eager_globals().emplace_back([this]() -> void {
			_maybe_init();
		});
	}

	operator T &() const {
		_maybe_init();
		return *_variant;
	}
	T &operator*() const {
		_maybe_init();
		return *_variant;
	}
	T *operator->() const {
		_maybe_init();
		return _variant;
	}
};

using LazyString = LazyVariant<godot::String, const char32_t *>;
using LazyStringName = LazyVariant<godot::StringName, const char32_t *>;
using LazyNodePath = LazyVariant<godot::NodePath, const char32_t *>;

extern const LazyStringName CHANGED;

void add_singleton_node_to_scene_tree(godot::Object *p_singleton);

template <typename T>
class RegisterSingleton {
public:
	explicit RegisterSingleton() noexcept {
		register_singletons().emplace_back(&_register);
	}

private:
	static void _register() {
		godot::Engine *engine = godot::Engine::get_singleton();
		T *singleton = memnew(T);

		engine->register_singleton(T::get_class_static(), singleton);

		if constexpr (godot::TypeInherits<godot::Node, T>::value) {
			init_eager_globals().push_back([]() -> void {
				add_singleton_node_to_scene_tree(T::get_singleton());
			});
		}

		free_singletons().push_back([engine, singleton]() -> void {
			engine->unregister_singleton(T::get_class_static());
			if constexpr (!godot::TypeInherits<godot::Node, T>::value) {
				static_assert(!godot::TypeInherits<godot::RefCounted, T>::value);
				memdelete(singleton);
			}
		});
	}
};
