#ifndef R2_CRYPTO_H
#define R2_CRYPTO_H

#include <r_types.h>
#include <r_list.h>

#ifdef __cplusplus
extern "C" {
#endif

R_LIB_VERSION_HEADER(r_crypto);

enum {
	R_CRYPTO_MODE_ECB,
	R_CRYPTO_MODE_CBC,
	R_CRYPTO_MODE_OFB,
	R_CRYPTO_MODE_CFB,
};

enum {
	R_CRYPTO_DIR_CIPHER,
	R_CRYPTO_DIR_DECIPHER,
};

typedef struct r_crypto_t {
	struct r_crypto_plugin_t* h;
	ut8 *key;
	ut8 *iv;
	int key_len;
	ut8 *output;
	int output_len;
	int output_size;
	int dir;
	void *user;
	RList *plugins;
} RCrypto;

typedef struct r_crypto_plugin_t {
	const char *name;
	const char *license;
	int (*get_key_size)(RCrypto *cry);
	bool (*set_iv)(RCrypto *cry, const ut8 *iv, int ivlen);
	bool (*set_key)(RCrypto *cry, const ut8 *key, int keylen, int mode, int direction);
	bool (*update)(RCrypto *cry, const ut8 *buf, int len);
	bool (*final)(RCrypto *cry, const ut8 *buf, int len);
	bool (*use)(const char *algo);
	int (*fini)(RCrypto *cry);
} RCryptoPlugin;

#ifdef R_API
R_API RCrypto *r_crypto_init(RCrypto *cry, int hard);
R_API RCrypto *r_crypto_as_new(RCrypto *cry);
R_API int r_crypto_add(RCrypto *cry, RCryptoPlugin *h);
R_API RCrypto *r_crypto_new(void);
R_API RCrypto *r_crypto_free(RCrypto *cry);
R_API bool r_crypto_use(RCrypto *cry, const char *algo);
R_API bool r_crypto_set_key(RCrypto *cry, const ut8* key, int keylen, int mode, int direction);
R_API bool r_crypto_set_iv(RCrypto *cry, const ut8 *iv, int ivlen);
R_API int r_crypto_update(RCrypto *cry, const ut8 *buf, int len);
R_API int r_crypto_final(RCrypto *cry, const ut8 *buf, int len);
R_API int r_crypto_append(RCrypto *cry, const ut8 *buf, int len);
R_API ut8 *r_crypto_get_output(RCrypto *cry, int *size);
R_API const char *r_crypto_name(ut64 bit);
#endif

/* plugin pointers */
extern RCryptoPlugin r_crypto_plugin_aes;
extern RCryptoPlugin r_crypto_plugin_des;
extern RCryptoPlugin r_crypto_plugin_rc4;
extern RCryptoPlugin r_crypto_plugin_xor;
extern RCryptoPlugin r_crypto_plugin_blowfish;
extern RCryptoPlugin r_crypto_plugin_rc2;
extern RCryptoPlugin r_crypto_plugin_rot;
extern RCryptoPlugin r_crypto_plugin_rol;
extern RCryptoPlugin r_crypto_plugin_ror;
extern RCryptoPlugin r_crypto_plugin_base64;
extern RCryptoPlugin r_crypto_plugin_base91;
extern RCryptoPlugin r_crypto_plugin_aes_cbc;
extern RCryptoPlugin r_crypto_plugin_punycode;
extern RCryptoPlugin r_crypto_plugin_rc6;
extern RCryptoPlugin r_crypto_plugin_cps2;
extern RCryptoPlugin r_crypto_plugin_serpent;

#define R_CRYPTO_NONE 0
#define R_CRYPTO_RC2 1
#define R_CRYPTO_RC4 1<<1
#define R_CRYPTO_RC6 1<<2
#define R_CRYPTO_AES_ECB 1<<3
#define R_CRYPTO_AES_CBC 1<<4
#define R_CRYPTO_ROR 1<<5
#define R_CRYPTO_ROL 1<<6
#define R_CRYPTO_ROT 1<<7
#define R_CRYPTO_BLOWFISH 1<<8
#define R_CRYPTO_CPS2 1<<9
#define R_CRYPTO_DES_ECB 1<<10
#define R_CRYPTO_XOR 1<<11
#define R_CRYPTO_SERPENT 1<<12
#define R_CRYPTO_ALL 0xFFFF

#ifdef __cplusplus
}
#endif

#endif
