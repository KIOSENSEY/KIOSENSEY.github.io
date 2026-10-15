#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

/* Структура для хранения состояния конкретного геймпада */
typedef struct {
    uint32_t buttons;       /* Битовая маска нажатых кнопок */
    
    /* Координаты аналоговых стиков (нейтраль ~128) */
    uint8_t left_stick_x;
    uint8_t left_stick_y;
    uint8_t right_stick_x;
    uint8_t right_stick_y;
    
    int is_connected;       /* Флаг подключения геймпада */
} PadState;

/* Инициализирует системный модуль геймпадов */
int32_t input_init(void);

/* Опрашивает состояние всех подключенных геймпадов */
void input_update(void);

/* Возвращает состояние конкретного геймпада по его индексу (0 - 6) */
PadState* input_get_pad(uint32_t pad_index);

/* Деинициализирует модуль геймпадов при выходе */
void input_shutdown(void);

#endif /* INPUT_H */
