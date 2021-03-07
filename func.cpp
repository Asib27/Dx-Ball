#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "my_header.h"
#include "my_graphics.h"

#define SPEED_INCREASE 2.0

Game_var game;
Score game_score;
Init_mode_t mode;
int time_change = 0;

char ** read_string(FILE * from);
int count_no_of_block(char **presence);
void explode(Blocks *block,const int i,const int j);
void remove_block(Point a, Blocks *block);

/**************************************** Draw Game ****************************************/

void draw_ball(const Ball *ball){
    if(is_fire_ball_active()){
        iSetColor(255, 0, 0);
        iFilledCircle(ball->centre.x, ball->centre.y, ball->radius);
    }

    else{
        iSetColor(ball->colour.r, ball->colour.g, ball->colour.b);
        iFilledCircle(ball->centre.x, ball->centre.y, ball->radius);
    }
}

void draw_bar(const Bar *bar){
    iSetColor(bar->colour.r, bar->colour.g, bar->colour.b);
    iFilledRectangle(bar->pos.x, bar->pos.y, bar->length, bar->width);
}

void draw_block(const Blocks *block){
    Colour colours[] = {
        {110, 59, 57}, {139, 109, 85}, {20, 20, 20},
    };

    Colour block_colour;
    int img_idx;
    char *img_name[] = {"Bricks\\brick 1.bmp", "Bricks\\brick 2.bmp", "Bricks\\brick 3.bmp", "Bricks\\brick 4.bmp", "Bricks\\brick 5.bmp",
                        "Bricks\\brick 6.bmp", "Bricks\\brick 7.bmp", "Bricks\\brick 8.bmp", "Bricks\\brick 9.bmp", "Bricks\\brick 10.bmp",
    };
    int Total_img = 10;

    for(int i = 0; i < BLOCK_DIM_Y; i++){
        for(int j = 0; j < BLOCK_DIM_X; j++){
            if(block->special[i][j] == '1' && block->presence[i][j] - '0'){
                block_colour = colours[block->presence[i][j] - '0'-1];

                iSetColor(block_colour.r, block_colour.g, block_colour.b);
                iFilledRectangle(BLOCK_SCR.LL.x + j * block->length, BLOCK_SCR.UR.y-(i+1) * block->width, block->length, block->width);

                iSetColor(255, 255, 255);
                iRectangle(BLOCK_SCR.LL.x + j * block->length, BLOCK_SCR.UR.y-(i+1) * block->width, block->length, block->width);
                //iShowBMP(BLOCK_SCR.LL.x + j * game.block.length, BLOCK_SCR.UR.y-(i+1) * game.block.width, "almostlvnin.bmp");
            }

            else if(block->special[i][j] != '0' && block->presence[i][j] - '0'){
                switch(block->special[i][j]){
                    case 'U':
                        block_colour = {177, 176, 179};
                        img_idx = 5;
                        break;

                    case 'E':
                        block_colour = {242, 156, 81};
                        img_idx = 9;
                        break;

                    case '2':
                        block_colour = {189, 98, 19};//{208, 178, 126};
                        img_idx = 1;
                }

                iSetColor(255, 255, 255);
                iRectangle(BLOCK_SCR.LL.x + j * block->length, BLOCK_SCR.UR.y-(i+1) * block->width, block->length, block->width);

                iSetColor(block_colour.r, block_colour.g, block_colour.b);
                iFilledRectangle(BLOCK_SCR.LL.x + j * block->length, BLOCK_SCR.UR.y-(i+1) * block->width, block->length, block->width);
                //iShowBMP(BLOCK_SCR.LL.x + j * game.block.length, BLOCK_SCR.UR.y-(i+1) * game.block.width, img_name[img_idx]);
            }
        }
    }
}


void draw_score_board(const Score *score){
    iSetColor(0, 0, 255);
    iRectangle(SCORE_DISP.LL.x + 2, SCORE_DISP.LL.y + 2, SCORE_DISP.UR.x - SCORE_DISP.LL.x-4, SCORE_DISP.UR.y - SCORE_DISP.LL.y-4);

    char temp[80];
    sprintf(temp, "Score: %8d", score->point);
    iSetColor(0, 0, 255);
    iText(30, 630, temp, GLUT_BITMAP_9_BY_15);

    iSetColor(0, 0, 255);
    iText(30, 610, "Lives: ", GLUT_BITMAP_9_BY_15);

    for(int i = 0; i < score->lives; i++){
        iShowBMP(100 + i * 55, 608, "paddle.bmp");
    }

    iSetColor(0, 255, 255);
    sprintf(temp, "Level  %2d", score->level);
    iText(880, 630, temp, GLUT_BITMAP_9_BY_15);

    clock_to_string(temp, score->time.now - score->time.start + score->time.paused);
    iText(860, 610, temp, GLUT_BITMAP_9_BY_15);

    draw_power_up();
}


const int total_mini_score = 10;
int idx_mini_score = 0;
typedef struct{
    Point pos ;
    char str[12] ;
    clock_t start;
    Colour colour;
} Mini_score;
Mini_score mini_score[total_mini_score];

void set_draw_pos(double x, double y, int score){
    mini_score[idx_mini_score].pos.x = x;
    mini_score[idx_mini_score].pos.y = y;
    mini_score[idx_mini_score].start = clock();

    sprintf(mini_score[idx_mini_score].str, "%9d", score);

    Colour temp = {rand()%255, rand()%255, rand()%255};
    if(temp.r < 100 && temp.g < 100 && temp.b < 100){
        temp = {255-temp.r, 255-temp.g, 255-temp.b};
    }

    mini_score[idx_mini_score].colour = temp;

    idx_mini_score = (idx_mini_score+1) % total_mini_score;
}

void draw_game(void){
    void draw_ball(const Ball *ball), draw_bar(const Bar *), draw_score_board(const Score *), draw_block(const Blocks *);

    char bg_img_name[80];

    if(what_image(bg_img_name)){
        iShowBMP(0, 0, bg_img_name);
    }

    clock_t now = clock();
    if(time_change) game.score.time.now = now;

    draw_bar(&game.bar);
    draw_block(&game.block);
    draw_ball(&game.ball);
    draw_score_board(&game.score);

    for(int i = 0; i < total_mini_score; i++){
        if( (now - mini_score[i].start)/CLOCKS_PER_SEC < 2){
            mini_score[i].pos.x--;
            mini_score[i].pos.y--;
            iSetColor(mini_score[idx_mini_score].colour.r, mini_score[idx_mini_score].colour.g, mini_score[idx_mini_score].colour.b);
            iText(mini_score[i].pos.x, mini_score[i].pos.y, mini_score[i].str, GLUT_BITMAP_9_BY_15);
        }
    }
}

/********************************* Time *********************************/
void start_game(Clock *t){
    t->paused = 0;
    time_change = 0;
    game.score.time.now = game.score.time.start = 0;
}

void resume_game(void){
    iResumeTimer(0);
    iResumeTimer(1);
    game.score.time.now = game.score.time.start = clock();
    time_change = 1;
}

void pause_game(void){
    iPauseTimer(0);
    game.score.time.paused += game.score.time.now - game.score.time.start;
    game.score.time.now = game.score.time.start = 0;
    time_change = 0;
}

void clock_to_string(char * des, clock_t t){
    Time played = clock_to_time(t);

    sprintf(des, "%3d: %02d: %02d", played.hour, played.minute, played.second);
}

void second_to_string(char * des, int total_time){
    Time total;

    total.second = total_time % 60;
    total.minute = (total_time/60) % 60;
    total.hour = (total_time/3600);

    sprintf(des, "%3d:%02d:%02d", total.hour, total.minute, total.second);

}

Time clock_to_time(clock_t t){
    int total_time = t / CLOCKS_PER_SEC;

    Time total;

    total.second = total_time % 60;
    total.minute = (total_time/60) % 60;
    total.hour = (total_time/3600);

    return total;
}
/***********************************************************************************************/
char *save_filename[] = {"save.txt"};
int NO_OF_LEVEL = 3;
char *level_filename[] = {"level1.txt", "level2.txt", "level3.txt"};

void save_game(void){
    FILE *fout = fopen(save_filename[0], "w");

    fprintf(fout, "%f %f %f %f %f\n", game.ball.centre.x, game.ball.centre.y, game.ball.speed.x, game.ball.speed.y, game.ball.radius);
    fprintf(fout, "%f %f %f %f %f %f\n", game.bar.pos.x, game.bar.pos.y, game.bar.speed.x, game.bar.speed.y, game.bar.length, game.bar.width);
    fprintf(fout, "%d %d %d %d\n", game.score.point, game.score.level, game.score.lives, game.score.time.paused/ CLOCKS_PER_SEC);

    fprintf(fout, "1\n");
    for(int i = 0; i < BLOCK_DIM_Y; i++){
        for(int j = 0; j < BLOCK_DIM_X; j++){
            fputc(game.block.presence[i][j], fout);
        }
        fputc('\n', fout);
    }

    fprintf(fout, "2\n");
    for(int i = 0; i < BLOCK_DIM_Y; i++){
        for(int j = 0; j < BLOCK_DIM_X; j++){
            fputc(game.block.special[i][j], fout);
        }
        fputc('\n', fout);
    }
    fprintf(fout, "%f %f\n", game.block.length, game.block.width);

    fclose(fout);
}


void read_from_file(FILE * to_read){
    int t;

    fscanf(to_read, "%LLf %LLf %LLf %LLf %LLf\n", &game.ball.centre.x, &game.ball.centre.y, &game.ball.speed.x, &game.ball.speed.y, &game.ball.radius);
    fscanf(to_read, "%LLf %LLf %LLf %LLf %LLf %LLf\n", &game.bar.pos.x, &game.bar.pos.y, &game.bar.speed.x, &game.bar.speed.y, &game.bar.length, &game.bar.width);
    fscanf(to_read, "%d %d %d %d\n", &game.score.point, &game.score.level, &game.score.lives, &t);
    game.score.time.paused = t * CLOCKS_PER_SEC;

    game.block.presence = read_string(to_read);
    game.block.special  = read_string(to_read);
    fscanf(to_read, "%LLf %LLf\n", &game.block.length, &game.block.width);
}

char ** read_string(FILE * from){
    char ** temp = (char **)malloc(sizeof(char *) * BLOCK_DIM_Y);

    if(from == NULL){
        perror("Read String: ");
        exit(1);
    }

    for(int i = 0; i < BLOCK_DIM_Y; i++){
        temp[i] = (char *)malloc(sizeof(char) * (BLOCK_DIM_X+2));
    }

    fscanf(from, "%*d%*c");
    for(int i = 0; i < BLOCK_DIM_Y; i++){
        fgets(temp[i], BLOCK_DIM_X+5, from);
    }

    return temp;
}

int count_no_of_block(char **presence){
    int cnt = 0;

    for(int i = 0; i < BLOCK_DIM_Y; i++){
        for(int j = 0; j < BLOCK_DIM_Y; j++){
            if(presence[i][j] - '0'){
                cnt++;
            }
        }
    }

    return cnt;
}

int count_char(char **str, char ch){
    int cnt = 0;

    for(int i = 0; i < BLOCK_DIM_Y; i++){
        for(int j = 0; j < BLOCK_DIM_Y; j++){
            if(str[i][j] == ch){
                cnt++;
            }
        }
    }

    return cnt;
}


void change_init_mode(Init_mode_t m){
    mode = m;

    if(m == RESET){
        for(int i = 0; i < BLOCK_DIM_Y; i++){
            free(game.block.presence[i]);
            free(game.block.special[i]);
        }

        free(game.block.presence);
        free(game.block.special);

        pause_game();
        game_score = game.score;
    }
}

void change_level(int level){
    cancel_all_power_up();

    if(level <= NO_OF_LEVEL){
        myPlaySound(11, 1);

        change_init_mode(LEVEL_UP);
        init_value();
        change_game_state(STOPPED);
    }
    else{

        myPlaySound(12, 1);
        change_game_state(GAME_OVER);
    }
}

int init_value(void){
    double ball_speed_increase = 1.5;
    Point SCREEN_MID = {(PLAY_REG.LL.x+PLAY_REG.UR.x)/2, (PLAY_REG.LL.y+PLAY_REG.UR.y)/2};

    if(mode == NEW_GAME){

        game.bar.length = 70.0;
        game.bar.width  = 8.;
        game.bar.pos = {SCREEN_MID.x - game.bar.length/2, PLAY_REG.LL.y};
        game.bar.speed = {30., 0.};
        game.bar.colour = {250, 250, 120};

        game.ball.radius = 7.;
        game.ball.centre = {SCREEN_MID.x, game.bar.pos.y + game.bar.width + game.ball.radius};
        game.ball.colour = {255., 100., 100.};
        game.ball.speed  = {8,10};

        game.score.point = 0;
        game.score.level = 1;
        game.score.lives = 3;
        start_game(&game.score.time);

        FILE *fin = fopen(level_filename[0], "r");
        game.block.presence = read_string(fin);
        game.block.special = read_string(fin);
        fclose(fin);

        game.block.length = (BLOCK_SCR.UR.x - BLOCK_SCR.LL.x)/ BLOCK_DIM_X;
        game.block.width  = (BLOCK_SCR.UR.y - BLOCK_SCR.LL.y)/ BLOCK_DIM_Y;
        game.block.no_of_block = count_no_of_block(game.block.presence) - count_char(game.block.special, 'U');
    }

    if(mode == CONTINUE){
        FILE *fin = fopen(save_filename[0], "r");
        read_from_file(fin);
        fclose(fin);
        game.block.no_of_block = count_no_of_block(game.block.presence) - count_char(game.block.special, 'U');

        game.bar.colour = {250, 250, 120};
        game.ball.colour = {255., 100., 100.};

        change_game_state(PAUSE);
    }

    if(mode == LEVEL_UP){
        game.bar.pos = {SCREEN_MID.x - game.bar.length/2, PLAY_REG.LL.y};

        game.ball.centre = {game.bar.pos.x + game.bar.length/ 2, game.bar.pos.y + game.bar.width + game.ball.radius};
        game.ball.speed  = {game.ball.speed.x / ball_speed_increase, game.ball.speed.y / ball_speed_increase};

        game.score.level++;

        if(game.score.level < NO_OF_LEVEL){
            change_init_mode(RESET);
            FILE *fin = fopen(level_filename[game.score.level-1], "r");
            game.block.presence = read_string(fin);
            game.block.special = read_string(fin);
            fclose(fin);

            game.block.no_of_block = count_no_of_block(game.block.presence) - count_char(game.block.special, 'U');
        }
    }

}
/********************************************* Moving Ball **************************************/
const int power_up_duration = 20;

clock_t grab_paddle_start = 0;
int grab_paddle = 0;

clock_t through_ball_start = 0;
int through_ball = 0;

clock_t fire_ball_start = 0;
int fire_ball = 0;

void start_grab_paddle(void){
    grab_paddle_start = clock();
    grab_paddle = 1;
}

int is_grab_paddle_active(void){
    if(grab_paddle){
        if( (clock() - grab_paddle_start) / CLOCKS_PER_SEC < power_up_duration){
            return 1;
        }
        else
            grab_paddle = 0;
    }

    return 0;
}

void start_through_ball(void){
    through_ball_start = clock();
    through_ball = 1;
}

int is_through_ball_active(void){
    if(through_ball){
        if( (clock() - through_ball_start) / CLOCKS_PER_SEC < power_up_duration){
            return 1;
        }
        else
            through_ball = 0;
    }

    return 0;
}

void start_fire_ball(void){
    fire_ball_start = clock();
    fire_ball = 1;
}

int is_fire_ball_active(void){
    if(fire_ball){
        if( (clock() - fire_ball_start) / CLOCKS_PER_SEC < power_up_duration){
            return 1;
        }
        else
            fire_ball = 0;
    }

    return 0;
}

void move_ball(void){
    int is_block_there(Point a, Idx_pair *);
    void remove_block(Point a, Blocks *);
    Point find_intersection(Point one, Point two, Point three, Point four);

    Point new_centre  = {game.ball.centre.x + game.ball.speed.x, game.ball.centre.y + game.ball.speed.y},
                down  = {new_centre.x, new_centre.y-game.ball.radius},
                up    = {new_centre.x, new_centre.y+game.ball.radius},
                left  = {new_centre.x - game.ball.radius, new_centre.y},
                right = {new_centre.x + game.ball.radius, new_centre.y};

    if(right.x >= PLAY_REG.UR.x || left.x <= PLAY_REG.LL.x){
        myPlaySound(7, 1);
        game.ball.speed.x *= -1;
        game.ball.centre.y = new_centre.y;
    }
    if(up.y >= PLAY_REG.UR.y){
        myPlaySound(7, 1);
        game.ball.speed.y *= -1;
        game.ball.centre.x = new_centre.x;
    }

    if( (down.x >= game.bar.pos.x-5 && down.x <= game.bar.pos.x + game.bar.length+ 5) && down.y <= game.bar.pos.y + game.bar.width){
        if( is_grab_paddle_active()){
            game.ball.centre = {game.bar.pos.x + game.bar.length/2, game.bar.pos.y + game.bar.width + game.ball.radius};
            game.ball.speed.y *= -1;
            change_game_state(STOPPED);
        }
        else{
            game.ball.speed.y *= -1;
            game.ball.centre.x = new_centre.x;
        }
         myPlaySound(7, 1);

        return ;
    }

    /**** Ball Fells Down *********/
    if(down.y < PLAY_REG.LL.y){
        myPlaySound(6, 1);
        //sleep(10);

        game.ball.centre.x = game.bar.pos.x + game.bar.length/ 2;
        game.ball.centre.y = game.bar.pos.y + game.bar.width + game.ball.radius;

        game.ball.speed.y = (game.ball.speed.y < 0)? -game.ball.speed.y: game.ball.speed.y;

        cancel_all_power_up();
        change_game_state(STOPPED);

        update_lives(-1);

        return ;
    }

    Point intersect;
    Idx_pair index;
    int enable_through_ball = is_through_ball_active(), enable_fire_ball = is_fire_ball_active();

    if( is_block_there(up, &index)){
        if(!enable_through_ball){
            game.ball.speed.y *= -1;
        }
        else{
            game.ball.centre.y = new_centre.y;
        }

        if(enable_fire_ball){
            explode(&game.block, index.i, index.j);
        }

        game.ball.centre.x = new_centre.x;
        remove_block(up, &game.block);
        update_score(SCORE_PER_BLOCK);
    }

    if( is_block_there(down, &index)){
        if(!enable_through_ball){
            game.ball.speed.y *= -1;
        }
        else{
            game.ball.centre.y = new_centre.y;
        }

        if(enable_fire_ball){
            explode(&game.block, index.i, index.j);
        }

        game.ball.centre.x = new_centre.x;
        remove_block(down, &game.block);
        update_score(SCORE_PER_BLOCK);
    }

    if( is_block_there(right, &index)){
        if(!enable_through_ball){
            game.ball.speed.x *= -1;
        }
        else{
            game.ball.centre.x = new_centre.x;

        }

        if(enable_fire_ball){
            explode(&game.block, index.i, index.j);
        }

        game.ball.centre.y = new_centre.y;
        remove_block(right, &game.block);
        update_score(SCORE_PER_BLOCK);
    }

    if( is_block_there(left, &index)){
        if(!enable_through_ball){
            game.ball.speed.x *= -1;
        }
        else{
            game.ball.centre.x = new_centre.x;
        }

        if(enable_fire_ball){
            explode(&game.block, index.i, index.j);
        }

        game.ball.centre.y = new_centre.y;
        remove_block(left, &game.block);
        update_score(SCORE_PER_BLOCK);
    }

    else game.ball.centre = new_centre;

    if(!game.block.no_of_block){
        change_level(game.score.level+1);
    }

    move_power_up();
}

void increse_ball_speed(){
    double speed_increase = .01;

    if(fabs(game.ball.speed.x) <= 20 && fabs(game.ball.speed.y) <= 20){
        game.ball.speed.y += (game.ball.speed.y > 0)? speed_increase: -speed_increase;
        game.ball.speed.x += ((game.ball.speed.x > 0)? speed_increase : -speed_increase) * .8;
    }
}

void increse_ball_speed2(double to_change){
    game.ball.speed.y += (game.ball.speed.y > 0)? to_change: -to_change;
    game.ball.speed.x += ((game.ball.speed.x > 0)? to_change : -to_change) * .8;

    if(fabs(game.ball.speed.x) >= 20 && fabs(game.ball.speed.y) >= 20){
        game.ball.speed.y = (game.ball.speed.x > 0)? 20: -20;
        game.ball.speed.x = (game.ball.speed.y > 0)? 16: -16;
    }
}
/****************************************** Breaking Block **********************************************/
const Idx_pair movement[8] = { {1 , 1}, {1 , 0}, {1 , -1}, {0 , 1}, {0 , -1}, {-1 , 1}, {-1 , 0}, {-1 , -1}, };

void break_it(Blocks *block, const int i, const int j,int score){
    block->special[i][j] = '0';
    block->presence[i][j] = '0';
    block->no_of_block--;

    update_score(score);
    increse_ball_speed();

    set_draw_pos(BLOCK_SCR.LL.x + j * block->length + block->length/2, BLOCK_SCR.UR.y-(i+1) * block->width, score);
    add_power_up(BLOCK_SCR.LL.x + j * block->length + block->length/2, BLOCK_SCR.UR.y-(i+1) * block->width);
}

void explode(Blocks *block,const int i,const int j){
    Idx_pair now;
    myPlaySound(8, 1);

    for(int k = 0; k < 8; k++){
        now = {i + movement[k].i, j + movement[k].j};

        Point a = {BLOCK_SCR.LL.x + now.j * block->length , BLOCK_SCR.UR.y-(now.i) * block->width}; //BLOCK_SCR.UR.y - i * block->width
        remove_block(a, block);
    }
}

void break_explosive_brick(Blocks *block,const int i,const int j){
    break_it(block, i, j, 150);

    Idx_pair now;
    for(int k = 0; k < 8; k++){
        now = {i + movement[k].i, j + movement[k].j};

        if(block->special[now.i][now.j] == 'E'){
            break_explosive_brick(block, now.i, now.j);
        }
    }
}

void remove_block(Point a, Blocks *block){
    int j = (a.x - BLOCK_SCR.LL.x)/ block->length, // j*block->length + BLOCK_SCR.LL.x
        i = (BLOCK_SCR.UR.y-a.y)/ block->width; // BLOCK_SCR.UR.y - i * block->width

    if( ! ((j >= 0 && j < BLOCK_DIM_X) && (i >= 0 && i < BLOCK_DIM_Y)) ) {
        return ;
    }

    switch(block->special[i][j]){
    case '1':
        myPlaySound(4, 1);
        break_it(block, i, j, SCORE_PER_BLOCK);
        break;

    case '2':
        myPlaySound(10, 1);
        set_draw_pos(BLOCK_SCR.LL.x + j * block->length + block->length/2, BLOCK_SCR.UR.y-(i+1) * block->width, 100);
        block->special[i][j] = '1';
        add_power_up(BLOCK_SCR.LL.x + j * block->length + block->length/2, BLOCK_SCR.UR.y-(i+1) * block->width);
        break;

    case 'U':
        myPlaySound(3, 1);
        break;

    case 'E':
        myPlaySound(9, 1);
        break_explosive_brick(block, i, j);
        break;

    }
}

Point find_intersection(Point one, Point two, Point three, Point four){
    double a1= two.y - one.y, b1 = -(two.x - one.x), c1 = one.x* a1 + one.y * b1;
    double a2= four.y - three.y, b2 = -(four.x - three.x), c2 = three.x* a2 + three.y * b2;

    double dx = c1*b2 - c2 * b1, dy = a1 * c2 - a2 * c1, d = a1*b2 - a2 * b1;

    Point ans = {dx/d, dy/d};
    return ans;
}

int is_block_there(Point a, Idx_pair *index){
    int j = (a.x - BLOCK_SCR.LL.x)/ game.block.length,
        i = (BLOCK_SCR.UR.y-a.y)/ game.block.width;;

    if((j >= 0 && j < BLOCK_DIM_X) && (i >= 0 && i < BLOCK_DIM_Y)){
        index->i = i;
        index->j = j;
        return game.block.presence[i][j] != '0';
    }
    else
        return 0;
}


void update_score(int to_change){
    game.score.point += to_change;
}
void update_lives(int to_change){
    game.score.lives += to_change;

    if(game.score.lives <= 0){
        change_init_mode(RESET);
        change_game_state(GAME_OVER);
    }

    if(game.score.lives > 5){
        game.score.lives = 5;
    }
}
/* Bar */
void move_bar(unsigned char key, Game_mode_t s){
    if(key == GLUT_KEY_RIGHT){
        game.bar.pos.x += game.bar.speed.x;
        game.bar.pos.x = (game.bar.pos.x + game.bar.length >= PLAY_REG.UR.x)? PLAY_REG.UR.x - game.bar.length : game.bar.pos.x;

    }else if(key == GLUT_KEY_LEFT){
        game.bar.pos.x -= game.bar.speed.x;
        game.bar.pos.x = (game.bar.pos.x <= PLAY_REG.LL.x)? PLAY_REG.LL.x : game.bar.pos.x;

    }

    if(s == START || s == STOPPED){
        game.ball.centre.x = game.bar.pos.x + game.bar.length/2;
    }
}

void move_bar2(int mx, int my, Game_mode_t s){
    if(mx < game.bar.pos.x)
        move_bar(GLUT_KEY_LEFT, s);
    else if(mx > game.bar.pos.y)
        move_bar(GLUT_KEY_RIGHT, s);
}

void change_len(int to_change){
    if(game.bar.length > to_change)
        game.bar.length += to_change;

    if(game.bar.length < 20){
        game.bar.length = 20;
    }
    else if(game.bar.length > 120){
        game.bar.length = 120;
    }
}

void change_ball_radius(double to_change){
    game.ball.radius += to_change;

    if(game.ball.radius < 3){
        game.ball.radius = 3;
    }
}

/************************************************************ High Score ************************************************************************/
const int NO_HIGH_SCORE = 10;
int highlight_score = NO_HIGH_SCORE;
High_score_mode_t high_score_state = FROM_GAME;

int cmp_score(const void *ap, const void *bp){
    const High_score *a = (const High_score *) ap, *b = (const High_score *) bp;

    if(a->point != b->point)
        return a->point - b->point;
    else if(a->level != b->level)
        return a->level - b->level;
    else
        return -(a->seconds - b->seconds); //Less time more score
}

int rev_cmp_score(const void *ap, const void *bp){
    return -1 * cmp_score(ap, bp);
}

int update_highscore(char * name){
    FILE *h_score = fopen("High_score.txt", "r");

    if(h_score == NULL){
        perror("File opened failed");
        exit(1);
    }


    High_score scores[NO_HIGH_SCORE+1];
    for(int i = 0; i < NO_HIGH_SCORE; i++){
        fscanf(h_score, "%s%d%d%d%*c", scores[i].player_name, &scores[i].point, &scores[i].level, &scores[i].seconds);
    }


    strcpy(scores[NO_HIGH_SCORE].player_name, name);
    scores[NO_HIGH_SCORE].level = game_score.level;
    scores[NO_HIGH_SCORE].point = game_score.point;
    scores[NO_HIGH_SCORE].seconds = (int)game_score.time.paused / CLOCKS_PER_SEC;

    fclose(h_score);

    qsort(scores, NO_HIGH_SCORE+1, sizeof(High_score), rev_cmp_score);

    //showing 1st 10
    h_score = fopen("High_score.txt", "w");

    for(int i = 0; i < NO_HIGH_SCORE; i++){
        fprintf(h_score, "%s %d %d %d\n", scores[i].player_name, scores[i].point, scores[i].level, scores[i].seconds);
    }

    fclose(h_score);

    highlight_score = NO_HIGH_SCORE;
    for(int i = 0; i < NO_HIGH_SCORE; i++){
        if(strcmp(scores[i].player_name, name) == 0 && scores[i].level == game_score.level && scores[i].point == game_score.point
           && scores[i].seconds == (int)game_score.time.paused / CLOCKS_PER_SEC){

            return (highlight_score = i);
        }
    }

    return highlight_score;
}

void disp_high_score(void){
    char bg_img_name[80];

    if(what_image(bg_img_name)){
        iShowBMP(0, 0, "help_bg.bmp");
        iShowBMP2(265, 500, bg_img_name, 0x000000);
    }

    if(high_score_state == FROM_GAME){
        char str[LEN_NAME];
        sprintf(str, "Your Score:  %d", game_score.point);

        iSetColor(200, 200, 200);
        iText(410, 460, str, GLUT_BITMAP_9_BY_15);

        if(highlight_score != NO_HIGH_SCORE){
            iSetColor(200, 200, 200);
            iRectangle(100 - 20, 375 - highlight_score * 30-10, 820, 30);
            iRectangle(100 - 18, 373 - highlight_score * 30-10, 824, 34);
        }
    }

    /***************************************** Show High Scores ******************************************/
    FILE *h_score = fopen("High_score.txt", "r");

    if(h_score == NULL){
        perror("File opened failed");
        exit(1);
    }

    High_score scores[NO_HIGH_SCORE];
    for(int i = 0; i < NO_HIGH_SCORE; i++){
        fscanf(h_score, "%s%d%d%d%*c", scores[i].player_name, &scores[i].point, &scores[i].level, &scores[i].seconds);
    }

    char str[500], time[12] ;

    iSetColor(255, 255, 255);
    iText(100, 425, "Pos                          Name             Point              Time           Level", GLUT_BITMAP_9_BY_15);

    for(int i = 0; i < NO_HIGH_SCORE; i++){
        iSetColor(255, 255, 255);

        second_to_string(time, scores[i].seconds);
        sprintf(str, "%3d.    %25s   %15d       %11s      %10d", i+1, scores[i].player_name, scores[i].point, time, scores[i].level);
        iText(100, 375 - i * 30, str, GLUT_BITMAP_9_BY_15);
    }

    fclose(h_score);
    /********************************************** Menu Beneath ***********************************************/
    Draw_high_score_menu(high_score_state);
}

void change_high_score_mode(High_score_mode_t m){
    high_score_state = m;
}

/************************************** Draw Power ups ************************************/
const int TOTAL_POWER_UP = 20;
int idx_power_up = 0;

Power_up all_power_ups[TOTAL_POWER_UP];

Power_up_t set_power_up_t(int i){
    switch(i % NO_POWER_UP){
    case SHORT_BAR:
        return SHORT_BAR;

    case WIDE_BAR:
        return WIDE_BAR;

    case EXTRA_LIFE:
        return EXTRA_LIFE;

    case FAST_BALL:
        return FAST_BALL;

    case SLOW_BALL:
        return SLOW_BALL;

    case IMMEDIATE_DEATH:
        return IMMEDIATE_DEATH;

    case SUPER_SHRINK:
        return SUPER_SHRINK;

    case GRAB_PADDLE:
        return GRAB_PADDLE;

    case SHRINK_BALL:
        return SHRINK_BALL;

    case THROUGH_BALL:
        return THROUGH_BALL;

    case FIRE_BALL:
        return FIRE_BALL;

    default:
        return NO_POWER_UP;
    }

    return NO_POWER_UP;
}

void add_power_up(double x, double y){
    int id = rand() % 5;

    if(id == 0){
        myPlaySound(2, 1);

        all_power_ups[idx_power_up].show = 1;
        all_power_ups[idx_power_up].name = set_power_up_t(rand());
        all_power_ups[idx_power_up].pos = {x, y};
        all_power_ups[idx_power_up].dim = {34, 32};
    }

    idx_power_up = (idx_power_up+1) % TOTAL_POWER_UP;
}

void remove_power_up(Touch_t t, int idx){
    if(t == TOUCHED){
        all_power_ups[idx].show = 0;

        switch(all_power_ups[idx].name){
        case SHORT_BAR:
            change_len(-25);
            break;

        case WIDE_BAR:
            change_len(+25);
            break;

        case EXTRA_LIFE:
            update_lives(1);
            break;

        case FAST_BALL:
            increse_ball_speed2(.5);
            break;


        case SLOW_BALL:
            increse_ball_speed2(-.5);
            break;

        case IMMEDIATE_DEATH:
            update_lives(-1);
            break;

        case SUPER_SHRINK:
            change_len(-40);
            break;

        case GRAB_PADDLE:
            start_grab_paddle();
            break;

        case SHRINK_BALL:
            change_ball_radius(-3);
            break;

        case THROUGH_BALL:
            start_through_ball();
            break;

        case FIRE_BALL:
            start_fire_ball();
            break;

        }

        myPlaySound(5, 1);
    }
    else if(t == NOT_TOUCHED){
        all_power_ups[idx].show = 0;
    }
}

void draw_power_up(void){
    char *img_name[] = {"Power up\\pw1.bmp", "Power up\\pw2.bmp", "Power up\\pw3.bmp", "Power up\\pw4.bmp", "Power up\\pw5.bmp",
                        "Power up\\pw6.bmp", "Power up\\pw7.bmp", "Power up\\pw8.bmp", "Power up\\pw9.bmp", "Power up\\pw10.bmp",
                        "Power up\\pw11.bmp", "Power up\\pw12.bmp", "Power up\\pw13.bmp", "Power up\\pw14.bmp", "Power up\\pw15.bmp",
                        "Power up\\pw16.bmp", "Power up\\pw17.bmp", "Power up\\pw18.bmp",
    };

    for(int i = 0; i < TOTAL_POWER_UP; i++){
        if(all_power_ups[i].show){
            iShowBMP2(all_power_ups[i].pos.x, all_power_ups[i].pos.y, img_name[all_power_ups[i].name], 0x000000);
        }
    }
}

int do_touch_bar(Power_up *t){
    Point left = {t->pos.x, t->pos.y}, right = {t->pos.x + t->dim.x, t->pos.y};

    if(t->pos.y <= game.bar.pos.y + game.bar.width && t->pos.y + t->dim.y >= game.bar.pos.y){
        if((left.x <= game.bar.pos.x + game.bar.length && left.x >= game.bar.pos.x) ||
           (right.x <= game.bar.pos.x + game.bar.length && right.x >= game.bar.pos.x) ){
            return 1;
        }
        else if(t->pos.y + t->dim.y < PLAY_REG.LL.y){
            return 0;
        }
    }
    else if(t->pos.y > game.bar.pos.y + game.bar.width){
        return -1;
    }

    return 2;
}

void move_power_up(void){
    Point speed = {2, -4};

    for(int i = 0; i < TOTAL_POWER_UP; i++){
        if(all_power_ups[i].show){
            if(all_power_ups[i].pos.x < PLAY_REG.LL.x + 100){
                speed.x = fabs(speed.x);
            }
            else if(all_power_ups[i].pos.x > PLAY_REG.UR.x - 100){
                speed.x = -fabs(speed.x);
            }
            else if(all_power_ups[i].pos.x < (PLAY_REG.LL.x + PLAY_REG.UR.x)/2){
                if(rand() % 1000 > 5) speed.x = fabs(speed.x);
                else  speed.x = -fabs(speed.x);
            }

            else if(all_power_ups[i].pos.x > (PLAY_REG.LL.x + PLAY_REG.UR.x)/2){
                if(rand() % 1000 < 5) speed.x = fabs(speed.x);
                else  speed.x = -fabs(speed.x);
            }

            all_power_ups[i].pos.x += speed.x;
            all_power_ups[i].pos.y += speed.y;

            int do_touch = do_touch_bar(&all_power_ups[i]);
            if(do_touch == 1){
                remove_power_up(TOUCHED, i);
            }
            else if(do_touch == 0){
                remove_power_up(NOT_TOUCHED, i);
            }
        }
    }
}

void cancel_all_power_up(void){
    for(int i = 0; i < TOTAL_POWER_UP; i++){
        all_power_ups[i].show = 0;
    }
}

void draw_quit_score_board(void){
    char str[LEN_NAME];
    sprintf(str, "Your Score:  %d", game_score.point);

    iSetColor(200, 200, 200);
    iText(410, 200, str, GLUT_BITMAP_9_BY_15);
}
