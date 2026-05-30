#include "framebuffer.h"

void kmain(void)
{
    fb_write_cell(FB_CELL(0, 0), 'A', FB_GREEN, FB_DARK_GREY);
    fb_write_cell(FB_CELL(0, 1), 'B', FB_GREEN, FB_DARK_GREY);

    while (1)
    {
    }
}
