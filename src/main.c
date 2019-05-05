#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mouse.h>
#include <tgi.h>

#define PADDLE_WIDTH 30
#define PADDLE_HALF_WIDTH 15
#define PADDLE_HEIGHT 2

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

static unsigned int max_x, max_y, paddle_y_top, paddle_y_bottom, error;
static unsigned int x, new_x = -1;
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
  new_x = min(max(info.pos.x, PADDLE_HALF_WIDTH), max_x-PADDLE_HALF_WIDTH);

  if(new_x == x) {
    return;
  }

  tgi_setcolor(TGI_COLOR_BLACK);
  if(new_x > x) {
    tgi_bar(x-PADDLE_HALF_WIDTH,paddle_y_top,new_x-PADDLE_HALF_WIDTH,paddle_y_bottom);
  } else if(new_x < x) {
    tgi_bar(new_x+PADDLE_HALF_WIDTH,paddle_y_top,x+PADDLE_HALF_WIDTH,paddle_y_bottom);
  }

  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_bar(new_x-PADDLE_HALF_WIDTH,paddle_y_top,new_x+PADDLE_HALF_WIDTH,paddle_y_bottom);
  x=new_x;
}

void update(void) {
  mouse_info(&info);
  draw_paddle();
}

int main (void)
{
  init_drivers();

  tgi_clear(); // load-bearing! getxres returns wrong result (24) until we do this
  max_y = tgi_getyres();
  max_x = tgi_getxres();
  paddle_y_bottom = max_y - 10;
  paddle_y_top = paddle_y_bottom - PADDLE_HEIGHT;

  do {
    update();
  } while(true);

  return 0;
}
