#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mouse.h>
#include <tgi.h>

#define PADDLE_WIDTH 30
#define PADDLE_HALF_WIDTH 15
#define PADDLE_HEIGHT 2
#define BALL_RADIUS 1

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

static unsigned int max_x, max_y, paddle_y_top, paddle_y_bottom, error;
static unsigned int paddle_x, paddle_new_x = -1;
static unsigned int ball_x, ball_y, ball_new_x, ball_new_y;
static signed char ball_vx, ball_vy;
static struct mouse_info info;

void init_drivers(void)
{
  error = mouse_install(&mouse_def_callbacks, mouse_static_stddrv);
  if(error != MOUSE_ERR_OK) {
    exit(EXIT_FAILURE);
  }

  tgi_install(tgi_static_stddrv);
  if(tgi_geterror() != TGI_ERR_OK) {
    exit(EXIT_FAILURE);
  }

  tgi_init();
  if(tgi_geterror() != TGI_ERR_OK) {
    exit(EXIT_FAILURE);
  }
  tgi_clear();
}

void draw_paddle(void)
{
  paddle_new_x = min(max(info.pos.x, PADDLE_HALF_WIDTH), max_x-PADDLE_HALF_WIDTH);

  if(paddle_new_x == paddle_x) {
    return;
  }

  // clear old paddle
  tgi_setcolor(TGI_COLOR_BLACK);
  if(paddle_new_x > paddle_x) {
    tgi_bar(paddle_x-PADDLE_HALF_WIDTH,paddle_y_top,paddle_new_x-PADDLE_HALF_WIDTH,paddle_y_bottom);
  } else if(paddle_new_x < paddle_x) {
    tgi_bar(paddle_new_x+PADDLE_HALF_WIDTH,paddle_y_top,paddle_x+PADDLE_HALF_WIDTH,paddle_y_bottom);
  }

  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_bar(paddle_new_x-PADDLE_HALF_WIDTH,paddle_y_top,paddle_new_x+PADDLE_HALF_WIDTH,paddle_y_bottom);
  paddle_x=paddle_new_x;
}

void draw_ball(void)
{
  ball_new_x += ball_vx;
  ball_new_y += ball_vy;

  // clear old ball
  tgi_setcolor(TGI_COLOR_BLACK);
  if(ball_new_x > ball_x) {
    tgi_bar(ball_x-2,ball_y-2,ball_new_x+2,ball_y+2);
  } else if (ball_new_x < ball_x) {
    tgi_bar(ball_new_x+2,ball_y-2,ball_x+2,ball_y+2);
  }

  if(ball_new_y > ball_y) {
    tgi_bar(ball_x-2,ball_y-2,ball_x+2,ball_new_y-2);
  } else if (ball_new_y < ball_y) {
    tgi_bar(ball_x-2,ball_new_y+2,ball_x+2,ball_y+2);
  }

  if(ball_new_x <= 0 || ball_new_x >= max_x) {
    ball_vx = -ball_vx;
  }

  if(ball_new_y <= 0 || ball_new_y >= max_y) {
    ball_vy = -ball_vy;
  }

  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_bar(ball_new_x-2,ball_new_y-2,ball_new_x+2,ball_new_y+2);

  ball_x = ball_new_x;
  ball_y = ball_new_y;
}

void update(void) {
  mouse_info(&info);
  draw_paddle();
  draw_ball();
}

int main (void)
{
  init_drivers();

  tgi_clear(); // load-bearing! getxres returns wrong result (24) until we do this
  max_y = tgi_getyres();
  max_x = tgi_getxres();
  paddle_y_bottom = max_y - 10;
  paddle_y_top = paddle_y_bottom - PADDLE_HEIGHT;
  ball_x = max_x/2;
  ball_y = max_y/2;
  ball_new_x = max_x/2;
  ball_new_y = max_y/2;
  ball_vx = -1;
  ball_vy = -1;

  do {
    update();
  } while(true);

  return 0;
}
