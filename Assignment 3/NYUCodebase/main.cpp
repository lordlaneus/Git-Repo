#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Ship.h"
#include "Tank.h"
#include "Bullet.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <vector>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


SDL_Window* displayWindow;
ShaderProgram* program;

GLuint loadImage(const char *path)
{
	SDL_Surface *surface = IMG_Load(path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(surface);

	return textureID;

}

void drawText(int fontTexture, std::string text, float x, float y, float size, float spacing) {
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	Matrix modelMatrix;
	Matrix viewMatrix;

	
	modelMatrix.Translate(x, y, 0);

	program->setModelMatrix(modelMatrix);
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i) + (-0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (0.5f * size), -0.5f * size,
			((size + spacing) * i) + (0.5f * size), 0.5f * size,
			((size + spacing) * i) + (-0.5f * size), -0.5f * size,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);

	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}
void setup(std::vector<Entity> &e)
{
	e.clear();
	e.push_back(Tank());
	for (int i = 0; i < 36; i++)
	{
		e.push_back(Ship(i));
	}
}
void fire(std::vector<Entity> &e)
{
	e.push_back(Bullet(e[0].x));
}
int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	float maximumTick = 100;
	SDL_Event event;
	bool done = false;
	//setup
	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(0.0, 160.0f, 0.0f, 100.0f, -1.0f, 1.0f);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);

	glUseProgram(program->programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	GLuint font = loadImage("font.png");
	GLuint sheet = loadImage("sheet.png");

	float lastFrameTime = 0.0f;
	glViewport(0, 0, 1280, 720);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::vector<Entity> e;
	int gameState = 1;
		while (!done) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
					done = true;
				} 
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_RETURN:
						if (gameState == 1)
						{
							gameState = 2;
						}
						setup(e);
						break;
					case SDLK_r:
						if (gameState > 1)
						{
							gameState = 1;
						}
						break;
					case SDLK_LEFT:
						e[0].accelX = -50;
						break;
					case SDLK_RIGHT:
						e[0].accelX = 50;
						break;

					}
				}
				if (event.type == SDL_KEYUP)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_LEFT:
						if (e[0].accelX < 0)
						{
							e[0].accelX = 0;
						}
						break;
					case SDLK_RIGHT:
						if (e[0].accelX > 0)
						{
							e[0].accelX = 0;
						}
						break;
					case SDLK_SPACE:
						fire(e);
						break;

					}
				}
			}
		
	
			//tick
			float totalTime = (float)SDL_GetTicks() / 1000;
			float tick = totalTime - lastFrameTime;
			
			lastFrameTime = totalTime;
			glClear(GL_COLOR_BUFFER_BIT);
			bool shipsLeft;
			switch (gameState)
			{
			case 1:
				drawText(font, "SPACE INVADERS", 15, 75, 5, 5);

				drawText(font, "Press \"enter\" to begin", 35, 20, 4, 0);
				break;
			case 2:
				//tick
				shipsLeft = false;
				while (tick > maximumTick)
				{
					for (int i = 0; i < e.size(); i++)\
					{
						e[i].update(tick);
						e[i].collide(e);
					}
					tick -= maximumTick;
				}
				for (int i = 0; i < e.size();i++)
				{

					e[i].update(tick);
					e[i].collide(e);
				}
				for (int i = e.size() - 1; i >= 0; i--)
				{
					if (e[i].type == BULLET&&e[i].y>110)
					{
						e.erase(e.begin() + i);
					}
					else if  (e[i].type == SHIP)
					{
						shipsLeft = true;
					}
				}
					
				for (int i = 0; i < e.size(); i++)\
				{
					e[i].update(tick);
				}
				e[0].update(tick);
				lastFrameTime = totalTime;

				for (int i = 0; i < e.size(); i++)
				{
					e[i].render(program,sheet,2,2);
				}
				if (!shipsLeft)
				{
					gameState = 3;
				}
				break;
			case 3:
				drawText(font, "GAME", 35, 75, 30, 1);
				drawText(font, "OVER", 35, 45, 30, 1);
				drawText(font, "Press \"r\" to restart.", 42, 20, 4, 0);
				break;
			}
			SDL_GL_SwapWindow(displayWindow);
		}
		SDL_Quit();
		return 0;
	}