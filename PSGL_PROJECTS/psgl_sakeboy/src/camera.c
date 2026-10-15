#include "common.h"


void camera_apply(float x, float y, float z, float rotX, float rotY, float rotZ)
{
    /* Сдвигаем камеру/мир */
    glTranslatef(x, y, z);
    
    /* Поворачиваем мир вокруг осей */
    if (rotX != 0.0f) glRotatef(rotX, 1.0f, 0.0f, 0.0f);
    if (rotY != 0.0f) glRotatef(rotY, 0.0f, 1.0f, 0.0f);
    if (rotZ != 0.0f) glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
}
