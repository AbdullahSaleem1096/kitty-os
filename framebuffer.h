#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define FB_BLACK      0
#define FB_BLUE       1
#define FB_GREEN      2
#define FB_CYAN       3
#define FB_RED        4
#define FB_MAGENTA    5
#define FB_BROWN      6
#define FB_LIGHT_GREY 7
#define FB_DARK_GREY  8
#define FB_LIGHT_BLUE 9
#define FB_LIGHT_GREEN 10
#define FB_LIGHT_CYAN 11
#define FB_LIGHT_RED  12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN   14
#define FB_WHITE      15

#define FB_COLS 80
#define FB_ROWS 25

/** Byte offset for cell at (row, col) in the 80-column text framebuffer. */
#define FB_CELL(row, col) (((row) * FB_COLS + (col)) * 2)

void fb_move_cursor(unsigned short pos);

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** write:
 *  Writes the contents of buf to the screen, advancing the cursor and
 *  scrolling when the bottom row is full.
 *
 *  @param buf Buffer to write
 *  @param len Number of bytes to write
 *  @return Number of bytes written
 */
int write(char *buf, unsigned int len);

#endif
