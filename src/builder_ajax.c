#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "web.h"
#include "beer.h"
#include "ibu.h"
#include "mash.h"
#include "yeast.h"
#include <sqlite3.h>

static struct recipe beer = {0};
static char buffer[64];
static unsigned int buf_len = 0;
static sqlite3 *db;
static sqlite3_stmt *qry;


#define ACTION_GETBEER		0x00
#define ACTION_ADDBEER		0x01
#define ACTION_ADDMALT		0x02
#define ACTION_ADDHOP		0x03
#define ACTION_ADDYEAST		0x04
#define ACTION_GETMALT		0x05
#define ACTION_GETHOPS		0x06
#define ACTION_GETSTYLES	0x07
#define ACTION_GETYEASTS	0x08
#define ACTION_SETMALT		0x09
#define ACTION_SETHOPS		0x0a
#define ACTION_SETYEASTS	0x0b

int main(int argc, char **argv)
{
	int i;
	unsigned long int beer_id = 0;
	unsigned long int style_id = 0;
	unsigned long int action = 0;
	unsigned long int ing_id = 0;
	double amount = 0, time = 0;
	char *beer_name;
	char *author_name;

	for (i=1; i<argc; i++)
	{
		// can't use str() here because gcc optimizer can mess this up (strncmp is a builtin)
		if (strncmp(argv[i], "beer_id", 7) == 0)
		{
			beer_id = strtoul(&argv[i][8], NULL, 0);
		}
		if (strncmp(argv[i], "action", 6) == 0)
		{
			if (strncmp(&argv[i][7], "getbeer", 7) == 0)
				action = ACTION_GETBEER;
			if (strncmp(&argv[i][7], "addbeer", 7) == 0)
				action = ACTION_ADDBEER;
			if (strncmp(&argv[i][7], "addmalt", 7) == 0)
				action = ACTION_ADDMALT;
			if (strncmp(&argv[i][7], "addhop", 6) == 0)
				action = ACTION_ADDHOP;
			if (strncmp(&argv[i][7], "addyeast", 8) == 0)
				action = ACTION_ADDYEAST;
			if (strncmp(&argv[i][7], "getmalt", 7) == 0)
				action = ACTION_GETMALT;
			if (strncmp(&argv[i][7], "gethops", 7) == 0)
				action = ACTION_GETHOPS;
			if (strncmp(&argv[i][7], "getyeasts", 9) == 0)
				action = ACTION_GETYEASTS;
			if (strncmp(&argv[i][7], "getstyles", 9) == 0)
				action = ACTION_GETSTYLES;
			if (strncmp(&argv[i][7], "setmalt", 7) == 0)
				action = ACTION_SETMALT;
			if (strncmp(&argv[i][7], "sethops", 7) == 0)
				action = ACTION_SETHOPS;
			if (strncmp(&argv[i][7], "setyeasts", 9) == 0)
				action = ACTION_SETYEASTS;
		}
		if (strncmp(argv[i], "ing_id", 6) == 0)
		{
			ing_id = strtoul(&argv[i][7], NULL, 0);
		}
		if (strncmp(argv[i], "amount", 6) == 0)
		{
			amount = strtod(&argv[i][7], NULL);
		}
		if (strncmp(argv[i], "time", 4) == 0)
		{
			time = strtod(&argv[i][5], NULL);
		}
		if (strncmp(argv[i], "name", 4) == 0)
		{
			beer_name = &argv[i][5];
		}
		if (strncmp(argv[i], "author", 6) == 0)
		{
			author_name = &argv[i][7];
		}
		if (strncmp(argv[i], "style_id", 8) == 0)
		{
			style_id = strtoul(&argv[i][9], NULL, 0);
		}
	}

	sqlite3_open(DATABASE, &db);

	if (action == ACTION_GETBEER)
		recipe_json(beer_id);

	if (action == ACTION_ADDBEER)
		add_recipe(beer_name, style_id, author_name);

	if (action == ACTION_GETMALT)
		malt_json();

	if (action == ACTION_GETHOPS)
		hops_json();

	if (action == ACTION_GETYEASTS)
		yeasts_json();

	if (action == ACTION_GETSTYLES)
		styles_json();

	sqlite3_close(db);
}

int add_recipe(const char *name, const unsigned long int style_id, const char *author)
{
	sqlite3_prepare_v2(db, str("insert into recipe (name, style_id, author, volume, malt_n, hops_n, yeast_n) values (?, ?, ?, 5, 0, 0, 0);"), &qry, NULL);
	sqlite3_bind_text(qry, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int64(qry, 2, style_id);
	sqlite3_bind_text(qry, 3, author, -1, SQLITE_STATIC);

	char sqlstr[512];

	int code, len;
	while ((code = sqlite3_step(qry)) != SQLITE_DONE)
	{
		sprintf(sqlstr,"%d %s\n%n", code, sqlite3_errmsg(db), &len);
		write(2, sqlstr, len);
		break;
	}

	sqlite3_finalize(qry);

	int beer_id = sqlite3_last_insert_rowid(db);

	write(1, str("{ \"beer_id\":"));
	sprintf(buffer, "%d%n", beer_id, &buf_len);
	write(1, buffer, buf_len);
	write(1, str(" }\n"));

}

int update_malt(unsigned long int beer_id, unsigned long int index_id, unsigned long int malt_id, unsigned long int mass)
{
	sqlite3_prepare_v2(db, str("update recipe set malt_n = ? where id = ?;"), &qry, NULL);
/*	sqlite3_bind_text(qry, 1, name, -1, SQLITE_STATIC);
	sqlite3_bind_int64(qry, 2, style_id);
	sqlite3_bind_text(qry, 3, author, -1, SQLITE_STATIC);

	char sqlstr[512];

	int code, len;
	while ((code = sqlite3_step(qry)) != SQLITE_DONE)
	{
		sprintf(sqlstr,"%d %s\n%n", code, sqlite3_errmsg(db), &len);
		write(2, sqlstr, len);
		break;
	}

	sqlite3_finalize(qry);

	beer_id = sqlite3_last_insert_rowid(db);

	write(1, str("{ \"beer_id\":"));
	sprintf(buffer, "%d%n", beer_id, &buf_len);
	write(1, buffer, buf_len);
	write(1, str(" }\n"));
*/
}

int malt_json()
{
	int i = 0;

	write(1, str("{ \"malts\": ["));

	sqlite3_prepare_v2(db, str("select name, potential, mcu, id from malts order by pts_potential desc;"), &qry, NULL);
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		if (i) write(1, str(", ")); i++;
		write(1, str("{ \"name\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 0));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"potential\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 1));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"mcu\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 2));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"id\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 3));
		write(1, buffer, strlen(buffer));
		write(1, str("\" }"));
	}
	sqlite3_finalize(qry);

	write(1, str("] }\n"));
}

int hops_json()
{
	int i = 0;

	write(1, str("{ \"hops\": ["));

	sqlite3_prepare_v2(db, str("select name, alpha, id from hops order by alpha;"), &qry, NULL);
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		if (i) write(1, str(", ")); i++;
		write(1, str("{ \"name\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 0));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"alpha\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 1));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"id\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 2));
		write(1, buffer, strlen(buffer));
		write(1, str("\" }"));
	}
	sqlite3_finalize(qry);

	write(1, str("] }\n"));
}

int yeasts_json()
{
	int i = 0;

	write(1, str("{ \"yeasts\": ["));

	sqlite3_prepare_v2(db, str("select name, attenuation, flocculation, id from yeasts order by attenuation;"), &qry, NULL);
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		if (i) write(1, str(", ")); i++;
		write(1, str("{ \"name\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 0));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"attenuation\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 1));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"flocculation\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 2));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"id\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 3));
		write(1, buffer, strlen(buffer));
		write(1, str("\" }"));
	}
	sqlite3_finalize(qry);

	write(1, str("] }\n"));
}

int styles_json()
{
	int i = 0;

	write(1, str("{ \"styles\": ["));

//	name varchar(64),
//	og_min double,
//	og_max double,
//	fg_min double,
//	fg_max double,
//	ibu_min double,
//	ibu_max double,
//	color_min double,
//	color_max double,
//	carb_min double,
//	carb_max double,
//	abv_min double,
//	abv_max double,

	sqlite3_prepare_v2(db, str("select name, og_min, og_max, fg_min, fg_max, ibu_min, ibu_max, color_min, color_max, carb_min, carb_max, abv_min, abv_max, id from style order by color_max;"), &qry, NULL);
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		if (i) write(1, str(", ")); i++;
		write(1, str("{ \"name\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 0));
		write(1, buffer, strlen(buffer));
		write(1, str("\", \"og\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 1));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 2));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"fg\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 3));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 4));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"ibu\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 5));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 6));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"color\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 7));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 8));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"carb\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 9));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 10));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"abv\": ["));
		strcpy(buffer, sqlite3_column_text(qry, 11));
		write(1, buffer, strlen(buffer));
		write(1, str(", "));
		strcpy(buffer, sqlite3_column_text(qry, 12));
		write(1, buffer, strlen(buffer));
		write(1, str("], \"id\": \""));
		strcpy(buffer, sqlite3_column_text(qry, 13));
		write(1, buffer, strlen(buffer));
		write(1, str("\" }"));
	}
	sqlite3_finalize(qry);

	write(1, str("] }\n"));
}

int recipe_json(unsigned long int beer_id)
{
	int i;

	sqlite3_prepare_v2(db, str("select name, author, volume, malt_n, hops_n, yeast_n, id from recipe where id = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);

	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.name, sqlite3_column_text(qry, 0));
		strcpy(beer.author, sqlite3_column_text(qry, 1));
		beer.vol = sqlite3_column_double(qry, 2);
		beer.malt_n = sqlite3_column_int(qry, 3);
		beer.hop_n = sqlite3_column_int(qry, 4);
		beer.yeast_n = sqlite3_column_int(qry, 5);
		if (beer_id != sqlite3_column_int(qry, 6))
		{
			write(1, str("No recipe with that number found. Something is wrong.\n"));
			// error
		}
	}
	sqlite3_finalize(qry);

	beer.malts = malloc(beer.malt_n * sizeof(struct malt));
	beer.hops = malloc(beer.hop_n * sizeof(struct hop));
	beer.yeasts = malloc(beer.yeast_n * sizeof(struct yeast));


	// Get the malts
	sqlite3_prepare_v2(db, str("select malts.name, malts.pts_potential, malts.mcu, ingredients.quantity, ingredients.adjustment from ingredients left join malts on malts.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_MALT);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.malts[i].name, sqlite3_column_text(qry, 0));
		beer.malts[i].pts_potential = sqlite3_column_double(qry, 1);
		beer.malts[i].mcu = sqlite3_column_double(qry, 2);
		beer.malts[i].mass = sqlite3_column_double(qry, 3);
		i++;
	}
	sqlite3_finalize(qry);

	// Get the hops
	sqlite3_prepare_v2(db, str("select hops.name, hops.alpha, hops.type, ingredients.quantity, ingredients.time, ingredients.adjustment from ingredients left join hops on hops.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_HOPS);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.hops[i].name, sqlite3_column_text(qry, 0));
		beer.hops[i].alpha = sqlite3_column_double(qry, 1);
//		beer.hops[i].type = sqlite3_column_int(qry, 2);
		beer.hops[i].mass = sqlite3_column_double(qry, 3);
		beer.hops[i].time = sqlite3_column_int(qry, 4);
		i++;
	}
	sqlite3_finalize(qry);

	// Get the hops
	sqlite3_prepare_v2(db, str("select yeasts.name, yeasts.attenuation, ingredients.quantity, ingredients.time, ingredients.adjustment from ingredients left join yeasts on yeasts.id = ingredients.ingredient_id where recipe_id = ? and ingredients.type = ?;"), &qry, NULL);
	sqlite3_bind_int(qry, 1, beer_id);
	sqlite3_bind_int(qry, 2, ING_TYPE_YEAST);

	i = 0;
	while (sqlite3_step(qry) != SQLITE_DONE)
	{
		strcpy(beer.yeasts[i].name, sqlite3_column_text(qry, 0));
		beer.yeasts[i].attenuation = sqlite3_column_double(qry, 1);
		beer.yeasts[i].amount = sqlite3_column_double(qry, 2);
		beer.yeasts[i].time = sqlite3_column_int(qry, 3);
		i++;
	}
	sqlite3_finalize(qry);

	calculate_recipe(&beer);

	write(1, str(
		"{\n"
	));
	write(1, str(
		"	\"name\" : \""
	));
	write(1, beer.name, strlen(beer.name));

	write(1, str(
		"\",\n"
		"	\"author\" : \""
	));
	write(1, beer.author, strlen(beer.author));
	write(1, str(
		"\",\n"
	));

	write(1, str("	\"og\" : \""));
	sprintf(buffer, "%.4lf%n", beer.og, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("\",\n"));

	write(1, str("	\"fg\": \""));
	sprintf(buffer, "%.4lf%n", beer.fg, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("\",\n"));

	write(1, str("	\"abv\": \""));
	sprintf(buffer, "%.1lf%n", beer.abv, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("\",\n"));

	write(1, str("	\"ibu\": \""));
	sprintf(buffer, "%.1lf%n", beer.ibu, &buf_len);
	write(1, buffer, buf_len);
	write(1, str("\",\n"));

	write(1, str(
		"	\"malts\":\n"
		"	[\n"
	));
	for (i=0; i < beer.malt_n; i++)
	{
		write(1, str("		{\n"));
		write(1, str("			\"name\" : \""));
		write(1, beer.malts[i].name, strlen(beer.malts[i].name));
		write(1, str("\",\n"));

		write(1, str("			\"weight\" : \""));
		sprintf(buffer, "%.2lf%n", beer.malts[i].mass, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\",\n"));

		write(1, str("			\"potential\" : \""));
		sprintf(buffer, "%.1lf%n", beer.malts[i].pts_potential, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\"\n"));
		if (i+1<beer.malt_n)
			write(1, str("		},\n"));
		else
			write(1, str("		}\n"));
	}
	write(1, str("	],\n"));

	write(1, str(
		"	\"hops\":\n"
		"	[\n"
	));
	for (i=0; i < beer.hop_n; i++)
	{
		write(1, str("		{\n"));
		write(1, str("			\"name\" : \""));
		write(1, beer.hops[i].name, strlen(beer.hops[i].name));
		write(1, str("\",\n"));

		write(1, str("			\"mass\" : \""));
		sprintf(buffer, "%.2lf%n", beer.hops[i].mass, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\",\n"));

		write(1, str("			\"time\" : \""));
		sprintf(buffer, "%d%n", beer.hops[i].time, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\",\n"));

		write(1, str("			\"time\" : \""));
		sprintf(buffer, "%.1lf%n", beer.hops[i].alpha, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\"\n"));

		if (i+1<beer.hop_n)
			write(1, str("		},\n"));
		else
			write(1, str("		}\n"));
	}
	write(1, str("	],\n"));

	write(1, str(
		"	\"yeast\":\n"
		"	[\n"
	));
	for (i=0; i < beer.yeast_n; i++)
	{
		write(1, str("		{\n"));
		write(1, str("			\"name\" : \""));
		write(1, beer.yeasts[i].name, strlen(beer.yeasts[i].name));
		write(1, str("\",\n"));

		write(1, str("			\"amount\" : \""));
		sprintf(buffer, "%.2lf%n", beer.yeasts[i].amount, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\",\n"));

		write(1, str("			\"attenuation\" : \""));
		sprintf(buffer, "%.1lf%n", beer.yeasts[i].attenuation, &buf_len);
		write(1, buffer, buf_len);
		write(1, str("\"\n"));

		if (i+1<beer.yeast_n)
			write(1, str("		},\n"));
		else
			write(1, str("		}\n"));
	}
	write(1, str("	]\n"));

	write(1, str(
		"}\n"
	));

	return 1;
}
