#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"
#include "yeast.h"
#include <sqlite3.h>

int main(int argc, char **argv)
{
	int i;
	unsigned long int beer_id = 0;
	struct recipe beer = {0};
	char buffer[256];
	unsigned int buf_len = 0;

	sqlite3 *db;
	sqlite3_stmt *qry;

	sqlite3_open(DATABASE, &db);

	sqlite3_prepare_v2(db, str("select recipe.name, style.name, recipe.id from recipe left join style on style.id = recipe.style_id order by style.id;"), &qry, NULL);

	write(1, str(
		"<html>\n"
		"<head>\n"
		"<title>beercalc</title>\n"
		"<link rel=\"stylesheet\" href=\"include/global.css\">\n"
		"</head>\n"
		"<body>\n"
	));
	show_menu(0);
	write(1, str(
		"<div class=\"main_content\">\n"
		"<div class=\"title\">beercalc</div>\n<br>\n"
		" beercalc is an open source beer recipe calculation and planning tool. It allows you to build a recipe and estimate certain properties of the resultant beer. beercalc is written in C, designed to be served by the BetterOS Web Unweave webserver, however, almost any CGI-enabled webserver can likely be used with little modification (untested).<br>"
		"<br>\n"
		"It should be noted that the whole idea of this implementation is a little bit silly. It was written for 4 reasons:<br>"
		" 1. I was not happy with any available desktop recipe calculators, all either have an insane list of dependencies, are not opensource, or are not sufficient in some other aspect.<br>\n"
		" 2. I wanted to develop another, more interactive, website to run off the unweave webserver.<br>\n"
		" 3. I want to end my reliance on others (in the software realm) step by step.<br>\n"
		" 4. I eventually want to extend this application to interface with custom brewing hardware to improve my brews.<br>\n"
		"Now, honestly, none of these reasons holds any value to anyone other than myself. Most people can be satisfied with other online, opensource brew recipe builders like malt.io or brewgr, but because the latter runs on C#, and the former node.js, I could not abide using either with a clear conscience.<br>"
		"C# is just about the worst thing in the world, but I admit brewgr works very well. Javascript is also horrid, but after writing this application, I realize that Javascript is actually a pretty good choice for beer recipe calculations.<br>"
		"Allow me to ellaborate: Javascript is a horrid language because it lacks any integer datatype. All numbers in Javascript are actually double precision IEEE floating point numbers. Because of this fact, javascript will always be a burden on your CPU. It also renders other parts of Javascript nonsensical. For instance, Javascript has bitwise operators, but applying bitwise operators to a number type will not actually operate on the number in a bitwise manner because the number, instead, javascript emulates the behavior of a bitwise operation on an integer instead. This also makes tricks like the famous fast inverse squareroot impossible. It also typically runs on the client side, so all the burden of processing is pushed onto the client.<br>"
		"However, pretty much all of the operations needed to make beer are fractional, so a language with only floating point numbers is almost as good of a fit as any. Also, since the normal use case involves making small recipe changes and observing the result, so client side calculations avoids expensive networking round-trips.<br>"
		"This implementation is in C, and calculations are done on the server-side. Althouhgh C does floating-point calculation as efficiently as javascript (better actually), this implementation could be improved, and hopefully will be. Future improvements will include the option to do all calculations using fixed-point numbers, a client-side command line interface, and a full distribution including software for recipe building and mash and brew monitoring and automation.<br><br>"
		"Beer recipes: "
		"<form action=\"recipe_from_db\" method=\"get\">"
		"<select name=\"beer_id\" id=\"beer_id\"><option value=-1>select a beer recipe</option>\n"
	));
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		sprintf(buffer, "<option value=%d>%s (%s)</option>%n", sqlite3_column_int(qry, 2), sqlite3_column_text(qry, 0), sqlite3_column_text(qry, 1), &buf_len);
		write(1, buffer, buf_len);
	}
	sqlite3_finalize(qry);

	write(1, str(
		"</select><input type=\"submit\" value=\"brew\">\n"
	));

	write(1, str(
		"</div>"
		"</body>"
		"</html>\n"
	));

	sqlite3_close(db);
}
