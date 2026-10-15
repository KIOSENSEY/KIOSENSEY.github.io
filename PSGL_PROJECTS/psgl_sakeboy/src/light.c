#include "common.h"
#include "light.h"

void light_init(void)
{
    /* 1. Настраиваем цвета солнечного света (значения RGBA от 0.0 до 1.0) */
    GLfloat ambient_light[]  = { 0.3f, 0.3f, 0.35f, 1.0f }; /* Мягкий синеватый заполняющий свет теней */
    GLfloat diffuse_light[]  = { 1.0f, 1.0f, 0.9f,  1.0f }; /* Теплый желтоватый прямой свет солнца */
    GLfloat specular_light[] = { 1.0f, 1.0f, 1.0f,  1.0f }; /* Белые блики на металле кузова */

    /* Передаем параметры в первый аппаратный источник света (Солнце) */
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    /* 2. Включаем аппаратный расчет освещения в видеочипе RSX */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /* 3. Включаем привязку цвета вершин к свету (в GLES 1.1 этого вызова достаточно) */
    glEnable(GL_COLOR_MATERIAL);

    /* Настраиваем мягкое глобальное освещение сцены по умолчанию */
    glShadeModel(GL_SMOOTH);
}

void light_update_position(float angle)
{
    /* Позиция солнца на небе (X, Y, Z, W) */
    /* Если W = 0.0f, свет направленный (бесконечно удаленный, как солнце) */
    GLfloat sun_position[] = { 2.0f, 5.0f, 3.0f, 0.0f };
    
    /* Обновляем позицию солнца в мировых координатах */
    glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
}

void light_shutdown(void)
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}
