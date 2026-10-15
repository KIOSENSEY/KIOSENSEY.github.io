#ifndef COMMON_H
#define COMMON_H

/* Стандартные библиотеки Си */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

/* Системные потоки, память и таймеры ядра PS3 (LV2) */
#include <sys/process.h>
#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/return_code.h>
#include <sys/synchronization.h>
#include <sys/memory.h> 

/* Системные утилиты оверлея XMB и модулей */
#include <cell/sysmodule.h>
#include <sysutil/sysutil_sysparam.h>
#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_common.h>

/* Графика PSGL (OpenGL ES 1.1) */
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

/* Системная библиотека геймпада */
#include <cell/pad.h>

/* НАШИ ИГРОВЫЕ МОДУЛИ */
#include "video.h"
#include "matrix.h"
#include "camera.h"
#include "input.h"
#include "grass.h" /* ДОБАВЛЕНО */
#include "control.h" /* ДОБАВЛЕНО */
#include "human.h" /* ДОБАВЛЕНО */
#include "car.h" /* ДОБАВЛЕНО */
#include "obj_loader.h" /* ДОБАВЛЕНО */
#include "light.h" /* ДОБАВЛЕНО */
/* КРИТИЧЕСКИ ВАЖНО: Эта строчка закрывает #ifndef COMMON_H */
#endif 
