#ifndef SDB_H
#define SDB_H

#if !defined(O_BINARY) && !defined(_MSC_VER)
#define O_BINARY 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "sdbht.h"
#include "ls.h"
#include "dict.h"
#include "cdb.h"
#include "cdb_make.h"
#include "sdb_version.h"

#undef r_offsetof
#define r_offsetof(type, member) ((unsigned long) &((type*)0)->member)

/* Key value sizes */
#define SDB_MIN_VALUE 1
#define SDB_MAX_VALUE 0xffffff
#define SDB_MIN_KEY 1
#define SDB_MAX_KEY 0xff

#if !defined(SZT_ADD_OVFCHK)
#define SZT_ADD_OVFCHK(x, y) ((SIZE_MAX - (x)) <= (y))
#endif

#if __SDB_WINDOWS__ && !__CYGWIN__
#include <windows.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#ifndef _MSC_VER
extern __attribute__((dllimport)) void *__cdecl _aligned_malloc(size_t, size_t);
extern char *strdup (const char *);
#else
#include <process.h>
#include <windows.h>
#include <malloc.h> // for _aligned_malloc
#define ftruncate _chsize
#endif
#undef r_offsetof
#define r_offsetof(type, member) ((unsigned long) (ut64)&((type*)0)->member)
//#define SDB_MODE 0
#define SDB_MODE _S_IWRITE | _S_IREAD
#else
#define SDB_MODE 0644
//#define SDB_MODE 0600
#endif

//#define SDB_RS '\x1e'
#define SDB_RS ','
#define SDB_SS ","
#define SDB_MAX_PATH 256
#define SDB_NUM_BASE 16
#define SDB_NUM_BUFSZ 64

#define SDB_OPTION_NONE 0
#define SDB_OPTION_ALL 0xff
#define SDB_OPTION_SYNC    (1 << 0)
#define SDB_OPTION_NOSTAMP (1 << 1)
#define SDB_OPTION_FS      (1 << 2)
#define SDB_OPTION_JOURNAL (1 << 3)

#define SDB_LIST_UNSORTED 0
#define SDB_LIST_SORTED 1

// This size implies trailing zero terminator, this is 254 chars + 0
#define SDB_KSZ 0xff
#define SDB_VSZ 0xffffff


typedef struct sdb_t {
	char *dir; // path+name
	char *path;
	char *name;
	int fd;
	int refs; // reference counter
	int lock;
	int journal;
	struct cdb db;
	struct cdb_make m;
	SdbHash *ht;
	ut32 eod;
	ut32 pos;
	int fdump;
	char *ndump;
	ut64 expire;
	ut64 last; // timestamp of last change
	int options;
	int ns_lock; // TODO: merge into options?
	SdbList *ns;
	SdbList *hooks;
	SdbKv tmpkv;
	ut32 depth;
	bool timestamped;
	SdbMini mht;
} Sdb;

typedef struct sdb_ns_t {
	char *name;
	ut32 hash;
	Sdb *sdb;
} SdbNs;

SDB_API Sdb* sdb_new0(void);
SDB_API Sdb* sdb_new(const char *path, const char *file, int lock);

SDB_API int sdb_open(Sdb *s, const char *file);
SDB_API void sdb_close(Sdb *s);

SDB_API void sdb_config(Sdb *s, int options);
SDB_API bool sdb_free(Sdb* s);
SDB_API void sdb_file(Sdb* s, const char *dir);
SDB_API bool sdb_merge(Sdb* d, Sdb *s);
SDB_API int sdb_count(Sdb* s);
SDB_API void sdb_reset(Sdb* s);
SDB_API void sdb_setup(Sdb* s, int options);
SDB_API void sdb_drain(Sdb*, Sdb*);
SDB_API bool sdb_stats(Sdb *s, ut32 *disk, ut32 *mem);
SDB_API bool sdb_dump_hasnext (Sdb* s);

typedef int (*SdbForeachCallback)(void *user, const char *k, const char *v);
bool sdb_foreach(Sdb* s, SdbForeachCallback cb, void *user);
SdbList *sdb_foreach_list(Sdb* s, bool sorted);
SdbList *sdb_foreach_match(Sdb* s, const char *expr, bool sorted);

int sdb_query(Sdb* s, const char *cmd);
int sdb_queryf(Sdb* s, const char *fmt, ...);
int sdb_query_lines(Sdb *s, const char *cmd);
char *sdb_querys(Sdb* s, char *buf, size_t len, const char *cmd);
char *sdb_querysf(Sdb* s, char *buf, size_t buflen, const char *fmt, ...);
int sdb_query_file(Sdb *s, const char* file);
bool sdb_exists(Sdb*, const char *key);
bool sdb_remove(Sdb*, const char *key, ut32 cas);
int sdb_unset(Sdb*, const char *key, ut32 cas);
int sdb_unset_like(Sdb *s, const char *k);
char** sdb_like(Sdb *s, const char *k, const char *v, SdbForeachCallback cb);

// Gets a pointer to the value associated with `key`.
char *sdb_get(Sdb*, const char *key, ut32 *cas);

// Gets a pointer to the value associated with `key` and returns in `vlen` the
// length of the value string.
char *sdb_get_len(Sdb*, const char *key, int *vlen, ut32 *cas);

// Gets a const pointer to the value associated with `key`
const char *sdb_const_get(Sdb*, const char *key, ut32 *cas);

// Gets a const pointer to the value associated with `key` and returns in
// `vlen` the length of the value string.
const char *sdb_const_get_len(Sdb* s, const char *key, int *vlen, ut32 *cas);
int sdb_set(Sdb*, const char *key, const char *data, ut32 cas);
int sdb_set_owned(Sdb* s, const char *key, char *val, ut32 cas);
int sdb_concat(Sdb *s, const char *key, const char *value, ut32 cas);
int sdb_uncat(Sdb *s, const char *key, const char *value, ut32 cas);
int sdb_add(Sdb* s, const char *key, const char *val, ut32 cas);
bool sdb_sync(Sdb*);
void sdb_kv_free(SdbKv *kv);

/* num.c */
int  sdb_num_exists(Sdb*, const char *key);
int  sdb_num_base(const char *s);
ut64 sdb_num_get(Sdb* s, const char *key, ut32 *cas);
int  sdb_num_set(Sdb* s, const char *key, ut64 v, ut32 cas);
int  sdb_num_add(Sdb *s, const char *key, ut64 v, ut32 cas);
ut64 sdb_num_inc(Sdb* s, const char *key, ut64 n, ut32 cas);
ut64 sdb_num_dec(Sdb* s, const char *key, ut64 n, ut32 cas);
int  sdb_num_min(Sdb* s, const char *key, ut64 v, ut32 cas);
int  sdb_num_max(Sdb* s, const char *key, ut64 v, ut32 cas);

/* ptr */
int sdb_ptr_set(Sdb *db, const char *key, void *p, ut32 cas);
void* sdb_ptr_get(Sdb *db, const char *key, ut32 *cas);

/* create db */
SDB_API bool sdb_disk_create(Sdb* s);
int sdb_disk_insert(Sdb* s, const char *key, const char *val);
SDB_API bool sdb_disk_finish(Sdb* s);
SDB_API bool sdb_disk_unlink(Sdb* s);

/* iterate */
SDB_API void sdb_dump_begin(Sdb* s);
SDB_API SdbKv *sdb_dump_next(Sdb* s);
SDB_API bool sdb_dump_dupnext(Sdb* s, char *key, char **value, int *_vlen);

/* journaling */
SDB_API bool sdb_journal_close(Sdb *s);
SDB_API bool sdb_journal_open(Sdb *s);
SDB_API int sdb_journal_load(Sdb *s);
SDB_API bool sdb_journal_log(Sdb *s, const char *key, const char *val);
SDB_API bool sdb_journal_clear(Sdb *s);
SDB_API bool sdb_journal_unlink(Sdb *s);

/* numeric */
SDB_API char *sdb_itoa(ut64 n, char *s, int base);
SDB_API ut64  sdb_atoi(const char *s);
SDB_API const char *sdb_itoca(ut64 n);

/* locking */
SDB_API bool sdb_lock(const char *s);
SDB_API const char *sdb_lock_file(const char *f);
SDB_API void sdb_unlock(const char *s);
SDB_API int sdb_unlink(Sdb* s);
SDB_API int sdb_lock_wait(const char *s UNUSED);

/* expiration */
SDB_API bool sdb_expire_set(Sdb* s, const char *key, ut64 expire, ut32 cas);
SDB_API ut64 sdb_expire_get(Sdb* s, const char *key, ut32 *cas);
SDB_API ut64 sdb_now(void);
SDB_API ut64 sdb_unow(void);
SDB_API ut32 sdb_hash(const char *key);
SDB_API ut32 sdb_hash_len(const char *key, ut32 *len);
SDB_API ut8 sdb_hash_byte(const char *s);

/* json api */
// SDB_API int sdb_js0n(const unsigned char *js, RangstrType len, RangstrType *out);
SDB_API bool sdb_isjson(const char *k);
SDB_API char *sdb_json_get_str (const char *json, const char *path);

SDB_API char *sdb_json_get(Sdb* s, const char *key, const char *p, ut32 *cas);
SDB_API bool sdb_json_set(Sdb* s, const char *k, const char *p, const char *v, ut32 cas);
SDB_API int sdb_json_num_get(Sdb* s, const char *k, const char *p, ut32 *cas);
SDB_API int sdb_json_num_set(Sdb* s, const char *k, const char *p, int v, ut32 cas);
SDB_API int sdb_json_num_dec(Sdb* s, const char *k, const char *p, int n, ut32 cas);
SDB_API int sdb_json_num_inc(Sdb* s, const char *k, const char *p, int n, ut32 cas);

char *sdb_json_indent(const char *s, const char *tab);
char *sdb_json_unindent(const char *s);

typedef struct {
	char *buf;
	size_t blen;
	size_t len;
} SdbJsonString;

const char *sdb_json_format(SdbJsonString* s, const char *fmt, ...);
#define sdb_json_format_free(x) free ((x)->buf)

// namespace
Sdb* sdb_ns(Sdb *s, const char *name, int create);
Sdb *sdb_ns_path(Sdb *s, const char *path, int create);
void sdb_ns_init(Sdb* s);
void sdb_ns_free(Sdb* s);
void sdb_ns_lock(Sdb *s, int lock, int depth);
void sdb_ns_sync(Sdb* s);
int sdb_ns_set(Sdb *s, const char *name, Sdb *r);
bool sdb_ns_unset(Sdb *s, const char *name, Sdb *r);

// array
int sdb_array_contains(Sdb* s, const char *key, const char *val, ut32 *cas);
int sdb_array_contains_num(Sdb *s, const char *key, ut64 val, ut32 *cas);
int sdb_array_indexof(Sdb *s, const char *key, const char *val, ut32 cas);
int sdb_array_set(Sdb* s, const char *key, int idx, const char *val, ut32 cas);
int sdb_array_set_num(Sdb* s, const char *key, int idx, ut64 val, ut32 cas);
bool sdb_array_append(Sdb *s, const char *key, const char *val, ut32 cas);
bool sdb_array_append_num(Sdb *s, const char *key, ut64 val, ut32 cas);
int sdb_array_prepend(Sdb *s, const char *key, const char *val, ut32 cas);
int sdb_array_prepend_num(Sdb *s, const char *key, ut64 val, ut32 cas);
char *sdb_array_get(Sdb* s, const char *key, int idx, ut32 *cas);
ut64 sdb_array_get_num(Sdb* s, const char *key, int idx, ut32 *cas);
int sdb_array_get_idx(Sdb *s, const char *key, const char *val, ut32 cas); // agetv
int sdb_array_insert(Sdb* s, const char *key, int idx, const char *val, ut32 cas);
int sdb_array_insert_num(Sdb* s, const char *key, int idx, ut64 val, ut32 cas);
int sdb_array_unset(Sdb* s, const char *key, int n, ut32 cas); // leaves empty bucket
int sdb_array_delete(Sdb* s, const char *key, int n, ut32 cas);
void sdb_array_sort(Sdb* s, const char *key, ut32 cas);
void sdb_array_sort_num(Sdb* s, const char *key, ut32 cas);
// set

// Adds string `val` at the end of array `key`.
int sdb_array_add(Sdb* s, const char *key, const char *val, ut32 cas);

// Adds number `val` at the end of array `key`.
int sdb_array_add_num(Sdb* s, const char *key, ut64 val, ut32 cas);

// Adds string `val` in the sorted array `key`.
int sdb_array_add_sorted(Sdb *s, const char *key, const char *val, ut32 cas);

// Adds number `val` in the sorted array `key`.
int sdb_array_add_sorted_num(Sdb *s, const char *key, ut64 val, ut32 cas);

// Removes the string `val` from the array `key`.
int sdb_array_remove(Sdb *s, const char *key, const char *val, ut32 cas);

// Removes the number `val` from the array `key`.
int sdb_array_remove_num(Sdb* s, const char *key, ut64 val, ut32 cas);

// helpers
char *sdb_anext(char *str, char **next);
const char *sdb_const_anext(const char *str, const char **next);
int sdb_alen(const char *str);
int sdb_alen_ignore_empty(const char *str);
int sdb_array_size(Sdb* s, const char *key);
int sdb_array_length(Sdb* s, const char *key);

int sdb_array_list(Sdb* s, const char *key);

// Adds the string `val` to the start of array `key`.
int sdb_array_push(Sdb *s, const char *key, const char *val, ut32 cas);

// Returns the string at the start of array `key` or
// NULL if there are no elements.
char *sdb_array_pop(Sdb *s, const char *key, ut32 *cas);

// Adds the number `val` to the start of array `key`.
int sdb_array_push_num(Sdb *s, const char *key, ut64 num, ut32 cas);

// Returns the number at the start of array `key`.
ut64 sdb_array_pop_num(Sdb *s, const char *key, ut32 *cas);

char *sdb_array_pop_head(Sdb *s, const char *key, ut32 *cas);
char *sdb_array_pop_tail(Sdb *s, const char *key, ut32 *cas);

typedef void (*SdbHook)(Sdb *s, void *user, const char *k, const char *v);

void sdb_global_hook(SdbHook hook, void *user);
SDB_API bool sdb_hook(Sdb* s, SdbHook cb, void* user);
SDB_API bool sdb_unhook(Sdb* s, SdbHook h);
int sdb_hook_call(Sdb *s, const char *k, const char *v);
void sdb_hook_free(Sdb *s);
/* Util.c */
int sdb_isnum(const char *s);

const char *sdb_type(const char *k);
bool sdb_match(const char *str, const char *glob);
int sdb_bool_set(Sdb *db, const char *str, bool v, ut32 cas);
bool sdb_bool_get(Sdb *db, const char *str, ut32 *cas);

// base64
ut8 *sdb_decode(const char *in, int *len);
char *sdb_encode(const ut8 *bin, int len);
void sdb_encode_raw(char *bout, const ut8 *bin, int len);
int sdb_decode_raw(ut8 *bout, const char *bin, int len);

// binfmt
char *sdb_fmt(const char *fmt, ...);
int sdb_fmt_init(void *p, const char *fmt);
void sdb_fmt_free(void *p, const char *fmt);
int sdb_fmt_tobin(const char *_str, const char *fmt, void *stru);
char *sdb_fmt_tostr(void *stru, const char *fmt);
SDB_API char** sdb_fmt_array(const char *list);
SDB_API ut64* sdb_fmt_array_num(const char *list);

// raw array helpers
char *sdb_array_compact(char *p);
char *sdb_aslice(char *out, int from, int to);
#define sdb_aforeach(x,y) \
	{ char *next; \
	if (y) for (x=y;;) { \
		x = sdb_anext (x, &next);
#define sdb_aforeach_next(x) \
	if (!next) break; \
	*(next-1) = ','; \
	x = next; } }

#ifdef __cplusplus
}
#endif

#endif
