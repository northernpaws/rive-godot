@tool
extends EditorImportPlugin

enum Presets { DEFAULT }

func _get_importer_name():
	return "demos.sillymaterial"

func _get_visible_name():
	return "Silly Material"

func _get_recognized_extensions():
	return ["mtxt"]

func _get_save_extension():
	return "material"

func _get_resource_type():
	return "StandardMaterial3D"

func _get_preset_count():
	return Presets.size()

func _get_preset_name(preset_index):
	match preset_index:
		Presets.DEFAULT:
			return "Default"
		_:
			return "Unknown"

func _get_priority():
	return 1.0

func _get_import_order():
	return 200

func _get_import_options(path, preset_index):
	match preset_index:
		Presets.DEFAULT:
			return [{
			   "name": "use_red_anyway",
			   "default_value": false
			}]
		_:
			return []

func _get_option_visibility(path, option_name, options):
	return true

func _import(source_file, save_path, options, r_platform_variants, r_gen_files):
	var file = FileAccess.open(source_file, FileAccess.READ)
	if file == null:
		return FileAccess.get_open_error()

	var line = file.get_line()
	
	var channels = line.split(",")
	if channels.size() != 3:
		return ERR_PARSE_ERROR

	var color
	if options.use_red_anyway:
		color = Color8(255, 0, 0)
	else:
		color = Color8(int(channels[0]), int(channels[1]), int(channels[2]))
	
	var material = StandardMaterial3D.new()
	material.albedo_color = color
	
	var next_pass = StandardMaterial3D.new()
	next_pass.albedo_color = color.inverted()
	var next_pass_path = "%s.next_pass.%s" % [save_path, _get_save_extension()]

	var err = ResourceSaver.save(next_pass, next_pass_path)
	if err != OK:
		return err
	r_gen_files.push_back(next_pass_path)

	return ResourceSaver.save(material, "%s.%s" % [save_path, _get_save_extension()])
	
