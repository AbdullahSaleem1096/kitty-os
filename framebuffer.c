#include "framebuffer.h"
#include "io.h"

static char *const fb = (char *) 0x000B8000;

#define FB_COLS 80
#define FB_ROWS 25
#define FB_SIZE (FB_COLS * FB_ROWS)

#define FB_COMMAND_PORT      0x3D4
#define FB_DATA_PORT         0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

static unsigned short cursor_pos;
static unsigned char default_fg = FB_LIGHT_GREY;
static unsigned char default_bg = FB_BLACK;

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, (unsigned char) ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, (unsigned char) (pos & 0x00FF));
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = (unsigned char) (((fg & 0x0F) << 4) | (bg & 0x0F));
}

static void fb_scroll(void)
{
    unsigned int row;
    unsigned int col;
    unsigned int dst;
    unsigned int src;

    for (row = 1; row < FB_ROWS; row++)
    {
        for (col = 0; col < FB_COLS; col++)
        {
            dst = FB_CELL(row - 1, col);
            src = FB_CELL(row, col);
            fb[dst] = fb[src];
            fb[dst + 1] = fb[src + 1];
        }
    }

    for (col = 0; col < FB_COLS; col++)
    {
        fb_write_cell(FB_CELL(FB_ROWS - 1, col), ' ', default_fg, default_bg);
    }
}

static void fb_put_char(char c)
{
    if (c == '\n')
    {
        cursor_pos = (unsigned short) (((cursor_pos / FB_COLS) + 1) * FB_COLS);
    }
    else if (c == '\r')
    {
        cursor_pos = (unsigned short) ((cursor_pos / FB_COLS) * FB_COLS);
    }
    else
    {
        fb_write_cell((unsigned int) cursor_pos * 2U, c, default_fg, default_bg);
        cursor_pos++;
    }

    if (cursor_pos >= FB_SIZE)
    {
        fb_scroll();
        cursor_pos = (unsigned short) (FB_SIZE - FB_COLS);
    }

    fb_move_cursor(cursor_pos);
}

int write(char *buf, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        fb_put_char(buf[i]);
    }

    return (int) len;
}
