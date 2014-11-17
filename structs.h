typedef struct st_hash_s {
    int id;
} st_hash_t;

typedef struct ut_hash_s {
    int id;
    UT_hash_handle hh;
} ut_hash_t;

typedef struct tommy_elm_s {
  tommy_node node;
  int id;
} tommy_elm_t;
