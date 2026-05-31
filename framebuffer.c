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
static unsigned char default_fg = FB_BLACK;
static unsigned char default_bg = FB_GREEN;


/** fb_move_cursor:
 *  Moves the hardware cursor of the framebuffer to the given position.
 *
 *  The cursor position is represented as a 16-bit value where:
 *      position = row * 80 + column
 *
 *  Since the VGA controller accepts only 8 bits at a time, the position
 *  is split into a high byte and a low byte and sent through the VGA I/O
 *  ports (0x3D4 and 0x3D5).
 *
 *  @param pos The new cursor position.
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, (unsigned char) ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, (unsigned char) (pos & 0x00FF));
}

/** fb_write_cell:
 *  Writes a character with the specified foreground and background colors
 *  to a framebuffer cell.
 *
 *  Each framebuffer cell occupies two bytes:
 *      Byte 0 -> ASCII character
 *      Byte 1 -> Color information
 *
 *  The color byte stores the foreground color in the upper 4 bits and
 *  the background color in the lower 4 bits.
 *
 *  @param i  Byte index within the framebuffer.
 *  @param c  Character to display.
 *  @param fg Foreground color.
 *  @param bg Background color.
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = (unsigned char) (((fg & 0x0F) << 4) | (bg & 0x0F));
}

/** fb_scroll:
 *  Scrolls the framebuffer up by one row.
 *
 *  All rows are shifted upward:
 *      Row 1 -> Row 0
 *      Row 2 -> Row 1
 *      ...
 *
 *  After shifting, the last row is cleared and filled with spaces
 *  using the default foreground and background colors.
 *
 *  This function is called when the cursor moves beyond the bottom
 *  of the screen.
 */
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

/** fb_put_char:
 *  Writes a single character at the current cursor position and updates
 *  the cursor accordingly.
 *
 *  Special characters are handled:
 *      '\n' -> Move cursor to beginning of next line.
 *      '\r' -> Move cursor to beginning of current line.
 *
 *  Normal characters are written to the framebuffer and the cursor
 *  advances by one position.
 *
 *  If the cursor reaches the end of the screen, the framebuffer is
 *  scrolled up automatically.
 *
 *  @param c The character to write.
 */
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

/** write:
 *  Writes a buffer of characters to the framebuffer.
 *
 *  Each character is processed individually through fb_put_char(),
 *  allowing newline handling, cursor movement, and automatic scrolling.
 *
 *  @param buf Pointer to the character buffer.
 *  @param len Number of characters to write.
 *
 *  @return The number of characters written.
 */

int write(char *buf, unsigned int len)
{
    unsigned int i;

    for (i = 0; i < len; i++)
    {
        fb_put_char(buf[i]);
    }

    return (int) len;
}
