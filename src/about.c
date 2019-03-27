#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "menu.h"

int main(int argc, char **argv)
{
	int i;

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
		" beercalc is an open source beer recipe calculation and planning tool. It allows you to build a recipe and estimate certain properties of the resultant beer. beercalc is also a set of functions that can be copied and used in other applications.<br>"
		"<br>\n"
		" beercalc is written in C for the server side code and beer recipe calculation code, and javascript for the client side.<br>"
		"It should be noted that the whole idea of this implementation is a little bit silly. It was written for 4 reasons:<br>"
		" 1. I was not happy with any available desktop recipe calculators, all either have an insane list of dependencies, are not opensource, or are not sufficient in some other aspect.<br>\n"
		" 2. I wanted to develop another, more interactive, website to run off the unweave webserver.<br>\n"
		" 3. I want to end my reliance on others (in the software realm) step by step.<br>\n"
		" 4. I eventually want to extend this application to interface with custom brewing hardware to improve my brews.<br>\n"
		"Honestly, none of these reasons hold any value to anyone other than myself, but this software may grow into something "
		"that other people might find useful. One of the unique features beercalc offers is the ability to define a recipe, and "
		"than based on the defined recipe, find beer styles that best match the recipe. This allows a brewer to reverse the "
		"process of developing a recipe, starting with ingredients and stepping backwards into a style, and then fine tuning "
		"based on the calculated style. A process like this might encourage more experimental brews and make the recipe building "
		"process more exciting.<br>"
		""
		"This implementation is in C, and calculations are done on the server-side. Although C does floating-point calculation as efficiently as javascript (better actually), this implementation could be improved, and hopefully will be. Future improvements will include the option to do all calculations using fixed-point numbers, a client-side command line interface, and a full distribution including software for recipe building and mash and brew monitoring and automation.<br>"
	));
}
