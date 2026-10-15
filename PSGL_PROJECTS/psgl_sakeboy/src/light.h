#ifndef LIGHT_H
#define LIGHT_H

/* Настраивает параметры глобального освещения (солнца) */
void light_init(void);

/* Обновляет позицию солнца на небе (можно передавать углы для движения солнца) */
void light_update_position(float angle);

/* Выключает свет при завершении программы */
void light_shutdown(void);

#endif /* LIGHT_H */
