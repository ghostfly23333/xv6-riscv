#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "slab.h"

slab_obj objs[64];
int obj_cnt = 0;

slab slabs[8];


slab_obj* get_new_object() {
    obj_cnt ++;
    return &objs[obj_cnt];
}

slab_obj* new_slab_obj(slab* s) {
    slab_obj* new_obj = get_new_object();
    int hash_code = (uint64)new_obj->start_address % 8;
    s->hash_map[hash_code][s->cnt[hash_code] ++].obj = new_obj;
    s->buffer[++ s->buffer_tl] = new_obj;
}

void* slab_alloc(pagetable_t* pg_tb, int size) {

}

int slab_free(pagetable_t* pg_tb, void* va) {
    uint64 page_addr = (uint64)
    uint64 page_offset = (uint64)va & 4095;
}

void slab_init() {

}
