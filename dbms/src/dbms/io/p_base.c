#include "p_base.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "page/p_base.h"

static base_header DUMPED = (base_header){.type = PAGE_DUMPED};

// Load only size of page from file
void page_select_size(pageoff_t *size, FILE *file, const fileoff_t page_start) {
  fseek(file, page_start.bytes + offsetof(base_header, size), SEEK_SET);
  int asr = fread(size, sizeof(*size), 1, file);
  assert(asr);
}

// Load page to malloced memory
void page_select(base_page *base, FILE *file, fileoff_t page_start) {
  fseek(file, page_start.bytes, SEEK_SET);
  int asr = fread(base, base->header.size.bytes, 1, file);
  assert(asr);
}

void page_create(struct base_page *base, FILE *file, fileoff_t page_start) {
  page_alter(base, file, page_start);
}

void page_alter(struct base_page *base, FILE *file, fileoff_t page_start) {
  fseek(file, page_start.bytes, SEEK_SET);
  fwrite(base, base->header.size.bytes, 1, file);
}

void page_drop(FILE *file, fileoff_t page_start) {
  fseek(file, page_start.bytes + offsetof(base_header, type), SEEK_SET);
  fwrite(&DUMPED.type, sizeof(DUMPED.type), 1, file);
}
