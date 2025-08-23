#include <rtthread.h>
#include <rtdevice.h>
#include "littlevgl2rtt.h"
#include "lv_event.h"
#include "lv_obj.h"
#include "lv_obj_style.h"
#include "lv_timer.h"
#include "lvgl.h"
#include "gui_app_fwk.h"

#include "app_clock_main.h"
#include "app_mem.h"

extern const lv_font_t ui_font_OpenSansMedium96;
extern const lv_font_t ui_font_OpenSansMediumItalic24;

struct wf_sw_anim {
	bool play;
	rt_uint32_t update_count;
	rt_uint32_t update_interval;
	rt_uint32_t update_idx;
};

struct wf_sw {
	struct wf_sw_anim anim;

	lv_timer_t *redraw_task;
	rt_uint8_t redraw_buf[16];

	lv_obj_t *sw_img;
	lv_obj_t *info;
	lv_obj_t *clock_label;
};

struct wf_sw sw_instence;
struct wf_sw *wf_sw = &sw_instence;
/*struct wf_sw *wf_sw = NULL;*/

LV_IMG_DECLARE(ui_img_anim_spring_wreath_0_png);    // assets\anim_spring_wreath_0.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_1_png);    // assets\anim_spring_wreath_1.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_10_png);    // assets\anim_spring_wreath_10.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_11_png);    // assets\anim_spring_wreath_11.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_12_png);    // assets\anim_spring_wreath_12.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_13_png);    // assets\anim_spring_wreath_13.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_14_png);    // assets\anim_spring_wreath_14.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_15_png);    // assets\anim_spring_wreath_15.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_16_png);    // assets\anim_spring_wreath_16.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_17_png);    // assets\anim_spring_wreath_17.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_18_png);    // assets\anim_spring_wreath_18.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_19_png);    // assets\anim_spring_wreath_19.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_2_png);    // assets\anim_spring_wreath_2.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_20_png);    // assets\anim_spring_wreath_20.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_21_png);    // assets\anim_spring_wreath_21.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_22_png);    // assets\anim_spring_wreath_22.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_23_png);    // assets\anim_spring_wreath_23.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_24_png);    // assets\anim_spring_wreath_24.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_25_png);    // assets\anim_spring_wreath_25.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_26_png);    // assets\anim_spring_wreath_26.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_3_png);    // assets\anim_spring_wreath_3.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_4_png);    // assets\anim_spring_wreath_4.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_5_png);    // assets\anim_spring_wreath_5.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_6_png);    // assets\anim_spring_wreath_6.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_7_png);    // assets\anim_spring_wreath_7.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_8_png);    // assets\anim_spring_wreath_8.png
LV_IMG_DECLARE(ui_img_anim_spring_wreath_9_png);    // assets\anim_spring_wreath_9.png

static const void *anim_spring_wreath[] = {
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_0_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_1_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_2_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_3_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_4_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_5_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_6_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_7_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_8_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_9_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_10_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_11_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_12_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_13_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_14_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_15_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_16_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_17_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_18_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_19_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_20_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_21_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_22_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_23_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_24_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_25_png),
    LV_EXT_IMG_GET(ui_img_anim_spring_wreath_26_png),
};
#define NUM_ANIM_SPRING_WREATH		(sizeof(anim_spring_wreath) / sizeof(anim_spring_wreath[0]))

static void _update_anim(void)
{
	if (wf_sw->anim.play &&
	    ((lv_tick_get() - wf_sw->anim.update_count) > wf_sw->anim.update_interval)) {
		lv_img_set_src(wf_sw->sw_img, anim_spring_wreath[wf_sw->anim.update_idx]);

		/* Next frame */
		wf_sw->anim.update_idx ++;
		wf_sw->anim.update_count = lv_tick_get();
		if (wf_sw->anim.update_idx > (NUM_ANIM_SPRING_WREATH - 1)) {
			wf_sw->anim.play = false;
			wf_sw->anim.update_idx = 0;
		}
	}

}

static char clock_buffer[16];

static void app_clock_simple_redraw(lv_timer_t *task)
{
	app_clock_time_t current_time;
	static rt_uint8_t hours = 0, minutes = 0, seconds;
	

	app_clock_main_get_current_time(&current_time);
	hours = current_time.h;
	minutes = current_time.m;
	seconds = current_time.s;

	rt_sprintf(clock_buffer, "%02d:%02d", hours, minutes);
	lv_label_set_text(wf_sw->clock_label, clock_buffer);

	_update_anim();
}

static rt_int32_t resume_callback(void)
{
	if (!wf_sw->redraw_task)
		wf_sw->redraw_task = lv_timer_create(app_clock_simple_redraw, 30, (void *)0);

	return RT_EOK;
}

static rt_int32_t pause_callback(void)
{
	if (wf_sw->redraw_task) {
		lv_timer_del(wf_sw->redraw_task);
		wf_sw->redraw_task = NULL;
	}

	return RT_EOK;
}

static void lv_event_cb(lv_event_t * e)
{
	/* Get event code */
	lv_event_code_t code = lv_event_get_code(e);

	/* Play anim */
	if (code == LV_EVENT_CLICKED) {
		wf_sw->anim.play = true;
		wf_sw->anim.update_idx = 0;
	}
}


static rt_int32_t init(lv_obj_t *parent)
{
	static char t_buf[16];
	wf_sw->anim.update_interval = 50;
	wf_sw->anim.play= true;

	lv_obj_t *sw = lv_obj_create(parent);
	lv_obj_set_align(sw, LV_ALIGN_CENTER);
	lv_obj_set_style_bg_color(sw, lv_color_hex(0x000000), LV_STATE_DEFAULT);
	lv_obj_add_event_cb(sw, lv_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_set_size(sw,
			lv_obj_get_width(parent),
			lv_obj_get_height(parent));

	wf_sw->sw_img = lv_img_create(sw);
	lv_obj_align(wf_sw->sw_img, LV_ALIGN_CENTER, 0, 0);
	lv_obj_clear_flag(wf_sw->sw_img, LV_OBJ_FLAG_SCROLLABLE);

	wf_sw->clock_label = lv_label_create(sw);
	lv_obj_set_align(wf_sw->clock_label, LV_ALIGN_CENTER);
	lv_obj_set_x(wf_sw->clock_label, 0);
	lv_obj_set_y(wf_sw->clock_label, -32);
	lv_label_set_text(wf_sw->clock_label, "23:32");
	lv_obj_set_style_text_color(wf_sw->clock_label, lv_color_hex(0xEFF2A5), LV_PART_MAIN);
	lv_obj_set_style_text_font(wf_sw->clock_label, &ui_font_OpenSansMedium96, LV_PART_MAIN);

	wf_sw->info = lv_label_create(sw);
	lv_obj_set_align(wf_sw->info, LV_ALIGN_CENTER);
	lv_obj_set_x(wf_sw->info, 0);
	lv_obj_set_y(wf_sw->info, 26);
	lv_label_set_text(wf_sw->info, "~~~~~~~~~~~~~~~~\n3/7 Mon.  2333 steps");
	lv_obj_set_style_text_color(wf_sw->info, lv_color_hex(0xEFF2A5), LV_PART_MAIN);
	lv_obj_set_style_text_font(wf_sw->info, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN);

	return RT_EOK;
}

static rt_int32_t deinit(void)
{
	return RT_EOK;
}

static const app_clock_ops_t ops =
{
    .init = init,
    .pause = pause_callback,
    .resume = resume_callback,
    .deinit = deinit,
};

void app_clock_spring_wreath_register(void)
{
    app_clock_register("spring_wreath", &ops);
}
