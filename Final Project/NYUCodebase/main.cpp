#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "ShaderProgram.h"
#include "Matrix.h"
#include "Player.h"
#include "Game.h"
#include "Vector.h"
#include "Util.h"
#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <vector>

#ifdef _WINDOWS
#define RESOURCE_FOLDER "Resources/"
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif


SDL_Window* displayWindow;
ShaderProgram* program;
ShaderProgram* guiProgram;
bool fs = false;





void toggleFullScreen()
{
	if (fs)
	{
		SDL_SetWindowFullscreen(displayWindow, 0);
		fs = false;
	}
	else
	{
		SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN);
		fs = true;
	}
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Gravity Well", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_SetWindowIcon(displayWindow, IMG_Load("resources\\icon.png"));
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif
	Mix_Init(0);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 4096);
	float maximumTick = .5;
	SDL_Event event;
	bool done = false;
	//setup
	program = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragment.glsl");
	guiProgram = new ShaderProgram(RESOURCE_FOLDER"vertex_gui.glsl", RESOURCE_FOLDER"fragment_gui.glsl");

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	projectionMatrix.setOrthoProjection(0.0, 160.0f, 0.0f, 100.0f, -1.0f, 1.0f);

	program->setModelMatrix(modelMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program->setViewMatrix(viewMatrix);
	program->setInvert(false);


	guiProgram->setModelMatrix(modelMatrix);
	guiProgram->setProjectionMatrix(projectionMatrix);
	guiProgram->setViewMatrix(viewMatrix);
	guiProgram->setAlpha(1);




	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);




	glViewport(0, 0, 1280, 720);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Game g;
	g.lastFrameTime = 0.0f;
	Util::drawText(guiProgram, g.sprites["font"].texture, "PRESS \"ENTER\"", 80, 50, 2, 1);
	SDL_GL_SwapWindow(displayWindow);
	SDL_ShowCursor(0);

	while (g.state != Game::done)
	{
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				g.state = Game::done;
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				float unitX = (((float)event.motion.x / 1280) * 160);
				float unitY = (((float)(720 - event.motion.y) / 720) * 100);
				g.cursor.position = Vector(unitX-80, unitY-50);
			}
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_TAB)
			{
				toggleFullScreen();
			}
			else
			{
				switch (g.state)
				{
				case Game::ready:
					if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN)
					{
						g.state = Game::startup;
						g.playSound("startup");
						g.lastFrameTime = (float)SDL_GetTicks() / 1000;
					}
					break;
				case Game::paused:
				case Game::victory:
				case Game::gameOver:
				case Game::title:
					if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_UP:
						case SDLK_w:
							g.currentMenu->up();
							g.playSound("boop");
							break;
						case SDLK_DOWN:
						case SDLK_s:
							g.currentMenu->down();
							g.playSound("boop");
							break;
						case SDLK_SPACE:
						case SDLK_RETURN:
						case SDLK_e:
							g.menuSelect();
							break;
						}
					}
					break;
				case Game::play:
					if (event.type == SDL_MOUSEBUTTONDOWN) {
						if (event.button.button == SDL_BUTTON_RIGHT)
						{
							g.player->hook->shoot(g.cursor.position, 100,50);
						}
						else
						{
							g.player->attack(g.cursor.position);
						}
					}
					if (event.type == SDL_KEYDOWN&& event.key.repeat == 0)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_w:
						case SDLK_a:
						case SDLK_s:
						case SDLK_d:
							g.player->wasd.add(event.key.keysym.sym);
							g.player->walk();
							break;
						case SDLK_e:
							g.player->hook->shoot(g.cursor.position, 400,30,2);
							break;
						case SDLK_LSHIFT:
							g.player->shift();
							break;
	
						}
					}
					if (event.type == SDL_KEYUP)
					{
						switch (event.key.keysym.sym)
						{
						case SDLK_w:
						case SDLK_a:
						case SDLK_s:
						case SDLK_d:
							g.player->wasd.remove(event.key.keysym.sym);
							g.player->walk();
							break;
						case SDLK_SPACE:
							g.player->jump();
							break;
						case SDLK_LSHIFT:
							g.player->shiftDown = false;
							break;
						case SDLK_ESCAPE:
							g.pause();
							break;
						}
					}

				}
			}
		}
		if (g.state == Game::startup)
		{
			float totalTime = (float)SDL_GetTicks() / 1000;
			float totalTick = totalTime - g.lastFrameTime;
			g.lastFrameTime = totalTime;

			if (g.startupTimer > g.startupLength)
			{
				g.state = Game::title;
				g.currentMenu = &g.mainMenu;
				g.playMusic("bgm");
			}
			g.startupTimer += totalTick;
			//render
			g.start(guiProgram);
			SDL_GL_SwapWindow(displayWindow);
		}
		else if (g.state == Game::play) {

			//tick
			float totalTime = (float)SDL_GetTicks() / 1000;
			float totalTick = totalTime - g.lastFrameTime;

			g.lastFrameTime = totalTime;
			do
			{
				float tick = totalTick < maximumTick ? totalTick : maximumTick;
				g.update(tick);
				totalTick -= maximumTick;
			} while (totalTick > maximumTick);
			//render
			glClear(GL_COLOR_BUFFER_BIT);
			Matrix view;
			view.Translate(-g.player->position.x + 80, -g.player->position.y + 50, 0);
			program->setViewMatrix(view);
			g.render(program);

			g.renderGui(guiProgram);

			SDL_GL_SwapWindow(displayWindow);
		}
		else if (g.state!=Game::ready)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			g.render(program);
			g.renderGui(guiProgram);
			SDL_GL_SwapWindow(displayWindow);
		}
	}
	SDL_Quit();
	return 0;
}
