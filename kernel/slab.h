typedef struct slab_obj {
    uint available_cnt;
    uint per_size;
    void* start_address;
    void* free_list[256]; // 4096 / 16 = 256
} slab_obj;

typedef struct slab_hash_node {
    slab_obj* obj;
} slab_hash_node;

typedef struct slab {
    slab_hash_node hash_map[8][16];
    slab_obj* buffer[64];
    uint buffer_hd, buffer_tl;
    uint cnt[8];
} slab;
