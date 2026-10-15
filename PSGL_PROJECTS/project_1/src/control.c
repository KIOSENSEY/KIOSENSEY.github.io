#include "common.h"

/* Внутренние переменные положения и углов камеры */
static float cam_x = 0.0f;
static float cam_y = 0.0f;
static float cam_z = -6.0f;
static float cam_rot_x = 0.0f;
static float cam_rot_y = 0.0f;
static int flag_exit = 0;

void control_update(void)
{
    /* Сначала обновляем состояние самих геймпадов */
    input_update();

    /* Опрашиваем первый подключенный геймпад */
    PadState* pad = input_get_pad(0);
    if (pad && pad->is_connected) {
        
        /* Кнопка START (выход) */
        if (pad->buttons & (1 << 3)) {
            flag_exit = 1;
        }
        
        /* ЛЕВЫЙ СТИК: Вращение камеры (Поворот / Обзор) */
        if (pad->left_stick_x < 80)  cam_rot_y -= 1.0f;
        if (pad->left_stick_x > 170) cam_rot_y += 1.0f;
        if (pad->left_stick_y < 80)  cam_rot_x -= 1.0f;
        if (pad->left_stick_y > 170) cam_rot_x += 1.0f;

        /* ПРАВЫЙ СТИК: Перемещение (Влево/Вправо и Вперед/Назад) */
        if (pad->right_stick_x < 80)  cam_x -= 0.05f;
        if (pad->right_stick_x > 170) cam_x += 0.05f;
        if (pad->right_stick_y < 80)  cam_z += 0.05f;
        if (pad->right_stick_y > 170) cam_z -= 0.05f;
    }
}

float control_get_cam_x(void)     { return cam_x; }
float control_get_cam_y(void)     { return cam_y; }
float control_get_cam_z(void)     { return cam_z; }
float control_get_cam_rot_x(void) { return cam_rot_x; }
float control_get_cam_rot_y(void) { return cam_rot_y; }
int control_should_exit(void)     { return flag_exit; }
