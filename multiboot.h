#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_INFO_MODS 0x00000008

typedef unsigned int multiboot_uint32_t;

struct multiboot_info
{
    multiboot_uint32_t flags;
    multiboot_uint32_t mem_lower;
    multiboot_uint32_t mem_upper;
    multiboot_uint32_t boot_device;
    multiboot_uint32_t cmdline;
    multiboot_uint32_t mods_count;
    multiboot_uint32_t mods_addr;
} __attribute__((packed));

typedef struct multiboot_info multiboot_info_t;

struct multiboot_mod_list
{
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;
    multiboot_uint32_t cmdline;
    multiboot_uint32_t pad;
} __attribute__((packed));

typedef struct multiboot_mod_list multiboot_module_t;

#endif
