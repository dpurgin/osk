TEMPLATE = lib

TARGET = jansson

DEFINES += HAVE_STDINT_H

DEF_FILE = jansson/src/jansson.def

SOURCES += \
    jansson/src/dump.c \
    jansson/src/error.c \
    jansson/src/hashtable.c \
    jansson/src/hashtable_seed.c \
    jansson/src/load.c \
    jansson/src/memory.c \
    jansson/src/pack_unpack.c \
    jansson/src/strbuffer.c \
    jansson/src/strconv.c \
    jansson/src/utf.c \
    jansson/src/value.c

HEADERS += \
    jansson_config.h \
    jansson/src/hashtable.h \
    jansson/src/jansson.h \
    jansson/src/jansson_private.h \
    jansson/src/lookup3.h \
    jansson/src/strbuffer.h \
    jansson/src/utf.h
