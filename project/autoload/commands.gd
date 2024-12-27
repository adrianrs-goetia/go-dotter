@tool
extends Node


func _ready():
	if !Engine.is_editor_hint():
		return
	var palette = EditorInterface.get_command_palette()
	var callable = Callable(self, "_internal_reload").bind()
	palette.add_command("Internal_Reload", "command/ireload", callable)

func _internal_reload():
	if !Engine.is_editor_hint():
		return
	EditorInterface.get_edited_scene_root().propagate_notification(3004) # defined in C++. INTERNAL_RELOAD notification
