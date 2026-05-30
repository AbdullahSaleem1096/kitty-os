void kmain(void)
{
    char *video_memory = (char *) 0xb8000;

    video_memory[0] = 'A';
    video_memory[1] = 0x07;

    video_memory[2] = 'B';
    video_memory[3] = 0x07;

    while (1)
    {
    }
}