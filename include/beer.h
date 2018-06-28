struct malt
{
	char name[32];     		// name of malt
	double mass;       		// in lbs
	double potential;  		// gravity potential (1.046 for pure sugar)
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
	double vol;        		// volume (gallons)
	double bg,og,fg;   		// gravity
	double ibu;        		// bitterness
	double mash_eff;   		// mash efficiency
	int hop_n;         		// num of hops
	struct hop *hops;  		// array of hops
};
