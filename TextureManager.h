#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Texture.h"
#include "Renderer.h"

class TextureManager
{
	
public:
	std::shared_ptr<Texture> GetTexture(const std::string &path);

private:
	struct TextureMeta
	{
		std::string path;
		std::weak_ptr<Texture> texture;
	};

private:
	std::vector<TextureMeta> textures;
};