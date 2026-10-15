#ifndef CONTROL_H
#define CONTROL_H

/* Обрабатывает ввод с геймпада и рассчитывает позицию/поворот камеры */
void control_update(void);

/* Функции получения актуальных координат для модуля камеры */
float control_get_cam_x(void);
float control_get_cam_y(void);
float control_get_cam_z(void);
float control_get_cam_rot_x(void);
float control_get_cam_rot_y(void);

/* Возвращает 1, если была нажата кнопка START для выхода */
int control_should_exit(void);

#endif /* CONTROL_H */
