QT += core

CONFIG += c++11

TARGET = re-typeset-src-unit-tests
CONFIG += console
CONFIG -= app_bundle

DEFINES += RE_TYPESET_PATCH

TEMPLATE = app

SOURCES += \ 
    src/gmock-all.cc \
    src/gmock_main.cc \
    src/gtest-all.cc \
    ../re-typeset-src/pixelfont/letters.cpp \
    ../re-typeset-src/pixelfont/letters-test.cpp \
    ../re-typeset-src/pixelfont/letterswriter.cpp \
    ../re-typeset-src/pixelfont/letterswriter-test.cpp \
    ../re-typeset-src/pixelfont/paintdeviceqimage.cpp \
    ../re-typeset-src/pixelfont/paintdeviceqimage-test.cpp

HEADERS += \
    gmock/internal/custom/gmock-generated-actions.h \
    gmock/internal/custom/gmock-generated-actions.h.pump \
    gmock/internal/custom/gmock-matchers.h \
    gmock/internal/custom/gmock-port.h \
    gmock/internal/gmock-generated-internal-utils.h \
    gmock/internal/gmock-generated-internal-utils.h.pump \
    gmock/internal/gmock-internal-utils.h \
    gmock/internal/gmock-port.h \
    gmock/gmock-actions.h \
    gmock/gmock-cardinalities.h \
    gmock/gmock-generated-actions.h \
    gmock/gmock-generated-actions.h.pump \
    gmock/gmock-generated-function-mockers.h \
    gmock/gmock-generated-function-mockers.h.pump \
    gmock/gmock-generated-matchers.h \
    gmock/gmock-generated-matchers.h.pump \
    gmock/gmock-generated-nice-strict.h \
    gmock/gmock-matchers.h \
    gmock/gmock-more-actions.h \
    gmock/gmock-more-matchers.h \
    gmock/gmock-spec-builders.h \
    gmock/gmock.h \
    gtest/internal/custom/gtest-port.h \
    gtest/internal/custom/gtest-printers.h \
    gtest/internal/custom/gtest.h \
    gtest/internal/gtest-death-test-internal.h \
    gtest/internal/gtest-filepath.h \
    gtest/internal/gtest-internal.h \
    gtest/internal/gtest-linked_ptr.h \
    gtest/internal/gtest-param-util-generated.h \
    gtest/internal/gtest-param-util.h \
    gtest/internal/gtest-port-arch.h \
    gtest/internal/gtest-port.h \
    gtest/internal/gtest-string.h \
    gtest/internal/gtest-tuple.h \
    gtest/internal/gtest-tuple.h.pump \
    gtest/internal/gtest-type-util.h \
    gtest/gtest-death-test.h \
    gtest/gtest-message.h \
    gtest/gtest-param-test.h \
    gtest/gtest-param-test.h.pump \
    gtest/gtest-printers.h \
    gtest/gtest-spi.h \
    gtest/gtest-test-part.h \
    gtest/gtest-typed-test.h \
    gtest/gtest.h \
    gtest/gtest_pred_impl.h \
    gtest/gtest_prod.h \
    src/gtest-internal-inl.h \
    ../re-typeset-src/pixelfont/letters.hpp \
    ../re-typeset-src/pixelfont/letterswriter.hpp \
    ../re-typeset-src/pixelfont/paintdeviceqimage.hpp

