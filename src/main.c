#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mouse.h>
#include <tgi.h>
#include <time.h>

#define PADDLE_WIDTH 30
#define PADDLE_HALF_WIDTH 15
#define PADDLE_HEIGHT 2
#define BALL_RADIUS 1
#define DEFAULT_VX -2
#define DEFAULT_VY -2


#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

static unsigned int max_x, max_y, paddle_y_top, paddle_y_bottom, error;
static unsigned int paddle_x, paddle_new_x = -1;
static unsigned int ball_x, ball_y, ball_new_x, ball_new_y;
static signed char ball_vx = DEFAULT_VX;
static signed char ball_vy = DEFAULT_VY;
static struct mouse_info info;
static unsigned int draw_page = 0;

void __fastcall__ init_drivers(void)
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

  tgi_setdrawpage(0);
  tgi_clear();
  tgi_setdrawpage(1);
  tgi_clear();
}

int main (void)
{
  init_drivers();

  tgi_setdrawpage(draw_page);
  tgi_clear(); // load-bearing! getxres returns wrong result (24) until we do this
  max_y = tgi_getyres();
  max_x = tgi_getxres();
  paddle_y_bottom = max_y - 10;
  paddle_y_top = paddle_y_bottom - PADDLE_HEIGHT;
  ball_x = max_x/2;
  ball_y = max_y/2;
  ball_new_x = max_x/2;
  ball_new_y = max_y/2;
  ball_vx = -2;
  ball_vy = -2;

  do {
    mouse_info(&info);

    // CALC POSITIONS
    paddle_new_x = min(max(info.pos.x, PADDLE_HALF_WIDTH), max_x-PADDLE_HALF_WIDTH);

    ball_new_x += ball_vx;
    ball_new_y += ball_vy;

    if(ball_new_x <= 0 || ball_new_x >= max_x) {
      ball_vx = -ball_vx;
    }

    if(ball_new_y <= 0) {
      ball_vy = -ball_vy;
    }
    
    if(ball_new_y >= max_y) {
      ball_new_x = max_x/2;
      ball_new_y = max_y/2;
      ball_vx = DEFAULT_VX;
      ball_vy = DEFAULT_VY;
    }

    if(ball_new_y >= paddle_y_top && 
        ball_new_y <= paddle_y_bottom && 
        ball_new_x+2 >= paddle_x-PADDLE_HALF_WIDTH &&
        ball_new_x+2 <= paddle_x+PADDLE_HALF_WIDTH)
    {
      ball_vy = -ball_vy;
    }

    // DRAW 
    tgi_setcolor(TGI_COLOR_WHITE);
    tgi_bar(paddle_new_x-PADDLE_HALF_WIDTH,paddle_y_top,paddle_new_x+PADDLE_HALF_WIDTH,paddle_y_bottom);
    tgi_bar(ball_new_x-2,ball_new_y-2,ball_new_x+2,ball_new_y+2);

    // CYCLE GRAPHICS PAGES
    tgi_setviewpage(draw_page);

    draw_page = !draw_page;
    tgi_setdrawpage(draw_page);

    // CLEAR
    tgi_setcolor(TGI_COLOR_BLACK);
    tgi_bar(paddle_x-PADDLE_HALF_WIDTH,paddle_y_top,paddle_x+PADDLE_HALF_WIDTH,paddle_y_bottom);
    tgi_bar(ball_x-2,ball_y-2,ball_x+2,ball_y+2);

    // COMMIT POSITIONS
    paddle_x=paddle_new_x;
    ball_x = ball_new_x;
    ball_y = ball_new_y;

  } while(true);

  return 0;
}
