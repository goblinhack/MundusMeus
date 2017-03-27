/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include "main.h"
#include "glapi.h"

void test(void);

static 
void cube (double size)
{
    glBegin(GL_QUADS);
 
    glVertex3f(size,size,size);
    glVertex3f(-size,size,size);
    glVertex3f(-size,-size,size);
    glVertex3f(size,-size,size);
 
    glVertex3f(size,size,-size);
    glVertex3f(-size,size,-size);
    glVertex3f(-size,-size,-size);
    glVertex3f(size,-size,-size);
 
    glVertex3f(size,size,size);
    glVertex3f(size,-size,size);
    glVertex3f(size,-size,-size);
    glVertex3f(size,size,-size);
 
    glVertex3f(-size,size,size);
    glVertex3f(-size,-size,size);
    glVertex3f(-size,-size,-size);
    glVertex3f(-size,size,-size);
 
    glVertex3f(size,size,size);
    glVertex3f(-size,size,size);
    glVertex3f(-size,size,-size);
    glVertex3f(size,size,-size);
 
    glVertex3f(size,-size,size);
    glVertex3f(-size,-size,size);
    glVertex3f(-size,-size,-size);
    glVertex3f(size,-size,-size);
 
    glEnd();
}

void test (void)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
    glBindTexture(GL_TEXTURE_2D, 0);
    cube(1.0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 
}
