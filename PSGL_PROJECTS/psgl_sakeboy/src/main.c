#include "common.h"

static int32_t receive_exit_game_request = 0;
static sys_memory_container_t sysutil_container = 0;
static sys_ppu_thread_t sysutil_thread_id;
static int32_t sysutil_thread_run = 1;

static void sysutil_callback(uint64_t status, uint64_t param, void* userdata)
{
    (void)param; (void)userdata;
    if (status == CELL_SYSUTIL_REQUEST_EXITGAME) {
        receive_exit_game_request = 1;
    }
}

static void sysutil_thread_entry(uint64_t arg)
{
    (void)arg;
    cellSysutilRegisterCallback(0, sysutil_callback, NULL);
    while (sysutil_thread_run) {
        cellSysutilCheckCallback();
        sys_timer_usleep(16000); 
    }
    cellSysutilUnregisterCallback(0);
    sys_ppu_thread_exit(0);
}
static ObjModel my_custom_model;
int main(void)
{
    int32_t ret;

    ret = sys_memory_container_create(&sysutil_container, 2 * 1024 * 1024);
    if (ret != CELL_OK) return -1;

    ret = init_video();
    if (ret != 0) {
        sys_memory_container_destroy(sysutil_container);
        return -2;
    }

    ret = input_init();
    if (ret != 0) {
        shutdown_video();
        sys_memory_container_destroy(sysutil_container);
        return -4;
    }

    ret = sys_ppu_thread_create(&sysutil_thread_id, sysutil_thread_entry, 0,
                                1001, 0x4000, SYS_PPU_THREAD_CREATE_JOINABLE, "sysutil");
    if (ret != CELL_OK) {
        input_shutdown();
        shutdown_video();
        sys_memory_container_destroy(sysutil_container);
        return -3;
    }

    matrix_set_perspective();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
	light_init();
    float anim_time = 0.0f;

	obj_load(&my_custom_model, "/dev_hdd0/game/000000007/USRDIR/sakeboy.obj", "/dev_hdd0/game/000000007/USRDIR/sakeboy.tga");
    /* Главный цикл */
    while (!receive_exit_game_request && !control_should_exit()) {
        control_update();

        clear_screen(0.15f, 0.15f, 0.15f, 1.0f);

        matrix_reset_modelview();
        
        camera_apply(
            control_get_cam_x(), 
            control_get_cam_y(), 
            control_get_cam_z(), 
            control_get_cam_rot_x(), 
            control_get_cam_rot_y(), 
            0.0f
        );
        /* ИСПРАВЛЕНО: Обновляем положение солнца в мире сразу после сдвига камеры */
        light_update_position(anim_time);
        /* 1. Отрисовка земли */
        grass_draw();

		obj_draw(&my_custom_model, 0.0f, 0.1f, 0.0f, 1.5f, 0.0);


        update_screen();

        /* Обновляем таймер общей анимации сцены */
        anim_time += 1.0f;

        sys_timer_usleep(1000); 
    }
	obj_free(&my_custom_model);
    sysutil_thread_run = 0;
    uint64_t thread_exit_status;
    sys_ppu_thread_join(sysutil_thread_id, &thread_exit_status);
	light_shutdown();
    input_shutdown();
    shutdown_video();
    
    if (sysutil_container != 0) {
        sys_memory_container_destroy(sysutil_container);
    }

    return 0;
}
