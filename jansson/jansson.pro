# Name: On-Screen Keyboard for Windows
# Author: Dmitriy Purgin <dmitriy.purgin@ibsproject.kz>
# Organization: IBS Project LLC
#
# Copyright (C) 2014 IBS Project LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
