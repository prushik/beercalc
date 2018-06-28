struct hop
{
	char name[32];
	double utilization;
	double alpha;
};

struct recipe
{
	double ibu;
	int hop_n;
	struct hop *hops;
};
