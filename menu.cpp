#include "my_graphics.h"
#include "my_header.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

const Point DIM = {290., 150.};
extern Controls set1;

typedef struct{
    char * text;
    Point pos;
    Point dim;
} Option;

typedef struct{
    int total_op;
    int selected;
} Menu_var;

void draw_option(const Menu_var *menu,const Option op_name[]);
int is_in_menu(int mx, int my,const Option *op);

Option new_game = {"New Game", {55, 300}, {DIM.x, DIM.y}},
        cont    = {"Continue", {55 + DIM.x + 10, 300}, {DIM.x, DIM.y}},
        high_score1 = {"High score", {55 + 2 * DIM.x + 2* 10, 300}, {DIM.x, DIM.y}},
        setting = {"Setting",     {55, 300-DIM.y - 10}, {DIM.x, DIM.y}},
        how_to_play = {"How To Play", {55+DIM.x + 10, 300-DIM.y -10}, {DIM.x, DIM.y}},
        quit1     = {"Quit", {55+ 2 * DIM.x + 2*10, 300-DIM.y -10}, {DIM.x, DIM.y}};

Option *all_op[] = {&new_game, &cont, &high_score1, &setting, &how_to_play, &quit1};
Menu_var menu = {6, 0};

void draw_dx_ball(void){
    Point centres[] = {
        {303.5, 562}, {304, 552}, {306, 540}, {306, 531}, {306, 520}, {316, 562}, {316, 520}, {326, 562}, {326, 520}, {335, 549},
        {335, 549}, {335, 529}, {345, 540}, /*D*/
        {365, 559}, {400, 560}, {373, 548}, {392, 548}, {383, 539}, {373, 529}, {389, 531}, {363, 519}, {399, 520}, /*X*/
        {419, 541}, {430, 542}, {442, 542}, /* - */
        {463, 562}, {462, 551}, {463, 542}, {463, 531}, {463, 522}, {475, 522}, {486, 520}, {497, 520}, {472, 562},
        {484, 562}, {496, 561}, {507, 550}, {495, 540}, {506, 531}, /* B */
        {543, 559}, {536, 548}, {523, 540}, {553, 547}, {562, 538}, {525, 528},
        {533, 528}, {542, 528}, {552, 527}, {560, 528}, {560, 520}, {525, 516}, /*A*/
        {580, 560}, {580, 548}, {580, 540}, {581, 529}, {579, 520}, {590, 521}, {600, 521}, {610, 522}, {625, 522}, /* L */
         {642, 563}, {642, 551}, {643, 543}, {645, 532}, {645, 522}, {656, 521}, {664, 522}, {678, 519}, {686, 519}, /* L */
    };
    double radius = 4;
    int no_of_circle = 69;

    for(int i = 0; i < no_of_circle; i++){
        if(clock() % 1000 > 500/* rand()%2*/)
            iSetColor(0, 0, 0);
        else
            iSetColor(255, 255, 255);

        //iSetColor(rand()% 255, rand()%255, rand()%255);
        iFilledCircle(centres[i].x, centres[i].y, radius);
    }

}

void draw_menu(){
    char *img_name[] = {"menu\\new_game.bmp", "menu\\continue.bmp", "menu\\high_score.bmp","menu\\setting.bmp",
                        "menu\\how_to_play.bmp", "menu\\quit.bmp", };
    int img_len[] = {117, 95, 126, 77, 145, 51};

    char bg_img_name[80];
    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
    }

    iShowBMP2(20, 20, "msg.bmp", 0x000000);

    draw_dx_ball();

    for(int i = 0; i < menu.total_op; i++){
        if(i != menu.selected){
            iSetColor(255, 255, 255);
            //iText(all_op[i]->pos.x + 10, all_op[i]->pos.y + 10, all_op[i]->text, GLUT_BITMAP_TIMES_ROMAN_24);
            iShowBMP2(all_op[i]->pos.x, all_op[i]->pos.y, img_name[i], 0x000000);
            iRectangle(all_op[i]->pos.x, all_op[i]->pos.y, all_op[i]->dim.x, all_op[i]->dim.y);


        }else{
            iSetColor(255, 255, 255);
            iShowBMP2(all_op[i]->pos.x + DIM.x/2 - (double)img_len[i]/2, all_op[i]->pos.y + DIM.y/2-15, img_name[i], 0x000000);
            iRectangle(all_op[i]->pos.x, all_op[i]->pos.y, all_op[i]->dim.x, all_op[i]->dim.y);

            iRectangle(all_op[i]->pos.x+2, all_op[i]->pos.y+2, all_op[i]->dim.x - 4, all_op[i]->dim.y -4);
        }
    }
}


void menu_navig_sk(unsigned char key){
    switch(key){
    case GLUT_KEY_RIGHT:
        if(menu.selected != menu.total_op-1) menu.selected++;
        break;

    case GLUT_KEY_LEFT:
        if(menu.selected != 0) menu.selected--;
        break;

    case GLUT_KEY_DOWN:
        if(menu.selected / 3 != menu.total_op/3 - 1) menu.selected += 3;
        break;

    case GLUT_KEY_UP:
        if(menu.selected /3 != 0)
            menu.selected -= 3;
        break;

    case GLUT_KEY_F2:
        change_game_state(HELP);
        break;

    }
}

void menu_key(unsigned char key){
    Game_mode_t menu_selection[] = {START, START, HIGH_SCORE, SETTING, HELP, QUIT};

    if(key == '\r'){
        if(menu.selected == 2){
            change_high_score_mode(FROM_MENU);
            change_game_state(HIGH_SCORE);
        }
        else if(menu.selected > 2)
            change_game_state(menu_selection[menu.selected]);

        else{
            change_init_mode((menu.selected)? CONTINUE: NEW_GAME);
            change_game_state(START);
        }
    }

    else if(key == 'h' || key == 'H'){
        change_game_state(HELP);
    }
}

void menu_mouse(int button, int state, int mx, int my){

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        for(int i = 0; i < menu.total_op; i++){
            if(is_in_menu(mx, my, all_op[i])){

                if(menu.selected == i)
                    menu_key('\r');
                else
                    menu.selected = i;

                return ;
            }
        }
    }
}

void menu_mouse_move(int mx, int my){
    for(int i = 0; i < menu.total_op; i++){
        if(is_in_menu(mx, my, all_op[i])){
            menu.selected = i;
            return ;
        }
    }
}

/******************************quit game menu *******************************************************/
Menu_var quit = {3, 0};

Option quit_menu[] = {
    {"Save and Quit", 220, 320, 500, 30},
    {"Yes ", 220, 280, 500, 30},
    {"No ", 220, 240, 500, 30},
};
enum {SAVE_AND_QUIT, YES, NO};

void draw_quit_box(void){
    char bg_img_name[80];
    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
    }

    iSetColor(255, 255, 255);
    iRectangle(200, 220, 550, 190);
    iRectangle(198, 218, 554, 194);

    iLine(198, 218, 200, 220);
    iLine(198, 412, 200, 410);
    iLine(752, 218, 750, 220);
    iLine(752, 412, 750, 410);

    iSetColor(255, 255, 255);
    iText(220, 375, "Do you want to exit?? Any unsaved progress will be lost.", GLUT_BITMAP_9_BY_15);

    draw_option(&quit, quit_menu);
}

void quit_msgbox_nvg_sk(unsigned char key){
    switch(key){
    case GLUT_KEY_DOWN:
        quit.selected = (quit.selected+1) % quit.total_op;
        break;

    case GLUT_KEY_UP:
        quit.selected = (quit.selected - 1 + quit.total_op) % quit.total_op;
        break;

    }
}

void quit_msgbox_nvg(unsigned char key){
    if(key == '\r'){
        switch(quit.selected){
        case SAVE_AND_QUIT:
            save_game();
            change_init_mode(RESET);
            change_game_state(MENU);
            break;

        case YES:
            change_init_mode(RESET);
            change_game_state(MENU);
            break;

        case NO:
            change_game_state(PAUSE);
            break;
        }
    }
}

void quitmsg_mouse(int button, int state, int mx, int my){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        for(int i = 0; i < quit.total_op; i++){
            if(is_in_menu(mx, my, &quit_menu[i])){

                if(quit.selected == i)
                    quit_msgbox_nvg('\r');
                else
                    quit.selected = i;
            }
        }
    }
}

void quitmsg_mouse_move(int mx, int my){
    for(int i = 0; i < quit.total_op; i++){
        if(is_in_menu(mx, my, &quit_menu[i])){

            quit.selected = i;
        }
    }
}

/******************************** game over *******************************************/
Menu_var game_over = {2, 0};

Option op_game_over[] = {
    {"Save Your Name", 220, 320, 500, 30},
    {"Continue Without Saving Name", 220, 280, 500, 30}
};

enum {WITHOUT_BOX, WITH_BOX} option = WITHOUT_BOX;
enum {SAVE, NOT_SAVE} ;

clock_t game_over_start = 0;
int game_over_show = 0;

void set_game_over(void){
    game_over_start = clock();
    game_over_show = 1;
}

void draw_game_over(void){
    if(game_over_show && (clock() - game_over_start)/CLOCKS_PER_SEC < 10){
        iSetColor(18, 13, 11);
        iFilledRectangle(0, 0, 1000, 650);
        iShowBMP2(140.5, 266, "game_over.bmp", 0x000000);
        draw_quit_score_board();
        return ;
    }

    else{
        myPlaySound(0, 0);
        game_over_show = 0;
    }

    char bg_img_name[80];
    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
    }

    if(option == WITHOUT_BOX){
        iSetColor(255, 255,255);
        iRectangle(200, 270, 550, 117);
        iRectangle(198, 268, 554, 121);

        iSetColor(255, 255, 255);
        iText(420, 370, "Game Over !", GLUT_BITMAP_9_BY_15);

        draw_option(&game_over, op_game_over);
    }

    else if(option == WITH_BOX){
        iSetColor(255, 255, 255);
        iRectangle(200, 220, 550, 185);
        iRectangle(198, 218, 554, 189);

        iSetColor(255, 255, 255);
        iText(420, 370, "Game Over !", GLUT_BITMAP_9_BY_15);

        for(int i = 0; i < game_over.total_op; i++){
            iSetColor(200, 200,200);
            iRectangle(op_game_over[i].pos.x, op_game_over[i].pos.y - i*40, op_game_over[i].dim.x, op_game_over[i].dim.y);
            iRectangle(op_game_over[i].pos.x-2, op_game_over[i].pos.y - i*40-2, op_game_over[i].dim.x+4, op_game_over[i].dim.y+4);
            iText(op_game_over[i].pos.x + 20, op_game_over[i].pos.y + 10 - i*40, op_game_over[i].text, GLUT_BITMAP_9_BY_15);
        }

        iSetColor(64, 64, 64);
        iRectangle(220, 280, 500, 30);
        iRectangle(218, 278, 504, 34);


        draw_text(240, 290);
    }
}


void game_over_nvg_sk(unsigned char key){
    if(game_over_show && (clock() - game_over_start)/CLOCKS_PER_SEC < 10){
        return ;
    }

    if(option == WITHOUT_BOX){
        switch(key){
        case GLUT_KEY_DOWN:
            game_over.selected = (game_over.selected+1) % game_over.total_op;
            break;

        case GLUT_KEY_UP:
            game_over.selected = (game_over.selected - 1 + game_over.total_op) % game_over.total_op;
            break;

        }
    }

    else if(option == WITH_BOX){
        if(key == GLUT_KEY_PAGE_DOWN || key == GLUT_KEY_PAGE_UP || key == GLUT_KEY_F1 || key == GLUT_KEY_DOWN || key == GLUT_KEY_UP){
            option = WITHOUT_BOX;
            return ;
        }
        text_key_nvg(key);

    }
}

void game_over_nvg(unsigned char key){
    if(game_over_show && (clock() - game_over_start)/CLOCKS_PER_SEC < 10){
        if(key == '\r'){
            game_over_show = 0;
        }
        return ;
    }

    if(option == WITHOUT_BOX){
        switch(key){
        case '\r':
            switch(game_over.selected){
            case SAVE:
                set_msg("Enter your name");
                set_max_char(LEN_NAME-1);
                option = WITH_BOX;
                break;

            case NOT_SAVE:

                //strcpy(name, "(without_name)");
                update_highscore("(without_name)");
                export_string(NULL, 0);

                change_high_score_mode(FROM_GAME);
                change_game_state(HIGH_SCORE);
                break;

            }
        }
    }

    else if(option == WITH_BOX){

        if(key == '\r'){
            char name[LEN_NAME];
            if(export_string(name, LEN_NAME-1)){
                update_highscore(name);

                change_high_score_mode(FROM_GAME);
                change_game_state(HIGH_SCORE);
            }

            else{
                set_error_msg("Name cannot be empty.Type a name and then press enter");
            }
        }

        else{
            text_key(key);
        }
    }
}

void game_over_mouse(int button, int state, int mx, int my){
    if(game_over_show && (clock() - game_over_start)/CLOCKS_PER_SEC < 10){
        return ;
    }

    if(option == WITHOUT_BOX){
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            for(int i = 0; i < game_over.total_op; i++){
                if(is_in_menu(mx, my, &op_game_over[i])){

                    if(game_over.selected == i)
                        game_over_nvg('\r');
                    else
                        game_over.selected = i;
                }
            }
        }
    }

    if(option == WITH_BOX){
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            if(! ((mx >= 220 && mx <= 220+500) && (my >= 280 && my <= 280+30))){
                option = WITHOUT_BOX;
            }
        }
    }
}

void game_over_mouse_move(int mx, int my){
    if(game_over_show && (clock() - game_over_start)/CLOCKS_PER_SEC < 10){
        return ;
    }

    if(option == WITHOUT_BOX){
        for(int i = 0; i < game_over.total_op; i++){
            if(is_in_menu(mx, my, &op_game_over[i])){
                game_over.selected = i;
            }
        }
    }
}


/****************************************** High Score Menu **************************************************/
Menu_var high_score = {2, 0};
Point DIM_HS = {350, 30};

Option op_high_score[]={
    {"Main Menu", 120, 30, DIM_HS.x, DIM_HS.y},
    {"New Game", 550, 30, DIM_HS.x, DIM_HS.y}
};

enum {MAIN_MENU, NEW};

void Draw_high_score_menu(High_score_mode_t m){
    draw_option(&high_score, op_high_score);
}

void high_score_nvg_sk(unsigned char key){
    switch(key){
    case GLUT_KEY_RIGHT:
        high_score.selected++;
        high_score.selected %= high_score.total_op;
        break;

    case GLUT_KEY_LEFT:
        high_score.selected = (high_score.selected + high_score.total_op-1) % high_score.total_op;
        break;
    }
}

void high_score_nvg(unsigned char key){
    if(key == '\r'){
        switch(high_score.selected){
        case MAIN_MENU:
            change_game_state(MENU);
            break;

        case NEW:
            change_init_mode(NEW_GAME);
            change_game_state(START);
            break;

        }
    }
}

void high_score_mouse(int button, int state, int mx, int my){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        for(int i = 0; i < high_score.total_op; i++){
            if(is_in_menu(mx, my, &op_high_score[i])){
                if(high_score.selected == i)
                    high_score_nvg('\r');
                else
                    high_score.selected = i;
            }
        }
    }
}

void high_score_mouse_move(int mx, int my){
    for(int i = 0; i < high_score.total_op; i++){
        if(is_in_menu(mx, my, &op_high_score[i])){
            high_score.selected = i;
        }
    }

}

/*********************************** Help *************************************/
int TOTAL_MSG = 17;
char *help_str[] = {"Launch the ball into the game","Don't let the ball fall out of screen",
                    "Use the bar to bounce the ball into the playing region",
                     "Bricks can be broken by hitting them with the Ball" ,"Break bricks to earn Points",
                     "Clear all blocks to finish a level", "Finish all levels to win the game",
                     "Earn as many point possible, to see yourself in Leader Board","Finish As early as possible, tie is broken by time!",
                     "Be careful! Ball speed will increase with time",
                     "Hit special bricks to earn extra points", "Some bricks are Unbreakable, Don't bang your heads on breaking them!",
                     "Collect power-ups", "Positive power up helps to win the game",
                     "Negative power-ups obstructs you from winning it", "To see button help press right/left arrow or w/e",
                     "Why are you waiting!  PLAY and ENJOY!"
};

int menu_key_msg = 6;
char *menu_key_str[] = {"Up", "Up Arrow",
                        "Right", "Right Arrow",
                        "Left", "Left Arrow",
                        "Down", "Down Arrow",
                        "Help", "h or F1",
                        "Select", "ENTER"
};

int in_game_key_msg = 4;
char *in_game_key_str[] = {"Move Bar Right", "Right Arrow", "d", "Drag Mouse Right",
                            "Move Bar Left", "Left Arrow", "a", "Drag Mouse Left",
                            "Quit", "END", "Q", "Click Middle Button",
                            "Pause/Resume Game", "PAGE DOWN", "Space", "Right Click",
};

Menu_var help = {3, 2};
Point DIM_HELP = {300, 30};
Option op_help[] = {
    {"     <--Previous Page", 40, 40, DIM_HELP.x, DIM_HELP.y},
    {"      Back to Main Menu", 350, 40, DIM_HELP.x, DIM_HELP.y},
    {"        Next Page-->", 660, 40, DIM_HELP.x, DIM_HELP.y}
};

Menu_var in_game_key_help = {4, 0};
Point DIM_IN_GAME = {100, 30};
Option op_in_game_help[] = {
    {"Change", 740, 347, DIM_IN_GAME.x, DIM_IN_GAME.y},
    {"Change", 740, 347-60, DIM_IN_GAME.x, DIM_IN_GAME.y},
    {"Change", 740, 347-60*2, DIM_IN_GAME.x, DIM_IN_GAME.y},
    {"Change", 740, 347-60*3, DIM_IN_GAME.x, DIM_IN_GAME.y},
};
enum {PREV_PAGE, BACK_TO_MAIN, NEXT_PAGE};

enum {DESCRIPTION, IN_GAME_KEY, MENU_KEY} ;
Menu_var help_page = {3, 0};

void draw_help(void){
    char bg_img_name[80];
    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
        iShowBMP2(344, 550, "how-to-play.bmp", 0x000000);
    }

    double offset = 20, offset1 = 30, offset2 = 20;

    //iShowBMP(0, 0, "help_bg.bmp");

    iSetColor(255, 255, 255);
    iRectangle(10, 10, 980, 630);

    iSetColor(150, 150, 150);
    iRectangle(20, 20, 960, 610);

    iSetColor(255, 255, 255);
    iRectangle(30, 30, 940, 590);

    iSetColor(200, 200, 200);
    iText(150, 520, "How-to-Play    In-Game-Control     Menu-Navigation", GLUT_BITMAP_9_BY_15);
    iRectangle(100, 100, 810, 410);

    switch(help_page.selected){
    case DESCRIPTION:
        iSetColor(200, 200, 200);
        iRectangle(140, 510, 125, 30);
        iSetColor(0, 0, 0);
        iLine(140, 510, 265, 510);

        for(int i = 0; i < TOTAL_MSG; i++){
            iSetColor(255, 0, 0);
            iFilledCircle(200, 500 - i * 20- offset1, 3);
            iSetColor(255, 255, 255);
            iText(280, 497 - i * 20 - offset1, help_str[i], GLUT_BITMAP_9_BY_15);
        }
        break;

    case MENU_KEY:
        iSetColor(200, 200, 200);
        iRectangle(445, 510, 170, 30);
        iSetColor(0, 0, 0);
        iLine(445, 510, 615, 510);

        iSetColor(0, 255, 255);
        iText(150, 499 - offset2, "--> You can use this keys in all menu of this game. ", GLUT_BITMAP_9_BY_15);
        iText(185, 499-20 - offset2, "This keys are hard codded So you cannot change them !", GLUT_BITMAP_9_BY_15);

        // Keyboard Help
        iSetColor(200, 200, 200);
        iText(185, 425 - offset2, "Menu Navigation keyboard", GLUT_BITMAP_9_BY_15);

        for(int i = 0; i < menu_key_msg; i++){
            iSetColor(255, 0, 0);
            iFilledCircle(200, 425-20 - i * 30 - offset, 3);
            iSetColor(255, 255, 255);
            iText(350, 402 - i * 30 - offset2, menu_key_str[2*i], GLUT_BITMAP_9_BY_15);
            iText(580, 402 - i * 30 - offset2, menu_key_str[2*i+1], GLUT_BITMAP_9_BY_15);
        }

        // Mouse Help
        iSetColor(200, 200, 200);
        iText(185, 200 - offset2, "Menu Navigation Mouse", GLUT_BITMAP_9_BY_15);

        iSetColor(255, 0, 0);
        iFilledCircle(200, 200-20  - offset, 3);

        iSetColor(255, 255, 255);
        iText(280, 180  - offset2, "You can select a option by left clicking it", GLUT_BITMAP_9_BY_15);
        iText(280, 160  - offset2, "and by dragging mouse clicking left button.", GLUT_BITMAP_9_BY_15);

        break;

    case IN_GAME_KEY:
        // Draws Box around option
        iSetColor(200, 200, 200);
        iRectangle(265, 510, 180, 30);
        iSetColor(0, 0, 0);
        iLine(265, 510, 445, 510);

        iSetColor(0, 255, 255);
        iText(150, 500 - offset, "--> You can use this keys during the game to control the bar.", GLUT_BITMAP_9_BY_15);
        iText(185, 500-20 - offset, "You can change them according to your convenience", GLUT_BITMAP_9_BY_15);

        for(int i = 0; i < in_game_key_msg; i++){
            char str[MAX_CHAR];
            if(set1.control[i] != ' '){
                sprintf(str, "%c", set1.control[i]);
            }
            else{
                sprintf(str, "Space");
            }

            iSetColor(255, 0, 0);
            iFilledCircle(200, 400-20 - i * 60 - offset, 3);
            iSetColor(255, 255, 255);
            iText(320, 377 - i * 60 - offset, in_game_key_str[4*i], GLUT_BITMAP_9_BY_15);
            iText(540, 377 - i * 60 - offset, in_game_key_str[4*i+1], GLUT_BITMAP_9_BY_15);
            iText(740, 377 - i * 60 - offset, str/*in_game_key_str[4*i+2]*/, GLUT_BITMAP_9_BY_15);
            iText(540, 377 - i * 60 - 30 - offset, in_game_key_str[4*i+3], GLUT_BITMAP_9_BY_15);
        }
    }
    draw_option(&help, op_help);
}

void help_key(unsigned char key){
    if(key == '\r'){
        if(help.selected == PREV_PAGE){ // previous page
            help_page.selected = (help_page.selected + help_page.total_op -1)% help_page.total_op;
        }

        else if(help.selected == BACK_TO_MAIN){ //
            change_game_state(MENU);
        }

        else if(help.selected == NEXT_PAGE){
            ++help_page.selected %= help_page.total_op;
        }
    }

    switch(key){
    case 'W': case 'w':
        help_page.selected = (help_page.selected + help_page.total_op -1)% help_page.total_op;
        break;

    case 'E': case 'e':
        ++help_page.selected %= help_page.total_op;
        break;

    }
}

void help_sp_key(unsigned char key){
    switch(key){
    case GLUT_KEY_RIGHT:
        help.selected++;
        help.selected %= help.total_op;
        break;

    case GLUT_KEY_LEFT:
        help.selected = (help.selected + help.total_op -1) % help.total_op;
        break;

    case GLUT_KEY_HOME:
        change_game_state(MENU);
        break;

    }


}

void help_mouse(int button, int state, int mx, int my){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        for(int i = 0; i < help.total_op; i++){
            if(is_in_menu(mx, my, &op_help[i])){
                if(help.selected == i){
                    help_key('\r');

                }else{
                    help.selected = i;
                }
            }
        }
    }
}

void help_mouse_move(int mx, int my){
    for(int i = 0; i < help.total_op; i++){
        if(is_in_menu(mx, my, &op_help[i])){
            help.selected = i;
            return ;
        }
    }
}


/***************************************************************************************/

void draw_option(const Menu_var *menu,const Option op_name[]){
    for(int i = 0; i < menu->total_op; i++){
        if(i != menu->selected){
            iSetColor(200, 200,200);
            iRectangle(op_name[i].pos.x, op_name[i].pos.y, op_name[i].dim.x, op_name[i].dim.y);
            iRectangle(op_name[i].pos.x-2, op_name[i].pos.y-2, op_name[i].dim.x+4, op_name[i].dim.y+4);
            iText(op_name[i].pos.x + 20, op_name[i].pos.y + 10, op_name[i].text, GLUT_BITMAP_9_BY_15);

        }else{
            iSetColor(140, 140,140); // 0, 255, 255
            iRectangle(op_name[i].pos.x, op_name[i].pos.y, op_name[i].dim.x, op_name[i].dim.y);
            iRectangle(op_name[i].pos.x-2, op_name[i].pos.y-2, op_name[i].dim.x+4, op_name[i].dim.y+4);
            iText(op_name[i].pos.x + 20, op_name[i].pos.y + 10, op_name[i].text, GLUT_BITMAP_9_BY_15);
        }

    }
}

int is_in_menu(int mx, int my,const Option *op){
    if((mx >= op->pos.x && mx <= op->pos.x + op->dim.x) && (my >= op->pos.y && my <= op->pos.y + op->dim.y)){
        return 1;
    }

    return 0;
}

/***************************** Text input ***************************/
char name[LEN_NAME], error_message[80], no_char_text[80];
int name_idx = 0, cursor = 0, show_error = 0, max_char = LEN_NAME;

void set_msg(const char *text){
    strncpy(no_char_text, text, 80);
    show_error = 0;
}

void set_max_char(int no_of_char){
    max_char = no_of_char;
    show_error = 0;
}

void set_error_msg(const char *text){
    strncpy(error_message, text, 80);
    show_error = 1;
}

void add_char(char text[], int idx, char key){
    for(int i = strlen(name); i > idx; i--){
        text[i] = text[i-1];
    }

    text[idx] = key;
}

void delete_char(char text[], int idx){
    if(idx < 0)
        return ;

    for(int i = idx; text[i] != '\0'; i++){
        text[i] = text[i+1];
    }
}

void text_key(unsigned char key){
    myPlaySound(13, 1);

    if(key != '\b' && name_idx >= max_char){
        return ;
    }

    if(key == '\b'){
        if(cursor != 0){
            delete_char(name, cursor-1);
            --name_idx;
            --cursor;
            if(name_idx < 0) name_idx = 0;
            if(cursor < 0) cursor = 0;
        }
    }

    else if(key == ' ' ){
        add_char(name, cursor, '_');
        name_idx++;
        cursor++;
    }

    else if(cursor == 0 || name[cursor-1] == '_'){
        add_char(name, cursor, toupper(key));
        name_idx++;
        cursor++;
    }

    else {
        add_char(name, cursor, key);
        name_idx++;
        cursor++;
    }

    name[name_idx] = '\0';

    if(key != '\b') show_error = 0;
}

int export_string(char * des, const int buffer_size){
    if(des != NULL){
        strncpy(des, name, buffer_size);
    }

    int a = name_idx;
    name_idx = 0;
    cursor = 0;
    name[0] = '\0';

    return a;
}

void draw_text(int x, int y){

    /* Types text */
    if(show_error){
        iSetColor(255, 0, 0);
        iText(x, y, error_message, GLUT_BITMAP_9_BY_15);
    }

    else if(name_idx == 0){
        iSetColor(100, 100, 100);
        iText(x, y, no_char_text, GLUT_BITMAP_9_BY_15);

    }else{
        iSetColor(255, 0, 255);
        iText(x, y, name , GLUT_BITMAP_9_BY_15);

    }

    if(name_idx >= LEN_NAME-1){
        iSetColor(255, 0, 0);
        iText(x + 2 + (LEN_NAME+5)*9, y-2, "Max Name size exceeded", GLUT_BITMAP_9_BY_15);
    }
    /* ---------------------- */

    /* makes cursor */
    if(!show_error && clock()%1000 < 700){
        iSetColor(200, 200, 200);
        iLine(x + 2 + (cursor)*9, y - 2, x+2 + cursor*9, y+15);
    }
    /* -------------------- */
}

void text_key_nvg(unsigned char key){
    myPlaySound(13, 1);

    switch(key){
    case GLUT_KEY_LEFT:
        cursor--;
        if(cursor < 0) cursor = 0;
        break;

    case GLUT_KEY_RIGHT:
        cursor++;
        if(cursor > name_idx) cursor = name_idx;
        break;

    case GLUT_KEY_HOME:
        cursor = 0;
        break;

    case GLUT_KEY_END:
        cursor = name_idx;
        break;

    }
}

/******************************* Setting *****************************/
int setting_msg = 4;
char *setting_str[] = {"Move Bar Right", "Right Arrow", "d", "Drag Mouse Right",
                            "Move Bar Left", "Left Arrow", "a", "Drag Mouse Left",
                            "Quit", "END", "Q", "Click Middle Button",
                            "Pause/Resume Game", "PAGE DOWN", "Space", "Right Click",
};

Menu_var setting_key = {5, 0};
Point DIM_SETTING_KEY = {100, 30};
Option op_setting_key[] = {
    {"Change", 770, 387, DIM_SETTING_KEY.x, DIM_SETTING_KEY.y},
    {"Change", 770, 387-60, DIM_SETTING_KEY.x, DIM_SETTING_KEY.y},
    {"Change", 770, 387-60*2, DIM_SETTING_KEY.x, DIM_SETTING_KEY.y},
    {"Change", 770, 387-60*3, DIM_SETTING_KEY.x, DIM_SETTING_KEY.y},
    {"Change", 770, 387-60*4, DIM_SETTING_KEY.x, DIM_SETTING_KEY.y},
};

enum {WRITING, NOT_WRITING} setting_key_state = NOT_WRITING;

int is_duplicate_control(unsigned char key){
    for(int i = 0; i < setting_key.total_op; i++){
        if(i != setting_key.selected && set1.control[i] == key){
            return 1;
        }
    }

    return 0;
}

void draw_setting(void){
    char bg_img_name[80];
    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
        iShowBMP2(376.5, 530, "settings.bmp", 0x000000);
    }

    double offset = -30;

    /* Rectangle */
    iSetColor(255, 255, 255);
    iRectangle(10, 10, 980, 630);

    iSetColor(150, 150, 150);
    iRectangle(20, 20, 960, 610);

    iSetColor(255, 255, 255);
    iRectangle(30, 30, 940, 590);

    iSetColor(200, 200, 200);
    iText(150, 520, "Change Control ", GLUT_BITMAP_9_BY_15);
    iRectangle(100, 100, 810, 410);

    /* Around text */
    iSetColor(200, 200, 200);
    iRectangle(140, 510, 140, 30);
    iSetColor(0, 0, 0);
    iLine(140, 510, 280, 510);

    /* Setting */
    iSetColor(0, 255, 255);
    iText(150, 490, "--> You can change them according to your convenience.", GLUT_BITMAP_9_BY_15);
    //iText(185, 500-20 - offset, "", GLUT_BITMAP_9_BY_15);

    for(int i = 0; i < setting_msg; i++){
        char str[MAX_CHAR];
        if(set1.control[i] != ' '){
            sprintf(str, "%c", set1.control[i]);
        }
        else{
            sprintf(str, "Space");
        }

        iSetColor(255, 0, 0);
        iFilledCircle(200-20, 400-20 - i * 60 - offset, 3);
        iSetColor(255, 255, 255);
        iText(320-50, 377 - i * 60 - offset, setting_str[4*i], GLUT_BITMAP_9_BY_15);
        iText(540-50, 377 - i * 60 - offset, setting_str[4*i+1], GLUT_BITMAP_9_BY_15);
        iText(740-50, 377 - i * 60 - offset, str, GLUT_BITMAP_9_BY_15);
        iText(540-50, 377 - i * 60 - 30 - offset, setting_str[4*i+3], GLUT_BITMAP_9_BY_15);
    }

    if(setting_key_state == NOT_WRITING){
        draw_option(&setting_key, op_setting_key);
    }
    else if(setting_key_state == WRITING && setting_key.selected != 4){
        iSetColor(0, 0, 0);
        iFilledRectangle(690, 377 - setting_key.selected * 60 - offset, 100, 15);

        draw_text(692, 377 - setting_key.selected * 60 - offset);
    }

    iSetColor(200, 200, 200);
    iRectangle(100, 45, 810, 30);
    iText(350 , 55, "      HOME for back to Main Menu", GLUT_BITMAP_9_BY_15);


    /*** sound setting ******/
    int sound = is_sound_on();
    char *img_name[] = { "sound_off.bmp", "sound_on.bmp",};
    iShowBMP2(200-30, 400-30 - 4 * 60 - offset, img_name[sound], 0x000000);

    iSetColor(255, 255, 255);
    iText(320-50, 377 - 4 * 60 - offset, "Sound Setting", GLUT_BITMAP_9_BY_15);

    char str[5];
    sprintf(str, "%-4s", ((sound)? "Yes" : "No"));
    iText(490, 377 - 4 * 60 - offset, str, GLUT_BITMAP_9_BY_15);
}

void key_setting(unsigned char key){
    if(key == '\r' && setting_key.selected == 4){
        change_sound_setting();
    }

    else if(setting_key_state == NOT_WRITING && key == '\r'){
        setting_key_state = WRITING;
        set_msg("Enter a char");
    }

    else if(setting_key_state == WRITING){
        if(key == '\r'){
            setting_key_state = NOT_WRITING;
        }

        else if( key != '\b'){
            if(is_duplicate_control(key)){
                set_error_msg("Not unique");
            }
            else{
                set1.control[setting_key.selected] = key;
                setting_key_state = NOT_WRITING;
            }
        }
    }
}

void sp_key_setting(unsigned char key){
    switch(key){
    case GLUT_KEY_DOWN:
        setting_key.selected = (setting_key.selected+1) % setting_key.total_op;
        break;

    case GLUT_KEY_UP:
        setting_key.selected = (setting_key.selected - 1 + setting_key.total_op) % setting_key.total_op;
        break;

    case GLUT_KEY_HOME:
        change_game_state(MENU);
        break;
    }
}

void mouse_setting(int button, int state, int mx, int my){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        for(int i = 0; i < setting_key.total_op; i++){
            if(is_in_menu(mx, my, &op_setting_key[i])){
                if(setting_key.selected == i){
                    key_setting('\r');

                }else{
                    setting_key.selected = i;
                }
            }
        }
    }
}

void mm_setting(int mx, int my){
    for(int i = 0; i < setting_key.total_op; i++){
        if(is_in_menu(mx, my, &op_setting_key[i])){
            setting_key.selected = i;
            return ;
        }
    }
}
