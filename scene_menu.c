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

/* Internal Variables*/
static ALLEGRO_BITMAP* gameTitle = NULL;
static ALLEGRO_SAMPLE_ID menuBGM;
static int gameTitleW ;
static int gameTitleH ;
static ALLEGRO_SAMPLE_ID effect_audio;
extern ALLEGRO_SAMPLE* effect;
extern ALLEGRO_BITMAP* leader;
extern ALLEGRO_TIMER* drop_down;

extern float music_volume;
extern float effect_volume;

int score_board[11];
char board[1000];
static drop = false;

FILE* leaderboard = NULL;

// [HACKATHON 3]
// TARGET : use a clickable button to enter setting scene.
// For `Button` struct(object) is defined in `scene_menu_object.h` and `scene_menu_object.c`
// STRONGLY recommend you trace both of them first. 

//	[HACKATHON 3-1]
//	TODO: Declare variable for button
//	Uncomment and fill the code below
	static Button btnSettings;
	static Button choose;
	static Button exit_btn;
	static Button highscore;

static void init() {

	// [HACKATHON 3-2]
	// TODO: Create button to settings
	//	Uncomment and fill the code below
	btnSettings = button_create(735, 72, 50, 50, "Assets/settings.png", "Assets/settings2.png");
	choose = button_create(719, 5, 80, 80, "Assets/choose_2.png", "Assets/choose_1.png");
	exit_btn = button_create(735, 183, 50, 50, "Assets/exit.png", "Assets/exit.png");
	highscore = button_create(735, 129, 50, 50, "Assets/highscore_1.png", "Assets/highscore_2.png");

	gameTitle = load_bitmap("Assets/title.png");
	gameTitleW = al_get_bitmap_width(gameTitle);
	gameTitleH = al_get_bitmap_height(gameTitle);
	stop_bgm(menuBGM);
	menuBGM = play_bgm(themeMusic, music_volume);
	leaderboard = fopen("Assets/Interface.txt", "r");

	for (int i = 0; i < 10; i++) {
		fscanf(leaderboard, "%d", &score_board[i]);
	}
	fclose(leaderboard);
	//game_log("%d %d %d", score_board[0], score_board[1], score_board[2]);
}


static void draw() {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	const float scale = 0.7;
	const float offset_w = (SCREEN_W >> 1) - 0.5 * scale * gameTitleW;
	const float offset_h = (SCREEN_H >> 1) - 0.5 * scale * gameTitleH;

	//draw title image
	al_draw_scaled_bitmap(
		gameTitle,
		0, 0,
		gameTitleW, gameTitleH,
		offset_w, offset_h,
		gameTitleW * scale, gameTitleH * scale,
		0
	);
	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 255),
		SCREEN_W/2,
		SCREEN_H - 150,
		ALLEGRO_ALIGN_CENTER,
		"PRESS \"ENTER\""
	);

	


		// [HACKATHON 3-3]
		// TODO: Draw button
		// Uncomment and fill the code below
			
			if (drop == true) {
				al_draw_scaled_bitmap(choose.hovered_img, 0, 0, 288, 288, 719, 5, 80, 80, 0);
				al_draw_filled_rounded_rectangle(730, 20, 790, 236, 10, 10, al_map_rgb(190, 219, 255));
				drawButton(btnSettings);
				drawButton(exit_btn);
				drawButton(highscore);
			}
			al_draw_filled_rounded_rectangle(730, 20, 790, 65, 10, 10, al_map_rgb(190, 219, 255));
			drawButton(choose);
			if (choose.hovered == 1) {
				al_draw_filled_rounded_rectangle(730, 20, 790, 65, 10, 10, al_map_rgb(190, 219, 255));
				al_draw_scaled_bitmap(choose.default_img, 0, 0, 288, 288, 719, 5, 80, 80, 0);
			}
			if (drop == true) {
				al_draw_filled_rounded_rectangle(730, 20, 790, 65, 10, 10, al_map_rgb(190, 219, 255));
				al_draw_scaled_bitmap(choose.hovered_img, 0, 0, 288, 288, 721, 5, 80, 80, 0);
			}
}

static void on_mouse_move(int a, int mouse_x, int mouse_y, int f) {
	//	[HACKATHON 3-7]
	//	TODO: Update button's status(hovered), and utilize the function `pnt_in_rect`, which you just implemented
	//	Uncomment and fill the code below
	btnSettings.hovered = buttonHover(btnSettings, mouse_x, mouse_y);
	choose.hovered = buttonHover(choose, mouse_x, mouse_y);
	exit_btn.hovered = buttonHover(exit_btn, mouse_x, mouse_y);
	highscore.hovered = buttonHover(highscore, mouse_x, mouse_y);
}


//	[HACKATHON 3-8]
//	TODO: When btnSettings clicked, switch to settings scene
//  `game_change_scene` is defined in `game.h`.
//  You can check line 121 `scene_menu.c` to see how to use this function.
//  And find the corresponding function for `scene_setting`
// 	Utilize the member defined in struct `Button`.
//  The logic here is 
//  `if clicked the mouse` && `mouse position is in the area of button`
// 	      `Enter the setting scene`
//	Uncomment and fill the code below

static void on_mouse_down() {
	if (btnSettings.hovered) {
		game_change_scene(scene_settings_create());
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
	}
	if (choose.hovered) {
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		if (drop == true) {
			drop = false;
		}
		else {
			drop = true;
		}
	}
	if (exit_btn.hovered) {
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
		exit(1);
	}
	if (highscore.hovered) {
		game_change_scene(scene_highscore_create());
		stop_bgm(effect_audio);
		effect_audio = play_audio(effect, effect_volume);
	}
}


static void destroy() {
	stop_bgm(menuBGM);
	al_destroy_bitmap(gameTitle);
	//	[HACKATHON 3-10]
	//	TODO: Destroy button images
	//	Uncomment and fill the code below
	
	al_destroy_bitmap(btnSettings.default_img);
	al_destroy_bitmap(btnSettings.hovered_img);
	al_destroy_bitmap(exit_btn.default_img);
	al_destroy_bitmap(exit_btn.hovered_img);
	al_destroy_bitmap(choose.default_img);
	al_destroy_bitmap(choose.hovered_img);
	al_destroy_bitmap(highscore.default_img);
	al_destroy_bitmap(highscore.hovered_img);
	
}

static void on_key_down(int keycode) {

	switch (keycode) {
		case ALLEGRO_KEY_ENTER:
			stop_bgm(effect_audio);
			effect_audio = play_audio(effect, effect_volume);
			game_change_scene(scene_main_create());
			break;
		default:
			break;
	}
}

// TODO: Add more event callback functions such as update, ...

// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_menu_create(void) {
	drop = false;
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Menu";
	scene.initialize = &init;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_move = &on_mouse_move;
	// [HACKATHON 3-9]
	// TODO: Register on_mouse_down.
	// Uncomment the code below.
	
	scene.on_mouse_down = &on_mouse_down;
	
	// -------------------------------------



	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Menu scene created");
	return scene;
}