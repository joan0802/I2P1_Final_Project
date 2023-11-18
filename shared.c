// [shared.c]
// you should define the shared variable declared in the header here.
#include "shared.h"
#include "utility.h"
#include "game.h"
// #include "scene_menu.h"

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_SAMPLE* themeMusic = NULL;
ALLEGRO_SAMPLE* PACMAN_MOVESOUND = NULL;
ALLEGRO_SAMPLE* PACMAN_DEATH_SOUND = NULL;
ALLEGRO_FONT* menuFont = NULL;
ALLEGRO_FONT* scoreFont = NULL;
ALLEGRO_BITMAP* sound = NULL;
ALLEGRO_BITMAP* plus = NULL;
ALLEGRO_BITMAP* minus = NULL;
ALLEGRO_BITMAP* sound_range = NULL;
ALLEGRO_BITMAP* sound_mute = NULL;
ALLEGRO_BITMAP* lose = NULL;
ALLEGRO_BITMAP* shoes = NULL;
ALLEGRO_BITMAP* cage = NULL;
ALLEGRO_BITMAP* keyboard = NULL;
ALLEGRO_BITMAP* wasd = NULL;
ALLEGRO_BITMAP* uldr = NULL;
ALLEGRO_BITMAP* leader = NULL;
ALLEGRO_BITMAP* win = NULL;
ALLEGRO_SAMPLE* set_bgm = NULL;
ALLEGRO_SAMPLE* effect = NULL;
ALLEGRO_SAMPLE* lose_bgm = NULL;
ALLEGRO_SAMPLE* game_bgm = NULL;
ALLEGRO_SAMPLE* win_bgm = NULL;
ALLEGRO_TIMER* pman_speed_up = NULL;
ALLEGRO_TIMER* drop_down = NULL;

//ALLEGRO_BITMAP* scoreboard = NULL;

int fontSize = 30;
int key_mode = 1;
float music_volume = 0.5;
float effect_volume = 0.5;
bool gameDone = false;

/*
	A way to accelerate is load assets once.
	And delete them at the end.
	This method does provide you better management of memory.
	
*/
void shared_init(void) {
	
	menuFont = load_font("Assets/Minecraft.ttf", fontSize);
	themeMusic = load_audio("Assets/Music/original_theme.ogg");
	PACMAN_MOVESOUND = load_audio("Assets/Music/pacman-chomp.ogg");
	PACMAN_DEATH_SOUND = load_audio("Assets/Music/pacman_death.ogg");
	scoreFont = load_font("Assets/Minecraft.ttf", fontSize);
	set_bgm = load_audio("Assets/setting_music.ogg");
	effect = load_audio("Assets/Music/button.ogg");
	lose_bgm = load_audio("Assets/Music/pacman_die.ogg");
	pman_speed_up = al_create_timer(1.0f);
	shoes = al_load_bitmap("Assets/shoes.png");
	cage = al_load_bitmap("Assets/cage.png");
	keyboard = al_load_bitmap("Assets/keyboard.png");
	wasd = al_load_bitmap("Assets/WASD.png");
	uldr = al_load_bitmap("Assets/ULDR.png");
	leader = al_load_bitmap("Assets/leaderboard.png");
	game_bgm = load_audio("Assets/Music/game_bgm.ogg");
	win = al_load_bitmap("Assets/win.png");
	win_bgm = load_audio("Assets/Music/win.ogg");
	drop_down = al_create_timer(1.0f);

}

void shared_destroy(void) {

	al_destroy_font(menuFont);
	al_destroy_sample(themeMusic);
	al_destroy_sample(PACMAN_MOVESOUND);
	al_destroy_sample(PACMAN_DEATH_SOUND);
	al_destroy_font(scoreFont);
	al_destroy_sample(set_bgm);
	al_destroy_sample(lose_bgm);
	al_destroy_timer(pman_speed_up);
	al_destroy_bitmap(shoes);
	al_destroy_bitmap(cage);
	al_destroy_bitmap(keyboard);
	al_destroy_bitmap(wasd);
	al_destroy_bitmap(uldr);
	al_destroy_bitmap(leader);
	al_destroy_sample(game_bgm);
	al_destroy_bitmap(win);
	al_destroy_sample(win_bgm);
	al_destroy_timer(drop_down);

}