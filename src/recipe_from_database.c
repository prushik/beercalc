#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"
#include <sqlite3.h>

int main(int argc, char **argv)
{
	int i;
	unsigned long int beer_id = 0;
	struct recipe beer = {0};
	char *buffer;

	for (i=1; i<argc; i++)
	{
		if (strncmp(argv[i], str("beer_id")) == 0)
		{
			beer_id = strtoul(&argv[i][8], NULL, 0);
		}
	}

	sqlite3 *db;
	sqlite3_stmt *qry;

	sqlite3_open("/var/db/beer.sqlite", &db);


	sqlite3_prepare_v2(db, str("select name, author, volume, malt_n, hops_n, yeast_n, id from recipe where id = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);

	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.name, sqlite3_column_text(qry, 0));
		strcpy(beer.author, sqlite3_column_text(qry, 1));
		beer.vol = sqlite3_column_double(qry, 2);
		beer.malt_n = sqlite3_column_int(qry, 3);
		beer.hop_n = sqlite3_column_int(qry, 4);
//		beer.yeast_n = sqlite3_column_int(qry, 5);
		if (beer_id != sqlite3_column_int(qry, 6))
		{
			write(1, str("No recipe with that number found. Something is wrong."));
			// error
		}
	}
	sqlite3_finalize(qry);

	beer.malts = malloc(beer.malt_n * sizeof(struct malt));
	beer.hops = malloc(beer.hop_n * sizeof(struct hop));
//	beer.yeasts = malloc(beer.hop_n * sizeof(struct yeast));



	// Get the malts
	sqlite3_prepare_v2(db, str("select malts.name, malts.pts_potential, malts.mcu, ingredients.quantity, ingredients.adjustment from ingredients left join malts on malts.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_MALT);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.malts[i].name, sqlite3_column_text(qry, 0));
		beer.malts[i].potential = sqlite3_column_double(qry, 1));
		beer.malts[i].mcu = sqlite3_column_double(qry, 2));
		beer.malts[i].mass = sqlite3_column_double(qry, 3));
		i++;
	}

	sqlite3_finalize(qry);


	write(1, str(
		"<html>\n"
		"<head><title>beercalc</title></head>"
		"<body>\n"
	));
	write(1, str(
		"<font class=\"title\">"
	));
	write(1, beer.name, strlen(beer.name));
	write(1, str(
		"</font><br>\n"
		"<font class=\"subtitle\">by "
	));
	write(1, beer.author, strlen(beer.author));
	write(1, str(
		"</font><br><br>\n"
	));
	

/*		"<table>"
		"<tr><td></td><td> Name </td><td> Volume </td><td> Efficiency </td></tr>"
		"<tr><td>Brew:</td><td><input name=\"beer_name\"></input></td><td><input name=\"beer_gal\"></input></td><td><input name=\"mash_eff\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td> Name </td><td> Weight (lbs) </td><td> Potential Gravity </td></tr>"
		"<tr><td>Malt:</td><td><input name=\"malt_name\"></input></td><td><input name=\"malt_lbs\"></input></td><td><input name=\"malt_pot\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td colspan=\"2\"> Name </td><td> Weight (oz) </td><td> Boil Time </td></tr>"
		"<tr><td>Hops:</td><td colspan=\"2\"><select name=\"hop_id\">\n"
		"<option value=\"-1\">Select Hop</option>\n"
	));

	write(1, str(
		"</select></td><td><input name=\"hop_oz\"></input></td><td><input name=\"hop_time\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td colspan=\"4\"></td><td><input type=\"submit\"></input></td></tr>"
		"</table>\n"
	));*/

	write(1, str(
		"</body>"
		"</html>\n"
	));

	sqlite3_close(db);
}
