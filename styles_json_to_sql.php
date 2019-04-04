#!/usr/bin/php
<?php

$styles = json_decode(file_get_contents('ba2018_styles.json'));

$index = 0;

foreach ($styles->styles as $category => $substyles)
{
	foreach ($substyles->styles as $style)
	{
		echo "insert into style values (\"" . $style->name . "\"," .
			$style->og[0] . "," . $style->og[1] . "," .
			$style->fg[0] . "," . $style->fg[1] . "," .
			$style->ibu[0] . "," . $style->ibu[1] . "," .
			$style->srm[0] . "," . $style->srm[1] . "," .
			0 . "," . 0 . "," .
			$style->abv[0] . "," . $style->abv[1] . "," .
			$index++ .
			");\n";
	}
}
