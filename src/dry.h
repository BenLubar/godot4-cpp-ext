// Don't Repeat Yourself (precompiled header).

#pragma once

#ifdef __GNUC__
#ifdef DEBUG_ENABLED
// some warnings that are very useful in development
#pragma GCC diagnostic error "-Wimplicit-fallthrough"
#pragma GCC diagnostic error "-Winit-self"
#pragma GCC diagnostic error "-Wsequence-point"
#pragma GCC diagnostic error "-Wswitch"
#endif
// and one that isn't
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif

// IWYU pragma: always_keep
// IWYU pragma: begin_exports

#include "lazy_globals.h"
#include "packed_array.h"
#include "property_macros.h"
#include "serialization_helpers.h"
#include "varint_helpers.h"

#ifdef SQUIRREL_ENABLED
#include "squirrel_helpers.h"
#endif

#include <godot_cpp/variant/typed_dictionary.hpp>

using namespace godot;

// IWYU pragma: end_exports

#include "dry_extra.h" // IWYU pragma: export // exported separately so clangd doesn't complain about an unused header

void initialize_shared_gdextension_types(godot::ModuleInitializationLevel p_level);
void initialize_shared_gdextension_singletons();
void startup_shared_gdextension();
void shutdown_shared_gdextension();
void uninitialize_shared_gdextension_types(godot::ModuleInitializationLevel p_level);
