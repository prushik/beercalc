COMMON_SRC = src/menu.c src/recipe.c src/mash.c src/ibu.c src/yeast.c
DEBUG ?= -DDEBUG -g -Og
CFLAGS ?= -Wno-implicit-function-declaration -I./include
LDFLAGS ?= -lm -lsqlite3
CC ?= gcc

WEBROOT ?= /var/unweave
WEBUSER ?= prushik
PAGES ?= recipe_from_db index builder_ajax builder_ui
DATABASE ?= /var/db/beer.sqlite

all: $(PAGES)

index: $(COMMON_SRC) src/index.c
	$(CC) $^ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

builder_ajax: $(COMMON_SRC) src/builder_ajax.c
	$(CC) $^ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

builder_ui: $(COMMON_SRC) src/builder_ui.c
	$(CC) $^ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

recipe_from_db: $(COMMON_SRC) src/recipe_from_database.c
	$(CC) $^ $(LDFLAGS) $(DEBUG) $(CFLAGS) -o $@

deploy:
	mkdir -p $(WEBROOT)/include
	install -m 0555 -o $(WEBUSER) include/global.css $(WEBROOT)/include
	install -m 0777 -o $(WEBUSER) $(PAGES) $(WEBROOT)

deploy_database:
	sqlite3 $(DATABASE) < db/default.sql
	sqlite3 $(DATABASE) < db/styles.sql

clean:
	rm $(PAGES)

clean_database:
	rm $(DATABASE)

.PHONY: all deploy clean clean_database install_database
