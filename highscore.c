#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include "scene_menu_object.h"
#include "scene_settings.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "utility.h"
#include "shared.h"
#include "highscore.h"

// Variables and functions with 'static' prefix at the top level of a
// source file is only accessible in that file ("file scope", also
// known as "internal linkage"). If other files has the same variable
// name, they'll be different variables.

/* Define your static vars / function prototypes below. */

// TODO: More variables and functions that will only be accessed
// inside this scene. They should all have the 'static' prefix.

extern float music_volume;
extern float effect_volume;
extern int key_mode;
extern int score_board[11];
extern char board[1000];
extern FILE* leaderboard;

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

void highscore_init();
void highscore_destroy();

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_draw_text(
		menuFont,
		al_map_rgb(249, 246, 108),
		SCREEN_W / 2 - 130, 40, 0, "LEADERBOARD");

	sprintf(board, "%d", score_board[0]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 90, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 90, 0, "1st");

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 130, 0, "2nd");
	sprintf(board, "%d", score_board[1]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 130, 0, board);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 170, 0, "3rd");
	sprintf(board, "%d", score_board[2]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 170, 0, board);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 210, 0, "4th");
	sprintf(board, "%d", score_board[3]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 210, 0, board);

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 250, 0, "5th");
	sprintf(board, "%d", score_board[4]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 250, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 290, 0, "6th");
	sprintf(board, "%d", score_board[5]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 290, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 330, 0, "7th");
	sprintf(board, "%d", score_board[6]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 330, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 370, 0, "8th");
	sprintf(board, "%d", score_board[7]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 370, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 410, 0, "9th");
	sprintf(board, "%d", score_board[8]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 410, 0, board);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W / 2 - 125, 450, 0, "10th");
	sprintf(board, "%d", score_board[9]);
	al_draw_text(menuFont, al_map_rgb(255, 255, 255), SCREEN_W / 2 + 30, 450, 0, board);

	drawButton(back_btn);


}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	back_btn.hovered = buttonHover(back_btn, mouse_x, mouse_y);

}

static void on_mouse_down() {

	if (back_btn.hovered && mouse_state[1]) {
		stop_bgm(setting_bgm);
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		game_change_scene(scene_menu_create());
	}

}


// The only function that is shared across files.
Scene scene_highscore_create(void) {

	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "highscore";
	scene.initialize = &highscore_init;
	scene.draw = &draw;
	scene.on_mouse_down = &on_mouse_down;
	scene.on_mouse_move = &on_mouse_move;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Settings scene created");
	return scene;
}

void highscore_init(void) {

	back_btn = button_create(680, 700, 120, 120, "Assets/back.png", "Assets/back.png");
	setting_bgm = play_bgm(set_bgm, music_volume);

	for (int i = 0; i < 10; i++) {
		fscanf(leaderboard, "%d", &score_board[i]);
	}
	fclose(leaderboard);

}

void highscore_destroy(void) {


	al_destroy_bitmap(back_btn.default_img);
	al_destroy_bitmap(back_btn.hovered_img);


}