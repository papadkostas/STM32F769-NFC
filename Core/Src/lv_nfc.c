/*
 * lv_nfc.c
 *
 *  Created on: Mar 22, 2020
 *      Author: kostas
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_nfc.h"
#include "sysmon.h"
#include <stdio.h>
#include "../../../../lvgl/src/lv_themes/lv_theme_material.h"

/**********************
 *      TYPEDEFS
 **********************/

static lv_obj_t * sb;
static lv_obj_t * win;
static lv_obj_t * scr;
static lv_obj_t * header;
static lv_coord_t vres;
static lv_coord_t hres;
extern const lv_img_dsc_t nfc;
extern const lv_img_dsc_t configuration;

/**********************
 *  STATIC PROTOTYPES
 **********************/

void nfc_create(void);
void sb_create(lv_obj_t * parent);
void header_create(lv_obj_t * parent);
void main_window_create(lv_obj_t * parent);
static void settings_action(lv_obj_t * btn, lv_event_t event);

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/* ToDo
 *
 * 1)How to handle screens
 * https://docs.littlevgl.com/en/html/overview/display.html?highlight=screen#screens
 *
 */

void nfc_create(void)
{
//    win = lv_win_create(lv_disp_get_scr_act(NULL), NULL);
//
//    lv_obj_t * settings_btn = lv_win_add_btn(win, LV_SYMBOL_SETTINGS);
//    lv_obj_set_event_cb(settings_btn, settings_action);
//    lv_obj_align(settings_btn, NULL, LV_ALIGN_IN_RIGHT_MID, -LV_DPI/10, 0);

//    lv_obj_t * scr = lv_obj_create(NULL, NULL);
	lv_obj_t * scr = lv_scr_act();
//    lv_disp_load_scr(scr);

    hres = lv_disp_get_hor_res(NULL);
    vres = lv_disp_get_ver_res(NULL);

	header_create(scr);
	main_window_create(scr);
//	sb_create(scr);
	sysmon_create();
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
void header_create(lv_obj_t * parent)
{
    header = lv_cont_create(parent, NULL);
    lv_obj_set_pos(header, 0, 0);
    lv_obj_set_width(header, hres);
    lv_obj_set_height(header, vres/6);

    LV_FONT_DECLARE(montserrat64);

	static lv_style_t day_font_label;
	lv_style_init(&day_font_label);
    lv_style_set_text_font(&day_font_label, LV_STATE_DEFAULT, &lv_font_montserrat_26);

	static lv_style_t daynum_font_label;
	lv_style_init(&daynum_font_label);
    lv_style_set_text_font(&daynum_font_label, LV_STATE_DEFAULT, &montserrat64);

    lv_obj_t * settings_btn = lv_btn_create(header, NULL);
    lv_theme_apply(settings_btn, LV_THEME_BTN);
    lv_obj_t * settings_btn_img = lv_img_create(settings_btn, NULL);
    lv_obj_set_size(settings_btn, configuration.header.w, configuration.header.h);
    lv_obj_set_click(settings_btn_img, false);
    lv_img_set_src(settings_btn_img, &configuration);
    lv_obj_align(settings_btn, NULL, LV_ALIGN_IN_RIGHT_MID, -LV_DPI/10, 0);
    lv_obj_set_event_cb(settings_btn, settings_action);

    lv_obj_t * time = lv_label_create(header, NULL);
    lv_label_set_text(time, "15:34");
    lv_obj_align(time, header, LV_ALIGN_CENTER, -60, -20);
    lv_obj_add_style(time, LV_LABEL_PART_MAIN, &daynum_font_label);

    lv_obj_t * date = lv_label_create(header, NULL);
    lv_obj_set_pos(date, 100, 15);
    lv_label_set_text(date, "Wednesday");
    lv_obj_add_style(date, LV_LABEL_PART_MAIN, &day_font_label);

    lv_obj_t * datenumber = lv_label_create(header, NULL);
    lv_obj_set_pos(datenumber, 20, 10);
    lv_label_set_text(datenumber, "27");
    lv_obj_add_style(datenumber, LV_LABEL_PART_MAIN, &daynum_font_label);

    lv_obj_t * month = lv_label_create(header, NULL);
    lv_obj_set_pos(month, 100, 45);
    lv_label_set_text(month, "September");

    lv_obj_t * year = lv_label_create(header, NULL);
    lv_obj_set_pos(year, 210, 45);
    lv_label_set_text(year, "2020");

}

void main_window_create(lv_obj_t * parent)
{

	static lv_style_t style_main_menu_btn;
	lv_style_init(&style_main_menu_btn);
    lv_style_set_radius(&style_main_menu_btn, LV_STATE_DEFAULT, 1);

    lv_obj_t * main_win = lv_cont_create(parent, NULL);
    lv_obj_set_pos(main_win, 0, lv_obj_get_height(header));
    lv_obj_set_size(main_win, LV_HOR_RES, LV_VER_RES - lv_obj_get_height(header));
    lv_cont_set_layout(main_win, LV_LAYOUT_GRID);

	lv_coord_t block_height = (LV_VER_RES-lv_obj_get_height(header))/4;
	lv_coord_t block_width = LV_HOR_RES/4;

//    lv_obj_t * alarm_btn = lv_btn_create(main_win, NULL);
//    lv_obj_set_size(alarm_btn, block_width, block_height);
//    lv_obj_add_style(alarm_btn, LV_BTN_PART_MAIN, &style_main_menu_btn);
//    lv_obj_t * lbl_alarm = lv_label_create(alarm_btn, NULL);
//    lv_label_set_text(lbl_alarm, "Alarm");
//    lv_obj_align(lbl_alarm, alarm_btn, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
//
//    lv_obj_t * access_btn = lv_btn_create(main_win, NULL);
//    lv_obj_set_size(access_btn, block_width, block_height);
//    lv_obj_add_style(access_btn, LV_BTN_PART_MAIN, &style_main_menu_btn);
//    lv_obj_t * lbl_access = lv_label_create(access_btn, NULL);
//    lv_label_set_text(lbl_access, "Access");
//    lv_obj_align(lbl_access, access_btn, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
//
//    lv_obj_t * heating_btn = lv_btn_create(main_win, NULL);
//    lv_obj_set_size(heating_btn, block_width, block_height);
//    lv_obj_add_style(heating_btn, LV_BTN_PART_MAIN, &style_main_menu_btn);
//    lv_obj_t * lbl_heating = lv_label_create(heating_btn, NULL);
//    lv_label_set_text(lbl_heating, "Heating");
//    lv_obj_align(lbl_heating, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
//
//    lv_obj_t * users_btn = lv_btn_create(main_win, NULL);
//    lv_obj_set_size(users_btn, block_width, block_height);
//    lv_obj_add_style(users_btn, LV_BTN_PART_MAIN, &style_main_menu_btn);
//    lv_obj_t * lbl_users = lv_label_create(users_btn, NULL);
//    lv_label_set_text(lbl_users, "Users");
//    lv_obj_align(lbl_users, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
//
//    lv_obj_t * settings_btn = lv_btn_create(main_win, NULL);
//    lv_obj_set_size(settings_btn, block_width, block_height);
//    lv_obj_add_style(settings_btn, LV_BTN_PART_MAIN, &style_main_menu_btn);
//    lv_obj_t * lbl_settings = lv_label_create(settings_btn, NULL);
//    lv_label_set_text(lbl_settings, "Settings");
//    lv_obj_align(lbl_settings, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);
//    lv_obj_set_event_cb(settings_btn, settings_action);

}

void sb_create(lv_obj_t * parent)
{
//    lv_coord_t hres = lv_disp_get_hor_res(NULL);
//    lv_coord_t vres = lv_disp_get_ver_res(NULL);
//
//    sb = lv_page_create(parent, NULL);
//    lv_page_set_scrl_layout(sb, LV_LAYOUT_COLUMN_LEFT);
//
//    lv_obj_t * th_roller = lv_roller_create(sb, NULL);
//
//    lv_obj_t * hue_label = lv_label_create(sb, NULL);
//    lv_label_set_text(hue_label, "\nColor");
//
//    lv_obj_t * hue_roller = lv_roller_create(sb, NULL);
//    lv_roller_set_options(hue_roller, "0\n30\n60\n90\n120\n150\n180\n210\n240\n270\n300\n330", true);
//
//    if(hres > vres) {
//        lv_obj_set_height(sb, vres - lv_obj_get_height(header));
//        lv_cont_set_fit2(sb, LV_FIT_TIGHT, LV_FIT_NONE);
//        lv_obj_align(sb, header, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
//    } else {
//        lv_obj_set_size(sb, hres, vres / 2 - lv_obj_get_height(header));
//        lv_obj_align(sb, header, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
//        lv_page_set_scrlbar_mode(sb, LV_SCRLBAR_MODE_AUTO);
//    }
}

/**
 * Called when the window's close button is clicked
 * @param btn pointer to the close button
 * @param event the current event
 */
static void settings_action(lv_obj_t * btn, lv_event_t event)
{
    (void) btn;    /*Unused*/

    if(event != LV_EVENT_CLICKED) return;
    sysmon_show(true);

//    lv_obj_del(header);
//    header = NULL;

//    lv_task_del(refr_task);
//    refr_task = NULL;
}
