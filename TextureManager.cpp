#include "Tools/pch.h"
#include "pch.h"
#include "TextureManager.h"

std::shared_ptr<Texture> TextureManager::GetTexture(const std::string &path)
{
	auto e{ std::find_if(textures.begin(), textures.end(), [&path](auto &&tm) {	
			return tm.path == path;
		})
	};
	if (e != textures.end())
	{
		if (e->texture.expired())
		{
			e->texture = Renderer::LoadImage(path);
		}
		return e->texture.lock();
	}

	auto img = Renderer::LoadImage(path);
	textures.push_back({ path,img });
	return img;
}

std::shared_ptr<Surface> TextureManager::GetSurface(const std::string& path)
{
	auto e{ std::find_if(surfaces.begin(), surfaces.end(),
		[&path](auto&& tm) {
			return tm.path == path;
		})
	};
	if (e != surfaces.end())
	{
		if (e->surface.expired())
		{
			e->surface = Renderer::SurfaceLoadImage(path);
		}
		return e->surface.lock();
	}

	auto img = Renderer::SurfaceLoadImage(path);
	surfaces.push_back({ path,img });
	return img;
}