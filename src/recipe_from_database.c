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
	char buffer[64];
	unsigned int buf_len = 0;

	for (i=1; i<argc; i++)
	{
		if (strncmp(argv[i], str("beer_id")) == 0)
		{
			beer_id = strtoul(&argv[i][8], NULL, 0);
		}
	}

	sqlite3 *db;
	sqlite3_stmt *qry;

	sqlite3_open(DATABASE, &db);

	sqlite3_prepare_v2(db, str("select name, author, volume, malt_n, hops_n, yeast_n, id from recipe where id = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);

	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.name, sqlite3_column_text(qry, 0));
		strcpy(beer.author, sqlite3_column_text(qry, 1));
		beer.vol = sqlite3_column_double(qry, 2);
		beer.malt_n = sqlite3_column_int(qry, 3);
		beer.hop_n = sqlite3_column_int(qry, 4);
		beer.yeast_n = sqlite3_column_int(qry, 5);
		if (beer_id != sqlite3_column_int(qry, 6))
		{
			write(1, str("No recipe with that number found. Something is wrong.\n"));
			// error
		}
	}
	sqlite3_finalize(qry);

	beer.malts = malloc(beer.malt_n * sizeof(struct malt));
	beer.hops = malloc(beer.hop_n * sizeof(struct hop));
	beer.yeasts = malloc(beer.yeast_n * sizeof(struct yeast));


	// Get the malts
	sqlite3_prepare_v2(db, str("select malts.name, malts.pts_potential, malts.mcu, ingredients.quantity, ingredients.adjustment from ingredients left join malts on malts.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_MALT);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.malts[i].name, sqlite3_column_text(qry, 0));
		beer.malts[i].pts_potential = sqlite3_column_double(qry, 1);
		beer.malts[i].mcu = sqlite3_column_double(qry, 2);
		beer.malts[i].mass = sqlite3_column_double(qry, 3);
		i++;
	}
	sqlite3_finalize(qry);

	// Get the hops
	sqlite3_prepare_v2(db, str("select hops.name, hops.alpha, hops.type, ingredients.quantity, ingredients.time, ingredients.adjustment from ingredients left join hops on hops.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_HOPS);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.hops[i].name, sqlite3_column_text(qry, 0));
		beer.hops[i].alpha = sqlite3_column_double(qry, 1);
//		beer.hops[i].type = sqlite3_column_int(qry, 2);
		beer.hops[i].mass = sqlite3_column_double(qry, 3);
		beer.hops[i].time = sqlite3_column_int(qry, 4);
		i++;
	}
	sqlite3_finalize(qry);

	// Get the hops
	sqlite3_prepare_v2(db, str("select yeasts.name, yeasts.attenuation, ingredients.quantity, ingredients.time, ingredients.adjustment from ingredients left join yeasts on yeasts.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_YEAST);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.yeasts[i].name, sqlite3_column_text(qry, 0));
		beer.yeasts[i].attenuation = sqlite3_column_double(qry, 1);
		beer.yeasts[i].amount = sqlite3_column_double(qry, 2);
		beer.yeasts[i].time = sqlite3_column_int(qry, 3);
		i++;
	}
	sqlite3_finalize(qry);

	calculate_recipe(&beer);

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
		"<table>\n"
		"<tr><td>"
	));

	write(1, str("og: "));
	sprintf(buffer, "%.4lf%n", beer.og, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("</td><td>"));

	write(1, str("fg: "));
	sprintf(buffer, "%.4lf%n", beer.fg, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("</td><td>"));

	write(1, str("abv: "));
	sprintf(buffer, "%.1lf%n", beer.abv, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("</td><td>"));

	write(1, str("ibu: "));
	sprintf(buffer, "%.1lf%n", beer.ibu, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("</td><td>"));

	write(1, str(
		"</td></tr>"
		"</table>"
	));
	write(1, str(
		"malts:<br><table>\n"
	));
	for (i=0; i < beer.malt_n; i++)
	{
		write(1, str("<tr><td>"));
		sprintf(buffer, "%.2lf%n", beer.malts[i].mass, &buf_len);
		write(1, buffer, buf_len);
		write(1, str(" lbs</td><td>"));
		write(1, beer.malts[i].name, strlen(beer.malts[i].name));
		write(1, str("</td><td>"));
		sprintf(buffer, "%.1lf%n", beer.malts[i].pts_potential, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("</td></tr>\n"));
	}
	write(1, str("</table>\n"));

	write(1, str(
		"hops:<br><table>\n"
	));
	for (i=0; i < beer.hop_n; i++)
	{
		write(1, str("<tr><td>"));
		sprintf(buffer, "%.2lf%n", beer.hops[i].mass, &buf_len);
		write(1, buffer, buf_len);
		write(1, str(" oz</td><td>"));
		sprintf(buffer, "%d%n", beer.hops[i].time, &buf_len);
		write(1, buffer, buf_len);
		write(1, str(" min</td><td>"));
		write(1, beer.hops[i].name, strlen(beer.hops[i].name));
		write(1, str("</td><td>"));
		sprintf(buffer, "%.1lf%n", beer.hops[i].alpha, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("</td></tr>\n"));
	}
	write(1, str("</table>\n"));

	write(1, str(
		"yeast:<br><table>\n"
	));
	for (i=0; i < beer.yeast_n; i++)
	{
		write(1, str("<tr><td>"));
		sprintf(buffer, "%.2lf%n", beer.yeasts[i].amount, &buf_len);
		write(1, buffer, buf_len);
		write(1, str(" package</td><td>"));
		write(1, beer.yeasts[i].name, strlen(beer.yeasts[i].name));
		write(1, str("</td><td>"));
		sprintf(buffer, "%.1lf%n", beer.yeasts[i].attenuation, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("</td></tr>\n"));
	}
	write(1, str("</table>\n"));

/*
		"<table>"
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
