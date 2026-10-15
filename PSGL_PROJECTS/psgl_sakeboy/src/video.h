#ifndef VIDEO_H
#define VIDEO_H

#include <stdint.h>

/* Инициализирует графический движок PSGL и видеовыход */
int32_t init_video(void);

/* Очищает экран заданным цветом (значения от 0.0f до 1.0f) */
void clear_screen(float r, float g, float b, float a);

/* Переключает буферы кадра (выводить изображение на телевизор) */
void update_screen(void);

/* Корректно закрывает графический движок и освобождает память */
void shutdown_video(void);

/* Возвращает физические размеры экрана, выбранные консолью */
uint32_t get_screen_width(void);
uint32_t get_screen_height(void);

#endif /* VIDEO_H */
