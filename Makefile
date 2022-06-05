SRCDIR := src
OBJDIR := obj
BINDIR := bin

CC := g++

IFLAGS = -Iinclude
CFLAGS = -Wextra -Wall -std=gnu++17 -MMD -O2 -DNDEBUG
#CFLAGS = -Wextra -Wall -std=gnu++17 -MMD -O2 -DNDEBUG -fsanitize=address
#LIBB = -Llib
LFLAGS = -lcurl
#LFLAGS = -lcurl -fsanitize=address

SRCFILES := $(shell find ${SRCDIR} -name '*.cpp')
SRCWITHOUTBIN := $(shell find ${SRCDIR} -name '*.cpp' -not -name 'main.cpp')
BINSRCFILES := $(shell find ${SRCDIR} -name 'main.cpp')
BINFILES := ${BINSRCFILES:${SRCDIR}%/main.cpp=${BINDIR}%}

MAINSRCFILE := $(shell find ${SRCDIR} -name 'main.cpp')
MAINBINFILE := ${MAINSRCFILE:${SRCDIR}%/main.cpp=${BINDIR}%}


.PHONY: all build clean rebuild mrproper
all: build

build: ${MAINBINFILE}
-include $(shell find ${OBJDIR} -name '*.d')

%.o:
	$(shell mkdir -p $(shell dirname $@))
	${CC} -o $@ -c $(@:${OBJDIR}/%.o=%.cpp) ${IFLAGS} ${CFLAGS}

${BINDIR}/%: ${OBJDIR}/${SRCDIR}/%/main.o $(SRCWITHOUTBIN:%.cpp=${OBJDIR}/%.o)
	$(shell mkdir -p $(shell dirname $@))
	${CC} ${LIBB} -o $@ $^ ${LFLAGS}

clean:
	rm -rf ${OBJDIR}/*

mrproper: clean
	rm -rf ${BINDIR}/*

rebuild: mrproper build
