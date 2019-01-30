#include <stdio.h>
#include <graph.h>
#include <math.h>

#define NUM_ASTEROIDS 10
#define ERASE 0
#define DRAW 1

#define X_COMP 0
#define Y_COMP 1
#define N_COMP 2

// The structure for a vertex
typedef struct vertex_typ
{
    float p[3];
} vertex, *vertex_ptr;

// Matrix structure
typedef struct matrix_typ
{
    float elem[3][3];
} matrix, *matrix_ptr;

// The structure for an object
typedef struct object_typ
{
    int num_vertices;
    int color;
    float x_pos, y_pos;
    float x_vel, y_vel;
    matrix scale;
    matrix rotation;
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

void Make_Identity(matrix_ptr m)
{
    // Turn into identity matrix
    m->elem[0][0] = m->elem[1][1] = m->elem[2][2] = 1;
    m->elem[0][1] = m->elem[1][0] = m->elem[1][2] = 0;
    m->elem[0][2] = m->elem[2][1] = m->elem[2][0] = 0;
}

void Clear_Matrix(matrix_ptr m)
{
    // Zero out matrix
    m->elem[0][0] = m->elem[1][1] = m->elem[2][2] = 0;
    m->elem[0][1] = m->elem[1][0] = m->elem[1][2] = 0;
    m->elem[0][2] = m->elem[2][1] = m->elem[2][0] = 0;
}

void Multiply_Matrix(vertex_ptr v, matrix_ptr m)
{
    float x_new, y_new;

    // Do multiplication of a 1x3 * 3x3
    x_new = v->p[0] * m->elem[0][0] + v->p[1] * m->elem[1][0] + m->elem[2][0];
    y_new = v->p[0] * m->elem[0][1] + v->p[1] * m->elem[1][1] + m->elem[2][1];

    v->p[X_COMP] = x_new;
    v->p[Y_COMP] = y_new;
}

void Scale_Object(object_ptr object)
{
    int i;

    // Scale x and y on all vertices
    for (i = 0; i < object->num_vertices; i++)
    {
        Multiply_Matrix((vertex_ptr)&object->vertices[i], (matrix_ptr)&object->scale);
    }
}

void Rotate_Object(object_ptr object)
{
    int i;

    // Rotate each vertex by angle
    for (i = 0; i < object->num_vertices; i++)
    {
        Multiply_Matrix((vertex_ptr)&object->vertices[i], (matrix_ptr)&object->rotation);
    }
}

void Create_Field(void)
{
    int i;
    float angle, c, s;

    for (i = 0; i < NUM_ASTEROIDS; i++)
    {
        asteroids[i].num_vertices = 6;
        asteroids[i].color = 1 + rand() % 14; // Keep always visible
        asteroids[i].x_pos = 41 + rand() % 599;
        asteroids[i].y_pos = 41 + rand() % 439;
        asteroids[i].x_vel = -10 + rand() % 20;
        asteroids[i].y_vel = -10 + rand() % 20;
        
        // Setup rotation
        Make_Identity((matrix_ptr)&asteroids[i].rotation);

        angle = (float)(-50 + (float)(rand() % 100)) / 100;
        c = cos(angle);
        s = sin(angle);

        asteroids[i].rotation.elem[0][0] = c;
        asteroids[i].rotation.elem[0][1] = -s;
        asteroids[i].rotation.elem[1][0] = s;
        asteroids[i].rotation.elem[1][1] = c;

        // Setup scaling
        Make_Identity((matrix_ptr)&asteroids[i].scale);
        asteroids[i].scale.elem[0][0] = (float)(rand() % 30) / 10;
        asteroids[i].scale.elem[1][1] = asteroids[i].scale.elem[0][0];

        asteroids[i].vertices[0].p[X_COMP] = 4.0;
        asteroids[i].vertices[0].p[Y_COMP] = 3.5;
        asteroids[i].vertices[0].p[N_COMP] = 1;

        asteroids[i].vertices[1].p[X_COMP] = 8.5;
        asteroids[i].vertices[1].p[Y_COMP] = -3.0;
        asteroids[i].vertices[1].p[N_COMP] = 1;

        asteroids[i].vertices[2].p[X_COMP] = 6;
        asteroids[i].vertices[2].p[Y_COMP] = -5;
        asteroids[i].vertices[2].p[N_COMP] = 1;

        asteroids[i].vertices[3].p[X_COMP] = 2;
        asteroids[i].vertices[3].p[Y_COMP] = -3;
        asteroids[i].vertices[3].p[N_COMP] = 1;

        asteroids[i].vertices[4].p[X_COMP] = -4;
        asteroids[i].vertices[4].p[Y_COMP] = -6;
        asteroids[i].vertices[4].p[N_COMP] = 1;

        asteroids[i].vertices[5].p[X_COMP] = -3.5;
        asteroids[i].vertices[5].p[Y_COMP] = 5.5;
        asteroids[i].vertices[5].p[N_COMP] = 1;

        Scale_Object((object_ptr)&asteroids[i]);
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
        _moveto((int)(xo + asteroids[i].vertices[0].p[X_COMP]), (int)(yo + asteroids[i].vertices[0].p[Y_COMP]));

        for (v = 1; v < asteroids[i].num_vertices; v++)
        {
            _lineto((int)(xo + asteroids[i].vertices[v].p[X_COMP]), (int)(yo + asteroids[i].vertices[v].p[Y_COMP]));
        }

        _lineto((int)(xo + asteroids[i].vertices[0].p[X_COMP]), (int)(yo + asteroids[i].vertices[0].p[Y_COMP]));
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

void Rotate_Asteroids(void)
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++)
    {
        Rotate_Object((object_ptr)&asteroids[i]);
    }
}

void main(void)
{
    _setvideomode(_VRES16COLOR); // 640x480 16 colors

    Create_Field();

    while(!kbhit())
    {
        Draw_Asteroids(ERASE);

        Rotate_Asteroids();
        Translate_Asteroids();

        Draw_Asteroids(DRAW);

        // Wait a bit to prevent de-sync
        Delay(500);
    }

    _setvideomode(_DEFAULTMODE); // Text mode
    
}

