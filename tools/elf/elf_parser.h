#pragma once

#include <lib/stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t offset;
    uint64_t vaddr;
    uint64_t paddr;
    uint64_t filesz;
    uint64_t memsz;
    uint32_t flags;  // PF_R | PF_W | PF_X
} ElfSegment;

typedef struct {
    uint64_t offset;
    uint64_t addr;
    uint64_t size;
} ElfSection;

typedef struct {
    ElfSegment *segments;
    int num_segments;

    ElfSection text_section;
    int text_found;
} ElfInfo;

int parse_elf_file(const char *filename, ElfInfo *info);
void free_elf_info(ElfInfo *info);

#ifdef __cplusplus
}
#endif
