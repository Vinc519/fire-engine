#pragma once

#include "Defines.h"

/// @brief Memory tags for categorizing memory allocations.
enum class MemoryTag : u8
{
	Unknown,
	Platform,
	Renderer,
	Texture,
	Buffer,
	String,
	Entity,
	Scene,
	Physics,
	Audio,
	Debug,
	Max
};

/// @brief Converts a memory tag into a string.
/// @param tag The memory tag to convert.
/// @return the corresponded string
constexpr const char* MemoryTagToString(MemoryTag tag)
{
	switch (tag)
	{
		case MemoryTag::Unknown:
			return "Unknown";
		case MemoryTag::Platform:
			return "Platform";
		case MemoryTag::Renderer:
			return "Renderer";
		case MemoryTag::Texture:
			return "Texture";
		case MemoryTag::Buffer:
			return "Buffer";
		case MemoryTag::String:
			return "String";
		case MemoryTag::Entity:
			return "Entity";
		case MemoryTag::Scene:
			return "Scene";
		case MemoryTag::Physics:
			return "Physics";
		case MemoryTag::Audio:
			return "Audio";
		case MemoryTag::Debug:
			return "Debug";
		default:
			return "Invalid";
	}
}
