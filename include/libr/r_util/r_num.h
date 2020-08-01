#ifndef R_NUM_H
#define R_NUM_H

#define R_NUMCALC_STRSZ 1024
#define r_num_abs(x) x>0?x:-x

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	double d;
	ut64 n;
} RNumCalcValue;

typedef enum {
	RNCNAME, RNCNUMBER, RNCEND, RNCINC, RNCDEC,
	RNCPLUS='+', RNCMINUS='-', RNCMUL='*', RNCDIV='/', RNCMOD='%',
	//RNCXOR='^', RNCOR='|', RNCAND='&',
	RNCNEG='~', RNCAND='&', RNCORR='|', RNCXOR='^',
	RNCPRINT=';', RNCASSIGN='=', RNCLEFTP='(', RNCRIGHTP=')',
	RNCSHL='<', RNCSHR = '>', RNCROL = '#', RNCROR = '$'
} RNumCalcToken;

typedef struct r_num_calc_t {
	RNumCalcToken curr_tok;
	RNumCalcValue number_value;
	char string_value[R_NUMCALC_STRSZ];
	int errors;
	char oc;
	const char *calc_err;
	int calc_i;
	const char *calc_buf;
	int calc_len;
	bool under_calc;
} RNumCalc;

typedef struct r_num_t {
	ut64 (*callback)(struct r_num_t *userptr, const char *str, int *ok);
	const char *(*cb_from_value)(struct r_num_t *userptr, ut64 value, int *ok);
//	RNumCallback callback;
	ut64 value;
	double fvalue;
	void *userptr;
	int dbz; /// division by zero happened
	RNumCalc nc;
} RNum;

typedef ut64 (*RNumCallback)(struct r_num_t *self, const char *str, int *ok);
typedef const char *(*RNumCallback2)(struct r_num_t *self, ut64, int *ok);

void r_srand(int seed);
int r_rand(int mod);

R_API RNum *r_num_new(RNumCallback cb, RNumCallback2 cb2, void *ptr);
R_API void r_num_free(RNum *num);
R_API char *r_num_units(char *buf, ut64 num);
R_API int r_num_conditional(RNum *num, const char *str);
R_API ut64 r_num_calc(RNum *num, const char *str, const char **err);
R_API const char *r_num_calc_index(RNum *num, const char *p);
R_API ut64 r_num_chs(int cylinder, int head, int sector, int sectorsize);
R_API int r_num_is_valid_input(RNum *num, const char *input_value);
R_API ut64 r_num_get_input_value(RNum *num, const char *input_value);
R_API const char *r_num_get_name(RNum *num, ut64 n);
R_API char* r_num_as_string(RNum *___, ut64 n, bool printable_only);
R_API ut64 r_num_tail(RNum *num, ut64 addr, const char *hex);
R_API void r_num_minmax_swap(ut64 *a, ut64 *b);
R_API void r_num_minmax_swap_i(int *a, int *b); // XXX this can be a cpp macro :??
R_API ut64 r_num_math(RNum *num, const char *str);
R_API ut64 r_num_get(RNum *num, const char *str);
R_API int r_num_to_bits(char *out, ut64 num);
R_API int r_num_to_trits(char *out, ut64 num);	//Rename this please
R_API int r_num_rand(int max);
R_API void r_num_irand(void);
R_API ut16 r_num_ntohs(ut16 foo);
R_API ut64 r_get_input_num_value(RNum *num, const char *input_value);
R_API int r_is_valid_input_num_value(RNum *num, const char *input_value);
R_API int r_num_between(RNum *num, const char *input_value);
R_API bool r_num_is_op(const char c);
R_API int r_num_str_len(const char *str);
R_API int r_num_str_split(char *str);
R_API RList *r_num_str_split_list(char *str);

#ifdef __cplusplus
}
#endif

#endif //  R_NUM_H
