/*
 * Md. Asib Rahman
 * CSE ,BUET
 * 20 Dec, 2020
*/

#include <stdio.h>
#include "my_header.h"
#include "iGraphics.h"

enum Game_mode_t game_state = LOADING;
Controls set1 = { {'a', 'd', 'q', ' '}, };
int do_play_sound = 1;

void kStart(unsigned char key);
void skStart(unsigned char key);
void mmStart(int mx, int my);
void mStart(int button, int state, int mx, int my);

void kRunning(unsigned char key);
void skRunning(unsigned char key);
void mRunning(int button, int state, int mx, int my);
void mmRunning(int mx, int my);

void kPause(unsigned char key);
void mPause(int button, int state, int mx, int my);
void skPause(unsigned char key);

void kStopped(unsigned char key);
void skStopped(unsigned char key);
void mmStopped(int mx, int my);
void mStopped(int button, int state, int mx, int my);

int is_loading = 1;
void draw(void){
    //iText(200, 500, "Page Under Construction", GLUT_BITMAP_TIMES_ROMAN_24);
    if(game_state == LOADING){
        iShowBMP2(244, 100, "dx_ball_loading.bmp", 0x000000);

        if(is_loading){
            iShowBMP2(475, 40, "loading.bmp", 0xFFFFFF);
        }
        else{
            iShowBMP2(280, 40, "msg2.bmp", 0x000000);
        }

    }

    else if(game_state == QUIT){
        iShowBMP(168, 217, "msg3.bmp");
    }
}
void key(unsigned char key){
    if(!is_loading && game_state == LOADING){
        change_game_state(MENU);

    }
}

void mouse(int button, int state, int mx, int my){
    ;
}

void mouse_m(int mx, int my){
    ;
}

void end_loading(void){
    if(game_state == LOADING){
        is_loading = 0;
        iPauseTimer(2);
    }
    else if(game_state == QUIT){
        exit(0);
    }
}

void (*draw_func[NUM_GAME_MODE])() = {draw_game, draw_game, draw_game, draw_game, draw_menu, draw, draw_setting, draw_quit_box, draw_help, disp_high_score, draw_game_over, draw};
void (*key_func[NUM_GAME_MODE])(unsigned char) = {kStart, kRunning, kStopped, kPause, menu_key, key, key_setting, quit_msgbox_nvg, help_key, high_score_nvg, game_over_nvg, key,};
void (*sp_key_func[NUM_GAME_MODE])(unsigned char) = {skStart, skRunning, skStopped, skPause, menu_navig_sk, key, sp_key_setting, quit_msgbox_nvg_sk, help_sp_key, high_score_nvg_sk, game_over_nvg_sk, key};
void (*mouse_func[NUM_GAME_MODE])(int button, int state, int mx, int my) = {mStart, mRunning, mStopped, mPause, menu_mouse, mouse, mouse_setting, quitmsg_mouse, help_mouse, high_score_mouse, game_over_mouse, mouse};
void (*mouse_move_func[NUM_GAME_MODE])(int, int) = {mmStart, mmRunning, mmStopped, mouse_m, menu_mouse_move, mouse_m, mm_setting, quitmsg_mouse_move, help_mouse_move, high_score_mouse_move, game_over_mouse_move, mouse_m};

void iDraw(void){
    iClear();

    draw_func[game_state]();
}

void iMouse(int button, int state, int mx, int my){
    mouse_func[game_state](button, state, mx, my);
}

void iMouseMove(int mx, int my){
    mouse_move_func[game_state](mx, my);
}

void iKeyboard(unsigned char key){
    key_func[game_state](key);
}

void iSpecialKeyboard(unsigned char key){
    sp_key_func[game_state](key);
}

int main(){
    iSetTimer(35, move_ball);
    iSetTimer(35, move_power_up);
    iSetTimer(2000, end_loading);

    iPauseTimer(0);
    srand(time(0));

    myPlaySound(1, 0);
    iInitialize(SCREEN.UR.x, SCREEN.UR.y, "DX_BALL");
    return 0;
}


/*********************** Start **************************/
void kStart(unsigned char key){
    if(key == set1.control[C_PAUSE])
        change_game_state(RUNNING);

    else if(key == set1.control[C_QUIT]){
        change_game_state(MENU);
    }

    else if(key == set1.control[MOVE_LEFT]){
        move_bar(GLUT_KEY_LEFT, game_state);
    }

    else if(key == set1.control[MOVE_RIGHT]){
        move_bar(GLUT_KEY_RIGHT, game_state);
    }
}

void skStart(unsigned char key){
    switch(key){
    case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
        move_bar(key, game_state);
        break;
    }
}

void mmStart(int mx, int my){
    move_bar2(mx, my, game_state);
}


void mStart(int button, int state, int mx, int my){
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
        change_game_state(RUNNING);
    }
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP){
        change_game_state(MENU);
    }

}

/************************** Running *************************/
void kRunning(unsigned char key){
    if(key == set1.control[C_PAUSE])
        change_game_state(PAUSE);

    else if(key == set1.control[C_QUIT]){
        change_game_state(MSGBOX);
    }

    else if(key == set1.control[MOVE_LEFT]){
        move_bar(GLUT_KEY_LEFT, game_state);
    }

    else if(key == set1.control[MOVE_RIGHT]){
        move_bar(GLUT_KEY_RIGHT, game_state);
    }
}

void skRunning(unsigned char key){
    switch(key){
    case GLUT_KEY_RIGHT: case GLUT_KEY_LEFT:
        move_bar(key, game_state);
        break;
    case GLUT_KEY_END:
        iPauseTimer(0);
        break;
    case GLUT_KEY_PAGE_UP:
        iResumeTimer(0);
        break;
    }
}

void mRunning(int button, int state, int mx, int my){
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
        change_game_state(PAUSE);
    }

    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        change_game_state(MSGBOX);
    }

}

void mmRunning(int mx, int my){
    move_bar2(mx, my, game_state);
}

/*********************************************** Pause **************************************/
void kPause(unsigned char key){
    if(key == set1.control[C_PAUSE])
        change_game_state(RUNNING);

    else if(key == set1.control[C_QUIT]){
        change_game_state(MSGBOX);
    }
}

void skPause(unsigned char key){
    switch(key){
    case GLUT_KEY_END:
        change_game_state(MSGBOX);
        break;

    case GLUT_KEY_PAGE_DOWN:
        change_game_state(RUNNING);
        break;
    }
}

void mPause(int button, int state, int mx, int my){
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
        change_game_state(RUNNING);
    }

    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN){
        change_game_state(MSGBOX);
    }

}
/**************************************************** Stopped ****************************************/
void kStopped(unsigned char key){
    if(key == set1.control[C_PAUSE])
        change_game_state(RUNNING);

    else if(key == set1.control[C_QUIT]){
        change_game_state(MSGBOX);
    }

    else if(key == set1.control[MOVE_LEFT]){
        move_bar(GLUT_KEY_LEFT, game_state);
    }

    else if(key == set1.control[MOVE_RIGHT]){
        move_bar(GLUT_KEY_RIGHT, game_state);
    }
}

void skStopped(unsigned char key){
    switch(key){
    case GLUT_KEY_LEFT: case GLUT_KEY_RIGHT:
        move_bar(key, game_state);
        break;
    }
}

void mmStopped(int mx, int my){
    move_bar2(mx, my, game_state);
}


void mStopped(int button, int state, int mx, int my){
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
        change_game_state(RUNNING);
    }
    else if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP){
        change_game_state(MENU);
    }

}

/********************* Game State Change *************************/
void change_game_state(Game_mode_t s){
    game_state = s;
    fflush(stdin);

    switch(s){
    case START:
        myPlaySound(0, 0);
        iPauseTimer(0);
        init_value();
        break;

    case RUNNING:
        //iResumeTimer(0);
        resume_game();
        break;

    case STOPPED:
        pause_game();
        break;

    case PAUSE:
        iPauseTimer(1);
        pause_game();
        break;

    case MENU:
        myPlaySound(1, 0);
        pause_game();
        break;

    case QUIT:
        iResumeTimer(2);
        break;

    case SETTING:
        break;

    case MSGBOX:
        pause_game();
        //iPauseTimer(0);
        break;

    case HELP:
        myPlaySound(1, 0);
        break;

    case HIGH_SCORE:
        myPlaySound(1, 0);
        break;

    case GAME_OVER:
        set_game_over();
        myPlaySound(12, 1);

        change_init_mode(RESET);

        break;
    }
}

int what_image(char *str){ //START, RUNNING, STOPPED, PAUSE, MENU, QUIT, SETTING, MSGBOX, HELP, HIGH_SCORE, GAME_OVER, NUM_GAME_MODE
    char * bg_img[NUM_GAME_MODE] = {"bg_img_game.bmp", "bg_img_game.bmp", "bg_img_game.bmp", "bg_img_game.bmp", "labiba.bmp",
                        NULL, "help_bg.bmp", "bg_img_game.bmp", "help_bg.bmp", "high_score-3-img.bmp", "help_bg.bmp",
    };
    int show_bg_img[NUM_GAME_MODE] = {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, };

    strcpy(str, bg_img[game_state]);
    return show_bg_img[game_state];
}

void myPlaySound(int index, int mode){
    static int last_played = 0;
    int max_sound = 14;
    char *sound_name[] = {NULL, "Sound\\Background Music(1).wav", "Sound\\Bang.wav", "Sound\\unbreakable.wav",
                        "Sound\\normal_brick.wav", "Sound\\Gather_power_up.wav", "Sound\\Orchestr.wav", "Sound\\Swordswi.wav",
                        "Sound\\Xploshor.wav", "Sound\\Xplosht1.wav", "Sound\\hard.wav", "Sound\\Level_up.wav",
                        "Sound\\GameOver.wav", "Sound\\Keyboard press.wav",
    };

    if(index == 0){
        PlaySound(0, NULL, 0);
        last_played = index;
    }
    else if(last_played == index && mode == 0){
        return ;
    }

    else if(do_play_sound && index < max_sound){
        if(mode == 0)
            PlaySound(sound_name[index], NULL, SND_FILENAME| SND_LOOP | SND_ASYNC);
        else{
            PlaySound(sound_name[index], NULL, SND_ASYNC);
        }
        last_played = index;
    }

}

int is_sound_on(void){
    return do_play_sound;
}

int change_sound_setting(void){
    if(do_play_sound){
        do_play_sound = 0;
        myPlaySound(0, 0);
    }
    else{
        do_play_sound = 1;
        myPlaySound(1 , 0);
    }

    return do_play_sound;
}
