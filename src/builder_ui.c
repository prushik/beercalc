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
		"<link rel=\"stylesheet\" href=\"include/builder.css\">\n"
		"<script src=\"include/builder_ui.js\">\n"
		"</script>\n"
		"</head>\n"
		"<body>\n"
	));
	show_menu(0);
	write(1, str(
		"<div class=\"main_content\">\n"
		"<div class=\"title\">beercalc recipe builder</div>\n"
		"<br>"
		"<font class=\"subtitle beername\" contenteditable=\"true\" id=\"beername\"></font> <font class=\"subtitle\" id=\"beerid\"></font><br>\n"
//		"<input type=\"button\" onclick=\"builder_ajax_send('getbeer', 'beer_id=2');\" value=\"get\" /><br>\n"
		"<div class=\"locked\"><table class=\"ing_table\" style=\"width:75%;\"><tbody><tr><td>ABV: </td><td>IBU: </td><td>SRM: </td></tr></tbody></table></div>"
		"<input type=\"button\" style=\"border-radius:8px; width:75%;\" onclick=\"builder_ajax_send('addbeer', 'name=' + document.getElementById('beername').innerText + '&author=beercalc');\" value=\"create beer\" /><br>\n"
		"<div class=\"locked\" id=\"editor\">\n"
		"<div id=\"malts\">"
		"<select id=\"malt_select\"><option value=\"-1\">Select Malt</option></select> <input type=\"button\" value=\" + \" onclick=\"table_add_malt(document.getElementById('malt_select').value);\"></input>"
		"<table class=\"ing_table\">"
		"<tbody id=\"malt_table\">"
		"<tr><td colspan=\"5\" style=\"text-align: center;\">Malts</td></tr>\n"
		"<tr>"
		"<td>amount</td><td>name</td><td>potential</td><td>color</td><td> - </td>"
		"</tr>\n"
		"</tbody>"
		"</table>\n"
		"<select id=\"hops_select\"><option value=\"-1\">Select Hops</option></select> <input type=\"button\" value=\" + \" onclick=\"table_add_hop(document.getElementById('hops_select').value);\"></input>"
		"<table class=\"ing_table\">"
		"<tbody id=\"hops_table\">"
		"<tr><td colspan=\"5\" style=\"text-align: center;\">Hops</td></tr>\n"
		"<tr>"
		"<td>amount</td><td>time</td><td>name</td><td>alpha acid</td><td> - </td>"
		"</tr>\n"
		"</tbody>"
		"</table>\n"
		"<select id=\"yeast_select\"><option value=\"-1\">Select Yeast</option></select> <input type=\"button\" value=\" + \" onclick=\"table_add_yeast(document.getElementById('yeast_select').value);\"></input>"
		"<table class=\"ing_table\">"
		"<tbody id=\"yeast_table\">"
		"<tr><td colspan=\"5\" style=\"text-align: center;\">Yeast</td></tr>\n"
		"<tr>"
		"<td>amount</td><td>name</td><td>attenuation</td><td>flocculation</td><td> - </td>"
		"</tr>\n"
		"</tbody>"
		"</table>\n"
		"</div>\n"
		"</div>"
		"</div></body></html>"
	));
}
