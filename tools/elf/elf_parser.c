#include <elf_parser.h>
#include <common/elf.h>

#include <lib/memops.h>

int parse_elf_file_from_memory(const void *data, size_t size, ElfInfo *info)
{
    memset(info, 0, sizeof(ElfInfo));

    const unsigned char *mem = (const unsigned char *)data;

    if (size < sizeof(Elf64_Ehdr)) return -1;
    const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)mem;

    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0 ||
        ehdr->e_ident[EI_CLASS] != ELFCLASS64)
        return -1;

    // --- Program Headers ---
    if (ehdr->e_phoff + ehdr->e_phnum * sizeof(Elf64_Phdr) > size)
        return -1;

    info->segments = malloc(sizeof(ElfSegment) * ehdr->e_phnum);
    if (!info->segments) return -1;

    int seg_count = 0;
    const Elf64_Phdr *phdrs = (const Elf64_Phdr *)(mem + ehdr->e_phoff);
    for (int i = 0; i < ehdr->e_phnum; i++) {
        const Elf64_Phdr *ph = &phdrs[i];
        if (ph->p_type == PT_LOAD) {
            info->segments[seg_count].offset = ph->p_offset;
            info->segments[seg_count].vaddr  = ph->p_vaddr;
            info->segments[seg_count].paddr  = ph->p_paddr;
            info->segments[seg_count].filesz = ph->p_filesz;
            info->segments[seg_count].memsz  = ph->p_memsz;
            info->segments[seg_count].flags  = ph->p_flags;
            seg_count++;
        }
    }
    info->num_segments = seg_count;

    // --- Section Headers ---
    if (ehdr->e_shoff + ehdr->e_shnum * sizeof(Elf64_Shdr) > size)
        goto fail;

    const Elf64_Shdr *sh_table = (const Elf64_Shdr *)(mem + ehdr->e_shoff);

    if (ehdr->e_shstrndx >= ehdr->e_shnum) goto fail;
    const Elf64_Shdr *sh_strtab = &sh_table[ehdr->e_shstrndx];

    if (sh_strtab->sh_offset + sh_strtab->sh_size > size) goto fail;
    const char *shstrtab = (const char *)(mem + sh_strtab->sh_offset);

    for (int i = 0; i < ehdr->e_shnum; i++) {
        const char *name = shstrtab + sh_table[i].sh_name;
        if (strcmp(name, ".text") == 0) {
            info->text_section.offset = sh_table[i].sh_offset;
            info->text_section.addr   = sh_table[i].sh_addr;
            info->text_section.size   = sh_table[i].sh_size;
            info->text_found = 1;
            break;
        }
    }

    return 0;

fail:
    free_elf_info(info);
    return -1;
}


void free_elf_info(ElfInfo *info) {
    if (info->segments) {
        free(info->segments);
        info->segments = NULL;
    }
    info->num_segments = 0;
    info->text_found = 0;
}
