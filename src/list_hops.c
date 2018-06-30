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
	struct recipe beer = {0};
	beer.hop_n = 1;
	beer.hops = malloc(sizeof(struct hop));
	beer.malt_n = 1;
	beer.malts = malloc(sizeof(struct malt));

	sqlite3 *db;
	sqlite3_stmt *qry;

	sqlite3_open("/var/db/beer.sqlite", &db);

	write(1, str(
		"<html>\n"
		"<head><title>beercalc</title></head>"
		"<body>\n"
	));
	write(1, str(
		"<form action=\"recipe_result\">"
		"<table>"
		"<tr><td></td><td> Name </td><td> Volume </td><td> Efficiency </td></tr>"
		"<tr><td>Brew:</td><td><input name=\"beer_name\"></input></td><td><input name=\"beer_gal\"></input></td><td><input name=\"mash_eff\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td> Name </td><td> Weight (lbs) </td><td> Potential Gravity </td></tr>"
		"<tr><td>Malt:</td><td><input name=\"malt_name\"></input></td><td><input name=\"malt_lbs\"></input></td><td><input name=\"malt_pot\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td colspan=\"2\"> Name </td><td> Weight (oz) </td><td> Boil Time </td></tr>"
		"<tr><td>Hops:</td><td colspan=\"2\"><select name=\"hop\">\n"
		"<option name=\"-1\">Select Hop</option>\n"
	));

	sqlite3_prepare_v2(db, str("select name, alpha, type, id from hops;"), &qry, NULL);

	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		const char *hop_name = sqlite3_column_text(qry, 0);
		const char *hop_alpha = sqlite3_column_text(qry, 1);
		int hop_type = sqlite3_column_int(qry, 2);
		const char *hop_id = sqlite3_column_text(qry, 3);
		write(1, str("<option name=\""));
		write(1, hop_id, strlen(hop_id));
		write(1, str("\">"));
		write(1, hop_name, strlen(hop_name));
		if (hop_type == 0)
		{
			write(1, str(" - pellet ("));
		}
		else
		{
			write(1, str(" - whole ("));
		}
		write(1, hop_alpha, strlen(hop_alpha));
		write(1, str(")</option>\n"));
	}

	sqlite3_finalize(qry);

	write(1, str(
		"</select></td><td><input name=\"hop_oz\"></input></td><td><input name=\"hop_time\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td colspan=\"4\"></td><td><input type=\"submit\"></input></td></tr>"
		"</table>"
		"</form>\n"
	));

	write(1, str(
		"</body>"
		"</html>\n"
	));

	sqlite3_close(db);
}
