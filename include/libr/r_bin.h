#ifndef R2_BIN_H
#define R2_BIN_H

#include <r_util.h>
#include <r_types.h>
#include <r_io.h>
#include <r_list.h>
#include <r_bin_dwarf.h>
#include <r_pdb.h>

#ifdef __cplusplus
extern "C" {
#endif

R_LIB_VERSION_HEADER (r_bin);

#define R_BIN_SCN_EXECUTABLE (1 << 0)
#define R_BIN_SCN_WRITABLE   (1 << 1)
#define R_BIN_SCN_READABLE   (1 << 2)
#define R_BIN_SCN_SHAREABLE  (1 << 3)

#define R_BIN_DBG_STRIPPED 0x01
#define R_BIN_DBG_STATIC   0x02
#define R_BIN_DBG_LINENUMS 0x04
#define R_BIN_DBG_SYMS     0x08
#define R_BIN_DBG_RELOCS   0x10

#define R_BIN_ENTRY_TYPE_PROGRAM 0
#define R_BIN_ENTRY_TYPE_MAIN    1
#define R_BIN_ENTRY_TYPE_INIT    2
#define R_BIN_ENTRY_TYPE_FINI    3
#define R_BIN_ENTRY_TYPE_TLS     4
#define R_BIN_ENTRY_TYPE_PREINIT 5

#define R_BIN_SIZEOF_STRINGS 512
#define R_BIN_MAX_ARCH 1024

#define R_BIN_REQ_ALL       UT64_MAX
#define R_BIN_REQ_UNK       0x000000
#define R_BIN_REQ_ENTRIES   0x000001
#define R_BIN_REQ_IMPORTS   0x000002
#define R_BIN_REQ_SYMBOLS   0x000004
#define R_BIN_REQ_SECTIONS  0x000008
#define R_BIN_REQ_INFO      0x000010
#define R_BIN_REQ_OPERATION 0x000020
#define R_BIN_REQ_HELP      0x000040
#define R_BIN_REQ_STRINGS   0x000080
#define R_BIN_REQ_FIELDS    0x000100
#define R_BIN_REQ_LIBS      0x000200
#define R_BIN_REQ_SRCLINE   0x000400
#define R_BIN_REQ_MAIN      0x000800
#define R_BIN_REQ_EXTRACT   0x001000
#define R_BIN_REQ_RELOCS    0x002000
#define R_BIN_REQ_LISTARCHS 0x004000
#define R_BIN_REQ_CREATE    0x008000
#define R_BIN_REQ_CLASSES   0x010000
#define R_BIN_REQ_DWARF     0x020000
#define R_BIN_REQ_SIZE      0x040000
#define R_BIN_REQ_PDB       0x080000
#define R_BIN_REQ_PDB_DWNLD 0x100000
#define R_BIN_REQ_DLOPEN    0x200000
#define R_BIN_REQ_EXPORTS   0x400000
#define R_BIN_REQ_VERSIONINFO 0x800000
#define R_BIN_REQ_PACKAGE   0x1000000
#define R_BIN_REQ_HEADER    0x2000000
#define R_BIN_REQ_LISTPLUGINS 0x4000000
#define R_BIN_REQ_RESOURCES 0x8000000
#define R_BIN_REQ_INITFINI 0x10000000
#define R_BIN_REQ_SEGMENTS 0x20000000

/* RBinSymbol->method_flags : */
#define R_BIN_METH_CLASS 0x0000000000000001L
#define R_BIN_METH_STATIC 0x0000000000000002L
#define R_BIN_METH_PUBLIC 0x0000000000000004L
#define R_BIN_METH_PRIVATE 0x0000000000000008L
#define R_BIN_METH_PROTECTED 0x0000000000000010L
#define R_BIN_METH_INTERNAL 0x0000000000000020L
#define R_BIN_METH_OPEN 0x0000000000000040L
#define R_BIN_METH_FILEPRIVATE 0x0000000000000080L
#define R_BIN_METH_FINAL 0x0000000000000100L
#define R_BIN_METH_VIRTUAL 0x0000000000000200L
#define R_BIN_METH_CONST 0x0000000000000400L
#define R_BIN_METH_MUTATING 0x0000000000000800L
#define R_BIN_METH_ABSTRACT 0x0000000000001000L
#define R_BIN_METH_SYNCHRONIZED 0x0000000000002000L
#define R_BIN_METH_NATIVE 0x0000000000004000L
#define R_BIN_METH_BRIDGE 0x0000000000008000L
#define R_BIN_METH_VARARGS 0x0000000000010000L
#define R_BIN_METH_SYNTHETIC 0x0000000000020000L
#define R_BIN_METH_STRICT 0x0000000000040000L
#define R_BIN_METH_MIRANDA 0x0000000000080000L
#define R_BIN_METH_CONSTRUCTOR 0x0000000000100000L
#define R_BIN_METH_DECLARED_SYNCHRONIZED 0x0000000000200000L

#define R_BIN_BIND_LOCAL_STR "LOCAL"
#define R_BIN_BIND_GLOBAL_STR "GLOBAL"
#define R_BIN_BIND_WEAK_STR "WEAK"
#define R_BIN_BIND_NUM_STR "NUM"
#define R_BIN_BIND_LOOS_STR "LOOS"
#define R_BIN_BIND_HIOS_STR "HIOS"
#define R_BIN_BIND_LOPROC_STR "LOPROC"
#define R_BIN_BIND_HIPROC_STR "HIPROC"
#define R_BIN_BIND_UNKNOWN_STR "UNKNOWN"

#define R_BIN_TYPE_NOTYPE_STR "NOTYPE"
#define R_BIN_TYPE_OBJECT_STR "OBJ"
#define R_BIN_TYPE_FUNC_STR "FUNC"
#define R_BIN_TYPE_SECTION_STR "SECT"
#define R_BIN_TYPE_FILE_STR "FILE"
#define R_BIN_TYPE_COMMON_STR "COMMON"
#define R_BIN_TYPE_TLS_STR "TLS"
#define R_BIN_TYPE_NUM_STR "NUM"
#define R_BIN_TYPE_LOOS_STR "LOOS"
#define R_BIN_TYPE_HIOS_STR "HIOS"
#define R_BIN_TYPE_LOPROC_STR "LOPROC"
#define R_BIN_TYPE_HIPROC_STR "HIPROC"
#define R_BIN_TYPE_UNKNOWN_STR "UNK"

enum {
	R_BIN_SYM_ENTRY,
	R_BIN_SYM_INIT,
	R_BIN_SYM_MAIN,
	R_BIN_SYM_FINI,
	R_BIN_SYM_LAST
};

// name mangling types
// TODO: Rename to R_BIN_LANG_
enum {
	R_BIN_NM_NONE = 0,
	R_BIN_NM_JAVA = 1,
	R_BIN_NM_CXX = 1<<1,
	R_BIN_NM_OBJC = 1<<2,
	R_BIN_NM_SWIFT = 1<<3,
	R_BIN_NM_DLANG = 1<<4,
	R_BIN_NM_MSVC = 1<<5,
	R_BIN_NM_RUST = 1<<6,
	R_BIN_NM_ANY = -1,
};

enum {
	R_STRING_TYPE_DETECT = '?',
	R_STRING_TYPE_ASCII = 'a',
	R_STRING_TYPE_UTF8 = 'u',
	R_STRING_TYPE_WIDE = 'w', // utf16 / widechar string
	R_STRING_TYPE_WIDE32 = 'W', // utf32
	R_STRING_TYPE_BASE64 = 'b',
};

enum {
	R_BIN_CLASS_PRIVATE,
	R_BIN_CLASS_PUBLIC,
	R_BIN_CLASS_FRIENDLY,
	R_BIN_CLASS_PROTECTED,
};

enum {
	R_BIN_RELOC_8 = 8,
	R_BIN_RELOC_16 = 16,
	R_BIN_RELOC_32 = 32,
	R_BIN_RELOC_64 = 64
};

enum {
	R_BIN_TYPE_DEFAULT = 0,
	R_BIN_TYPE_CORE = 1
};

typedef struct r_bin_addr_t {
	ut64 vaddr;
	ut64 paddr;
	ut64 haddr;
	int type;
	int bits;
} RBinAddr;

typedef struct r_bin_hash_t {
	const char *type;
	ut64 addr;
	int len;
	ut64 from;
	ut64 to;
	ut8 buf[32];
	const char *cmd;
} RBinHash;

typedef struct r_bin_info_t {
	char *file;
	char *type;
	char *bclass;
	char *rclass;
	char *arch;
	char *cpu;
	char *machine;
	char *os;
	char *subsystem;
	char *rpath;
	char *guid;
	char *debug_file_name;
	const char *lang;
	int bits;
	int has_va;
	int has_pi; // pic/pie
	int has_canary;
	int has_crypto;
	int has_nx;
	int big_endian;
	bool has_lit;
	char *actual_checksum;
	char *claimed_checksum;
	int pe_overlay;
	bool signature;
	ut64 dbg_info;
	RBinHash sum[3];
	ut64 baddr;
	char *intrp;
#if 0
// stored in sdb
	/* crypto (iOS bins) */
	int crypt_offset;
	int crypt_size;
	int crypt_enabled;
#endif
} RBinInfo;

typedef struct r_bin_object_t {
	ut32 id;
	ut64 baddr;
	ut64 baddr_shift;
	ut64 loadaddr;
	ut64 boffset;
	ut64 size;
	ut64 obj_size;
	RList/*<RBinSection>*/ *sections;
	RList/*<RBinImport>*/ *imports;
	RList/*<RBinSymbol>*/ *symbols;
	RList/*<??>*/ *entries;
	RList/*<??>*/ *fields;
	RList/*<??>*/ *libs;
	RList/*<RBinReloc>*/ *relocs;
	RList/*<??>*/ *strings;
	RList/*<RBinClass>*/ *classes;
	RList/*<RBinDwarfRow>*/ *lines;
	RList/*<??>*/ *mem;	//RBinMem maybe?
	RList/*<BinMap*/ *maps;
	char *regstate;
	RBinInfo *info;
	RBinAddr *binsym[R_BIN_SYM_LAST];
	struct r_bin_plugin_t *plugin;
	int referenced;
	int lang;
	Sdb *kv;
	Sdb *addr2klassmethod;
	void *bin_obj; // internal pointer used by formats
} RBinObject;

// XXX: this is a copy of RBinObject
typedef struct r_bin_file_t {
	char *file;
	int fd;
	int size;
	int rawstr;
	int strmode;
	ut32 id;
	RBuffer *buf;
	ut64 offset;
	RBinObject *o;
	void *xtr_obj;
	ut64 loadaddr;
	/* values used when searching the strings */
	int minstrlen;
	int maxstrlen;
	int narch;
	struct r_bin_xtr_plugin_t *curxtr;
	struct r_bin_plugin_t *curplugin;
	RList *objs;
	RList *xtr_data;
	Sdb *sdb;
	Sdb *sdb_info;
	Sdb *sdb_addrinfo;
	struct r_bin_t *rbin;
} RBinFile;

typedef struct RBinFileOptions {
	int rawstr;
	ut64 baddr; // base address
	ut64 laddr; // load address
	ut64 paddr; // offset
	const char *plugname; // force a plugin? why do i need this?
	// const char *xtrname;
} RBinFileOptions;

typedef struct r_bin_t {
	const char *file;
	RBinFile *cur;
	int narch;
	void *user;
	/* preconfigured values */
	int debase64;
	int minstrlen;
	int maxstrlen;
	ut64 maxstrbuf;
	int rawstr;
	Sdb *sdb;
	RIDStorage *ids;
	RList/*<RBinPlugin>*/ *plugins;
	RList/*<RBinXtrPlugin>*/ *binxtrs;
	RList/*<RBinLdrPlugin>*/ *binldrs;
	RList/*<RBinFile>*/ *binfiles;
	PrintfCallback cb_printf;
	int loadany;
	RIOBind iob;
	char *force;
	int is_debugger;
	bool want_dbginfo;
	int filter; // symbol filtering
	char strfilter; // string filtering
	char *strpurge; // purge false positive strings
	char *srcdir; // dir.source
	char *prefix; // bin.prefix
	ut64 filter_rules;
	bool demanglercmd;
	bool verbose;
	bool io_owned;
	bool use_xtr; // use extract plugins when loading a file?
	bool use_ldr; // use loader plugins when loading a file?
} RBin;

typedef struct r_bin_xtr_metadata_t {
	char *arch;
	int bits;
	char *libname;
	char *machine;
	char *type;
} RBinXtrMetadata;

typedef int (*FREE_XTR)(void *xtr_obj);
typedef struct r_bin_xtr_extract_t {
	char *file;
	ut8 *buffer;
	ut64 size;
	ut64 offset;
	ut64 baddr;
	ut64 laddr;
	int file_count;
	int loaded;
	RBinXtrMetadata *metadata;
} RBinXtrData;

R_API RBinXtrData * r_bin_xtrdata_new (RBuffer *buf, ut64 offset, ut64 size, ut32 file_count, RBinXtrMetadata *metadata);
R_API void r_bin_xtrdata_free (void /*RBinXtrData*/ *data);
R_API void r_bin_info_free (RBinInfo *rb);
R_API void r_bin_import_free(void *_imp);
R_API void r_bin_symbol_free(void *_sym);
R_API void r_bin_string_free(void *_str);
// R_API void r_bin_field_free(void *_fld);

typedef struct r_bin_xtr_plugin_t {
	char *name;
	char *desc;
	char *license;
	int (*init)(void *user);
	int (*fini)(void *user);
// XXX: ut64 for size is maybe too much, what about st64? signed sizes are useful for detecting errors
	bool (*check_bytes)(const ut8 *bytes, ut64 sz);

	RBinXtrData * (*extract_from_bytes)(RBin *bin, const ut8 *buf, ut64 size, int idx);
	RList * (*extractall_from_bytes)(RBin *bin, const ut8 *buf, ut64 size);
	RBinXtrData * (*extract)(RBin *bin, int idx);
	RList * (*extractall)(RBin *bin);

	bool (*load)(RBin *bin);
	int (*size)(RBin *bin);
	int (*destroy)(RBin *bin);
	int (*free_xtr)(void *xtr_obj);
} RBinXtrPlugin;

typedef struct r_bin_ldr_plugin_t {
	char *name;
	char *desc;
	char *license;
	int (*init)(void *user);
	int (*fini)(void *user);
	bool (*load)(RBin *bin);
} RBinLdrPlugin;

typedef struct r_bin_plugin_t {
	char *name;
	char *desc;
	char *author;
	char *version;
	char *license;
	int (*init)(void *user);
	int (*fini)(void *user);
	Sdb * (*get_sdb)(RBinFile *obj);
	bool (*load)(RBinFile *arch);
	void *(*load_bytes)(RBinFile *bf, const ut8 *buf, ut64 sz, ut64 loadaddr, Sdb *sdb);
	void *(*load_buffer)(RBinFile *bf, RBuffer *buf, ut64 loadaddr, Sdb *sdb);
	ut64 (*size)(RBinFile *bin); // return ut64 maybe? meh
	int (*destroy)(RBinFile *arch);
	bool (*check_bytes)(const ut8 *buf, ut64 length);
	ut64 (*baddr)(RBinFile *arch);
	ut64 (*boffset)(RBinFile *arch);
	RBinAddr* (*binsym)(RBinFile *arch, int num);
	RList/*<RBinAddr>*/* (*entries)(RBinFile *arch);
	RList/*<RBinSection>*/* (*sections)(RBinFile *arch);
	RList/*<RBinDwarfRow>*/* (*lines)(RBinFile *arch);
	RList/*<RBinSymbol>*/* (*symbols)(RBinFile *arch);
	RList/*<RBinImport>*/* (*imports)(RBinFile *arch);
	RList/*<RBinString>*/* (*strings)(RBinFile *arch);
	RBinInfo/*<RBinInfo>*/* (*info)(RBinFile *arch);
	RList/*<RBinField>*/* (*fields)(RBinFile *arch);
	RList/*<char *>*/* (*libs)(RBinFile *arch);
	RList/*<RBinReloc>*/* (*relocs)(RBinFile *arch);
	RList/*<RBinClass>*/* (*classes)(RBinFile *arch);
	RList/*<RBinMem>*/* (*mem)(RBinFile *arch);
	RList/*<RBinReloc>*/* (*patch_relocs)(RBin *bin);
	RList/*<RBinMap>*/* (*maps)(RBinFile *arch);
	void (*header)(RBinFile *arch);
	char* (*signature)(RBinFile *arch, bool json);
	int (*demangle_type)(const char *str);
	struct r_bin_dbginfo_t *dbginfo;
	struct r_bin_write_t *write;
	int (*get_offset)(RBinFile *arch, int type, int idx);
	char* (*get_name)(RBinFile *arch, int type, int idx);
	ut64 (*get_vaddr)(RBinFile *arch, ut64 baddr, ut64 paddr, ut64 vaddr);
	RBuffer* (*create)(RBin *bin, const ut8 *code, int codelen, const ut8 *data, int datalen);
	char* (*demangle)(const char *str);
	char* (*regstate)(RBinFile *arch);
	int (*file_type)(RBinFile *arch);
	/* default value if not specified by user */
	int minstrlen;
	char strfilter;
	void *user;
} RBinPlugin;

typedef struct r_bin_section_t {
	char name[R_BIN_SIZEOF_STRINGS + 1]; // TODO: must be char*
	ut64 size;
	ut64 vsize;
	ut64 vaddr;
	ut64 paddr;
	ut32 srwx;
	// per section platform info
	const char *arch;
	char *format;
	int bits;
	bool has_strings;
	bool add; // indicates when you want to add the section to io `S` command
	bool is_data;
	bool is_segment;
} RBinSection;

typedef struct r_bin_class_t {
	char *name;
	// TODO: char *module;
	char *super;
	char *visibility_str;
	int index;
	ut64 addr;
	RList *methods; // <RBinSymbol>
	RList *fields; // <RBinField>
	int visibility;
} RBinClass;

#define RBinSectionName r_offsetof(RBinSection, name)
#define RBinSectionOffset r_offsetof(RBinSection, offset)

#define REBASE_PADDR(o, l, type_t)\
	do { \
		RListIter *_it;\
		type_t *_el;\
		r_list_foreach ((l), _it, _el) { \
			_el->paddr += (o)->loadaddr;\
		}\
	} while (0)


typedef struct r_bin_symbol_t {
	/* heap-allocated */
	char *name;
	char *dname;
	char *classname;
	/* const-unique-strings */
	const char *forwarder;
	const char *bind;
	const char *type;
  	const char *rtype;
	/* only used by java */
	const char *visibility_str;
	// ----------------
	//char descriptor[R_BIN_SIZEOF_STRINGS+1];
	ut64 vaddr;
	ut64 paddr;
	ut32 size;
	ut32 ordinal;
	ut32 visibility;
	int bits;
	/* see R_BIN_METH_* constants */
	ut64 method_flags;
	int dup_count;
} RBinSymbol;

typedef struct r_bin_import_t {
	char *name;
	const char *bind;
	const char *type;
	char *classname;
	char *descriptor;
	ut32 ordinal;
	ut32 visibility;
} RBinImport;

typedef struct r_bin_reloc_t {
	ut8 type;
	ut8 additive;
	RBinSymbol *symbol;
	RBinImport *import;
	st64 addend;
	ut64 vaddr;
	ut64 paddr;
	ut32 visibility;
	/* is_ifunc: indirect function, `addend` points to a resolver function
	 * that returns the actual relocation value, e.g. chooses
	 * an optimized version depending on the CPU.
	 * cf. https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
	 */
	bool is_ifunc;
} RBinReloc;

typedef struct r_bin_string_t {
	// TODO: rename string->name (avoid colisions)
	char *string;
	ut64 vaddr;
	ut64 paddr;
	ut32 ordinal;
	ut32 size; // size of buffer containing the string in bytes
	ut32 length; // length of string in chars
	char type; // Ascii Wide cp850 utf8 base64 ...
} RBinString;

typedef struct r_bin_field_t {
	ut64 vaddr;
	ut64 paddr;
	int size;
	ut32 visibility;
	char *name;
	char *type;
	char *comment;
	char *format;
	ut64 flags;
} RBinField;

R_API RBinField *r_bin_field_new(ut64 paddr, ut64 vaddr, int size, const char *name, const char *comment, const char *format);
R_API void r_bin_field_free(void *); //RBinField *field);

typedef struct r_bin_mem_t {	//new toy for esil-init
	char *name;
	ut64 addr;
	int size;
	int perms;
	RList *mirrors;		//for mirror access; stuff here should only create new maps not new fds
} RBinMem;

typedef struct r_bin_map_t {
	ut64 addr;
	ut64 offset;
	int size;
	int perms;
	char *file;
} RBinMap;

typedef struct r_bin_dbginfo_t {
	int (*get_line)(RBinFile *arch, ut64 addr, char *file, int len, int *line);
} RBinDbgInfo;

typedef struct r_bin_write_t {
	ut64 (*scn_resize)(RBinFile *arch, const char *name, ut64 size);
	bool (*scn_perms)(RBinFile *arch, const char *name, int perms);
	int (*rpath_del)(RBinFile *arch);
	bool (*entry)(RBinFile *arch, ut64 addr);
	bool (*addlib)(RBinFile *arch, const char *lib);
} RBinWrite;

// TODO: deprecate r_bin_is_big_endian
// TODO: has_dbg_syms... maybe flags?

typedef int (*RBinGetOffset)(RBin *bin, int type, int idx);
typedef const char *(*RBinGetName)(RBin *bin, int type, int idx);
typedef RList *(*RBinGetSections)(RBin *bin);
typedef RBinSection *(*RBinGetSectionAt)(RBin *bin, ut64 addr);

typedef struct r_bin_bind_t {
	RBin *bin;
	RBinGetOffset get_offset;
	RBinGetName get_name;
	RBinGetSections get_sections;
	RBinGetSectionAt get_vsect_at;
	ut32 visibility;
} RBinBind;

#ifdef R_API

/* fd api */

typedef struct r_bin_options_t {
	ut64 offset; // starting physical address to read from the target file
	ut64 baseaddr; // where the linker maps the binary in memory
	ut64 loadaddr; // the desired offset where the binary should be loaded
	ut64 size; // restrict the size of the target fd
	int xtr_idx; // load Nth binary
	int rawstr;
	int iofd;
	char *name; // or comment :?
} RBinOptions;

R_API RBinImport *r_bin_import_clone(RBinImport *o);
R_API RBinSymbol *r_bin_symbol_clone(RBinSymbol *o);

R_API RBinOptions *r_bin_options_new (ut64 offset, ut64 baddr, int rawstr);
R_API void r_bin_options_free(RBinOptions *bo);
R_API int r_bin_open(RBin *bin, const char *filename, RBinOptions *bo);
R_API RBinFile *r_bin_get_file (RBin *bin, int bd);
R_API bool r_bin_close(RBin *bin, int bd);
R_API bool r_bin_query(RBin *bin, const char *query);

/* load */
R_API int r_bin_load(RBin *bin, const char *file, ut64 baseaddr, ut64 loadaddr, int xtr_idx, int fd, int rawstr);
R_API int r_bin_load_as(RBin *bin, const char *file, ut64 baseaddr, ut64 loadaddr, int xtr_idx, int fd, int rawstr, int fileoffset, const char *name);
R_API int r_bin_load_io(RBin *bin, int fd, ut64 baseaddr, ut64 loadaddr, int xtr_idx);
R_API int r_bin_load_io_at_offset_as_sz(RBin *bin, int fd, ut64 baseaddr, ut64 loadaddr, int xtr_idx, ut64 offset, const char *name, ut64 sz);

/* bin.c */
R_API void r_bin_load_filter(RBin *bin, ut64 rules);
R_API int r_bin_load_languages(RBinFile *binfile);

R_API int r_bin_reload(RBin *bin, int fd, ut64 baseaddr);
R_API void r_bin_bind(RBin *b, RBinBind *bnd);
R_API bool r_bin_add(RBin *bin, RBinPlugin *foo);
R_API bool r_bin_xtr_add(RBin *bin, RBinXtrPlugin *foo);
R_API bool r_bin_ldr_add(RBin *bin, RBinLdrPlugin *foo);
R_API void* r_bin_free(RBin *bin);
R_API RList *r_bin_raw_strings(RBinFile *a, int min);
R_API int r_bin_dump_strings(RBinFile *a, int min, int raw);
//io-wrappers
R_API int r_bin_read_at (RBin *bin, ut64 addr, ut8 *buf, int size);
R_API int r_bin_write_at (RBin *bin, ut64 addr, const ut8 *buf, int size);

/* file.c */
R_API RBinFile *r_bin_file_new(RBin *bin, const char *file, const ut8 *bytes, ut64 sz, ut64 file_sz, int rawstr, int fd, const char *xtrname, Sdb *sdb, bool steal_ptr);
R_API bool r_bin_file_object_new_from_xtr_data(RBin *bin, RBinFile *bf, ut64 baseaddr, ut64 loadaddr, RBinXtrData *data);
R_API RBinFile *r_bin_file_new_from_bytes(RBin *bin, const char *file, const ut8 *bytes, ut64 sz, ut64 file_sz, int rawstr, ut64 baseaddr, ut64 loadaddr, int fd, const char *pluginname, const char *xtrname, ut64 offset, bool steal_ptr);
R_API RBinFile *r_bin_file_new_from_fd(RBin *bin, int fd, RBinFileOptions *options);
R_API RBinFile *r_bin_file_find_by_arch_bits(RBin *bin, const char *arch, int bits, const char *name);
R_API RBinObject *r_bin_file_object_find_by_id(RBinFile *binfile, ut32 binobj_id);
R_API RList *r_bin_file_get_strings(RBinFile *a, int min, int dump, int raw);
R_API void r_bin_file_get_strings_range(RBinFile *bf, RList *list, int min, int raw, ut64 from, ut64 to);
R_API RBinFile *r_bin_file_find_by_object_id(RBin *bin, ut32 binobj_id);
R_API RBinFile *r_bin_file_find_by_id(RBin *bin, ut32 binfile_id);
R_API int r_bin_file_object_add(RBinFile *binfile, RBinObject *o);
R_API int r_bin_file_delete_all(RBin *bin);
R_API int r_bin_file_delete(RBin *bin, ut32 bin_fd);
R_API RBinFile *r_bin_file_find_by_fd(RBin *bin, ut32 bin_fd);
R_API RBinFile *r_bin_file_find_by_name(RBin *bin, const char *name);
R_API RBinFile *r_bin_file_find_by_name_n(RBin *bin, const char *name, int idx);
R_API bool r_bin_file_set_cur_binfile_obj(RBin *bin, RBinFile *bf, RBinObject *obj);
R_API int r_bin_file_set_cur_binfile(RBin *bin, RBinFile *bf);
R_API int r_bin_file_set_cur_by_name(RBin *bin, const char *name);
R_API RBinObject *r_bin_file_object_get_cur(RBinFile *binfile);
R_API int r_bin_file_cur_set_plugin(RBinFile *binfile, RBinPlugin *plugin);
R_API int r_bin_file_ref(RBin *bin, RBinFile *a);
R_API void r_bin_file_free(void /*RBinFile*/ *bf_);
R_API RBinFile *r_bin_file_create_append(RBin *bin, const char *file, const ut8 *bytes, ut64 sz, ut64 file_sz, int rawstr, int fd, const char *xtrname, bool steal_ptr);
R_API RBinFile *r_bin_file_xtr_load_bytes(RBin *bin, RBinXtrPlugin *xtr, const char *filename, const ut8 *bytes, ut64 sz, ut64 file_sz, ut64 baseaddr, ut64 loadaddr, int idx, int fd, int rawstr);
R_API bool r_bin_file_set_bytes(RBinFile *binfile, const ut8 *bytes, ut64 sz, bool steal_ptr);
R_API RBinPlugin *r_bin_file_cur_plugin(RBinFile *binfile);

/* obj.c */
R_API void r_bin_object_free(void /*RBinObject*/ *o_);
R_API ut64 r_bin_object_get_baddr(RBinObject *o);
R_API void r_bin_object_filter_strings (RBinObject *bo);
R_API void r_bin_object_set_baddr(RBinObject *o, ut64 baddr);
R_API RBinObject *r_bin_object_new(RBinFile *binfile, RBinPlugin *plugin, ut64 baseaddr, ut64 loadaddr, ut64 offset, ut64 sz);
R_API int r_bin_object_set_items(RBinFile *binfile, RBinObject *o);
R_API RBinObject *r_bin_object_get_cur(RBin *bin);
R_API RBinObject *r_bin_object_find_by_arch_bits(RBinFile *binfile, const char *arch, int bits, const char *name);
R_API int r_bin_object_delete(RBin *bin, ut32 binfile_id, ut32 binobj_id);
R_API void r_bin_object_delete_items(RBinObject *o);

// ref
R_API int r_bin_file_deref_by_bind (RBinBind * binb);
R_API int r_bin_file_deref (RBin *bin, RBinFile * a);
R_API int r_bin_file_ref_by_bind (RBinBind * binb);
R_API int r_bin_file_ref (RBin *bin, RBinFile * a);
R_API bool r_bin_file_object_new_from_xtr_data(RBin *bin, RBinFile *bf, ut64 baseaddr, ut64 loadaddr,

#define MODE_PRINT 0x000
#define MODE_RADARE 0x001
#define MODE_SIMPLE 0x004

						RBinXtrData *xtr_data);
R_API int r_bin_list(RBin *bin, int json);
R_API int r_bin_list_plugin(RBin *bin, const char* name, int json);
R_API RBinObject *r_bin_get_object(RBin *bin);
R_API ut64 r_binfile_get_baddr (RBinFile *binfile);
R_API ut64 r_bin_get_baddr(RBin *bin);
R_API void r_bin_set_baddr(RBin *bin, ut64 baddr);
R_API ut64 r_bin_get_laddr(RBin *bin);
R_API ut64 r_bin_get_boffset(RBin *bin);
R_API RBinAddr* r_bin_get_sym(RBin *bin, int sym);
R_API const char *r_bin_entry_type_string(int etype);

R_API RBinXtrPlugin *r_bin_get_xtrplugin_by_name(RBin *bin, const char *name);
R_API RBinPlugin *r_bin_get_binplugin_by_name(RBin *bin, const char *name);
R_API RBinPlugin *r_bin_get_binplugin_any(RBin *bin);

R_API char* r_bin_demangle(RBinFile *binfile, const char *lang, const char *str, ut64 vaddr);
R_API int r_bin_demangle_type (const char *str);
R_API char *r_bin_demangle_java(const char *str);
R_API char *r_bin_demangle_cxx(RBinFile *binfile, const char *str, ut64 vaddr);
R_API char *r_bin_demangle_msvc(const char *str);
R_API char *r_bin_demangle_swift(const char *s, bool syscmd);
R_API char *r_bin_demangle_objc(RBinFile *binfile, const char *sym);
R_API char *r_bin_demangle_rust(RBinFile *binfile, const char *str, ut64 vaddr);
R_API int r_bin_lang_type(RBinFile *binfile, const char *def, const char *sym);
R_API bool r_bin_lang_objc(RBinFile *binfile);
R_API bool r_bin_lang_swift(RBinFile *binfile);
R_API bool r_bin_lang_cxx(RBinFile *binfile);
R_API bool r_bin_lang_msvc(RBinFile *binfile);
R_API bool r_bin_lang_dlang(RBinFile *binfile);
R_API bool r_bin_lang_rust(RBinFile *binfile);
R_API const char *r_bin_get_meth_flag_string(ut64 flag, bool compact);

R_API RList* r_bin_get_entries(RBin *bin);
R_API RList* r_bin_get_fields(RBin *bin);
R_API RList* r_bin_get_imports(RBin *bin);
R_API RBinInfo* r_bin_get_info(RBin *bin);
R_API RList* r_bin_get_libs(RBin *bin);
R_API ut64 r_bin_get_size (RBin *bin);
R_API RList* r_bin_patch_relocs(RBin *bin);
R_API RList* r_bin_get_relocs(RBin *bin);
R_API RList* r_bin_get_sections(RBin *bin);
R_API RList* /*<RBinClass>*/r_bin_get_classes(RBin *bin);

// TODO: rename to r_bin_file_get_class() etc
R_API RBinClass *r_bin_class_get (RBinFile *binfile, const char *name);
R_API RBinClass *r_bin_class_new (RBinFile *binfile, const char *name, const char *super, int view);
R_API void r_bin_class_free(RBinClass *c);
R_API RBinSymbol *r_bin_class_add_method (RBinFile *binfile, const char *classname, const char *name, int nargs);
R_API void r_bin_class_add_field (RBinFile *binfile, const char *classname, const char *name);
R_API RList *r_bin_classes_from_symbols (RBinFile *bf, RBinObject *o);

R_API RBinSection* r_bin_get_section_at(RBinObject *o, ut64 off, int va);
R_API RList* r_bin_get_strings(RBin *bin);
R_API int r_bin_is_string(RBin *bin, ut64 va);
R_API RList* r_bin_reset_strings(RBin *bin);
R_API RList* r_bin_get_symbols(RBin *bin);
R_API RBinSymbol *r_bin_get_symbol_at_vaddr(RBin *bin, ut64 addr);
R_API RBinSymbol *r_bin_get_symbol_at_paddr(RBin *bin, ut64 addr);
R_API int r_bin_is_big_endian(RBin *bin);
R_API int r_bin_is_stripped(RBin *bin);
R_API int r_bin_is_static(RBin *bin);
R_API int r_bin_has_dbg_linenums(RBin *bin);
R_API int r_bin_has_dbg_syms(RBin *bin);
R_API int r_bin_has_dbg_relocs(RBin *bin);
R_API RBin* r_bin_new(void);
R_API void r_bin_iobind(RBin *bin, RIO *io);
R_API RBinFile * r_bin_cur(RBin *bin);
R_API RBinObject * r_bin_cur_object(RBin *bin);
R_API int r_bin_io_load(RBin *bin, RIO *io, int fd, ut64 baseaddr, ut64 loadaddr, int dummy);

R_API int r_bin_select(RBin *bin, const char *arch, int bits, const char *name);
R_API int r_bin_select_idx(RBin *bin, const char *name, int idx);
R_API int r_bin_select_by_ids(RBin *bin, ut32 binfile_id, ut32 binobj_id);
R_API int r_bin_use_arch(RBin *bin, const char *arch, int bits, const char *name);
R_API RBinFile * r_bin_file_find_by_arch_bits(RBin *bin, const char *arch, int bits, const char *name);
R_API void r_bin_list_archs(RBin *bin, int mode);
R_API void r_bin_set_user_ptr(RBin *bin, void *user);
R_API RBuffer *r_bin_create (RBin *bin, const ut8 *code, int codelen, const ut8 *data, int datalen);
R_API RBuffer *r_bin_package (RBin *bin, const char *type, const char *file, RList *files);
R_API ut64 r_bin_get_vaddr (RBin *bin, ut64 paddr, ut64 vaddr);
R_API ut64 r_bin_a2b (RBin *bin, ut64 addr);
R_API int r_bin_file_delete(RBin *bin, ut32 bin_fd);
R_API int r_bin_file_delete_all(RBin *bin);
R_API int r_bin_file_set_cur_by_fd (RBin *bin, ut32 bin_fd);
R_API RBinFile * r_bin_file_find_by_fd (RBin *bin, ut32 bin_fd);
R_API RBinFile * r_bin_file_find_by_name (RBin * bin, const char * name);
R_API RBinFile * r_bin_file_find_by_name_n (RBin * bin, const char * name, int idx);
R_API RBinPlugin * r_bin_file_cur_plugin (RBinFile *binfile);
R_API void r_bin_force_plugin (RBin *bin, const char *pname);
R_API const char *r_bin_string_type (int type);

/* dbginfo.c */
R_API int r_bin_addr2line(RBin *bin, ut64 addr, char *file, int len, int *line);
R_API char *r_bin_addr2text(RBin *bin, ut64 addr, int origin);
R_API char *r_bin_addr2fileline(RBin *bin, ut64 addr);
/* bin_write.c */
R_API bool r_bin_wr_addlib(RBin *bin, const char *lib);
R_API ut64 r_bin_wr_scn_resize(RBin *bin, const char *name, ut64 size);
R_API bool r_bin_wr_scn_perms(RBin *bin, const char *name, int perms);
R_API bool r_bin_wr_rpath_del(RBin *bin);
R_API bool r_bin_wr_entry(RBin *bin, ut64 addr);
R_API bool r_bin_wr_output(RBin *bin, const char *filename);
R_API int r_bin_dwarf_parse_info(RBinDwarfDebugAbbrev *da, RBin *a, int mode);
R_API RList *r_bin_dwarf_parse_line(RBin *a, int mode);
R_API RList *r_bin_dwarf_parse_aranges(RBin *a, int mode);
R_API RBinDwarfDebugAbbrev *r_bin_dwarf_parse_abbrev(RBin *a, int mode);

R_API RBinPlugin * r_bin_get_binplugin_by_bytes (RBin *bin, const ut8* bytes, ut64 sz);

R_API void r_bin_demangle_list(RBin *bin);
R_API char *r_bin_demangle_plugin(RBin *bin, const char *name, const char *str);

R_API RList *r_bin_get_mem (RBin *bin);

/* filter.c */
R_API void r_bin_filter_name(Sdb *db, ut64 addr, char *name, int maxlen);
R_API void r_bin_filter_symbols (RList *list);
R_API void r_bin_filter_sections (RList *list);
R_API void r_bin_filter_classes (RList *list);
R_API bool r_bin_strpurge(RBin *bin, const char *str, ut64 addr);
R_API bool r_bin_string_filter(RBin *bin, const char *str, ut64 addr);

/* plugin pointers */
extern RBinPlugin r_bin_plugin_any;
extern RBinPlugin r_bin_plugin_fs;
extern RBinPlugin r_bin_plugin_cgc;
extern RBinPlugin r_bin_plugin_elf;
extern RBinPlugin r_bin_plugin_elf64;
extern RBinPlugin r_bin_plugin_p9;
extern RBinPlugin r_bin_plugin_pe;
extern RBinPlugin r_bin_plugin_mz;
extern RBinPlugin r_bin_plugin_pe64;
extern RBinPlugin r_bin_plugin_pebble;
extern RBinPlugin r_bin_plugin_bios;
extern RBinPlugin r_bin_plugin_bf;
extern RBinPlugin r_bin_plugin_te;
extern RBinPlugin r_bin_plugin_mach0;
extern RBinPlugin r_bin_plugin_mach064;
extern RBinPlugin r_bin_plugin_mdmp;
extern RBinPlugin r_bin_plugin_java;
extern RBinPlugin r_bin_plugin_dex;
extern RBinPlugin r_bin_plugin_dummy;
extern RBinPlugin r_bin_plugin_coff;
extern RBinPlugin r_bin_plugin_ningb;
extern RBinPlugin r_bin_plugin_ningba;
extern RBinPlugin r_bin_plugin_ninds;
extern RBinPlugin r_bin_plugin_nin3ds;
extern RBinPlugin r_bin_plugin_xbe;
extern RBinPlugin r_bin_plugin_bflt;
extern RBinXtrPlugin r_bin_xtr_plugin_xtr_fatmach0;
extern RBinXtrPlugin r_bin_xtr_plugin_xtr_dyldcache;
extern RBinLdrPlugin r_bin_ldr_plugin_ldr_linux;
extern RBinPlugin r_bin_plugin_zimg;
extern RBinPlugin r_bin_plugin_omf;
extern RBinPlugin r_bin_plugin_art;
extern RBinPlugin r_bin_plugin_bootimg;
extern RBinPlugin r_bin_plugin_dol;
extern RBinPlugin r_bin_plugin_nes;
extern RBinPlugin r_bin_plugin_mbn;
extern RBinPlugin r_bin_plugin_smd;
extern RBinPlugin r_bin_plugin_sms;
extern RBinPlugin r_bin_plugin_psxexe;
extern RBinPlugin r_bin_plugin_spc700;
extern RBinPlugin r_bin_plugin_vsf;
extern RBinPlugin r_bin_plugin_dyldcache;
extern RBinPlugin r_bin_plugin_avr;
extern RBinPlugin r_bin_plugin_menuet;
extern RBinPlugin r_bin_plugin_wasm;
extern RBinPlugin r_bin_plugin_nro;
extern RBinPlugin r_bin_plugin_nso;
extern RBinPlugin r_bin_plugin_sfc;
extern RBinPlugin r_bin_plugin_z64;

#ifdef __cplusplus
}
#endif

#endif
#endif
