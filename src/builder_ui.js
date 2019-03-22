beer_id = 0;
malt_array = [];
malt_n = 0;
hops_array = [];
hops_n = 0;
yeast_array = [];
yeast_n = 0;

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
		document.getElementById('beer_id').innerText = "#" + beer_id;
		document.getElementById('editor').classList.remove('locked');
		builder_ajax_send("getmalt", "");
		builder_ajax_send("gethops", "");
		builder_ajax_send("getyeasts", "");
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

function table_add_malt(malt_id)
{
	var table = document.getElementById('malt_table');
	table.innerHTML += "<tr id=\"malt_" + malt_n + "\" data-malt_id=\"" + malt_id + "\"><td><input type=\"number\" value=\"0\"></input> lbs</td><td>" + malt_array[malt_id].name + "</td><td>" + malt_array[malt_id].potential + "</td><td>" + malt_array[malt_id].mcu + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('malt_" + malt_n + "'); malt_n += -1;\"></input></td></tr>";
	malt_n += 1;
}

function table_add_hop(hop_id)
{
	var table = document.getElementById('hops_table');
	table.innerHTML += "<tr id=\"hops_" + hops_n + "\" data-hop_id=\"" + hop_id + "\"><td><input type=\"number\" value=\"0\"></input> oz</td><td><input type=\"number\" value=\"0\"></input> min</td><td>" + hops_array[hop_id].name + "</td><td>" + hops_array[hop_id].alpha + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('hops_" + hops_n + "'); hops_n += -1;\"></input></td></tr>";
	hops_n += 1;
}

function table_add_yeast(yeast_id)
{
	var table = document.getElementById('yeast_table');
	table.innerHTML += "<tr id=\"yeast_" + yeast_n + "\" data-yeast_id=\"" + yeast_id + "\"><td><input type=\"number\" value=\"0\"></input> pkg</td><td>" + yeast_array[yeast_id].name + "</td><td>" + yeast_array[yeast_id].attenuation + "</td><td>" + (["none","very low","low","mid-low","medium","mid-high","high","extreme"])[yeast_array[yeast_id].flocculation] + "</td><td><input type=\"button\" value=\" - \" onclick=\"table_rm_row('yeast_" + yeast_n + "'); yeast_n += -1;\"></input></td></tr>";
	yeast_n += 1;
}

function table_rm_row(row_id)
{
	var row = document.getElementById(row_id);
	row.parentNode.removeChild(row);
}

function save_recipe()
{
	var i, row, table;
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
			malt_n += 1;
			recipe.malts[malt_n] = { "id": row.attributes['data-malt_id'].value, "mass": row.cells[0].children[0].value};
		}
	}

	table = document.getElementById('hops_table');
	if (table.rows.length > 2)
	{
		for (i = 2; row = table.rows[i]; i++)
		{
			hops_n += 1;
			recipe.hops[hops_n] = { "id": row.attributes['data-hop_id'].value, "mass": row.cells[0].children[0].value, "time": row.cells[1].children[0].value};
		}
	}

	table = document.getElementById('yeast_table');
	if (table.rows.length > 2)
	{
		for (i = 2; row = table.rows[i]; i++)
		{
			hops_n += 1;
			recipe.hops[hops_n] = { "id": row.attributes['data-hop_id'].value, "mass": row.cells[0].children[0].value, "time": row.cells[1].children[0].value};
		}
	}

	console.log(recipe);
}
