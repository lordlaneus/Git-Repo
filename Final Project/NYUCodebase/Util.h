#pragma once
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Level.h"
#include "Planet.h"
#include "Cluster.h"
#include "Entity.h"
#include "Player.h"
#include "Game.h"
#include "Util.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

class Util{
public:
	static GLuint loadImage(const char *path);
	static void drawText(ShaderProgram*, int fontTexture, std::string text, float x, float y, float size, float spacing);
	static float randFloat();
};