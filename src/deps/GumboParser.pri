
QT *= core network
CONFIG *= c++11


INCLUDEPATH *= \
    $$PWD/gumbo-parser/include/ \
    $$PWD/QGumboParser/include/


HEADERS *= \
    $$PWD/gumbo-parser/include/attribute.h \
    $$PWD/gumbo-parser/include/char_ref.h \
    $$PWD/gumbo-parser/include/char_ref.rl \
    $$PWD/gumbo-parser/include/error.h \
    $$PWD/gumbo-parser/include/gumbo.h \
    $$PWD/gumbo-parser/include/gumbo.hpp \
    $$PWD/gumbo-parser/include/insertion_mode.h \
    $$PWD/gumbo-parser/include/parser.h \
    $$PWD/gumbo-parser/include/string_buffer.h \
    $$PWD/gumbo-parser/include/string_piece.h \
    $$PWD/gumbo-parser/include/tag_enum.h \
    $$PWD/gumbo-parser/include/tag_gperf.h \
    $$PWD/gumbo-parser/include/tag_sizes.h \
    $$PWD/gumbo-parser/include/tag_strings.h \
    $$PWD/gumbo-parser/include/token_type.h \
    $$PWD/gumbo-parser/include/tokenizer_states.h \
    $$PWD/gumbo-parser/include/tokenizer.h \
    $$PWD/gumbo-parser/include/utf8.h \
    $$PWD/gumbo-parser/include/util.h \
    $$PWD/gumbo-parser/include/vector.h \
    $$PWD/QGumboParser/include/HtmlTag.h \
    $$PWD/QGumboParser/include/qgumboattribute.h \
    $$PWD/QGumboParser/include/qgumbodocument.h \
    $$PWD/QGumboParser/include/qgumbonode.h

SOURCES += \
    $$PWD/gumbo-parser/src/attribute.c \
    $$PWD/gumbo-parser/src/char_ref.c \
    $$PWD/gumbo-parser/src/error.c \
    $$PWD/gumbo-parser/src/parser.c \
    $$PWD/gumbo-parser/src/string_buffer.c \
    $$PWD/gumbo-parser/src/string_piece.c \
    $$PWD/gumbo-parser/src/tag.c \
    $$PWD/gumbo-parser/src/tokenizer.c \
    $$PWD/gumbo-parser/src/utf8.c \
    $$PWD/gumbo-parser/src/util.c \
    $$PWD/gumbo-parser/src/vector.c \
    $$PWD/QGumboParser/src/qgumboattribute.cpp \
    $$PWD/QGumboParser/src/qgumbodocument.cpp \
    $$PWD/QGumboParser/src/qgumbonode.cpp
