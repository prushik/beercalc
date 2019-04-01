PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

create table users(
	name varchar(64),
	salt varchar(64),
	hash blob null,
	id integer primary key
);

create table malts(
	name varchar(64),
	potential double,
	pts_potential double,
	mcu double,
	id integer primary key
);

create table hops(
	name varchar(64),
	alpha double,
	type int,
	id integer primary key
);

create table yeasts(
	name varchar(64),
	attenuation double,
	flocculation int,
	temperature int,
	id integer primary key
);

create table ingredient_types(
	name varchar(64),
	id integer primary key
);

create table ingredients(
	recipe_id int,
	ingredient_id int,
	quantity double,
	time unsigned int,
	adjustment double null,
	type int,
	id integer primary key
);

create table recipe(
	name varchar(64),
	author varchar(64),
	style_id int null,
	volume double,
	boil_volume double,
	malt_n unsigned int,
	hops_n unsigned int,
	yeast_n unsigned int,
	id integer primary key
);

create table style(
	name varchar(64),
	og_min double,
	og_max double,
	fg_min double,
	fg_max double,
	ibu_min double,
	ibu_max double,
	color_min double,
	color_max double,
	carb_min double,
	carb_max double,
	abv_min double,
	abv_max double,
	id integer primary key
);

COMMIT;

BEGIN TRANSACTION;

insert into ingredient_types values('malt',0);
insert into ingredient_types values('hops',1);
insert into ingredient_types values('yeast',2);

insert into malts values('pilsner malt', 1.034, 34, 1, 0);
insert into malts values('2-row pale malt', 1.037, 37, 1.8, 1);
insert into malts values('6-row pale malt', 1.035, 35, 1.8, 2);
insert into malts values('maris otter malt', 1.038, 38, 3, 3);
insert into malts values('red caramel malt', 1.035, 35, 20, 4);
insert into malts values('crystal 45l malt', 1.034, 34, 45, 5);
insert into malts values('biscuit malt', 1.030, 30, 45, 6);
insert into malts values('black patent malt', 1.022, 22, 500, 7);
insert into malts values('special b malt', 1.022, 22, 45, 8);
insert into malts values('peated malt', 1.037, 37, 1.5, 9);
insert into malts values('flaked maize', 1.040, 40, 1, 10);
insert into malts values('molasses', 1.036, 36, 80, 11);
insert into malts values('munich malt', 1.034, 34, 10, 12);
insert into malts values('vienna malt', 1.035, 35, 3.75, 13);
insert into malts values('honey', 1.032, 32, 1, 14);
insert into malts values('amber malt', 1.032, 32, 27, 15);
insert into malts values('brown malt', 1.033, 33, 65, 16);
insert into malts values('chocolate malt', 1.034, 34, 350, 17);
insert into malts values('malted oats', 1.037, 37, 1, 18);
insert into malts values('malted rye', 1.037, 37, 3.7, 19);
insert into malts values('white wheat malt', 1.040, 40, 2, 20);
insert into malts values('dark wheat malt', 1.039, 39, 9, 21);
insert into malts values('caramel/crystal malt 10', 1.035, 35, 10, 22);
insert into malts values('caramel/crystal malt 20', 1.035, 35, 20, 23);
insert into malts values('caramel/crystal malt 30', 1.035, 35, 30, 24);
insert into malts values('caramel/crystal malt 40', 1.035, 35, 40, 25);
insert into malts values('caramel/crystal malt 60', 1.036, 36, 60, 26);
insert into malts values('caramel/crystal malt 80', 1.035, 36, 80, 27);
insert into malts values('caramel/crystal malt 90', 1.035, 36, 90, 28);
insert into malts values('caramel/crystal malt 120', 1.035, 36, 120, 29);
insert into malts values('caramel/crystal malt 150', 1.035, 36, 150, 30);
insert into malts values('carapils malt', 1.033, 33, 2, 31);

insert into hops values('tettnanger',3.7,0,0);
insert into hops values('glacier',5.8,0,1);
insert into hops values('eroica',13.5,0,2);
insert into hops values('wye viking',9.0,0,3);
insert into hops values('liberty',4.5,0,4);
insert into hops values('magnum',14.5,0,5);
insert into hops values('ultra',3,0,6);
insert into hops values('kent golding',5,0,7);
insert into hops values('cascade',5.5,0,8);
insert into hops values('centennial',10.5,0,9);
insert into hops values('citra',13,0,10);
insert into hops values('columbus',15.5,0,11);
insert into hops values('mosaic',12.5,0,12);
insert into hops values('saaz (czech)',3.5,0,13);
insert into hops values('simcoe',13,0,14);
insert into hops values('hallertau',4,0,15);
insert into hops values('saaz (usa)',4,0,16);
insert into hops values('spalt',4.1,0,17);
insert into hops values('amarillo',9.5,0,18);
insert into hops values('willamette',5.25,0,19);
insert into hops values('fuggle',4.3,0,20);
insert into hops values('nugget',14.75,0,21);
insert into hops values('ahtanum',6,0,22);

insert into yeasts values('american ale 1056', 75, 4, 66, 0);
insert into yeasts values('american ale II 1272', 74, 5, 66, 1);
insert into yeasts values('american ale wlp060', 76, 5, 70, 2);
insert into yeasts values('liberty bell ale m36', 76, 6, 68, 3);
insert into yeasts values('london fog ale wlp066', 68, 6, 68, 4);
insert into yeasts values('scottish ale 1728', 76, 7, 70, 5);

COMMIT;

BEGIN TRANSACTION;

-- example recipes
insert into recipe values ("scotch ale","philip rushik",22,5,5.5,4,2,1,0);

insert into ingredients values (0,1,12,60,null,0,0);
insert into ingredients values (0,9,2,60,null,0,1);
insert into ingredients values (0,4,1,60,null,0,2);
insert into ingredients values (0,5,1,60,null,0,3);

insert into ingredients values (0,0,1,45,null,1,4);
insert into ingredients values (0,1,0.5,15,null,1,5);

insert into ingredients values (0,4,1,0,null,2,6);

-- 
insert into recipe values ("american ale","philip rushik",58,5,5.5,3,3,1,1);

insert into ingredients values (1,2,10,60,null,0,7);
insert into ingredients values (1,10,2,60,null,0,8);
insert into ingredients values (1,11,1,60,null,0,9);

insert into ingredients values (1,4,1.0,60,null,1,10);
insert into ingredients values (1,5,0.5,15,null,1,11);
insert into ingredients values (1,6,0.5,5,null,1,12);

insert into ingredients values (1,3,1,0,null,2,13);

--
insert into recipe values ("poor richard's colonial ale","tony simmons",40,5,5.5,6,3,1,2);

insert into ingredients values (2,3,8.5,90,null,0,14);
insert into ingredients values (2,10,2.75,90,null,0,15);
insert into ingredients values (2,6,1.75,90,null,0,16); -- biscuit
insert into ingredients values (2,8,1,90,null,0,17); -- special roast
insert into ingredients values (2,7,0.125,90,null,0,18); -- black patent
insert into ingredients values (2,11,0.25,90,null,0,19);

insert into ingredients values (2,7,0.5,60,null,1,20);
insert into ingredients values (2,7,0.75,45,null,1,21);
insert into ingredients values (2,7,0.5,30,null,1,22);

insert into ingredients values (2,3,1,0,null,2,23);

COMMIT;
