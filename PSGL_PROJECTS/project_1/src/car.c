#include "common.h"
#include "car.h"

#define PI 3.14159265f

/* Базовый полигональный блок для моделирования деталей кузова */
static const GLfloat supra_cube[] = {
    -0.5f,-0.5f, 0.5f,   0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
     0.5f,-0.5f,-0.5f,  -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,   0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,   0.5f, 0.5f, 0.5f,   0.5f, 0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,  -0.5f,-0.5f, 1.0f,
     0.5f,-0.5f, 0.5f,   0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,   0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,  -0.5f,-0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,-0.5f
};

/* Функция рисования элементов Супры (серый металлик и другие цвета) */
static void draw_supra_part(float sx, float sy, float sz, GLubyte r, GLubyte g, GLubyte b)
{
    GLubyte colors[24 * 4];
    for (int i = 0; i < 24; i++) {
        colors[i*4] = r; colors[i*4+1] = g; colors[i*4+2] = b; colors[i*4+3] = 255;
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, supra_cube);
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
    glPushMatrix();
        glScalef(sx, sy, sz);
        for (int i = 0; i < 6; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
        }
    glPopMatrix();
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/* Отрисовка колеса: теперь это боковой плоский 3D-диск с шиной, жестко зафиксированный по осям */
static void draw_supra_wheel(float radius, float width, float rot)
{
    const int segments = 16;
    GLfloat vertices[(16 + 1) * 2 * 3];
    GLubyte colors[(16 + 1) * 2 * 4];
    int v_idx = 0; int c_idx = 0;

    for (int i = 0; i <= segments; i++) {
        float angle = (2.0f * PI * i) / segments;
        float c = cosf(angle); float s = sinf(angle);

        // Внутренний обод колеса (прилегает к ступице)
        vertices[v_idx++] = 0.0f; vertices[v_idx++] = c * radius; vertices[v_idx++] = s * radius;
        colors[c_idx++] = 30; colors[c_idx++] = 30; colors[c_idx++] = 30; colors[c_idx++] = 255; /* Черная резина */

        // Внешний протектор шины
        vertices[v_idx++] = width; vertices[v_idx++] = c * radius; vertices[v_idx++] = s * radius;
        colors[c_idx++] = 50; colors[c_idx++] = 50; colors[c_idx++] = 50; colors[c_idx++] = 255;
    }
    glPushMatrix();
        /* ИСПРАВЛЕНО: Сначала крутим колесо вокруг оси X (оно катится вперед) */
        glRotatef(rot, 1.0f, 0.0f, 0.0f); 

        /* ИСПРАВЛЕНО: Только ПОСЛЕ этого разворачиваем его боком параллельно кузову Супры */
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glColorPointer(4, GL_UNSIGNED_BYTE, 0, colors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (segments + 1) * 2);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void car_draw(float time)
{
    float wheel_rot = time * -4.0f;
    
    /* Базовые цвета для культовой Toyota Supra */
    GLubyte body_r = 140, body_g = 146, body_b = 153; /* Фирменный серый металлик (Silver Metallic) */
    GLubyte glass_r = 25,  glass_g = 30,  glass_b = 35;   /* Глухая тонировка стекол */

    glPushMatrix();
        /* Ставим машину на лужайку справа от человека */
        glTranslatef(1.8f, -0.6f, 0.0f);

        // 1. СИЛУЭТ КУЗОВА (Обтекаемая нижняя плита Супры)
        glPushMatrix();
            glTranslatef(0.0f, 0.25f, 0.0f);
            draw_supra_part(2.4f, 0.35f, 1.15f, body_r, body_g, body_b);
        glPopMatrix();

        // 2. СПОРТИВНАЯ КАБИНА (Сглаженная, скошенная назад крыша A80)
        glPushMatrix();
            glTranslatef(-0.1f, 0.55f, 0.0f);
            draw_supra_part(1.1f, 0.32f, 0.98f, glass_r, glass_g, glass_b);
        glPopMatrix();

        // 3. ЛЕГЕНДАРНОЕ АНТИКРЫЛО (Высокий спойлер Supra на багажнике)
        // Левая стойка
        glPushMatrix(); glTranslatef(-1.1f, 0.5f, 0.45f); draw_supra_part(0.1f, 0.25f, 0.08f, body_r, body_g, body_b); glPopMatrix();
        // Правая стойка
        glPushMatrix(); glTranslatef(-1.1f, 0.5f, -0.45f); draw_supra_part(0.1f, 0.25f, 0.08f, body_r, body_g, body_b); glPopMatrix();
        // Верхняя горизонтальная плоскость крыла
        glPushMatrix(); glTranslatef(-1.1f, 0.62f, 0.0f); draw_supra_part(0.15f, 0.05f, 1.1f, body_r, body_g, body_b); glPopMatrix();

        // 4. ПЕРЕДНИЕ ФАРЫ (Культовая круглая оптика)
        glPushMatrix(); glTranslatef(1.21f, 0.32f, 0.35f); draw_supra_part(0.02f, 0.12f, 0.15f, 255, 255, 230); glPopMatrix();
        glPushMatrix(); glTranslatef(1.21f, 0.32f, -0.35f); draw_supra_part(0.02f, 0.12f, 0.15f, 255, 255, 230); glPopMatrix();

        // 5. КОЛЕСА (Идеальная центровка внутри арок кузова)
        float w_radius = 0.26f;
        float w_width = 0.16f;

        /* Корректное смещение: ширина колеса (w_width) теперь вычитается для правых колес,
           чтобы они полностью спрятались внутрь кузова и не косились наружу */
        
        // Переднее левое
        glPushMatrix(); glTranslatef(0.75f, 0.14f, 0.56f); draw_supra_wheel(w_radius, w_width, wheel_rot); glPopMatrix();
        // Переднее правое
        glPushMatrix(); glTranslatef(0.75f, 0.14f, -0.56f - w_width); draw_supra_wheel(w_radius, w_width, wheel_rot); glPopMatrix();
        // Заднее левое
        glPushMatrix(); glTranslatef(-0.75f, 0.14f, 0.56f); draw_supra_wheel(w_radius, w_width, wheel_rot); glPopMatrix();
        // Заднее правое
        glPushMatrix(); glTranslatef(-0.75f, 0.14f, -0.56f - w_width); draw_supra_wheel(w_radius, w_width, wheel_rot); glPopMatrix();

    glPopMatrix();
}
