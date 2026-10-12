#include "dry.h"

std::vector<std::function<void()>> &register_singletons() noexcept {
	static std::vector<std::function<void()>> vec;
	return vec;
}
LocalVector<Node *> &add_singletons_to_scene_tree() noexcept {
	static LocalVector<Node *> vec;
	return vec;
}
std::vector<std::function<void()>> &pre_init_eager_globals() noexcept {
	static std::vector<std::function<void()>> vec;
	return vec;
}
std::vector<std::function<void()>> &init_eager_globals() noexcept {
	static std::vector<std::function<void()>> vec;
	return vec;
}
LocalVector<std::function<void()>> &free_lazy_globals() noexcept {
	static LocalVector<std::function<void()>> vec;
	return vec;
}
LocalVector<std::function<void()>> &free_singletons() noexcept {
	static LocalVector<std::function<void()>> vec;
	return vec;
}

#ifdef DEBUG_ENABLED
thread_local bool lazy_globals_must_be_first_used_from_the_main_thread = false;
#endif

const LazyStringName CHANGED{ U"changed" };
