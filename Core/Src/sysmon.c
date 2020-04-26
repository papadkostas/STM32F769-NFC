/**
 * @file lv_sysmon.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "sysmon.h"
#if LV_USE_SYSMON

#include "adc.h"
#include <stdio.h>
#include "cpu_utils.h"

/*********************
 *      DEFINES
 *********************/

#define TEMP_REFRESH_PERIOD   1000    /* Internal temperature refresh period */
#define MAX_CONVERTED_VALUE   4095    /* Max converted value */
#define AMBIENT_TEMP            25    /* Ambient Temperature */
#define VSENS_AT_AMBIENT_TEMP  760    /* VSENSE value (mv) at ambient temperature */
#define AVG_SLOPE               25    /* Avg_Solpe multiply by 10 */
#define VREF                  3300

#define CPU_LABEL_COLOR     	"008000"
#define CPU_TEMP_LABEL_COLOR	"FF0000"
#define MEM_LABEL_COLOR     	"0000FF"
#define CHART_POINT_NUM     	100
#define REFR_TIME    			1000
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
void sysmon_task(lv_task_t * param);
static void mem_cpu_tab(lv_obj_t * parent);
static void tasks_tab(lv_obj_t * parent);
static void network_tab(lv_obj_t * parent);
static void win_hide_action(lv_obj_t * btn, lv_event_t event);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t * win;
static lv_obj_t * tabview;
static lv_obj_t * tab1;
static lv_obj_t * tab2;
static lv_obj_t * tab3;
static lv_obj_t * chart;
static lv_chart_series_t * cpu_ser;
static lv_chart_series_t * temp_ser;
static lv_chart_series_t * mem_ser;
static lv_obj_t * info_label;
static lv_task_t * refr_task;
extern const lv_img_dsc_t lv_logo;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the system monitor
 */
void sysmon_create(void)
{
    refr_task = lv_task_create(sysmon_task, REFR_TIME, LV_TASK_PRIO_LOW, NULL);

    tabview = lv_tabview_create(lv_disp_get_scr_act(NULL), NULL);
    lv_obj_set_style_local_pad_right(tabview, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, LV_HOR_RES / 2);
    tab1 = lv_tabview_add_tab(tabview, "CPU & Memory");
    tab2 = lv_tabview_add_tab(tabview, "Tasks");
    tab3 = lv_tabview_add_tab(tabview, "Network");

    lv_obj_t *backbtn = lv_btn_create(tabview, NULL);
    lv_obj_t *lbl = lv_label_create(backbtn, NULL);
    lv_label_set_text(lbl, "Back");
    lv_obj_align(backbtn, tabview, LV_ALIGN_IN_RIGHT_MID, -20, -200);
	lv_obj_set_event_cb(backbtn, win_hide_action);

    mem_cpu_tab(tab1);
    tasks_tab(tab2);
    network_tab(tab3);

    lv_obj_set_hidden(tabview, true);
}

void sysmon_show(bool visibility)
{
	if(visibility == true)
		lv_obj_set_hidden(tabview, false);
	else
		lv_obj_set_hidden(tabview, true);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#define SYSMON_STRING_BUFFER_SIZE 256


static void mem_cpu_tab(lv_obj_t * parent)
{
    lv_coord_t hres = lv_disp_get_hor_res(NULL);
    lv_coord_t vres = lv_disp_get_ver_res(NULL);

    /*Create a chart with two data lines*/
    chart = lv_chart_create(parent, NULL);
    lv_obj_set_size(chart, hres / 2, vres / 2);
    lv_obj_set_pos(chart, LV_DPI / 10, LV_DPI / 10);
    lv_chart_set_point_count(chart, CHART_POINT_NUM);
    lv_chart_set_range(chart, 0, 100);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);

    cpu_ser =  lv_chart_add_series(chart, LV_COLOR_GREEN);
    temp_ser = lv_chart_add_series(chart, LV_COLOR_RED);
    mem_ser =  lv_chart_add_series(chart, LV_COLOR_BLUE);

    /*Set the data series to zero*/
    uint16_t i;
    for(i = 0; i < CHART_POINT_NUM; i++) {
        lv_chart_set_next(chart, cpu_ser, 0);
        lv_chart_set_next(chart, temp_ser, 0);
        lv_chart_set_next(chart, mem_ser, 0);
    }

    /*Create a label for the details of Memory and CPU usage*/
    info_label = lv_label_create(parent, NULL);
    lv_label_set_recolor(info_label, true);
    lv_obj_align(info_label, chart, LV_ALIGN_OUT_RIGHT_TOP, LV_DPI / 4, 0);

    lv_obj_t * lv_logo_img = lv_img_create(parent, NULL);
    lv_img_set_src(lv_logo_img, &lv_logo);
    lv_obj_align(lv_logo_img, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 20, -20);

    /*Refresh the chart and label manually at first*/
    sysmon_task(NULL);
}

static void tasks_tab(lv_obj_t * parent)
{

}

static void network_tab(lv_obj_t * parent)
{

}

/**
 * Called periodically to monitor the CPU and memory usage.
 * @param param unused
 */
void sysmon_task(lv_task_t * param)
{

    (void) param;    /*Unused*/

    LV_LOG_TRACE("sys_mon task started");

    uint8_t cpu_busy = (uint8_t)osGetCPUUsage();
    uint8_t cpu_temp = 42; //((((HAL_ADC_GetValue(&hadc1) * VREF)/MAX_CONVERTED_VALUE) - VSENS_AT_AMBIENT_TEMP) * 10 / AVG_SLOPE) + AMBIENT_TEMP;
    uint8_t mem_used_pct = 0;

#if  LV_MEM_CUSTOM == 0
    lv_mem_monitor_t mem_mon;
    lv_mem_monitor(&mem_mon);
    mem_used_pct = mem_mon.used_pct;
#endif

    /*Add the CPU and memory data to the chart*/
    lv_chart_set_next(chart, cpu_ser, cpu_busy);
    lv_chart_set_next(chart, temp_ser, cpu_temp);
    lv_chart_set_next(chart, mem_ser, mem_used_pct);

    /*Refresh the and windows*/
    char buf_long[SYSMON_STRING_BUFFER_SIZE];
    int len = 0;
    len += lv_snprintf(buf_long+len, SYSMON_STRING_BUFFER_SIZE-len, "%s%s CPU Utilization: %d %%%s\n\n",
            LV_TXT_COLOR_CMD,
            CPU_LABEL_COLOR,
            cpu_busy,
            LV_TXT_COLOR_CMD);

    len += lv_snprintf(buf_long+len, SYSMON_STRING_BUFFER_SIZE-len, "%s%s CPU Temperature: %d %\n\n",
            LV_TXT_COLOR_CMD,
            CPU_TEMP_LABEL_COLOR,
			cpu_temp,
            LV_TXT_COLOR_CMD);

#if LV_MEM_CUSTOM == 0
    len += lv_snprintf(buf_long+len, SYSMON_STRING_BUFFER_SIZE-len, LV_TXT_COLOR_CMD"%s MEMORY: %d %%"LV_TXT_COLOR_CMD"\n"
            "Total: %d bytes\n"
            "Used: %d bytes\n"
            "Free: %d bytes\n"
            "Frag: %d %%",
            MEM_LABEL_COLOR,
            mem_used_pct,
            (int)mem_mon.total_size,
            (int)mem_mon.total_size - mem_mon.free_size, mem_mon.free_size, mem_mon.frag_pct);

#else
    len += lv_snprintf(buf_long+len, SYSMON_STRING_BUFFER_SIZE-len, LV_TXT_COLOR_CMD"%s HEAP Usage:"LV_TXT_COLOR_CMD"\n"
            "Total: %d bytes\n"
            "Used: %d bytes\n"
            "Free: %d bytes\n"
    		"Min. Free: %d bytes\n",
            MEM_LABEL_COLOR,
            configTOTAL_HEAP_SIZE,
            configTOTAL_HEAP_SIZE - xPortGetFreeHeapSize(),
			xPortGetFreeHeapSize(),
			xPortGetMinimumEverFreeHeapSize());

#endif
    lv_label_set_text(info_label, buf_long);


    LV_LOG_TRACE("sys_mon task finished");
}

/**
 * Called when the window's close button is clicked
 * @param btn pointer to the close button
 * @param event the current event
 */
static void win_hide_action(lv_obj_t * btn, lv_event_t event)
{
    (void) btn;    /*Unused*/

    if(event != LV_EVENT_CLICKED) return;

    lv_obj_set_hidden(tabview, true);

//    lv_obj_del(win);
//    win = NULL;

//    lv_task_del(refr_task);
//    refr_task = NULL;
}

#endif /*LV_USE_SYMON*/
