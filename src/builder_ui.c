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
/*			"beer_id = 0;\n"
			"malt_array = [];"
			"malt_n = 0;"
			"hops_array = [];"
			"hops_n = 0;"
			"yeast_array = [];"
			"yeast_n = 0;"

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
				"{\n"
					"beer_id = obj.beer_id;\n"
					"document.getElementById('beerid').innerText = \"#\" + beer_id;\n"
					"document.getElementById('editor').classList.remove('locked');\n"
					"builder_ajax_send(\"getmalt\", \"\");\n"
					"builder_ajax_send(\"gethops\", \"\");\n"
					"builder_ajax_send(\"getyeasts\", \"\");\n"
				"}\n"

				"if (obj.hasOwnProperty(\"malts\"))\n"
				"{\n"
					"populate_malt_select(obj.malts);\n"
				"}\n"

				"if (obj.hasOwnProperty(\"hops\"))\n"
				"{\n"
					"populate_hops_select(obj.hops);\n"
				"}\n"

				"if (obj.hasOwnProperty(\"yeasts\"))\n"
				"{\n"
					"populate_yeast_select(obj.yeasts);\n"
				"}\n"
			"}\n"
			""
			"function populate_malt_select(malts)\n"
			"{\n"
				"var i, malt, opts;\n"
				"opts = \"<option>Select Malt</option>\";\n"
				"for (i = 0; malt = malts[i]; i++)\n"
				"{\n"
					"opts += \"<option value=\" + malt.id + \">\" + malt.name + \" (\" + malt.potential + \")</option>\";\n"
					"malt_array[malt.id] = {\"name\": malt.name, \"potential\": malt.potential, \"mcu\": malt.mcu};\n"
				"}\n"
				"document.getElementById('malt_select').innerHTML = opts;\n"
			"}\n"
			""
			"function populate_hops_select(hops)\n"
			"{\n"
				"var i, hop, opts;\n"
				"opts = \"<option>Select Hops</option>\";\n"
				"for (i = 0; hop = hops[i]; i++)\n"
				"{\n"
					"opts += \"<option value=\" + hop.id + \">\" + hop.name + \" (\" + hop.alpha + \")</option>\";\n"
					"hops_array[hop.id] = {\"name\": hop.name, \"alpha\": hop.alpha};\n"
				"}\n"
				"document.getElementById('hops_select').innerHTML = opts;\n"
			"}\n"
			""
			"function populate_yeast_select(yeasts)\n"
			"{\n"
				"var i, yeast, opts;\n"
				"opts = \"<option>Select Yeast</option>\";\n"
				"for (i = 0; yeast = yeasts[i]; i++)\n"
				"{\n"
					"opts += \"<option value=\" + yeast.id + \">\" + yeast.name + \" (\" + yeast.attenuation + \")</option>\";\n"
					"yeast_array[yeast.id] = {\"name\": yeast.name, \"attenuation\": yeast.attenuation, \"flocculation\": yeast.flocculation};\n"
				"}\n"
				"document.getElementById('yeast_select').innerHTML = opts;\n"
			"}\n"
			""
			"function table_add_malt(malt_id)\n"
			"{\n"
				"var table = document.getElementById('malt_table');\n"
				"table.innerHTML += \"<tr id=\\\"malt_\" + malt_n + \"\\\">"
					"<td><input type=\\\"number\\\" value=\\\"0\\\"></input> lbs</td>"
					"<td>\" + malt_array[malt_id].name + \"</td>"
					"<td>\" + malt_array[malt_id].potential + \"</td>"
					"<td>\" + malt_array[malt_id].mcu + \"</td>"
					"<td><input type=\\\"button\\\" value=\\\" - \\\" onclick=\\\"table_rm_row('malt_\" + malt_n + \"'); malt_n += -1;\\\"></input></td>"
					"</tr>\";\n"
				"malt_n += 1;\n"
			"}\n"
			""
			"function table_add_hop(hop_id)\n"
			"{\n"
				"var table = document.getElementById('hops_table');\n"
				"table.innerHTML += \"<tr id=\\\"hops_\" + hops_n + \"\\\">"
					"<td><input type=\\\"number\\\" value=\\\"0\\\"></input> oz</td>"
					"<td><input type=\\\"number\\\" value=\\\"0\\\"></input> min</td>"
					"<td>\" + hops_array[hop_id].name + \"</td>"
					"<td>\" + hops_array[hop_id].alpha + \"</td>"
					"<td><input type=\\\"button\\\" value=\\\" - \\\" onclick=\\\"table_rm_row('hops_\" + hops_n + \"'); hops_n += -1;\\\"></input></td>"
					"</tr>\";\n"
				"hops_n += 1;\n"
			"}\n"
			""
			"function table_add_yeast(yeast_id)\n"
			"{\n"
				"var table = document.getElementById('yeast_table');\n"
				"table.innerHTML += \"<tr id=\\\"yeast_\" + yeast_n + \"\\\">"
					"<td><input type=\\\"number\\\" value=\\\"0\\\"></input> pkg</td>"
					"<td>\" + yeast_array[yeast_id].name + \"</td>"
					"<td>\" + yeast_array[yeast_id].attenuation + \"</td>"
					"<td>\" + yeast_array[yeast_id].flocculation + \"</td>"
					"<td><input type=\\\"button\\\" value=\\\" - \\\" onclick=\\\"table_rm_row('yeast_\" + yeast_n + \"'); yeast_n += -1;\\\"></input></td>"
					"</tr>\";\n"
				"yeast_n += 1;\n"
			"}\n"
			""
			"function table_rm_row(row_id)\n"
			"{\n"
				"var row = document.getElementById(row_id);\n"
				"row.parentNode.removeChild(row);\n"
			"}\n"*/
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
		"<select size=\"2\" id=\"malt_select\" style=\"height:auto; width:auto;float: left;\"><option value=\"-1\">Select Malt</option></select> <input type=\"button\" value=\" + \" onclick=\"table_add_malt(document.getElementById('malt_select').value);\"></input>"
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
