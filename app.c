#include "libbip.h"
#include "app.h"
#include "biptask.h"

#define SCREEN_ONE 0
#define SCREEN_TWO 1
#define SCREEN_THREE 2

// Уникальный идентификатор приложения для BipTask
char app_id[3] = { 0x00, 0x00, 0x10 };

struct regmenu_ screen_data = {
	60,
	1,
	0,
	dispatch_screen,
	key_press_screen,
	0,
	0,
	show_screen,
	0,
	0
};

int main(int param0, char** argv){
	show_screen((void*)param0);
}

void show_screen (void *param0){
	struct app_data_** app_data_p = get_ptr_temp_buf_2();
	struct app_data_* app_data;

	Elf_proc_* proc;

	if ((param0 == *app_data_p) && get_var_menu_overlay()){
		app_data = *app_data_p;
		*app_data_p = NULL;

		reg_menu(&screen_data, 0);
		
		*app_data_p = app_data;
	} else {
		reg_menu(&screen_data, 0);

		*app_data_p = (struct app_data_*)pvPortMalloc(sizeof(struct app_data_));
		app_data = *app_data_p;
		
		_memclr(app_data, sizeof(struct app_data_));
		
		proc = param0;
		
		if (param0 && proc->ret_f)
			app_data->ret_f = proc->elf_finish;
		else
			app_data->ret_f = show_watchface;

		app_data->current_screen = SCREEN_TWO;
		app_data->mode = 0;
		app_data->data = 0;
	}

	draw_buttons_screen(SCREEN_TWO);
}

void key_press_screen(){
	struct app_data_** app_data_p = get_ptr_temp_buf_2();
	struct app_data_ * app_data = *app_data_p;

	set_display_state_value(8, 0);
	set_display_state_value(4, 0);
	set_display_state_value(2, 0);

	show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);	
};


int dispatch_screen(void* param) {
	struct app_data_** app_data_p = get_ptr_temp_buf_2();
	struct app_data_* app_data = *app_data_p;

	struct gesture_* gest = param;

	switch (gest->gesture) {
	case GESTURE_CLICK:
		handle_click(gest, app_data);
		break;

	case GESTURE_SWIPE_RIGHT:
		set_display_state_value(8, 0);
		set_display_state_value(4, 0);
		set_display_state_value(2, 0);

		show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
		break;

	case GESTURE_SWIPE_LEFT:
		set_display_state_value(8, 0);
		set_display_state_value(4, 0);
		set_display_state_value(2, 0);

		show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_LEFT);
		break;

	case GESTURE_SWIPE_UP:
		switch (app_data->current_screen) {
		case SCREEN_ONE:
			show_menu_animate(draw_buttons_screen, SCREEN_TWO, ANIMATE_DOWN);
			app_data->current_screen = SCREEN_TWO;
			break;
		case SCREEN_TWO:
			show_menu_animate(draw_buttons_screen, SCREEN_THREE, ANIMATE_DOWN);
			app_data->current_screen = SCREEN_THREE;
			break;
		case SCREEN_THREE:
			break;
		}
		break;

	case GESTURE_SWIPE_DOWN:
		switch (app_data->current_screen) {
		case SCREEN_ONE:
			break;
		case SCREEN_TWO:
			show_menu_animate(draw_buttons_screen, SCREEN_ONE, ANIMATE_UP);
			app_data->current_screen = SCREEN_ONE;
			break;
		case SCREEN_THREE:
			show_menu_animate(draw_buttons_screen, SCREEN_TWO, ANIMATE_UP);
			app_data->current_screen = SCREEN_TWO;
			break;
		}
		break;
	}
    draw_buttons_screen(app_data->current_screen);
	repaint_screen_lines(0, 176);

	return 0;
}

void handle_click(struct gesture_* gest, struct app_data_*	app_data) {
	char message[2] = { 0,0 };
	switch (app_data->current_screen) {
	case SCREEN_ONE:
		if (
			(gest->touch_pos_x > 10) && (gest->touch_pos_x < 56) &&
			(gest->touch_pos_y > 25) && (gest->touch_pos_y < 65)
			) {
			// Decrease mode number
			app_data->mode = app_data->mode - 1;
		} else if (
			(gest->touch_pos_x > 120) && (gest->touch_pos_x < 166) &&
			(gest->touch_pos_y > 25) && (gest->touch_pos_y < 65)
			) {
			// Increase mode number
			app_data->mode = app_data->mode + 1;
		} else if (
			(gest->touch_pos_x > 10) && (gest->touch_pos_x < 56) &&
			(gest->touch_pos_y > 90) && (gest->touch_pos_y < 130)
			) {
			// Decrease data number
			app_data->data = app_data->data - 1;
		} else if (
			(gest->touch_pos_x > 120) && (gest->touch_pos_x < 166) &&
			(gest->touch_pos_y > 90) && (gest->touch_pos_y < 130)
			) {
			// Increase data number
			app_data->data = app_data->data + 1;
		} else if (
			(gest->touch_pos_x > 44) && (gest->touch_pos_x < 132) &&
			(gest->touch_pos_y > 140) && (gest->touch_pos_y < 170)
			) {
			// Send message
			message[0] = app_data->mode;
			message[1] = app_data->data;
			biptask_send_bytes(app_id, message, 2);
		}
		break;
	case SCREEN_TWO:
	case SCREEN_THREE:
		if (
			(gest->touch_pos_x > 58) && (gest->touch_pos_x < 118) &&
			(gest->touch_pos_y > 58) && (gest->touch_pos_y < 118)
			) {
			// Mode Increase
			app_data->mode = app_data->mode + 1;
		} else if (
			(gest->touch_pos_x > 10) && (gest->touch_pos_x < 78) &&
			(gest->touch_pos_y > 10) && (gest->touch_pos_y < 78)
			) {
			// Button 1
			message[0] = app_data->mode;
			message[1] = app_data->current_screen == SCREEN_TWO ? 1 : 5;
			biptask_send_bytes(app_id, message, 2);
		} else if (
			(gest->touch_pos_x > 98) && (gest->touch_pos_x < 166) &&
			(gest->touch_pos_y > 10) && (gest->touch_pos_y < 78)
			) {
			// Button 2
			message[0] = app_data->mode;
			message[1] = app_data->current_screen == SCREEN_TWO ? 2 : 6;
			biptask_send_bytes(app_id, message, 2);
		} else if (
			(gest->touch_pos_x > 10) && (gest->touch_pos_x < 78) &&
			(gest->touch_pos_y > 78) && (gest->touch_pos_y < 166)
			) {
			// Button 3
			message[0] = app_data->mode;
			message[1] = app_data->current_screen == SCREEN_TWO ? 3 : 7;
			biptask_send_bytes(app_id, message, 2);
		} else if (
			(gest->touch_pos_x > 98) && (gest->touch_pos_x < 166) &&
			(gest->touch_pos_y > 98) && (gest->touch_pos_y < 166)
			) {
			// Button 4
			message[0] = app_data->mode;
			message[1] = app_data->current_screen == SCREEN_TWO ? 4 : 8;
			biptask_send_bytes(app_id, message, 2);
		}
		break;
	}
}

void draw_buttons_screen(int screen){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;
	load_font();
	set_bg_color(COLOR_BLACK);
	fill_screen_bg();
	set_fg_color(COLOR_WHITE);
	char mode[5];
	char data[5];
	itoa(app_data->mode, mode, 10);
	itoa(app_data->data, data, 10);

	switch (app_data->current_screen) {
	case SCREEN_ONE:
		text_out_center("MODE", 88, 5);
		draw_rect(10, 25, 56, 65);
		text_out_center("<<", 33, 35);

		text_out_center(mode, 88, 35);

		draw_rect(120, 25, 166, 65);
		text_out_center(">>", 146, 35);

		text_out_center("DATA", 88, 70);
		draw_rect(10, 90, 56, 130);
		text_out_center("<<", 34, 100);

		text_out_center(data, 88, 100);

		draw_rect(120, 90, 166, 130);
		text_out_center(">>", 147, 100);

		draw_rect(44, 140, 132, 170);
		text_out_center("Send", 88, 148);

		break;
	case SCREEN_TWO:
	case SCREEN_THREE:
		draw_rect(10, 10, 78, 78);
		text_out_center(app_data->current_screen == SCREEN_TWO ? "1":"5", 48, 38);

		draw_rect(98, 10, 166, 78);
		text_out_center(app_data->current_screen == SCREEN_TWO ? "2":"6", 136, 38);

		draw_rect(10, 98, 78, 166);
		text_out_center(app_data->current_screen == SCREEN_TWO ? "3":"7", 48, 126);

		draw_rect(98, 98, 166, 166);
		text_out_center(app_data->current_screen == SCREEN_TWO ? "4":"8", 136, 126);

		draw_filled_rect_bg(58, 58, 118, 118);
		draw_rect(58, 58, 118, 118);
		text_out_center(mode, 90, 80);
		break;
	}
}