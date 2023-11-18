#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "shared.h"
#include "utility.h"
#include "scene_game.h"
#include "scene_menu.h"
#include "pacman_obj.h"
#include "ghost.h"
#include "map.h"
#include "lose.h"
#include "win.h"


// [HACKATHON 2-0]
// Just modify the GHOST_NUM to 1
#define GHOST_NUM 4
/* global variables*/
extern const uint32_t GAME_TICK_CD;
extern uint32_t GAME_TICK;
extern ALLEGRO_TIMER* game_tick_timer;
extern ALLEGRO_TIMER* pman_speed_up;
extern ALLEGRO_FONT* scoreFont;
extern ALLEGRO_BITMAP* scoreboard;
extern ALLEGRO_SAMPLE* game_bgm;
extern int beansNum;
extern FILE* leaderboard;
int game_main_Score = 0;
bool game_over = false;
char score[10000];

/* Internal variables*/
ALLEGRO_TIMER* power_up_timer;
const int power_up_duration = 10;
static Pacman* pman;
static Map* basic_map;
static Ghost** ghosts;
ALLEGRO_SAMPLE_ID game_music;
FILE* tmp;
bool debug_mode = false;
bool cheat_mode = false;

/* Declare static function prototypes */
static void init(void);
static void step(void);
static void checkItem(void);
static void status_update(void);
static void update(void);
static void draw(void);
static void printinfo(void);
static void destroy(void);
static void on_key_down(int key_code);
static void on_mouse_down(void);
static void render_init_screen(void);
static void draw_hitboxes(void);
extern int score_board[11];
extern int key_mode;
void pacman_power_up(void);
void pacman_power_down(void);
void ghost_cage();

extern void go_back_to_cage(Ghost* ghost);
extern void pacman_shoes(Pacman* pman);

void ghost_cage() {
	for (int i = 0; i < GHOST_NUM; i++) {
		game_log("ghost_cage create");
		go_back_to_cage(ghosts[i]);
	}
}

void pacman_power_up(void) {

	//al_set_timer_count(power_up_timer, 0);
	al_start_timer(power_up_timer);
	
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_toggle_FLEE(ghosts[i], 1);
	}
}

void pacman_power_down(void) {

	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_toggle_FLEE(ghosts[i], 0);
	}

	al_stop_timer(power_up_timer);
	al_set_timer_count(power_up_timer, 0);
}

static void init(void) {
	game_over = false;
	game_main_Score = 0;
	// create map
	//basic_map = create_map(NULL);
	// [TODO]
	// Create map from .txt file and design your own map !!
	basic_map = create_map("Assets/map_nthu.txt");
	if (!basic_map) {
		game_abort("error on creating map");
	}	
	// create pacman
	pman = pacman_create();
	if (!pman) {
		game_abort("error on creating pacamn\n");
	}
	
	// allocate ghost memory
	// [HACKATHON 2-1]
	// TODO: Allocate dynamic memory for ghosts array.
	
	ghosts = (Ghost**)malloc(sizeof(Ghost*) * GHOST_NUM);
	
	if(!ghosts){
		game_log("We haven't create any ghosts!\n");
	}
	else {
		// [HACKATHON 2-2]
		// TODO: create a ghost.
		// Try to look the definition of ghost_create and figure out what should be placed here.
		for (int i = 0; i < GHOST_NUM; i++) {
			
			game_log("creating ghost %d\n", i);
			ghosts[i] = ghost_create(i);  
			if (!ghosts[i])
				game_abort("error creating ghost\n");
			
		}
	}
	GAME_TICK = 0;

	render_init_screen();
	power_up_timer = al_create_timer(1.0f); // 1 tick / sec
	if (!power_up_timer)
		game_abort("Error on create timer\n");
	if (!scoreFont) {
		game_abort("Error on scoreboard\n");
	}

	return ;
}

static void step(void) {
	if (pman->objData.moveCD > 0)
		pman->objData.moveCD -= pman->speed;
	for (int i = 0; i < GHOST_NUM; i++) {
		// important for movement
		if (ghosts[i]->objData.moveCD > 0)
			ghosts[i]->objData.moveCD -= ghosts[i]->speed;
	}
}
static void checkItem(void) {
	int Grid_x = pman->objData.Coord.x, Grid_y = pman->objData.Coord.y;
	if (Grid_y >= basic_map->row_num - 1 || Grid_y <= 0 || Grid_x >= basic_map->col_num - 1 || Grid_x <= 0)
		return;
	// [HACKATHON 1-3]
	// TODO: check which item you are going to eat and use `pacman_eatItem` to deal with it.
	
	switch (basic_map->map[Grid_y][Grid_x])
	{
	case '.':
		pacman_eatItem(pman, '.');
		game_main_Score += 10;
		basic_map->beansNum--;
		//basic_map->map[Grid_x][Grid_y] = ' ';
		break;
	case 'P':
		pacman_eatItem(pman, 'P');
		game_main_Score += 10;
		break;
	case 'S':
		pacman_eatItem(pman, 'S');
		game_main_Score += 10;
		break;
	case 'C':
		pacman_eatItem(pman, 'C');
		game_main_Score += 10;
		break;
	default:
		break;
	}
	
	// [HACKATHON 1-4]
	// erase the item you eat from map
	// be careful no erasing the wall block.
	
		basic_map->map[Grid_y][Grid_x] = ' ';
	
}
static void status_update(void) {
	for (int i = 0; i < GHOST_NUM; i++) {
		if (ghosts[i]->status == GO_IN)
			continue;
		// [TODO]
		// use `getDrawArea(..., GAME_TICK_CD)` and `RecAreaOverlap(..., GAME_TICK_CD)` functions to detect
		// if pacman and ghosts collide with each other.
		// And perform corresponding operations.
		// [NOTE]
		// You should have some branch here if you want to implement power bean mode.
		// Uncomment Following Code
		const RecArea gst_pos = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		const RecArea p_pos = getDrawArea(pman->objData, GAME_TICK_CD);
		if (!cheat_mode && (RecAreaOverlap(gst_pos, p_pos) == true) && ghosts[i]->status != FLEE){
			game_log("collide with ghost\n");
			al_rest(1.0);
			pacman_die();
			game_over = true;
			break;
		}
		else if (ghosts[i]->status == FLEE && (RecAreaOverlap(gst_pos, p_pos) == true)) {
			game_log("ghost has been eaten by Pacman\n");
			game_main_Score += 200;
			ghost_collided(ghosts[i]);
			break;
		}
		
	}
}

static void update(void) {

	if (game_over) {
		/*
			[TODO]
			start pman->death_anim_counter and schedule a game-over event (e.g change scene to menu) after Pacman's death animation finished
			game_change_scene(...);
		*/
		stop_bgm(game_music);
		al_start_timer(pman->death_anim_counter);
		if (al_get_timer_count(pman->death_anim_counter) == 13*8) {

			tmp = fopen("Assets/Interface.txt", "w");
			int t;
			
			score_board[10] = game_main_Score;

			for (int i = 0; i < 11; i++) {
				for (int j = i; j < 11; j++) {
					if (score_board[i] < score_board[j]) {
						int t = score_board[i];
						score_board[i] = score_board[j];
						score_board[j] = t;
					}
				}
			}

			for (int i = 0; i < 11; i++) {
				fprintf(tmp, "%d ", score_board[i]);
			}
			fclose(tmp);

			game_change_scene(scene_lose_create());
		}
		return;
	}
	if (basic_map->beansNum == 0) {
		game_log("win");
		
		game_change_scene(scene_win_create());
		return;
	}
	step();
	checkItem();
	status_update();
	pacman_move(pman, basic_map);

	if (al_get_timer_count(power_up_timer) == power_up_duration) {
		pacman_power_down();
		pman->powerUp = false;
	}
	if (al_get_timer_count(pman_speed_up) == 5) {
		pacman_shoes(pman);
	}

	for (int i = 0; i < GHOST_NUM; i++) 
		ghosts[i]->move_script(ghosts[i], basic_map, pman);
}

static void draw(void) {

	al_clear_to_color(al_map_rgb(0, 0, 0));

	
	//	[TODO
	//	Draw scoreboard, something your may need is sprinf();
	/*
		al_draw_text(...);
	*/
	//al_draw_bitmap(scoreboard, 10, 10, 0);
	sprintf(score, "Score = %d", game_main_Score);
	al_draw_text(scoreFont, al_map_rgb(255,255,255), 10, 10, 0, score);

	draw_map(basic_map);

	pacman_draw(pman);
	if (game_over)
		return;
	// no drawing below when game over
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
		//game_log("%d", i);
	}
	
	//debugging mode
	if (debug_mode) {
		draw_hitboxes();
	}

}

static void draw_hitboxes(void) {
	RecArea pmanHB = getDrawArea(pman->objData, GAME_TICK_CD);
	al_draw_rectangle(
		pmanHB.x, pmanHB.y,
		pmanHB.x + pmanHB.w, pmanHB.y + pmanHB.h,
		al_map_rgb_f(1.0, 0.0, 0.0), 2
	);

	for (int i = 0; i < GHOST_NUM; i++) {
		RecArea ghostHB = getDrawArea(ghosts[i]->objData, GAME_TICK_CD);
		al_draw_rectangle(
			ghostHB.x, ghostHB.y,
			ghostHB.x + ghostHB.w, ghostHB.y + ghostHB.h,
			al_map_rgb_f(1.0, 0.0, 0.0), 2
		);
	}

}

static void printinfo(void) {
	game_log("pacman:\n");
	game_log("coord: %d, %d\n", pman->objData.Coord.x, pman->objData.Coord.y);
	game_log("PreMove: %d\n", pman->objData.preMove);
	game_log("NextTryMove: %d\n", pman->objData.nextTryMove);
	game_log("Speed: %f\n", pman->speed);
}


static void destroy(void) {

	stop_bgm(game_music);
	pacman_destroy(pman);
	delete_map(basic_map);
	int index = 0;
	for (int i = 0; i < GHOST_NUM; i++) {
		free(ghosts[i]);
	}
	free(ghosts);
	/*
		[TODO]
		free map array, Pacman and ghosts
	*/
}

static void on_key_down(int key_code) {

	if (key_mode == 1) {
		switch (key_code)
		{
			// [HACKATHON 1-1]	
			// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
			// we provided you a function `pacman_NextMove` to set the pacman's next move direction.

		case ALLEGRO_KEY_W:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_A:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_S:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_D:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode)
				printf("cheat mode on\n");
			else
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;

		default:
			break;
		}
	}
	else {
		switch (key_code)
		{
			// [HACKATHON 1-1]	
			// TODO: Use allegro pre-defined enum ALLEGRO_KEY_<KEYNAME> to controll pacman movement
			// we provided you a function `pacman_NextMove` to set the pacman's next move direction.

		case ALLEGRO_KEY_UP:
			pacman_NextMove(pman, UP);
			break;
		case ALLEGRO_KEY_LEFT:
			pacman_NextMove(pman, LEFT);
			break;
		case ALLEGRO_KEY_DOWN:
			pacman_NextMove(pman, DOWN);
			break;
		case ALLEGRO_KEY_RIGHT:
			pacman_NextMove(pman, RIGHT);
			break;
		case ALLEGRO_KEY_C:
			cheat_mode = !cheat_mode;
			if (cheat_mode) {
				printf("cheat mode on\n");
			}
			else
				printf("cheat mode off\n");
			break;
		case ALLEGRO_KEY_G:
			debug_mode = !debug_mode;
			break;

		default:
			break;
		}
	}

}

static void on_mouse_down(void) {
	// nothing here

}

static void render_init_screen(void) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	draw_map(basic_map);
	pacman_draw(pman);
	for (int i = 0; i < GHOST_NUM; i++) {
		ghost_draw(ghosts[i]);
	}

	al_draw_text(
		menuFont,
		al_map_rgb(255, 255, 0),
		400, 400,
		ALLEGRO_ALIGN_CENTER,
		"READY!"
	);
	game_music = play_bgm(game_bgm, music_volume);
	al_flip_display();
	al_rest(2.0);

}
// Functions without 'static', 'extern' prefixes is just a normal
// function, they can be accessed by other files using 'extern'.
// Define your normal function prototypes below.

// The only function that is shared across files.
Scene scene_main_create(void) {
	Scene scene;
	memset(&scene, 0, sizeof(Scene));
	scene.name = "Start";
	scene.initialize = &init;
	scene.update = &update;
	scene.draw = &draw;
	scene.destroy = &destroy;
	scene.on_key_down = &on_key_down;
	scene.on_mouse_down = &on_mouse_down;
	// TODO: Register more event callback functions such as keyboard, mouse, ...
	game_log("Start scene created");
	return scene;
}
