
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
