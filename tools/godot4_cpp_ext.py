import SCons

def exists(env):
	return True

def generate(env):
	env.Append(CPPPATH=["godot4-cpp-ext/src/"])

	if env.get("is_msvc", False):
		env["CXXFLAGS"].remove("/std:c++17")
		env.Append(CXXFLAGS=["/std:c++20"])
	else:
		env["CXXFLAGS"].remove("-std=c++17")
		env.Append(CXXFLAGS=["-std=c++20"])

	env.AddMethod(init_precompiled_header, "InitDRY")

def init_precompiled_header(env, sources):
	sources += env.Glob("godot4-cpp-ext/src/*.cpp", exclude=["godot4-cpp-ext/src/dry_pch.cpp"])
	non_pch_sources = []

	if "SQUIRREL_ENABLED" in env["CPPDEFINES"]:
		non_pch_sources += env.SConscript("godot4-squirrel/SCsub", {"env": env})

	if env.get("is_msvc", False):
		env["PCHSTOP"] = "dry.h"

		pch = env.PCH("godot4-cpp-ext/src/dry_pch.cpp")
		env.Depends(sources, pch)

		env["PCH"] = pch[0]

		return sources + non_pch_sources

	pch_file = env.File("godot4-cpp-ext/src/dry.h")

	shared_object_builder = env["BUILDERS"]["SharedObject"]
	shared_object_builder.add_action(".h", SCons.Defaults.ShCXXAction)

	pch = env.SharedObject(
		target="godot4-cpp-ext/src/dry.h.gch",
		source=pch_file,
		CXXFLAGS=["-x", "c++-header", "$CXXFLAGS"])

	if env.get("use_llvm", False):
		sources = [env.SharedObject(f, CXXFLAGS=["-include-pch", str(pch[0]), "$CXXFLAGS"]) for f in sources]
	else:
		sources = [env.SharedObject(f) for f in sources]

	env.Depends(sources, pch)

	return sources + non_pch_sources
