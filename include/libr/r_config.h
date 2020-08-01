#ifndef R2_CONFIG_H
#define R2_CONFIG_H

#include "r_types.h"
#include "r_util.h"

#ifdef __cplusplus
extern "C" {
#endif

R_LIB_VERSION_HEADER(r_config);

#define CN_BOOL  0x000001
#define CN_INT   0x000002
#define CN_OFFT  0x000004
#define CN_STR   0x000008
#define CN_RO    0x000010
#define CN_RW    0x000020

typedef int (*RConfigCallback)(void *user, void *data);

typedef struct r_config_node_t {
	char *name;
	int flags;
	char *value;
	ut64 i_value;
	ut64 *cb_ptr_q;
	int *cb_ptr_i;
	char **cb_ptr_s;
	RConfigCallback getter;
	RConfigCallback setter;
	char *desc;
	RList *options;
} RConfigNode;

R_API const char *r_config_node_type(RConfigNode *node);

typedef struct r_config_t {
	int lock;
	int last_notfound;
	int n_nodes;
	void *user;
	RNum *num;
	PrintfCallback cb_printf;
	RList *nodes;
	SdbHash *ht;
} RConfig;

typedef struct r_config_hold_num_t {
	char *key;
	ut64 value;
} RConfigHoldNum;

typedef struct r_config_hold_char_t {
	char *key;
	const char *value;
} RConfigHoldChar;

typedef struct r_config_hold_t { 
	RConfig *cfg;
	RList *list_num; //list of RConfigHoldNum to hold numeric values 
	RList *list_char; //list of RConfigHoldChar to hold char values
} RConfigHold;

#ifdef R_API
R_API bool r_config_save_num(RConfigHold *h, ...);
R_API bool r_config_save_char(RConfigHold *h, ...);
R_API RConfigHold* r_config_hold_new(RConfig *cfg);
R_API void r_config_hold_free(RConfigHold *h);
R_API void r_config_restore(RConfigHold *h);

R_API RConfig *r_config_new(void *user);
R_API RConfig *r_config_clone (RConfig *cfg);
R_API int r_config_free(RConfig *cfg);
R_API void r_config_lock(RConfig *cfg, int l);
R_API int r_config_eval(RConfig *cfg, const char *str);
R_API void r_config_bump(RConfig *cfg, const char *key);
R_API RConfigNode *r_config_set_i(RConfig *cfg, const char *name, const ut64 i);
R_API RConfigNode *r_config_set_cb(RConfig *cfg, const char *name, const char *value, int (*callback)(void *user, void *data));
R_API RConfigNode *r_config_set_i_cb(RConfig *cfg, const char *name, int ivalue, int (*callback)(void *user, void *data));
R_API RConfigNode *r_config_set(RConfig *cfg, const char *name, const char *value);
R_API int r_config_rm(RConfig *cfg, const char *name);
R_API ut64 r_config_get_i(RConfig *cfg, const char *name);
R_API const char *r_config_get(RConfig *cfg, const char *name);
R_API const char *r_config_desc(RConfig *cfg, const char *name, const char *desc);
R_API const char *r_config_node_desc(RConfigNode *node, const char *desc);
R_API void r_config_list(RConfig *cfg, const char *str, int rad);
R_API RConfigNode *r_config_node_get(RConfig *cfg, const char *name);
R_API RConfigNode *r_config_node_new(const char *name, const char *value);
R_API void r_config_node_free(void *n);
R_API int r_config_toggle(RConfig *cfg, const char *name);
R_API int r_config_readonly (RConfig *cfg, const char *key);

/*----------------------------------------------------------------------------------------------*/
R_API void r_config_set_sort_column (char *column);
/*----------------------------------------------------------------------------------------------*/

R_API int r_config_set_setter (RConfig *cfg, const char *key, RConfigCallback cb);
R_API int r_config_set_getter (RConfig *cfg, const char *key, RConfigCallback cb);
#endif

#ifdef __cplusplus
}
#endif

#endif
