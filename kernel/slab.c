#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"
#include "slab.h"

#define PAGESIZE 4096
#define BUFFER_LEN 64
slab_obj objs[64];
int obj_cnt = 0;

slab slabs[8];


slab_obj* get_new_object(uint per_size) {
    slab_obj* now_obj = &objs[obj_cnt];
    obj_cnt ++;
    now_obj->available_cnt = PAGESIZE / per_size;
    now_obj->per_size = per_size;
    now_obj->start_address = kalloc();
    int i;
    for(i = 0; i < PAGESIZE / per_size; i++) {
        now_obj->free_list[i] = now_obj->start_address + i * per_size;
    }
    return now_obj;
}

slab_obj* new_slab_obj(slab* s, uint per_size) {
    slab_obj* new_obj = get_new_object(per_size);
    uint hash_code = (uint)new_obj->start_address % 8;
    s->hash_map[hash_code][s->cnt[hash_code] ++].obj = new_obj;
    s->buffer_tl = (s->buffer_tl + 1) % BUFFER_LEN;
    s->buffer[s->buffer_tl] = new_obj;
}

void* slab_alloc(pde_t* pg_tb, void* va, uint sz) {
    if(sz > 2048 || sz <= 0) return 0;
    int size = 16, i = 0, j;
    while(size < sz) size *= 2, i++;
    if(slabs[i].buffer_hd == (slabs[i].buffer_tl + 1) % BUFFER_LEN) {
        new_slab_obj(slabs[i], size);
    }
    slab_obj* obj = slabs[i].buffer[slabs[i].buffer_tl];
    uint pa = obj->free_list[obj->available_cnt - 1];
    mappages(pg_tb, va, PAGESIZE, V2P(pa), PTE_W | PTE_U);
    obj->available_cnt --;
    if(obj->available_cnt == 0) {
        slabs[i].buffer_hd ++;
    }
    void* result = (void*)((uint*)va + pa - obj->start_address);
    (uint*)va += PAGESIZE;
    return result; // return va
}

int slab_free(pde_t* pg_tb, void* va) {
    uint page_addr = (uint) uva2ka(pg_tb, va);
    uint page_offset = (uint)va & 4095;
    uint pa = page_addr + page_offset;

    int i, j;
    int hash_code = page_addr % 8;
    for(i = 0; i < 8; i++) {
        uint find = 0;
        for(j = 0; j < slabs[i].cnt[hash_code]; j++) {
            uint start = slabs[i].hash_map[hash_code][j].obj->start_address;
            uint end = start + PAGESIZE;
            if(start <= pa && pa < end) {
                find = 1;
                slab_obj* obj = slabs[i].hash_map[hash_code][j].obj;
                uint index = (pa - start) / obj->per_size;
                obj->free_list[obj->available_cnt] = (void*)(start + index * obj->per_size);
                if(obj->available_cnt == 0) {
                    if((slabs[i].buffer_tl + 2) % BUFFER_LEN == slabs[i].buffer_hd)
                        printf("warning! buffer full.\n");
                    slabs[i].buffer_tl = (slabs[i].buffer_tl + 1) % BUFFER_LEN;
                    slabs[i].buffer[slabs[i].buffer_tl] = obj;
                }
                obj->available_cnt ++;
            }
        }
        if(find) return 1;
    }
    return 0;
}

void slab_init() {
    int i, size;
    for(i = 0, size = 16; i < 8; i++, size *= 2) {
        slabs[i].buffer_hd = 0;
        slabs[i].buffer_tl = BUFFER_LEN - 1;
        new_slab_obj(&slabs[i], size);
    }
}
