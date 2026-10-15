#include "common.h"


#define MAX_PADS 7

static PadState g_pads[MAX_PADS];
static int g_input_initialized = 0;

int32_t input_init(void)
{
    int32_t ret;

    ret = cellSysmoduleLoadModule(CELL_SYSMODULE_IO);
    if (ret != CELL_OK) return ret;

    ret = cellPadInit(MAX_PADS);
    if (ret != CELL_OK) {
        cellSysmoduleUnloadModule(CELL_SYSMODULE_IO);
        return ret;
    }

    memset(g_pads, 0, sizeof(g_pads));
    g_input_initialized = 1;
    return 0;
}

void input_update(void)
{
    if (!g_input_initialized) return;

    CellPadInfo2 pad_info;
    if (cellPadGetInfo2(&pad_info) != CELL_OK) return;

    for (uint32_t i = 0; i < MAX_PADS; i++) {
        if (pad_info.port_status[i] == CELL_PAD_STATUS_CONNECTED) {
            CellPadData pad_data;
            
            if (cellPadGetData(i, &pad_data) == CELL_OK && pad_data.len > 0) {
                g_pads[i].is_connected = 1;
                
                /* Считываем 16-битную маску цифровых кнопок */
                g_pads[i].buttons = (pad_data.button[2] << 8) | pad_data.button[3];
                
                /* ИСПРАВЛЕНО: Стики на PS3 считываются из массива button (индексы 4, 5, 6, 7) */
                g_pads[i].left_stick_x  = pad_data.button[4];
                g_pads[i].left_stick_y  = pad_data.button[5];
                g_pads[i].right_stick_x = pad_data.button[6];
                g_pads[i].right_stick_y = pad_data.button[7];
            }
        } else {
            g_pads[i].is_connected = 0;
            g_pads[i].buttons = 0;
            g_pads[i].left_stick_x  = 128;
            g_pads[i].left_stick_y  = 128;
            g_pads[i].right_stick_x = 128;
            g_pads[i].right_stick_y = 128;
        }
    }
}

PadState* input_get_pad(uint32_t pad_index)
{
    if (pad_index >= MAX_PADS) return NULL;
    return &g_pads[pad_index];
}

void input_shutdown(void)
{
    if (!g_input_initialized) return;

    cellPadEnd();
    cellSysmoduleUnloadModule(CELL_SYSMODULE_IO);
    g_input_initialized = 0;
}
