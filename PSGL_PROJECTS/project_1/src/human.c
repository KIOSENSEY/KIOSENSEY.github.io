#include "common.h"
#include "human.h"

#define PI 3.14159265f

/* Генерация и отрисовка сглаженной 3D-сферы через чистый OpenGL ES 1.1 (без glBegin/glEnd) */
static void draw_sphere(float radius, GLubyte r, GLubyte g, GLubyte b)
{
    const int rings = 10;
    const int sectors = 10;
    
    GLfloat vertices[11 * 11 * 3];
    GLubyte colors[11 * 11 * 4];
    GLushort indices[10 * 10 * 6];
    
    int v_idx = 0;
    int c_idx = 0;
    int i_idx = 0;

    float R = 1.0f / (float)(rings - 1);
    float S = 1.0f / (float)(sectors - 1);

    /* 1. Генерируем массив вершин и цветов */
    for (int r_idx = 0; r_idx < rings; r_idx++) {
        for (int s_idx = 0; s_idx < sectors; s_idx++) {
            float y = sinf(-PI / 2.0f + PI * r_idx * R);
            float x = cosf(2.0f * PI * s_idx * S) * sinf(PI * r_idx * R);
            float z = sinf(2.0f * PI * s_idx * S) * sinf(PI * r_idx * R);

            vertices[v_idx++] = x * radius;
            vertices[v_idx++] = y * radius;
            vertices[v_idx++] = z * radius;

            colors[c_idx++] = r;
            colors[c_idx++] = g;
            colors[c_idx++] = b;
            colors[c_idx++] = 255;
        }
    }

    /* 2. Генерируем индексы треугольников для сетки сферы */
    for (int r_idx = 0; r_idx < rings - 1; r_idx++) {
        for (int s_idx = 0; s_idx < sectors - 1; s_idx++) {
            GLushort k1 = r_idx * sectors + s_idx;
            GLushort k2 = k1 + sectors;

            indices[i_idx++] = k1;
            indices[i_idx++] = k2;
            indices[i_idx++] = k1 + 1;

            indices[i_idx++] = k1 + 1;
            indices[i_idx++] = k2;
            indices[i_idx++] = k2 + 1;
        }
    }

    /* 3. Передаем массивы в видеочип RSX и рисуем одной командой */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);

    /* Аппаратная отрисовка проиндексированных треугольников */
    glDrawElements(GL_TRIANGLES, i_idx, GL_UNSIGNED_SHORT, indices);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/* Отрисовка сглаженного 3D-цилиндра через Triangle Strip */
static void draw_cylinder(float base_radius, float top_radius, float height, GLubyte r, GLubyte g, GLubyte b)
{
    const int segments = 12;
    GLfloat vertices[(12 + 1) * 2 * 3];
    GLubyte colors[(12 + 1) * 2 * 4];
    int v_idx = 0;
    int c_idx = 0;

    for (int i = 0; i <= segments; i++) {
        float angle = (2.0f * PI * i) / segments;
        float c = cosf(angle);
        float s = sinf(angle);

        // Нижняя точка
        vertices[v_idx++] = c * base_radius;
        vertices[v_idx++] = s * base_radius;
        vertices[v_idx++] = 0.0f;
        colors[c_idx++] = r; colors[c_idx++] = g; colors[c_idx++] = b; colors[c_idx++] = 255;

        // Верхняя точка
        vertices[v_idx++] = c * top_radius;
        vertices[v_idx++] = s * top_radius;
        vertices[v_idx++] = height;
        colors[c_idx++] = r; colors[c_idx++] = g; colors[c_idx++] = b; colors[c_idx++] = 255;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void human_draw(float time)
{
    float walk_angle = sinf(time * 0.05f) * 28.0f; 

    glPushMatrix();
        glTranslatef(-2.0f, -0.2f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        // 1. ТОРС
        glPushMatrix();
            draw_cylinder(0.35f, 0.45f, 1.2f, 30, 144, 255);
        glPopMatrix();

        // 2. ШЕЯ И ГОЛОВА
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.2f);
            draw_cylinder(0.12f, 0.12f, 0.15f, 255, 218, 185);
            
            glTranslatef(0.0f, 0.0f, 0.35f);
            draw_sphere(0.28f, 255, 218, 185);
        glPopMatrix();

        // 3. ЛЕВАЯ РУКА
        glPushMatrix();
            glTranslatef(-0.55f, 0.0f, 1.0f);
            draw_sphere(0.12f, 30, 144, 255);
            glRotatef(walk_angle, 0.0f, 1.0f, 0.0f);
            glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
            draw_cylinder(0.1f, 0.08f, 0.7f, 220, 20, 60);
        glPopMatrix();

        // 4. ПРАВАЯ РУКА
        glPushMatrix();
            glTranslatef(0.55f, 0.0f, 1.0f);
            draw_sphere(0.12f, 30, 144, 255);
            glRotatef(-walk_angle, 0.0f, 1.0f, 0.0f);
            glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
            draw_cylinder(0.1f, 0.08f, 0.7f, 220, 20, 60);
        glPopMatrix();

        // 5. ЛЕВАЯ НОГА
        glPushMatrix();
            glTranslatef(-0.25f, 0.0f, 0.0f);
            glRotatef(-walk_angle, 0.0f, 1.0f, 0.0f);
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f); 
            draw_cylinder(0.14f, 0.1f, 0.9f, 105, 105, 105);
        glPopMatrix();

        // 6. ПРАВАЯ НОГА
        glPushMatrix();
            glTranslatef(0.25f, 0.0f, 0.0f);
            glRotatef(walk_angle, 0.0f, 1.0f, 0.0f);
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
            draw_cylinder(0.14f, 0.1f, 0.9f, 105, 105, 105);
        glPopMatrix();

    glPopMatrix();
}
