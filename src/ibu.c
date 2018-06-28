#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "web.h"
#include "beer.h"

// tinseth method
// ibu = (1.65 * 0.000125^(Ggravity - 1)) * ((1 - e^(-0.04*time)) * a * m * 1000
// ibu = (1.65*0.000125^(gravity-1))*(1-EXP(-0.04*time))*alpha*mass*1000
double hop_to_tinseth(double vol, double mass, double alpha, double og, double minutes)
{
	return (1.65 * pow(0.000125, og - 1.0) * 
				(1.0 - exp(-0.04 * minutes)) * alpha * mass * 1000 / 
				(vol * 4.15)) *
				(28.3 / 3.785);
}

// rager method
// ibu = (Woz * U * a * 7489) / (Vgal * gravity)
double hop_to_rager(double vol, double mass, double alpha, double og, double utilization)
{
	return (mass * utilization * alpha * 7489.0) / (vol * og);
}

int main(int argc, char **argv)
{
	double alpha = 1.5;
	write(1, str("<html>\n"));
	if (argc > 1)
	{
		alpha = strtod(argv[1], NULL);
		printf("%s = %lf\n", argv[1], alpha);
		printf("ibu = %lf\n", hop_to_tinseth(5, 0.01, 5, 1.06, 60));
		printf("ibu = %lf\n", hop_to_rager(5, 0.01, 5, 1.06, 0.211));
	}
	write(1, str("</html>\n"));
}
