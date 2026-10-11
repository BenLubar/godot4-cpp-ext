#include "dry.h"

String enum_property_hint(const String &type_name, bool is_bitfield) {
#ifdef WEB_ENABLED
	return {};
#else
	// TODO: https://github.com/godotengine/godot-proposals/issues/12228
	if (type_name == "Orientation") {
		return "VERTICAL:1,HORIZONTAL:0";
	}
	if (type_name == "ClockDirection") {
		return "CLOCKWISE:0,COUNTERCLOCKWISE:1";
	}
	if (type_name == "HorizontalAlignment") {
		return "HORIZONTAL_ALIGNMENT_LEFT:0,HORIZONTAL_ALIGNMENT_CENTER:1,HORIZONTAL_ALIGNMENT_RIGHT:2,HORIZONTAL_ALIGNMENT_FILL:3";
	}
	if (type_name == "VerticalAlignment") {
		return "VERTICAL_ALIGNMENT_TOP:0,VERTICAL_ALIGNMENT_CENTER:1,VERTICAL_ALIGNMENT_BOTTOM:2,VERTICAL_ALIGNMENT_FILL:3";
	}
	if (type_name == "Key" || type_name == "KeyLocation" || type_name == "KeyModifierMask" || type_name == "MouseButton" || type_name == "MouseButtonMask" || type_name == "JoyAxis" || type_name == "JoyButton") {
		return {};
	}

	const PackedStringArray class_name_parts = type_name.split(".");
	DEV_ASSERT(class_name_parts.size() == 2);

	const StringName class_name = class_name_parts[0];
	const StringName enum_name = class_name_parts[1];

	DEV_ASSERT(ClassDB::class_has_enum(class_name, enum_name, true));
	DEV_ASSERT(is_bitfield == ClassDB::is_class_enum_bitfield(class_name, enum_name, true));

	const PackedStringArray enum_constants = ClassDB::class_get_enum_constants(class_name, enum_name, true);
	DEV_ASSERT(!enum_constants.is_empty());

	PackedStringArray enum_hint_parts;
	enum_hint_parts.resize(enum_constants.size());
	for (int64_t i = 0; i < enum_constants.size(); i++) {
		DEV_ASSERT(ClassDB::class_get_integer_constant_enum(class_name, enum_constants[i], true) == enum_name);
		enum_hint_parts[i] = vformat("%s:%d", enum_constants[i], ClassDB::class_get_integer_constant(class_name, enum_constants[i]));
	}

	return String(",").join(enum_hint_parts);
#endif
}
