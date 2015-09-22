#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include "ShaderProgram.h"
#include "Matrix.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
GLuint loadImage(const char *path)
{
	SDL_Surface *surface = IMG_Load(path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}
void drawSprite(GLuint texture, float x = 0, float y = 0, float angle = 0, float xscale = 1, float yscale =1)
{
	//setup
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

	//transformations
	modelMatrix.Translate(x,y,0);
	modelMatrix.Scale(xscale, yscale, 1);
	modelMatrix.Rotate(angle);
	
	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float vertices[] = { 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };


	float texCoords[] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);

	

	//drawing
	glEnableVertexAttribArray(program.texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, texture);

	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);


	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);


}
int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Test Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	//setup

	float lastFrameTicks =0.0f;
	glViewport(0, 0, 1280, 720);
	glClearColor(0.4f, 0.4f, 0.8f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float cycle = 0;
	float cycleLength = 2;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		glClear(GL_COLOR_BUFFER_BIT);

		GLuint ballSprite = loadImage("ball.png");
		GLuint racketSprite = loadImage("racket.png");
		GLuint netSprite = loadImage("net.png");

		

		cycle += elapsed;
		while (cycle > cycleLength)
		{
			cycle -= cycleLength;
		}
		
		//setup ball
		float ballAngle = abs((cycle * 3) - 3);
		float ballY = abs(sin(3.14*cycle));
		float ballX = abs(cycle - 1)*-3 + 1.5;
		float ballSquash = 1;
		if (ballY < 0.2)
		{
			ballSquash = ballY*3 + 0.4;
		}
		
		
		//draw right racket
		float racketRAngle = 0;
		if (ballX > 1.35)
		{
			racketRAngle = 1.2;
		}

		drawSprite(racketSprite, 1.75, -0.25, racketRAngle);

		//draw left racket
		float racketLAngle = 0;
		if (ballX < -1.35)
		{
			racketLAngle = 1.2;
		}

		drawSprite(racketSprite, -1.75, -0.25, racketLAngle,-1);
	
		//draw net and ball
		if (ballX > 0)
		{
			drawSprite(ballSprite, ballX, ballY, ballAngle,0.5*ballSquash,0.5);
			drawSprite(netSprite, 0, 0, 0, 2,2);
		}
		else
		{
				drawSprite(netSprite,0,-0,0,2,2);
				drawSprite(ballSprite, ballX, ballY, ballAngle,  0.5*ballSquash,0.5);
		}

		SDL_GL_SwapWindow(displayWindow);

	}

	SDL_Quit();
	return 0;
}