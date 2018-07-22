COMMON_SRC = src/menu.c src/recipe.c src/mash.c src/ibu.c src/yeast.c
DEBUG ?= -DDEBUG -g -Og
CFLAGS ?= -Wno-implicit-function-declaration -I./include
LDFLAGS ?= -lm -lsqlite3
CC ?= gcc

WEBROOT ?= /var/unweave
WEBUSER ?= prushik
PAGES ?= recipe_from_db

all: $(PAGES)

recipe_from_db: $(COMMON_SRC) src/recipe_from_database.c
	$(CC) $^ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

deploy:
	mkdir -p $(WEBROOT)/include
	install -m 555 -o $(WEBUSER) include/global.css $(WEBROOT)/include
	install -m 777 -o $(WEBUSER) $(PAGES) $(WEBROOT)

clean:
	rm $(PAGES)

.PHONY: all deploy clean
