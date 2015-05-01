#include "Star.h"

#include <cmath>

using namespace std;

Star::Star()
{
    x = 0;
    y = 0;
    speed = 0;
    velocity[0] = 0;
    velocity[1] = 0;
    size = 1;
}

Star::~Star() {;}

void Star::updateSize()
{
    float length = sqrt(pow(velocity[0], 2) + pow(velocity[1], 2));
    if (length >= 3)
    {
        size = 3;
    }
    else if (length >= 1)
    {
        size = 2;
    }
}
