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

TEMPLATE = app

QT       += core gui

TARGET = osk

LIBS += User32.lib

#CONFIG(debug) {
#    LIBS += ../jansson/Debug/jansson.lib
#} else {
    LIBS += ../jansson/Release/jansson.lib
#}

INCLUDEPATH += ../jansson/jansson/src ../jansson

RC_FILE = osk.rc

SOURCES += main.cpp\
    onscreenkeyboard.cpp \
    onscreenbutton.cpp \
    application.cpp

HEADERS  += \
    onscreenkeyboard.h \
    onscreenbutton.h \
    application.h

OTHER_FILES += osk.rc
