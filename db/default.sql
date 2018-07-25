PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

create table users(
	name varchar(64),
	salt varchar(64),
	hash blob null,
	id unsigned int primary key
);

create table malts(
	name varchar(64),
	potential double,
	pts_potential double,
	mcu double,
	id unsigned int primary key
);

create table hops(
	name varchar(64),
	alpha double,
	type int,
	id unsigned int primary key
);

create table yeasts(
	name varchar(64),
	attenuation double,
	flocculation int,
	temperature int,
	id unsigned int primary key
);

create table ingredient_types(
	name varchar(64),
	id unsigned int primary key
);

create table ingredients(
	recipe_id unsigned int,
	ingredient_id unsigned int,
	quantity double,
	time unsigned int,
	adjustment double null,
	type unsigned int,
	id unsigned int primary key
);

create table recipe(
	name varchar(64),
	author varchar(64),
	style_id unsigned int null,
	volume double,
	boil_volume double,
	malt_n unsigned int,
	hops_n unsigned int,
	yeast_n unsigned int,
	id unsigned int primary key
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
	id unsigned int primary key
);

COMMIT;

BEGIN TRANSACTION;

insert into ingredient_types values('malt',0);
insert into ingredient_types values('hops',1);
insert into ingredient_types values('yeast',2);

insert into malts values('pilsner malt', 1.034, 34, 1, 0);
insert into malts values('2-row pale malt', 1.037, 37, 2, 1);
insert into malts values('6-row pale malt', 1.033, 33, 2, 2);
insert into malts values('maris otter malt', 1.038, 38, 3, 3);
insert into malts values('red caramel malt', 1.035, 35, 20, 4);
insert into malts values('crystal 45l malt', 1.034, 34, 45, 5);
insert into malts values('biscuit malt', 1.030, 30, 45, 6);
insert into malts values('black patent malt', 1.022, 22, 45, 7);
insert into malts values('special b malt', 1.022, 22, 45, 8);
insert into malts values('peated malt', 1.037, 37, 1.5, 9);
insert into malts values('flaked maize', 1.040, 40, 1, 10);
insert into malts values('molasses', 1.036, 36, 1, 11);

insert into hops values('tettnanger',3.7,0,0);
insert into hops values('glacier',5.8,0,1);
insert into hops values('eroica',13.5,0,2);
insert into hops values('wye viking',9.0,0,3);
insert into hops values('liberty',4.5,0,4);
insert into hops values('magnum',14.5,0,5);
insert into hops values('ultra',3,0,6);
insert into hops values('kent goldings',5,0,7);

insert into yeasts values('american ale 1056', 75, 2, 66, 0);
insert into yeasts values('american ale II 1272', 74, 3, 66, 1);
insert into yeasts values('american ale wlp060', 76, 3, 70, 2);
insert into yeasts values('liberty bell ale m36', 76, 4, 68, 3);
insert into yeasts values('london fog ale wlp066', 68, 4, 68, 4);
insert into yeasts values('scottish ale 1728', 76, 5, 70, 5);

COMMIT;

BEGIN TRANSACTION;

-- example recipes
insert into recipe values ("tastes like scotch ale","philip rushik",22,5,5.5,4,2,1,0);

insert into ingredients values (0,1,12,60,null,0,0);
insert into ingredients values (0,9,2,60,null,0,1);
insert into ingredients values (0,4,1,60,null,0,2);
insert into ingredients values (0,5,1,60,null,0,3);

insert into ingredients values (0,0,1,45,null,1,4);
insert into ingredients values (0,1,0.5,15,null,1,5);

insert into ingredients values (0,4,1,0,null,2,6);

-- 
insert into recipe values ("president donald drunk","philip rushik",58,5,5.5,3,3,1,1);

insert into ingredients values (1,2,10,60,null,0,7);
insert into ingredients values (1,10,2,60,null,0,8);
insert into ingredients values (1,11,1,60,null,0,9);

insert into ingredients values (1,4,1.0,60,null,1,10);
insert into ingredients values (1,5,0.5,15,null,1,11);
insert into ingredients values (1,6,0.5,5,null,1,12);

insert into ingredients values (1,3,1,0,null,2,13);

--
insert into recipe values ("poor richard's colonial ale","tony simmons",40,5,5.5,3,3,1,2);

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
