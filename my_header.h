#ifndef _MY__HEADER_
#define _MY__HEADER_

#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

#define FILE_OPENED_FAILED 1
#define SCORE_PER_BLOCK 100
#define MAX_CHAR 80

enum Game_mode_t {START, RUNNING, STOPPED, PAUSE, MENU, QUIT, SETTING, MSGBOX, HELP, HIGH_SCORE, GAME_OVER, LOADING, NUM_GAME_MODE};
enum Init_mode_t{NEW_GAME, CONTINUE, RESET, LEVEL_UP, NUM_INIT_MODE};
enum High_score_mode_t{FROM_GAME, FROM_MENU};

typedef struct Point{
    double x;
    double y;
} Point;

typedef struct Colour{
    double r, g, b;
} Colour;

typedef struct{
    clock_t start;
    clock_t paused;
    clock_t now;
} Clock;

typedef struct{
    short int second;
    short int minute;
    short int hour;
} Time;

typedef struct Ball{
    double radius;
    Point centre;
    Point speed;
    Colour colour;
} Ball;

typedef struct Bar{
    double length;
    double width;
    Point pos;
    Point speed;
    Colour colour;
} Bar;

/* Blocks */
#define BLOCK_DIM_X 20
#define BLOCK_DIM_Y 20

typedef struct Blocks{
    char **presence;//[BLOCK_DIM_Y][BLOCK_DIM_X+5];
    char **special;
    double length;
    double width;
    int no_of_block;
    int Total_block;
} Blocks;

/* Score */
#define LEN_NAME 25

typedef struct Score{
    char player_name[LEN_NAME];
    int point;
    int lives;
    int level;
    Clock time;
} Score;

typedef struct {
    char player_name[LEN_NAME];
    int point;
    int level;
    int seconds;
} High_score;


typedef struct Game_var{
    Ball ball;
    Bar bar;
    Blocks block;
    Score score;
} Game_var;

typedef struct Box{
    Point LL, UR;
} Box;

typedef struct{int i; int j;} Idx_pair;

enum Power_up_t {SHORT_BAR, WIDE_BAR, EXTRA_LIFE, FAST_BALL, SLOW_BALL, IMMEDIATE_DEATH, SUPER_SHRINK, SHRINK_BALL, GRAB_PADDLE,
                THROUGH_BALL, FIRE_BALL, NO_POWER_UP};
enum Touch_t{TOUCHED, NOT_TOUCHED};
typedef struct{
    int show;
    Point pos;
    Point dim;
    Power_up_t name;
} Power_up;

enum Control_t {MOVE_LEFT, MOVE_RIGHT, C_QUIT, C_PAUSE, NO_CONTROL};
typedef struct{
    unsigned char control[NO_CONTROL];
} Controls;

const Box PLAY_REG = {{0, 0}, {1000, 600}}, SCORE_DISP = {{0, 600}, {1000, 650}}, BLOCK_SCR = {{2, 200},{998, 550} };
const Box SCREEN = {{0, 0}, {1000, 650}};

void draw_game(void);

void move_ball(void);
void move_bar(unsigned char key, Game_mode_t);
void move_bar2(int mx, int my, Game_mode_t);
void change_len(int);

void draw_block(void);

void update_score(int);
void update_lives(int);
void change_name(char *);

void draw_score_board(void);

int init_value(void);
void change_game_state(Game_mode_t s);
void change_init_mode(Init_mode_t m);

void draw_menu(void);
void menu_navig_sk(unsigned char key);
void menu_key(unsigned char key);
void menu_mouse(int button, int state, int mx, int my);
void menu_mouse_move(int mx, int my);

void draw_quit_box(void);
void quit_msgbox_nvg_sk(unsigned char key);
void quit_msgbox_nvg(unsigned char key);
void quitmsg_mouse(int button, int state, int mx, int my);
void quitmsg_mouse_move(int mx, int my);

void draw_game_over(void);
void game_over_nvg_sk(unsigned char key);
void game_over_nvg(unsigned char key);
void game_over_mouse(int button, int state, int mx, int my);
void game_over_mouse_move(int mx, int my);

void disp_high_score(void);
void high_score_nvg(unsigned char key);
void high_score_nvg_sk(unsigned char key);
void high_score_mouse(int button, int state, int mx, int my);
void high_score_mouse_move(int mx, int my);

void draw_help(void);
void help_key(unsigned char key);
void help_sp_key(unsigned char key);
void help_mouse(int button, int state, int mx, int my);
void help_mouse_move(int mx, int my);

void start_game(Clock *t);
void resume_game(void);
void pause_game(void);
void clock_to_string(char * des, clock_t t);
Time clock_to_time(clock_t t);

void save_game(void);
int update_highscore(char*);
void Draw_high_score_menu(High_score_mode_t m);
void change_high_score_mode(High_score_mode_t m);

void draw_text(int x, int y);
int export_string(char * des, const int buffer_size);
void text_key(unsigned char key);
void text_key_nvg(unsigned char key);
void set_error_msg(const char *text);
void set_msg(const char *text);
void set_max_char(int no_of_char);

void draw_setting(void);
void mm_setting(int mx, int my);
void mouse_setting(int button, int state, int mx, int my);
void sp_key_setting(unsigned char key);
void key_setting(unsigned char key);

void add_power_up(double x, double y);
void remove_power_up(Touch_t t, int idx);
Power_up_t set_power_up_t(int i);
void draw_power_up(void);
void move_power_up(void);
void cancel_all_power_up(void);

void start_fire_ball(void);
int is_fire_ball_active(void);
void start_through_ball(void);
int is_through_ball_active(void);
void start_grab_paddle(void);
int is_grab_paddle_active(void);

int what_image(char *str);
void myPlaySound(int index, int mode);
void set_game_over(void);
void draw_quit_score_board(void);
int change_sound_setting(void);
int is_sound_on(void);

#endif // _MY__HEADER_
