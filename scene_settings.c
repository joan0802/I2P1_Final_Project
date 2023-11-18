// [HACKATHON 3-9]
// TODO: Create scene_settings.h and scene_settings.c.
// No need to do anything for this part. We've already done it for
// you, so this 2 files is like the default scene template.
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

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

extern ALLEGRO_BITMAP* sound;
extern ALLEGRO_BITMAP* plus;
extern ALLEGRO_BITMAP* minus;
extern ALLEGRO_BITMAP* sound_range;
extern ALLEGRO_BITMAP* sound_mute;
extern ALLEGRO_BITMAP* keyboard;
extern ALLEGRO_BITMAP* wasd;
extern ALLEGRO_BITMAP* uldr;
extern float music_volume;
extern float effect_volume;
extern int key_mode;

static ALLEGRO_SAMPLE_ID setting_bgm;
static ALLEGRO_SAMPLE_ID effect_audio;
extern ALLEGRO_SAMPLE* set_bgm;
extern ALLEGRO_SAMPLE* effect;

Button plus_btn;
Button minus_btn;
Button back_btn;
Button effect_plus_btn;
Button effect_minus_btn;
Button mode_1;
Button mode_2;

void setting_init();
void setting_destroy();
void sound_draw();
void effect_draw();

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_text(menuFont, al_map_rgb(117, 182, 213), 315, 50, 0, "BGM Volume");
	al_draw_text(menuFont, al_map_rgb(117, 182, 213), 300, 210, 0, "Effect Volume");
	al_draw_bitmap(sound, 50, 85, 0);
	al_draw_bitmap(sound, 50, 235, 0);
	al_draw_text(menuFont, al_map_rgb(117, 182, 213), 300, 400, 0, "Keyboard Mode");
	al_draw_scaled_bitmap(wasd, 0, 0, 1060, 409, 350, 460, 150, 58, 0);
	al_draw_scaled_bitmap(uldr, 0, 0, 1110, 401, 350, 550, 150, 54, 0);

	al_draw_scaled_bitmap(keyboard, 0, 0, 300, 300, 130, 460, 100, 100, 0);

	if (key_mode == 1) {
		al_draw_scaled_bitmap(mode_1.hovered_img, 0, 0, 288, 288, 230, 450, 100, 100, 0);
		al_draw_scaled_bitmap(mode_2.default_img, 0, 0, 288, 288, 230, 530, 100, 100, 0);
	}
	if (key_mode == 2) {
		al_draw_scaled_bitmap(mode_1.default_img, 0, 0, 288, 288, 230, 450, 100, 100, 0);
		al_draw_scaled_bitmap(mode_2.hovered_img, 0, 0, 288, 288, 230, 530, 100, 100, 0);
	}

	drawButton(plus_btn);
	drawButton(minus_btn);
	drawButton(back_btn);
	drawButton(effect_minus_btn);
	drawButton(effect_plus_btn);
	drawButton(mode_1);
	drawButton(mode_2);

	sound_draw();
	effect_draw();

}

void sound_draw() {

	int x = (int)(music_volume * 10);

	if (music_volume > 0) {
		for (int i = 0; i < x; i++) {
			//game_log("i = %d, x = %d", i, x);
			al_draw_bitmap(sound_range, 210 + 40 * i, 97, 0);
		}
	}
	else if (x == 0) {
		game_log("HELLO");
		al_draw_bitmap(sound_mute, 47.5, 82, 0);
	}
}

void effect_draw() {
	int x = (int)(effect_volume * 10);

	if (effect_volume > 0) {
		for (int i = 0; i < x; i++) {
			//game_log("i = %d, x = %d", i, x);
			al_draw_bitmap(sound_range, 210 + 40 * i, 247, 0);
		}
	}
	/*else if (x == 0) {
		game_log("HELLO");
		al_draw_bitmap(sound_mute, 47.5, 82, 0);
	}*/
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	plus_btn.hovered = buttonHover(plus_btn, mouse_x, mouse_y);
	back_btn.hovered = buttonHover(back_btn, mouse_x, mouse_y);
	minus_btn.hovered = buttonHover(minus_btn, mouse_x, mouse_y);
	effect_plus_btn.hovered = buttonHover(effect_plus_btn, mouse_x, mouse_y);
	effect_minus_btn.hovered = buttonHover(effect_minus_btn, mouse_x, mouse_y);
	mode_1.hovered = buttonHover(mode_1, mouse_x, mouse_y);
	mode_2.hovered = buttonHover(mode_2, mouse_x, mouse_y);

}

static void on_mouse_down() {

	if (plus_btn.hovered && mouse_state[1]) {

		if (music_volume < 1.0) music_volume += 0.1;

		stop_bgm(setting_bgm);
		stop_bgm(effect_audio);
		game_log("music = %f", music_volume);
		effect_audio = play_audio(effect, effect_volume);
		setting_bgm = play_bgm(set_bgm, music_volume);

		sound_draw();
	}
	if (minus_btn.hovered && mouse_state[1]) {
		if (music_volume > 0.1) music_volume -= 0.1;
		stop_bgm(setting_bgm);
		stop_bgm(effect_audio);
		game_log("music = %f", music_volume);
		effect_audio = play_audio(effect, effect_volume);
		setting_bgm = play_bgm(set_bgm, music_volume);
		sound_draw();
	}
	if (back_btn.hovered && mouse_state[1]) {
		stop_bgm(setting_bgm);
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		game_change_scene(scene_menu_create());
	}
	if (effect_plus_btn.hovered && mouse_state[1]) {
		if (effect_volume < 1.0) effect_volume += 0.1;
		game_log("audio = %f", effect_volume);
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		sound_draw();
	}
	if (effect_minus_btn.hovered && mouse_state[1]) {
		if (effect_volume > 0.1) effect_volume -= 0.1;
		game_log("audio = %f", effect_volume);
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		sound_draw();
	}
	if (mode_1.hovered && mouse_state[1]) {
		game_log("mode 1");
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		key_mode = 1;
	}
	if (mode_2.hovered && mouse_state[1]) {
		game_log("mode 2");
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		key_mode = 2;
	}
}


// The only function that is shared across files.
Scene scene_settings_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Settings";
	scene.initialize = &setting_init;
	scene.draw = &draw;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_move = &on_mouse_move;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

void setting_init(void) {

	sound = al_load_bitmap("Assets/small_sound.png");
	plus_btn.default_img = al_load_bitmap("Assets/plus.png");
	minus_btn.default_img = al_load_bitmap("Assets/minus.png");
	sound_range = al_load_bitmap("Assets/sound_range.png");
	sound_mute = al_load_bitmap("Assets/sound_mute.png");
	back_btn = button_create(680, 700, 120, 120, "Assets/back.png", "Assets/back.png");
	plus_btn = button_create(630, 90, 105, 105, "Assets/plus.png", "Assets/plus.png");
	minus_btn = button_create(150, 95, 96, 96, "Assets/minus.png", "Assets/minus.png");
	effect_plus_btn = button_create(630, 240, 105, 105, "Assets/plus.png", "Assets/plus.png");
	effect_minus_btn = button_create(150, 245, 96, 96, "Assets/minus.png", "Assets/minus.png");
	mode_1 = button_create(230, 450, 100, 100, "Assets/button_0.png", "Assets/button_1.png");
	mode_2 = button_create(230, 530, 100, 100, "Assets/button_0.png", "Assets/button_1.png");

	setting_bgm = play_bgm(set_bgm, music_volume);

}

void setting_destroy(void) {

	al_destroy_bitmap(plus_btn.default_img);
	al_destroy_bitmap(plus_btn.hovered_img);
	al_destroy_bitmap(minus_btn.default_img);
	al_destroy_bitmap(minus_btn.hovered_img);
	al_destroy_bitmap(sound_range);
	al_destroy_bitmap(sound_mute);
	al_destroy_bitmap(back_btn.default_img);
	al_destroy_bitmap(back_btn.hovered_img);
	al_destroy_bitmap(effect_plus_btn.default_img);
	al_destroy_bitmap(effect_plus_btn.hovered_img);
	al_destroy_bitmap(effect_minus_btn.default_img);
	al_destroy_bitmap(effect_minus_btn.hovered_img);
	al_destroy_bitmap(mode_1.default_img);
	al_destroy_bitmap(mode_1.hovered_img);
	al_destroy_bitmap(mode_2.default_img);
	al_destroy_bitmap(mode_2.hovered_img);

}