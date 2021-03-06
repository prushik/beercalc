#include <stdio.h>
#include <stdlib.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"

int main(int argc, char **argv)
{
	double alpha = 1.5;
	write(1, str(
		"<html>\n"
		"<head><title>beercalc</title></head>"
		"<body>\n"
	));
	if (argc > 1)
	{
		alpha = strtod(argv[1], NULL);
		printf("og  = %lf\n", points_to_gravity(0.75 * potential_points_to_gravity(5, 14, 37)));
		printf("ibu = %lf\n", hop_to_tinseth(5, 1, 0.05, 1.06, 60));
		printf("ibu = %lf\n", hop_to_rager(5, 1, 0.05, 1.06, 0.211));
	}
	write(1, str(
		"</body>"
		"</html>\n"
	));
}
