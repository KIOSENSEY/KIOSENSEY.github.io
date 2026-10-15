#include "common.h" /* Подключаем всё одной строчкой */

/* Ручное объявление системной функции RESC для безопасного гашения экрана */
#ifdef __cplusplus
extern "C" {
#endif
void cellRescExit(void);
#ifdef __cplusplus
}
#endif

/* ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ МОДУЛЯ */
static PSGLdevice*  gl_device  = NULL;
static PSGLcontext* gl_context = NULL;
static GLuint       screen_width  = 0;
static GLuint       screen_height = 0;

int32_t init_video(void)
{
    int32_t ret;

    /* 1. Загрузка графического модуля ядра */
    ret = cellSysmoduleLoadModule(CELL_SYSMODULE_GCM);
    if (ret != CELL_OK) return -1;

    /* 2. Конфигурация инициализации PSGL */
    PSGLinitOptions initOptions;
    memset(&initOptions, 0, sizeof(PSGLinitOptions));
    initOptions.enable = PSGL_INIT_MAX_SPUS | PSGL_INIT_INITIALIZE_SPUS | PSGL_INIT_HOST_MEMORY_SIZE;
    initOptions.maxSPUs = 1;
    initOptions.initializeSPUs = 0;
    initOptions.hostMemorySize = 64 * 1024 * 1024;

    psglInit(&initOptions);

    /* 3. Настройка параметров устройства вывода */
    PSGLdeviceParameters devParams;
    memset(&devParams, 0, sizeof(PSGLdeviceParameters));
    devParams.enable = PSGL_DEVICE_PARAMETERS_COLOR_FORMAT | PSGL_DEVICE_PARAMETERS_DEPTH_FORMAT;
    devParams.colorFormat = GL_ARGB_SCE;
    devParams.depthFormat = GL_DEPTH_COMPONENT24;

    gl_device = psglCreateDeviceExtended(&devParams);
    if (!gl_device) return -2;

    /* 4. Создание и привязка контекста OpenGL */
    gl_context = psglCreateContext();
    if (!gl_context) return -3;

    psglMakeCurrent(gl_context, gl_device);

    /* 5. Получение и настройка размеров области вывода */
    psglGetRenderBufferDimensions(gl_device, &screen_width, &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    return 0;
}

void clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void update_screen(void)
{
    psglSwap();
}

void shutdown_video(void)
{
    if (gl_context) {
        glFinish(); 
        cellRescExit();
        psglMakeCurrent(NULL, NULL);
        psglDestroyContext(gl_context);
        gl_context = NULL;
    }
    /* ... остальное без изменений ... */
}
uint32_t get_screen_width(void)  { return (uint32_t)screen_width; }
uint32_t get_screen_height(void) { return (uint32_t)screen_height; }
