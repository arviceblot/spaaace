/**
 * Attempt at an OpenGL "hypespace" simluation.
 */

#include <cstdlib>
#include <cmath>
#include <memory>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "Star.h"

using namespace std;

const static int STARS = 2500;
const static int MAP_W = 800;
const static int MAP_H = 600;
const static int CENTER_X = MAP_W / 2;
const static int CENTER_Y = MAP_H / 2;


void init();
void quit();
bool isOutside(const shared_ptr<Star>);

int main()
{
    init();

    SDL_Event event;
    bool isRunning = true;

    //create initial stars
    vector<shared_ptr<Star>> stars;
    for (int i = 0; i < STARS; i++)
    {
        shared_ptr<Star> star(new Star());
        star->x = rand() % MAP_W;
        star->y = rand() % MAP_H;
        star->speed = rand() % 1000;
        star->velocity[0] = (star->x - CENTER_X) / star->speed;
        star->velocity[1] = (star->y - CENTER_Y) / star->speed;
        star->size = 1;
        stars.push_back(star);
    }

    //run the simulation
    while (isRunning)
    {
        //Input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
            }
        }

        for (auto star: stars)
        {
            // modify in place the star if it is outside the screen space
            if (isOutside(star))
            {
                star->x = rand() % MAP_W;
                star->y = rand() % MAP_H;
                star->speed = rand() % 1000;
                star->size = 1;
            }
            // calculate the new position and size
            star->velocity[0] = (star->x - CENTER_X) / star->speed;
            star->velocity[1] = (star->y - CENTER_Y) / star->speed;
            star->x = star->x + star->velocity[0];
            star->y = star->y + star->velocity[1];
            star->updateSize();
        }

        //render all the stars
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glOrtho(0, MAP_W, MAP_H, 0, -1, 1);

        //star
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        for (auto star: stars)
        {
            // render it
            glVertex2f(star->x, star->y);
            glVertex2f(star->x + star->size, star->y);
            glVertex2f(star->x + star->size, star->y + star->size);
            glVertex2f(star->x, star->y + star->size);
        }
        glEnd();

        //end rendering
        glPopMatrix();
        SDL_GL_SwapBuffers();
    }

    return 0;
}

void init()
{
    // initialize SDL and OpenGL stuff
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_WM_SetCaption("Space Simulation Test", NULL);
    SDL_SetVideoMode(MAP_W, MAP_H, 32, SDL_OPENGL);
    glClearColor(0, 0, 0, 1);
    glViewport(0, 0, MAP_W, MAP_H);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
}

void quit()
{
    SDL_Quit();
}

bool isOutside(const shared_ptr<Star> s)
{
    return (((s->x > MAP_W) || (s->x < 0))
        || ((s->y > MAP_H) || (s->y < 0)));
}
