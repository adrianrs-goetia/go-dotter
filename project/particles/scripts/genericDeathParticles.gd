extends GPUParticles3D

func _enter_tree():
	restart()

func _on_timer_timeout():
	queue_free()

