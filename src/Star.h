#ifndef __STAR_H__
#define __STAR_H__

class Star
{
public:
    Star();
    virtual ~Star();

    void updateSize();

    float x, y, speed;
    float velocity[2];
    int size;
};

#endif
