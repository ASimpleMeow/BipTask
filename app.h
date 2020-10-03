#ifndef __APP_TEMPLATE_H__

#define __APP_TEMPLATE_H__

struct app_data_ {
	void* ret_f;

	unsigned int current_screen;
    int mode;
    int data;
};

void 	show_screen (void *return_screen);
void 	key_press_screen();
int 	dispatch_screen (void *param);
void	draw_buttons_screen(int screen);
void	handle_click(struct gesture_* gest, struct app_data_*	app_data);

#endif