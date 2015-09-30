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
void drawRect(ShaderProgram program, float x, float y, float width, float height)
{
	

	float vertices[] = { x,y, x,y+height, x+width,y+height, x+width,y};


	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);


	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


	glDisableVertexAttribArray(program.positionAttribute);
}
class Paddle
{
public:
	float x;
	float y;
	float height;
	float width;
	float speed;
	bool ai;
	Paddle(bool left)
	{
		ai = left;
		x = ai ? -80 : 80;
		y = 0;
		height = 50;
		width = 5;
		speed = ai ? 50 : 0;


	}

	void draw(ShaderProgram program)
	{
		drawRect(program, x - width / 2, y - height / 2, width, height);
	}
	void tick(float t)
	{
		if (ai && abs(y) + height / 2 > 50)
		{
			speed *= -1;
		}
		if ((speed > 0 && y + height / 2 < 60) || (speed < 0 && y + height / 2 > -60))
		{
			y += speed*t;
		}

	}
};
class Ball
{
	const float startSpeed = 60;
public:
	float x;
	float y;
	float xSpeed;
	float ySpeed;
	float size;
	bool bumped;

	Ball()
	{
		
		x = 0;
		y = 0;
		xSpeed = startSpeed;
		ySpeed = 0;
		size = 5;
		bumped = false;
	}
	void draw(ShaderProgram program)
	{
		drawRect(program, x-size/2, y-size/2, size, size);
	}
	void hit(Paddle p)
	{
		bumped = true;
		if (abs(y-p.y) < p.height / 2)
		{
			xSpeed *= -1.1;
			ySpeed += p.speed / 4;
		}

	}
	void reset()
	{
		if (x < 0)
		{
			xSpeed = startSpeed;
		}
		else
		{
			xSpeed = startSpeed * -1;
		}
		ySpeed = 0;
		x = 0;
		y = 0;
		bumped = false;
		
	}
	void tick(float t, Paddle left, Paddle right)
	{
		if (!bumped && x > 80-right.width/2)
		{
			hit(right);
		}
		else if (!bumped && x< -80 +left.width/2)
		{
			hit(left);
		}
		else if (abs(x) < 70)
		{
			bumped = false;
		}
		if (abs(y)>50)
		{
			ySpeed *= -1;
		}
		x += xSpeed*t;
		y += ySpeed*t;
	}
};

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

	SDL_Event event;
	bool done = false;
	//setup
	//setup
	ShaderProgram program(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(-88.75f, 88.75f, -50.0f, 50.0f, -1.0f, 1.0f);

	program.setModelMatrix(modelMatrix);
	program.setProjectionMatrix(projectionMatrix);
	program.setViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Ball ball;
	Paddle left(true);
	Paddle right(false);
	float lastFrameTime =0.0f;
	glViewport(0, 0, 1280, 720);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float rightY = right.y;
	float rightSpeed = 0;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
			else if (event.type == SDL_MOUSEMOTION) {
				rightSpeed = (360 - event.motion.y) / 6-rightY;
				rightY = (360 - event.motion.y) / 6;
			}
		}

		//tick
		float totalTime = (float)SDL_GetTicks() / 1000;
		float tick = totalTime - lastFrameTime;
		lastFrameTime = totalTime;
		right.y=rightY;
		right.speed = rightSpeed*50;
		left.tick(tick);
		ball.tick(tick,left,right);
		

		if (ball.x < -100)
		{
			ball.reset();
		}
		else if (ball.x>100)
		{
			ball.reset();
		}
		//render
		glClear(GL_COLOR_BUFFER_BIT);

		left.draw(program);
		right.draw(program);
		ball.draw(program);
		SDL_GL_SwapWindow(displayWindow);

	}

	SDL_Quit();
	return 0;
}