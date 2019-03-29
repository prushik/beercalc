beer_id = 0;
malt_array = [];
malt_n = 0;
hops_array = [];
hops_n = 0;
yeast_array = [];
yeast_n = 0;
style_array = [];

function builder_ajax_send_sync(action, arguments)
{
	var ajax = new XMLHttpRequest();
	ajax.onreadystatechange = function()
	{
		if (this.readyState == 4 && this.status == 200)
		{
			process_ajax(action, this);
		}
	};
	ajax.open("GET", "builder_ajax?action=" + action + "&" + arguments, false);
	ajax.send();
}

function builder_ajax_send(action, arguments)
{
	var ajax = new XMLHttpRequest();
	ajax.onreadystatechange = function()
	{
		if (this.readyState == 4 && this.status == 200)
		{
			process_ajax(action, this);
		}
	};
	ajax.open("GET", "builder_ajax?action=" + action + "&" + arguments, true);
	ajax.send();
}

function process_ajax(action, ajax)
{
	var obj = JSON.parse(ajax.responseText);
	console.log(obj);
	if (obj.hasOwnProperty("beer_id"))
	{
		beer_id = obj.beer_id;
		document.getElementById('beerid').innerText = "#" + beer_id;
		document.getElementById('editor').classList.remove('locked');
		builder_ajax_send("getmalt", "");
		builder_ajax_send("gethops", "");
		builder_ajax_send("getyeasts", "");
		builder_ajax_send("getstyles", "");
		document.getElementById('save_create').value = "save";
		document.getElementById('save_create').onclick = save_recipe;
	}
	if (obj.hasOwnProperty("malts"))
	{
		populate_malt_select(obj.malts);
	}
	if (obj.hasOwnProperty("hops"))
	{
		populate_hops_select(obj.hops);
	}
	if (obj.hasOwnProperty("yeasts"))
	{
		populate_yeast_select(obj.yeasts);
	}
	if (obj.hasOwnProperty("styles"))
	{
		populate_style_select(obj.styles);
	}
	if (obj.hasOwnProperty("og") && obj.hasOwnProperty("fg") && obj.hasOwnProperty("ibu") && obj.hasOwnProperty("abv"))
	{
		populate_current_properties(obj);
	}
	if (obj.hasOwnProperty("style_id"))
	{
		document.getElementById('style_select').value = obj.style_id;
		populate_style_ranges(obj.style_id);
	}
	if (obj.hasOwnProperty("ingredients"))
	{
		document.getElementById('beername').innerText = obj.name;
		populate_recipe(obj);
	}
}

function populate_malt_select(malts)
{
	var i, malt, opts;
	opts = "<option>Select Malt</option>";
	for (i = 0; malt = malts[i]; i++)
	{
		opts += "<option value=" + malt.id + ">" + malt.name + " (" + malt.potential + ")</option>";
		malt_array[malt.id] = {"name": malt.name, "potential": malt.potential, "mcu": malt.mcu};
	}
	document.getElementById('malt_select').innerHTML = opts;
}

function populate_hops_select(hops)
{
	var i, hop, opts;
	opts = "<option>Select Hops</option>";
	for (i = 0; hop = hops[i]; i++)
	{
		opts += "<option value=" + hop.id + ">" + hop.name + " (" + hop.alpha + ")</option>";
		hops_array[hop.id] = {"name": hop.name, "alpha": hop.alpha};
	}
	document.getElementById('hops_select').innerHTML = opts;
}

function populate_yeast_select(yeasts)
{
	var i, yeast, opts;
	opts = "<option>Select Yeast</option>";
	for (i = 0; yeast = yeasts[i]; i++)
	{
		opts += "<option value=" + yeast.id + ">" + yeast.name + " (" + yeast.attenuation + ")</option>";
		yeast_array[yeast.id] = {"name": yeast.name, "attenuation": yeast.attenuation, "flocculation": yeast.flocculation};
	}
	document.getElementById('yeast_select').innerHTML = opts;
}

function populate_style_select(styles)
{
	if (style_array.length > 0) {
		return;
	}

	var i, style, opts;
	opts = "<option>Select Style</option>";
	for (i = 0; style = styles[i]; i++)
	{
		opts += "<option value=" + style.id + ">" + style.name + "</option>";
		style_array[style.id] = style;
	}
	document.getElementById('style_select').innerHTML = opts;
}

function populate_style_ranges(style_id)
{
	var table = document.getElementById('styles_table');
	var row = document.getElementById('og_row');
	row.cells[1].innerHTML = style_array[style_id].og[0] + " - " + style_array[style_id].og[1];
	var row = document.getElementById('fg_row');
	row.cells[1].innerHTML = style_array[style_id].fg[0] + " - " + style_array[style_id].fg[1];
	var row = document.getElementById('ibu_row');
	row.cells[1].innerHTML = style_array[style_id].ibu[0] + " - " + style_array[style_id].ibu[1];
	var row = document.getElementById('abv_row');
	row.cells[1].innerHTML = style_array[style_id].abv[0] + " - " + style_array[style_id].abv[1];
}

function populate_current_properties(obj)
{
	var row;
//	var table = document.getElementById('styles_table');
	row = document.getElementById('og_row');
	row.cells[2].innerHTML = obj.og;
	row = document.getElementById('fg_row');
	row.cells[2].innerHTML = obj.fg;
	row = document.getElementById('ibu_row');
	row.cells[2].innerHTML = obj.ibu;
	row = document.getElementById('abv_row');
	row.cells[2].innerHTML = obj.abv;
}

function populate_recipe(obj)
{
	var i, ing;
	for (i = 0; ing = obj.ingredients.malts[i]; i++) {
		table_add_malt(ing.id, ing.weight)
	}
	for (i = 0; ing = obj.ingredients.hops[i]; i++) {
		table_add_hop(ing.id, ing.weight, ing.time)
	}
	for (i = 0; ing = obj.ingredients.yeasts[i]; i++) {
		table_add_yeast(ing.id, ing.amount)
	}
}

function table_add_malt(malt_id, qty = 0)
{
	var table = document.getElementById('malt_table');
	table.innerHTML += "<tr id=\"malt_" + malt_n + "\" data-malt_id=\"" + malt_id + "\"><td><input type=\"number\" value=\"" + qty + "\"></input> lbs</td><td>" + malt_array[malt_id].name + "</td><td>" + malt_array[malt_id].potential + "</td><td>" + malt_array[malt_id].mcu + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('malt_" + malt_n + "'); malt_n += -1;\"></input></td></tr>";
	malt_n += 1;
}

function table_add_hop(hop_id, qty = 0, time = 0)
{
	var table = document.getElementById('hops_table');
	table.innerHTML += "<tr id=\"hops_" + hops_n + "\" data-hop_id=\"" + hop_id + "\"><td><input type=\"number\" value=\"" + qty + "\"></input> oz</td><td><input type=\"number\" value=\"" + time + "\"></input> min</td><td>" + hops_array[hop_id].name + "</td><td>" + hops_array[hop_id].alpha + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('hops_" + hops_n + "'); hops_n += -1;\"></input></td></tr>";
	hops_n += 1;
}

function table_add_yeast(yeast_id, qty = 1)
{
	var table = document.getElementById('yeast_table');
	table.innerHTML += "<tr id=\"yeast_" + yeast_n + "\" data-yeast_id=\"" + yeast_id + "\"><td><input type=\"number\" value=\"" + qty + "\"></input> pkg</td><td>" + yeast_array[yeast_id].name + "</td><td>" + yeast_array[yeast_id].attenuation + "</td><td>" + (["none","very low","low","mid-low","medium","mid-high","high","extreme"])[yeast_array[yeast_id].flocculation] + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('yeast_" + yeast_n + "'); yeast_n += -1;\"></input></td></tr>";
	yeast_n += 1;
}

function table_rm_row(row_id)
{
	var row = document.getElementById(row_id);
	row.parentNode.removeChild(row);
}

function evaluate_style()
{
	var i, j, row, og, fg, ibu, abv, style, score, component_score;
	var candidates = [
		{id: undefined, score: 0, in_range: 0},
		{id: undefined, score: 0, in_range: 0},
		{id: undefined, score: 0, in_range: 0},
		{id: undefined, score: 0, in_range: 0},
		{id: undefined, score: 0, in_range: 0},
	]

	row = document.getElementById('og_row');
	og = row.cells[2].innerHTML;
	row = document.getElementById('fg_row');
	fg = row.cells[2].innerHTML;
	row = document.getElementById('ibu_row');
	ibu = row.cells[2].innerHTML;
	row = document.getElementById('abv_row');
	abv = row.cells[2].innerHTML;

	for (row = 0; style = style_array[row]; row++)
	{
		score = 100; // start out with score of 100

		if (og >= style.og[0] && og <= style.og[1])
			component_score = 1.5 / (style.og[1] - style.og[0]); // favor styles that have tighter ranges
		else
			component_score = (og < style.og[0]) ? og - style.og[0] : style.og[1] - og; // if out of range, how far?

		score += component_score * 1; // add weighted component score to total score

		if (fg >= style.fg[0] && fg <= style.fg[1])
			component_score = 1.5 / (style.fg[1] - style.fg[0]);
		else
			component_score = (fg < style.fg[0]) ? fg - style.fg[0] : style.fg[1] - fg;

		score += component_score * 1;

		if (ibu >= style.ibu[0] && ibu <= style.ibu[1])
			component_score = 75 / (style.ibu[1] - style.ibu[0]);
		else
			component_score = (ibu < style.ibu[0]) ? ibu - style.ibu[0] : style.ibu[1] - ibu;

		score += component_score * 1;

		if (abv >= style.abv[0] && abv <= style.abv[1])
			component_score = 8 / (style.abv[1] - style.abv[0]);
		else
			component_score = (abv < style.abv[0]) ? abv - style.abv[0] : style.abv[1] - abv;

		score += component_score * 1;

		for (i = 0; i < 5; i++)
		{
			if (score > candidates[i].score)
			{
				for (j = 4; j > i+1; j--)
				{
					candidates[j].id = candidates[j-1].id;
					candidates[j].score = candidates[j-1].id;
				}
				candidates[i].id = style.id;
				candidates[i].score = score;
				break;
			}
		}
	}

	console.log(candidates);
	document.getElementById('style_select').value = candidates[0].id;
	populate_style_ranges(candidates[0].id);
}

function save_recipe()
{
	var i, row, table;

	builder_ajax_send_sync("clearbeer", "beer_id=" + beer_id);

	builder_ajax_send("setstyle", "beer_id=" + beer_id + "&style_id=" + document.getElementById('style_select').value);

	var recipe =
	{
		malt_n: 0,
		malts: [],
		hops_n: 0,
		hops: [],
		yeast_n: 0,
		yeasts: []
	};

	table = document.getElementById('malt_table');
	if (table.rows.length > 2)
	{
		for (i = 2; row = table.rows[i]; i++)
		{
			builder_ajax_send("addmalt", 
				"beer_id=" + beer_id + 
				"&ing_id=" + row.attributes['data-malt_id'].value + 
				"&amount=" + row.cells[0].children[0].value
			);

			recipe.malt_n += 1;
			recipe.malts[recipe.malt_n-1] = { "id": row.attributes['data-malt_id'].value, "mass": row.cells[0].children[0].value};
		}
	}

	table = document.getElementById('hops_table');
	if (table.rows.length > 2)
	{
		for (i = 2; row = table.rows[i]; i++)
		{
			builder_ajax_send("addhops", 
				"beer_id=" + beer_id + 
				"&ing_id=" + row.attributes['data-hop_id'].value + 
				"&amount=" + row.cells[0].children[0].value + 
				"&time=" + row.cells[1].children[0].value
			);

			recipe.hops_n += 1;
			recipe.hops[recipe.hops_n-1] = { "id": row.attributes['data-hop_id'].value, "mass": row.cells[0].children[0].value, "time": row.cells[1].children[0].value};
		}
	}

	table = document.getElementById('yeast_table');
	if (table.rows.length > 2)
	{
		for (i = 2; row = table.rows[i]; i++)
		{
			builder_ajax_send("addyeast", 
				"beer_id=" + beer_id + 
				"&ing_id=" + row.attributes['data-yeast_id'].value + 
				"&amount=" + row.cells[0].children[0].value
			);

			recipe.yeast_n += 1;
			recipe.yeasts[recipe.yeast_n-1] = { "id": row.attributes['data-yeast_id'].value, "mass": row.cells[0].children[0].value};
		}
	}

	console.log(recipe);
}

function calculate()
{
	builder_ajax_send("calculate", "beer_id=" + beer_id);
}

function load_recipe(beer_id)
{
	builder_ajax_send_sync("getmalt", "");
	builder_ajax_send_sync("gethops", "");
	builder_ajax_send_sync("getyeasts", "");
	builder_ajax_send_sync("getstyles", "");

	builder_ajax_send("editbeer", "beer_id=" + beer_id);
}
