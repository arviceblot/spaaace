/**
 * Attempt at an OpenGL "hypespace" simluation.
 * Compile for Linux:
 *     g++ -o space space.cpp -lSDLmain -lSDL -lGL
 */

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <list>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

using namespace std;

const static int STARS = 2500;
const static int MAP_W = 800;
const static int MAP_H = 600;
const static int CENTER_X = MAP_W / 2;
const static int CENTER_Y = MAP_H / 2;

class Star {
public:
    Star();
    virtual ~Star();

    void updateSize();

    float x, y, speed;
    float vector[2];
    int size;
};

class IsOutside {
public:
    IsOutside();
    bool operator() (const Star& s);
};


void init();
void quit();

int main() {
    init();

    SDL_Event event;
    bool isRunning = true;
    int frameStart = 0;
    int frameEnd = 0;
    int deltaTimeMS = 0;
    float deltaTimeS = 0.0;

    //create initial stars
    list<Star> stars;
    for (int i = 0; i < STARS; i++) {
        Star star;
        star.x = rand() % MAP_W;
        star.y = rand() % MAP_H;
        star.speed = rand() % 1000;
        star.vector[0] = (star.x - CENTER_X) / star.speed;
        star.vector[1] = (star.y - CENTER_Y) / star.speed;
        star.size = 1;
        stars.push_back(star);
    }

    //run the simulation
    while (isRunning) {
        //Input
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                isRunning = false;
            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    isRunning = false;
            }
        }

        // to maintain a constant number of stars, delete any that are off
        // the map then create as many as were deleted
        stars.remove_if(IsOutside());
        int toAdd = STARS - stars.size();

        for (int i = 0; i < toAdd; i++) {
            Star star;
            star.x = rand() % MAP_W;
            star.y = rand() % MAP_H;
            star.speed = rand() % 1000;
            star.vector[0] = (star.x - CENTER_X) / star.speed;
            star.vector[1] = (star.y - CENTER_Y) / star.speed;
            star.updateSize();
            stars.push_back(star);
        }

        frameStart = SDL_GetTicks();
        deltaTimeS = (float) (deltaTimeMS) / 1000;

        //render all the stars
        glClear(GL_COLOR_BUFFER_BIT);
        glPushMatrix();
        glOrtho(0, MAP_W, MAP_H, 0, -1, 1);

        //star
        glColor4ub(255,255,255,255);
        glBegin(GL_QUADS);
        for (list<Star>::iterator i = stars.begin(); i != stars.end(); i++) {
            // calculate the new position
            i->vector[0] = (i->x - CENTER_X) / i->speed;
            i->vector[1] = (i->y - CENTER_Y) / i->speed;
            i->x = i->x + i->vector[0];
            i->y = i->y + i->vector[1];
            i->updateSize();
            // render it
            glVertex2f(i->x, i->y);
            glVertex2f(i->x + i->size, i->y);
            glVertex2f(i->x + i->size, i->y + i->size);
            glVertex2f(i->x, i->y + i->size);
        }
        glEnd();

        //end rendering
        glPopMatrix();
        SDL_GL_SwapBuffers();
    }

    return 0;
}

void init() {
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

void quit() {
    SDL_Quit();
}

Star::Star() {
    x = 0;
    y = 0;
    speed = 0;
    vector[0] = 0;
    vector[1] = 0;
    size = 1;
}

Star::~Star() {;}

void Star::updateSize() {
    float length = sqrt( pow(vector[0], 2) + pow(vector[1], 2) );
    if (length >= 3) {
        size = 3;
    }
    else if (length >= 1) {
        size = 2;
    }
}

IsOutside::IsOutside() {;}

bool IsOutside::operator() (const Star& s) {
    if ( (s.x > MAP_W) || (s.x < 0) )
        return true;
    else if ( (s.y > MAP_H) || (s.y < 0) )
        return true;
    else
        return false;
}
