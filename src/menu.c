#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "menu.h"

static char menu_links[MENU_OPTIONS][46] = {
	"index",
	"builder",
	"http://prushik.dedyn.io:8083/prushik/beercalc"
};
static char menu_names[MENU_OPTIONS][15] = {
	"index",
	"recipe builder",
	"source"
};

void show_menu(unsigned int current)
{
	int i;
	write(1, str(
		"<table class=\"nav\">"
	));

	for (i = 0; i < MENU_OPTIONS; i++)
	{
		write(1, str(
			"<tr><td>"
			"<a href=\""
		));
		write(1, menu_links[i], strlen(menu_links[i]));
		write(1, str(
			"\">"
		));
		write(1, menu_names[i], strlen(menu_names[i]));
		write(1, str(
			"</a>"
			"</td></tr>\n"
		));
	}

	write(1, str(
		"</table>\n"
	));
}
