// rager method
// ibu = (Woz * U * a * 7489) / (Vgal * Cgravity)

// tinseth method
// ibu = (1.65 * 0.000125^(Ggravity - 1)) * ((1 - e^(-0.04*time)) * a * m * 1000
// ibu = (1.65*0.000125^(gravity-1))*(1-EXP(-0.04*time))*alpha*mass*1000

#include <stdio.h>
#include <stdlib.h>
#include "web.h"

int main(int argc, char **argv)
{
	double alpha = 1.5;
	write(1, str("<html>\n"));
	if (argc > 1)
	{
		alpha = strtod(argv[1], NULL);
		printf("%s = %lf\n", argv[1], alpha);
	}
	write(1, str("</html>\n"));
}
