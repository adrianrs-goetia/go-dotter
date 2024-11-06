#pragma once

#include <core/core.hpp>

#include <godot_cpp/classes/node.hpp>

#include <filesystem>
#include <optional>

namespace godot {
	class Timer;
}

class ConfigReader : public godot::Node {
	GDCLASS(ConfigReader, godot::Node)

	std::filesystem::file_time_type m_lastWriteTime;
	std::string m_file = "config.json";
	bool m_readNextFrame = false;
	godot::Timer* m_readerTimer = nullptr;

public:
	static void _bind_methods();

	void _enter_tree() override;
	void _physics_process(double delta) override;

	void setReadStatusTrue();
	std::optional<std::filesystem::file_time_type> checkFileChanged() const;
};