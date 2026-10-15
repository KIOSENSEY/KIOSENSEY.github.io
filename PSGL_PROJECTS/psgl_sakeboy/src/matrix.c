#include "common.h"

void matrix_set_perspective(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)get_screen_width() / (float)get_screen_height();
    
    /* Математический аналог gluPerspective(45.0f, aspect, 0.1f, 100.0f) */
    float fov = 45.0f * 3.14159265f / 360.0f;
    float top = 0.1f * tanf(fov);
    float bottom = -top;
    float left = bottom * aspect;
    float right = top * aspect;
    
    glFrustumf(left, right, bottom, top, 0.1f, 100.0f);
}

void matrix_reset_modelview(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
