#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"
#include "yeast.h"
#include <sqlite3.h>

int main(int argc, char **argv)
{
	int i;
	unsigned long int beer_id = 0;
	struct recipe beer = {0};
	char buffer[256];
	unsigned int buf_len = 0;

	sqlite3 *db;
	sqlite3_stmt *qry;

	sqlite3_open(DATABASE, &db);

	sqlite3_prepare_v2(db, str("select recipe.name, style.name, recipe.id from recipe left join style on style.id = recipe.style_id order by style.id;"), &qry, NULL);

	write(1, str(
		"<html>\n"
		"<head>\n"
		"<title>beercalc</title>\n"
		"<link rel=\"stylesheet\" href=\"include/global.css\">\n"
		"</head>\n"
		"<body>\n"
	));
	show_menu(0);
	write(1, str(
		"<div class=\"main_content\">\n"
		"<div class=\"title\">beercalc</div>\n<br>\n"
		"Welcome to beercalc, the open source homebrew beer recipe planning tool.<br><br>"
		"To get started, either choose \"Recipe Builder\" from the menu on the right, or select a previously created recipe from the list below.<br><br>"
		"Beer recipes: "
		"<form action=\"recipe_from_db\" method=\"get\">"
		"<select name=\"beer_id\" id=\"beer_id\"><option value=-1>select a beer recipe</option>\n"
	));
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		sprintf(buffer, "<option value=%d>%s (%s)</option>%n", sqlite3_column_int(qry, 2), sqlite3_column_text(qry, 0), sqlite3_column_text(qry, 1), &buf_len);
		write(1, buffer, buf_len);
	}
	sqlite3_finalize(qry);

	write(1, str(
		"</select>"
		"<input type=\"submit\" name=\"edit\" value=\"edit\">"
		"<input type=\"submit\" name=\"brew\" value=\"brew\">\n"
	));

	write(1, str(
		"</div>"
		"</body>"
		"</html>\n"
	));

	sqlite3_close(db);
}
