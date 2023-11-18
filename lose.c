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


static ALLEGRO_SAMPLE_ID effect_audio;
static ALLEGRO_SAMPLE_ID lose_music;

extern ALLEGRO_BITMAP* lose;
extern ALLEGRO_SAMPLE* lose_bgm;
extern ALLEGRO_SAMPLE* effect;

extern float music_volume;
extern float effect_volume;
extern Button back_btn;

void lose_init(void);
void lose_destroy(void);
void draw(void);


static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(
		menuFont,
		al_map_rgb(249, 246, 108),
		SCREEN_W / 2,
		200,
		ALLEGRO_ALIGN_CENTER,
		"YOU LOSE!"
	);
	al_draw_bitmap(lose, SCREEN_W / 4, 270, 0);
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
		stop_bgm(lose_music);
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
		stop_bgm(lose_music);
		effect_audio = play_audio(effect, effect_volume);
		game_change_scene(scene_menu_create());
	}
}


// The only function that is shared across files.
Scene scene_lose_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Lose";
	scene.initialize = &lose_init;
	scene.draw = &draw;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_move = &on_mouse_move;
	scene.destroy = &lose_destroy;
	scene.on_key_down = &on_key_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

void lose_init(void) {

	lose = al_load_bitmap("Assets/lose.png");
	back_btn = button_create(680, 700, 120, 120, "Assets/back.png", "Assets/back.png");
	lose_music = play_bgm(lose_bgm, music_volume);
}

void lose_destroy(void) {

	al_destroy_bitmap(lose);
	al_destroy_bitmap(back_btn.default_img);
	al_destroy_bitmap(back_btn.hovered_img);

}