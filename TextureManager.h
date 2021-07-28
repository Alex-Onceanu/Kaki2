#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Texture.h"
#include "Renderer.h"
#include "Surface.h"

namespace TextureManager
{
	std::shared_ptr<Texture> GetTexture(const std::string& path);
	std::shared_ptr<Surface> GetSurface(const std::string& path);

	struct TextureMeta
	{
		std::string path;
		std::weak_ptr<Texture> texture;
	};

	struct SurfaceMeta
	{
		std::string path;
		std::weak_ptr<Surface> surface;
	};

	static std::vector<TextureMeta> textures;
	static std::vector<SurfaceMeta> surfaces;

}