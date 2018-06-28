struct malt
{
	char name[32];     		// name of malt
};

struct hop
{
	char name[32];     		// name of hop
	double mass;       		// in oz
	double utilization;		//
	double alpha;      		// in percent
};

struct recipe
{
	double og,fg;      		// gravity
	double ibu;        		// bitterness
	int hop_n;         		// num of hops
	struct hop *hops;  		// array of hops
};
