#include "common.h"

/* ИСПРАВЛЕНО: Вершины переписаны строго ПРОТИВ ЧАСОВОЙ СТРЕЛКИ (лицевая сторона) 
   и приподняты до -1.8f, чтобы плоскость была отлично видна под кубом */
static const GLfloat grass_vertices[] = {
    -10.0f, -1.8f,  10.0f,  /* Передний левый угол */
     10.0f, -1.8f,  10.0f,  /* Передний правый угол */
     10.0f, -1.8f, -10.0f,  /* Задний правый угол */
    -10.0f, -1.8f, -10.0f   /* Задний левый угол */
};

/* Цвета вершин (RGBA) — сочный зеленый градиент */
static const GLubyte grass_colors[] = {
    34,  139, 34,  255,     /* Лесной зеленый */
    50,  205, 50,  255,     /* Лаймовый зеленый */
    34,  139, 34,  255,
    50,  205, 50,  255
};

void grass_draw(void)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, grass_vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, grass_colors);

    /* Рисуем плоскость */
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
