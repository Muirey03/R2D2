#ifndef R2_CONFIGURE_H
#define R2_CONFIGURE_H

#include "r_version.h"

#define DEBUGGER 1
#define HAVE_DECL_ADDR_NO_RANDOMIZE 0
#define HAVE_ARC4RANDOM_UNIFORM 1
#define HAVE_EXPLICIT_BZERO 0

#ifdef R_MESON_VERSION
#define R2_PREFIX "/usr"
#define R2_LIBDIR "/usr/lib"
#define R2_INCDIR "/usr/include"
#define R2_DATDIR "/usr/share"
#define R2_WWWROOT "@WWWROOT@"
#define R2_PLUGINS "@PLUGINS@"
#define R2_EXTRAS "@EXTRAS@"
#define R2_BINDINGS "@BINDINGS@"
#define R2_SDB "@SDB@"
#define R2_ZIGNS "@ZIGNS@"
#define R2_THEMES "@THEMES@"
#define R2_FORTUNES "@FORTUNES@"
#define R2_FLAGS "@FLAGS@"
#define R2_HUD "@HUD@"
#else
#if __WINDOWS__ || __CYGWIN__ || MINGW32 || _MSC_VER || CUTTER
#define R2_PREFIX "."
#define R2_LIBDIR "lib"
#define R2_INCDIR "include\\libr"
#define R2_DATDIR "share"
#define R2_WWWROOT "www"
#define R2_PLUGINS "lib\\plugins"
#define R2_EXTRAS "lib\\extras"
#define R2_BINDINGS "lib\\bindings"
#else
#define R2_PREFIX "/usr"
#define R2_LIBDIR "/usr/lib"
#define R2_INCDIR "/usr/include/libr"
#define R2_DATDIR "/usr/share"
#define R2_WWWROOT R2_DATDIR "/radare2/" R2_VERSION "/www"
#define R2_PLUGINS "lib/radare2/" R2_VERSION
#define R2_EXTRAS "lib/radare2-extras/" R2_VERSION
#define R2_BINDINGS "lib/radare2-bindings/" R2_VERSION
#endif
#define R2_SDB              R_JOIN_3_PATHS ("share", "radare2", R2_VERSION)
#define R2_ZIGNS            R_JOIN_4_PATHS ("share", "radare2", R2_VERSION, "zigns")
#define R2_THEMES           R_JOIN_4_PATHS ("share", "radare2", R2_VERSION, "cons")
#define R2_FLAGS            R_JOIN_4_PATHS ("share", "radare2", R2_VERSION, "flag")
#define R2_FORTUNES         R_JOIN_3_PATHS ("share", "doc", "radare2")
#define R2_HUD              R_JOIN_4_PATHS ("share", "radare2", R2_VERSION, "hud")
#endif

#define R2_SDB_FCNSIGN      R_JOIN_2_PATHS (R2_SDB, "fcnsign")
#define R2_SDB_OPCODES      R_JOIN_2_PATHS (R2_SDB, "opcodes")
#define R2_SDB_MAGIC        R_JOIN_2_PATHS (R2_SDB, "magic")
#define R2_SDB_FORMAT       R_JOIN_2_PATHS (R2_SDB, "format")

#define R2_HOME_CONFIGDIR   R_JOIN_2_PATHS (".config", "radare2")
#define R2_HOME_DATADIR     R_JOIN_3_PATHS (".local", "share", "radare2")
#define R2_HOME_CACHEDIR    R_JOIN_2_PATHS (".cache", "radare2")

#define R2_HOME_THEMES      R_JOIN_2_PATHS (R2_HOME_DATADIR, "cons")
#define R2_HOME_PLUGINS     R_JOIN_2_PATHS (R2_HOME_DATADIR, "plugins")
#define R2_HOME_ZIGNS       R_JOIN_2_PATHS (R2_HOME_DATADIR, "zigns")
#define R2_HOME_PDB         R_JOIN_2_PATHS (R2_HOME_DATADIR, "pdb")
#define R2_HOME_PROJECTS    R_JOIN_2_PATHS (R2_HOME_DATADIR, "projects")
#define R2_HOME_WWWROOT     R_JOIN_2_PATHS (R2_HOME_DATADIR, "www")
#define R2_HOME_SDB_FORMAT  R_JOIN_2_PATHS (R2_HOME_DATADIR, "format")
#define R2_HOME_HUD         R_JOIN_2_PATHS (R2_HOME_DATADIR, "hud")
#define R2_HOME_BINRC       R_JOIN_2_PATHS (R2_HOME_DATADIR, "rc.d")
#define R2_HOME_BIN         R_JOIN_3_PATHS (R2_HOME_DATADIR, "prefix", "bin")

#define R2_HOME_RC          R_JOIN_2_PATHS (R2_HOME_CONFIGDIR, "radare2rc")
#define R2_HOME_RC_DIR      R_JOIN_2_PATHS (R2_HOME_CONFIGDIR, "radare2rc.d")

#define R2_HOME_HISTORY     R_JOIN_2_PATHS (R2_HOME_CACHEDIR, "history")

#define HAVE_LIB_MAGIC 0
#define USE_LIB_MAGIC 0
#ifndef HAVE_LIB_SSL
#define HAVE_LIB_SSL 0
#endif

#define HAVE_FORK 1

#define WITH_GPL 1

#if __APPLE__ && __POWERPC__
#define HAVE_JEMALLOC 0
#else
#define HAVE_JEMALLOC 1
#endif

#endif
