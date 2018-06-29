#include <stdio.h>
#include <stdlib.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"

int main(int argc, char **argv)
{
	int i;
	struct recipe beer = {0};
	beer.hop_n = 1;
	beer.hops = malloc(sizeof(struct hop));
	beer.malt_n = 1;
	beer.malts = malloc(sizeof(struct malt));
	write(1, str(
		"<html>\n"
		"<head><title>beercalc</title></head>"
		"<body>\n"
	));

	for (i=1; i<argc; i++)
	{
		if (strncmp(argv[i], str("beer_name")) == 0)
		{
			strcpy(beer.name, &argv[i][10]);
		} else if (strncmp(argv[i], str("beer_gal")) == 0)
		{
			beer.vol = strtod(&argv[i][9], NULL);
		} else if (strncmp(argv[i], str("mash_eff")) == 0)
		{
			beer.mash_eff = strtod(&argv[i][9], NULL);
		} else if (strncmp(argv[i], str("malt_name")) == 0)
		{
			strcpy(beer.malts[0].name, &argv[i][10]);
		} else if (strncmp(argv[i], str("malt_lbs")) == 0)
		{
			beer.malts[0].mass = strtod(&argv[i][9], NULL);
		} else if (strncmp(argv[i], str("malt_pot")) == 0)
		{
			beer.malts[0].potential = strtod(&argv[i][9], NULL);
		}
	}
	beer.og = points_to_gravity(potential_points_to_gravity(beer.vol, beer.malts[0].mass, gravity_to_points(beer.malts[0].potential))*(beer.mash_eff/100));
	printf("<br><br>BEER: \"%s\" - %lf gal <br>OG: %lf<br><br>\n", beer.name, beer.vol, beer.og);

	write(1, str(
		"<form action=\"recipe_result\">"
		"<table>"
		"<tr><td></td><td> Name </td><td> Volume </td><td> Efficiency </td></tr>"
		"<tr><td>Brew:</td><td><input name=\"beer_name\"></input></td><td><input name=\"beer_gal\"></input></td><td><input name=\"mash_eff\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td> Name </td><td> Weight (lbs) </td><td> Potential Gravity </td></tr>"
		"<tr><td>Malt:</td><td><input name=\"malt_name\"></input></td><td><input name=\"malt_lbs\"></input></td><td><input name=\"malt_pot\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td>Ingredient</td><td> Name </td><td> Weight (oz) </td><td> Alpha Acid (%) </td><td> Boil Time </td></tr>"
		"<tr><td>Hops:</td><td><input name=\"hop_name\"></input></td><td><input name=\"hop_oz\"></input></td><td><input name=\"hop_alpha\"></input></td><td><input name=\"hop_time\"></input></td></tr>"
		"<tr><td colspan=\"5\"></td></tr>"
		"<tr><td colspan=\"4\"></td><td><input type=\"submit\"></input></td></tr>"
		"</table>"
		"</form>\n"
	));

	write(1, str(
		"</body>"
		"</html>\n"
	));
}
