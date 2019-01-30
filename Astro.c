#include <stdio.h>
#include <graph.h>
#include <math.h>

#define NUM_ASTEROIDS 10
#define ERASE 0
#define DRAW 1

// The structure for a vertex
typedef struct vertex_typ
{
    float x, y;
} vertex, *vertex_ptr;

// The structure for an object
typedef struct object_typ
{
    int num_vertices;
    int color;
    float x_pos, y_pos;
    float x_vel, y_vel;
    float scale;
    vertex vertices[16];
} object, *object_ptr;

object asteroids[NUM_ASTEROIDS];

void Delay(int t)
{
    // Take up compute cycles
    float x = 1;

    while(t-->0)
        x = cos(x);
}

void Scale_Object(object_ptr object, float scale)
{
    int i;

    // Scale x and y on all vertices
    for (i = 0; i < object->num_vertices; i++)
    {
        object->vertices[i].x *= scale;
        object->vertices[i].y *= scale;
    }
}

void Create_Field(void)
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++)
    {
        asteroids[i].num_vertices = 6;
        asteroids[i].color = 1 + rand() % 14; // Keep always visible
        asteroids[i].x_pos = 41 + rand() % 599;
        asteroids[i].y_pos = 41 + rand() % 439;

        asteroids[i].x_vel = -10 + rand() % 20;
        asteroids[i].y_vel = -10 + rand() % 20;
        asteroids[i].scale = (float)(rand() % 30) / 10;

        asteroids[i].vertices[0].x = 4.0;
        asteroids[i].vertices[0].y = 3.5;
        asteroids[i].vertices[1].x = 8.5;
        asteroids[i].vertices[1].y = -3.0;
        asteroids[i].vertices[2].x = 6;
        asteroids[i].vertices[2].y = -5;
        asteroids[i].vertices[3].x = 2;
        asteroids[i].vertices[3].y = -3;
        asteroids[i].vertices[4].x = -4;
        asteroids[i].vertices[4].y = -6;
        asteroids[i].vertices[5].x = -3.5;
        asteroids[i].vertices[5].y = 5.5;

        Scale_Object((object_ptr)&asteroids[i], asteroids[i].scale);
    }
}

void Draw_Asteroids(int erase)
{
    int i, v; // Index and vertex
    float xo, yo;

    for (i = 0; i < NUM_ASTEROIDS; i++)
    {
        // Draw or erase
        if (erase == ERASE)
            _setcolor(0);
        else
            _setcolor(asteroids[i].color);

        xo = asteroids[i].x_pos;
        yo = asteroids[i].y_pos;

        // Move to first vertex
        _moveto((int)(xo + asteroids[i].vertices[0].x), (int)(yo + asteroids[i].vertices[0].y));

        for (v = 1; v < asteroids[i].num_vertices; v++)
        {
            _lineto((int)(xo + asteroids[i].vertices[v].x), (int)(yo + asteroids[i].vertices[v].y));
        }

        _lineto((int)(xo + asteroids[i].vertices[0].x), (int)(yo + asteroids[i].vertices[0].y));
    }
}

void Translate_Asteroids()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++)
    {
        asteroids[i].x_pos += asteroids[i].x_vel;
        asteroids[i].y_pos += asteroids[i].y_vel;

        // Collide with screen boundaries
        if (asteroids[i].x_pos > 600 || asteroids[i].x_pos < 40)
        {
            asteroids[i].x_vel = -asteroids[i].x_vel;
            asteroids[i].x_pos += asteroids[i].x_vel;
        }

        if (asteroids[i].y_pos > 440 || asteroids[i].y_pos < 40)
        {
            asteroids[i].y_vel = -asteroids[i].y_vel;
            asteroids[i].y_pos += asteroids[i].y_vel;
        }
    }
}

void main(void)
{
    _setvideomode(_VRES16COLOR); // 640x480 16 colors

    Create_Field();

    while(!kbhit())
    {
        Draw_Asteroids(ERASE);

        Translate_Asteroids();

        Draw_Asteroids(DRAW);

        // Wait a bit to prevent de-sync
        Delay(500);
    }

    _setvideomode(_DEFAULTMODE); // Text mode
    
}

