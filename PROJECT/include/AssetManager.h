#pragma once
#include "raylib.h"
#include <vector>
#include <unordered_map>
#include <string>

static std::vector<Texture> tileTextures;

void SendTileTextures(std::vector<Texture> t = {});

std::vector<Texture> GetTileTextures();
