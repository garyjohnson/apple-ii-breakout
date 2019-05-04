#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <mouse.h>
#include <pen.h>
#include <conio.h>
#include <ctype.h>
#include <dbg.h>
#include <cc65.h>

#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) < (b)) ? (a) : (b))

static void __fastcall__ CheckError (const char* S, unsigned char Error)
{
    if (Error != MOUSE_ERR_OK) {
        cprintf ("\n%s: %s(%u)\r\n", S, mouse_geterrormsg (Error), Error);

        /* Wait for a key-press, so that some platforms can show the error
        ** message before they remove the current screen.
        */
        if (doesclrscrafterexit ()) {
            cgetc ();
        }
        exit (EXIT_FAILURE);
    }
}

int main (void)
{
    struct mouse_info info;
    struct mouse_box full_box;
    unsigned char width, height;
    bool Invisible = true, Done = false, Jailed = false;

    DbgInit (0);

    (void) bordercolor (COLOR_BLUE);
    (void) bgcolor (COLOR_WHITE);
    (void) textcolor (COLOR_BLACK);
    cursor (0);
    clrscr ();

    CheckError ("mouse_install", mouse_install (&mouse_def_callbacks, mouse_static_stddrv));

    /* Get the initial bounding box. */
    mouse_getbox (&full_box);

    screensize (&width, &height);

    clrscr ();

    do {
        mouse_info (&info);
        gotoxy (0, 2);
        cprintf (" X  = %3d\r\n", info.pos.x);
        cprintf (" Y  = %3d\r\n", info.pos.y);
        cprintf (" B1 = %c\r\n", (info.buttons & MOUSE_BTN_LEFT) ?  'v' : '^');
        cprintf (" B2 = %c", (info.buttons & MOUSE_BTN_RIGHT) ?  'v' : '^');
    } while (!Done);

    CheckError ("mouse_uninstall", mouse_uninstall ());
    cputsxy (0, height / 2 + 3, "Goodbye!");
    return EXIT_SUCCESS;
}
