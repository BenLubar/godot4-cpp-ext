#include "dry.h"

#ifdef SQUIRREL_ENABLED
#include "godot_squirrel_register_types.h"
#endif

void initialize_shared_gdextension_types(godot::ModuleInitializationLevel p_level) {
#ifdef SQUIRREL_ENABLED
	initialize_squirrel_module(p_level);
#endif

#ifdef DEBUG_ENABLED
	lazy_globals_must_be_first_used_from_the_main_thread = true;
#endif
}

void initialize_shared_gdextension_singletons() {
	for (const std::function<void()> &register_singleton : register_singletons()) {
		register_singleton();
	}
	register_singletons().clear();
}

void startup_shared_gdextension() {
	for (const std::function<void()> &pre_init_eager_global : pre_init_eager_globals()) {
		pre_init_eager_global();
	}
	pre_init_eager_globals().clear();

	for (const std::function<void()> &init_eager_global : init_eager_globals()) {
		init_eager_global();
	}
	init_eager_globals().clear();
}

void shutdown_shared_gdextension() {
	for (const std::function<void()> &free_lazy_global : free_lazy_globals()) {
		free_lazy_global();
	}
	free_lazy_globals().clear();
}

void uninitialize_shared_gdextension_types(godot::ModuleInitializationLevel p_level) {
#ifdef SQUIRREL_ENABLED
	uninitialize_squirrel_module(p_level);
#endif

	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		for (const std::function<void()> &free_singleton : free_singletons()) {
			free_singleton();
		}
		free_singletons().clear();
	}
}
