#include "AssetManager.h"

void SendTileTextures(std::vector<Texture> t)
{
	tileTextures = t;
}
std::vector<Texture> GetTileTextures()
{
	return tileTextures;
}