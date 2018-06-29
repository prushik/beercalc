#include <stdio.h>
#include <stdlib.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"

int main(int argc, char **argv)
{
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
