#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <mouse.h>
#include <tgi.h>

#define COLOR_BACK TGI_COLOR_BLACK
#define COLOR_FORE TGI_COLOR_WHITE
#define PADDLE_WIDTH 30
#define PADDLE_HALF_WIDTH 15
#define PADDLE_HEIGHT 2

static unsigned maxY;
static unsigned int paddleYTop, paddleYBottom;
static unsigned int x = -1;

int main (void)
{
  struct mouse_info info;
  struct mouse_box full_box;
  unsigned char error;
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

  maxY = tgi_getmaxy();
  paddleYBottom = maxY - 10;
  paddleYTop = paddleYBottom - PADDLE_HEIGHT;

  mouse_getbox(&full_box);

  do {
    mouse_info(&info);

    if(info.pos.x == x) {
      continue;
    }

    tgi_setcolor(COLOR_BLACK);
    if(info.pos.x > x) {
      tgi_bar(x-PADDLE_HALF_WIDTH,paddleYTop,info.pos.x-PADDLE_HALF_WIDTH,paddleYBottom);
    } else if(info.pos.x < x) {
      tgi_bar(info.pos.x+PADDLE_HALF_WIDTH,paddleYTop,x+PADDLE_HALF_WIDTH,paddleYBottom);
    }
    tgi_setcolor(COLOR_FORE);
    tgi_bar(info.pos.x-PADDLE_HALF_WIDTH,paddleYTop,info.pos.x+PADDLE_HALF_WIDTH,paddleYBottom);
    x=info.pos.x;
  } while(true);
}
