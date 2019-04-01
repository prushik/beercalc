#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "menu.h"

int main(int argc, char **argv)
{
	int i;
	int preload = 0;
	char *recipe_id;

	for (i=1; i<argc; i++)
	{
		if (strncmp(argv[i], "edit", 4) == 0)
		{
			preload = 1;
		}
		if (strncmp(argv[i], "beer_id", 7) == 0)
		{
			recipe_id = &argv[i][8];
		}
	}
	
	write(1, str(
		"<html>\n"
		"<head>\n"
		"<title>beercalc</title>\n"
		"<link rel=\"stylesheet\" href=\"include/global.css\">\n"
		"<link rel=\"stylesheet\" href=\"include/builder.css\">\n"
		"<script src=\"include/builder_ui.js\">\n"
		"</script>\n"
		"</head>\n"
		"<body"
	));

	if (preload) {
		write(1, str(
			" onload=\"load_recipe("
		));
		write(1, recipe_id, strlen(recipe_id));
		write(1, str(
			");\""
		));
		
	}

	write(1, str(
		">\n"
	));
	show_menu(0);
	write(1, str(
		"<div class=\"main_content\">\n"
		"<div class=\"title\">beercalc recipe builder</div>\n"
		"<br>"
		"<font class=\"subtitle beername\" contenteditable=\"true\" id=\"beername\"></font> <font class=\"subtitle\" id=\"beerid\"></font><br>\n"
//		"<input type=\"button\" onclick=\"builder_ajax_send('getbeer', 'beer_id=2');\" value=\"get\" /><br>\n"
//		"<div class=\"locked\"><table class=\"ing_table\" style=\"width:75%;\"><tbody><tr><td>ABV: </td><td>IBU: </td><td>SRM: </td></tr></tbody></table></div>"
		"<input type=\"button\" style=\"border-radius:8px; width:75%;\" onclick=\"builder_ajax_send('addbeer', 'name=' + document.getElementById('beername').innerText + '&author=beercalc');\" value=\"create beer\" id=\"save_create\" /><br>\n"
		"<div class=\"locked\" id=\"editor\">\n"

		"<div style=\"float:right; width:25%\">"
		"<select id=\"style_select\" style=\"width: 100%;\" onchange=\"populate_style_ranges(document.getElementById('style_select').value);\"><option value=\"-1\">Select Style</option></select>"
		"<br><input type=\"button\" value=\"auto style\" style=\"width: 100%;\" onclick=\"evaluate_style();\"></input>"
		"<br><input type=\"button\" value=\"calculate\" style=\"width: 100%;\" onclick=\"calculate();\"></input>"
		"<table class=\"ing_table\" style=\"width: 100%;\">"
		"<tbody id=\"style_table\">"
		"<tr><td colspan=\"3\" style=\"text-align: center;\">Style</td></tr>\n"
		"<tr>"
		"<td>Property</td><td>Target Range</td><td>Current</td>"
		"</tr>\n"
		"<tr id=\"og_row\">"
		"<td>OG</td><td></td><td></td>"
		"</tr>\n"
		"<tr id=\"fg_row\">"
		"<td>FG</td><td></td><td></td>"
		"</tr>\n"
		"<tr id=\"ibu_row\">"
		"<td>IBU</td><td></td><td></td>"
		"</tr>\n"
		"<tr id=\"abv_row\">"
		"<td>ABV</td><td></td><td></td>"
		"</tr>\n"
		"<tr id=\"srm_row\">"
		"<td>SRM</td><td></td><td></td>"
		"</tr>\n"
		"</tbody>"
		"</table>\n"
		"<br>"
		"<div style=\"margin: auto; border: 1px solid #000000; border-radius: 0px 0px 8px 8px; width:32px; height:64px;\" id=\"srm_example\"><div style=\"height: 10%; width: 100%; margin: 0px; background-color:#e0d0b0;\"></div></div>"
		"</div>"

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
		"</div>\n"
		"</div>\n"
		"<div id=\"mask\" class=\"mask\"></div>\n"
		"</body></html>"
	));
}
