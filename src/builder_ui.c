#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "menu.h"

int main(int argc, char **argv)
{
	write(1, str(
		"<html>\n"
		"<head>\n"
		"<title>beercalc</title>\n"
		"<link rel=\"stylesheet\" href=\"include/global.css\">\n"
		"<script>\n"
			"beer_id = 0;\n"

			"function builder_ajax_send(action, arguments)\n"
			"{\n"
				"var ajax = new XMLHttpRequest();\n"
				"ajax.onreadystatechange = function()\n"
				"{\n"
					"if (this.readyState == 4 && this.status == 200)\n"
					"{\n"
						"process_ajax(action, this);\n"
					"}\n"
				"};\n"
				"ajax.open(\"GET\", \"builder_ajax?action=\" + action + \"&\" + arguments, true);\n"
				"ajax.send();\n"
			"}\n"

			"function process_ajax(action, ajax)\n"
			"{\n"
				"var obj = JSON.parse(ajax.responseText);\n"
				"console.log(obj);\n"
				"if (obj.hasOwnProperty(\"beer_id\"))\n"
					"beer_id = obj.beer_id;"
				""
			"}\n"
		"</script>"
		"</head>\n"
		"<body>\n"
	));
	show_menu(0);
	write(1, str(
		"<div class=\"main_content\">\n"
		"<div class=\"title\">beercalc recipe builder</div>\n"
		"<font class=\"subtitle\" contenteditable=\"true\" style=\"width:75%; background-color:#999999;\" id=\"beername\">________</font><br>\n"
		"<input type=\"button\" onClick=\"builder_ajax_send('getbeer', 'beer_id=2');\" value=\"get\" /><br>\n"
		"<input type=\"button\" onClick=\"builder_ajax_send('addbeer', 'name=' + document.getElementById('beername').innerText + '&author=beercalc');\" value=\"add\" /><br>\n"
		"</div></body></html>"
	));
}
