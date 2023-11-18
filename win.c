#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "scene_settings.h"
#include "shared.h"
#include "scene_menu_object.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "lose.h"
#include "scene_game.h"
#include "win.h"


static ALLEGRO_SAMPLE_ID effect_audio;
static ALLEGRO_SAMPLE_ID win_music;

extern ALLEGRO_BITMAP* win;
extern ALLEGRO_SAMPLE* win_bgm;
extern ALLEGRO_SAMPLE* effect;

extern float music_volume;
extern float effect_volume;
extern Button back_btn;

void win_init(void);
void win_destroy(void);
void draw(void);


static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		100,
		ALLEGRO_ALIGN_CENTER,
		"WELL LOOKS LIKE WE HAVE A WINNER..."
	);
	al_draw_text(
		menuFont,
		al_map_rgb(249, 246, 108),
		SCREEN_W / 2,
		200,
		ALLEGRO_ALIGN_CENTER,
		"YOU WIN!"
	);
	al_draw_scaled_bitmap(win, 0, 0, 512, 512, SCREEN_W / 2-120, SCREEN_H / 4+100, 250, 250, 0);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2,
		600,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\" to  RESTART"
	);
	drawButton(back_btn);
}

static void on_key_down(int keycode) {

	switch (keycode) {
	case ALLEGRO_KEY_ENTER:
		stop_bgm(win_music);
		effect_audio = play_audio(effect, effect_volume);
		game_change_scene(scene_main_create());
		break;
	default:
		break;
	}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
		//	Uncomment and fill the code below
	back_btn.hovered = buttonHover(back_btn, mouse_x, mouse_y);

}

static void on_mouse_down() {

	if (back_btn.hovered && mouse_state[1]) {
		stop_bgm(win_music);
		effect_audio = play_audio(effect, effect_volume);
		game_change_scene(scene_menu_create());
	}
}


// The only function that is shared across files.
Scene scene_win_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Win";
	scene.initialize = &win_init;
	scene.draw = &draw;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.destroy = &win_destroy;
	scene.on_key_down = &on_key_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

void win_init(void) {

	back_btn = button_create(680, 700, 120, 120, "Assets/back.png", "Assets/back.png");
	win_music = play_bgm(win_bgm, music_volume);
}

void win_destroy(void) {

	al_destroy_bitmap(back_btn.default_img);
	al_destroy_bitmap(back_btn.hovered_img);

}