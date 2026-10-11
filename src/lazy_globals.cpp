#include "dry.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

std::vector<std::function<void()>> &register_singletons() noexcept {
	static std::vector<std::function<void()>> vec;
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

void add_singleton_node_to_scene_tree(Object *p_singleton) {
	Node *node = Object::cast_to<Node>(p_singleton);
	SceneTree *tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
	node->set_name(p_singleton->get_class());
	tree->get_root()->add_child(node);
}
