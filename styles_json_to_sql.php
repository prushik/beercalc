#!/usr/bin/php
<?php

$styles = json_decode(file_get_contents('styles_fmt.json'));

$index = 0;

foreach ($styles as $category => $substyles)
{
	foreach ($substyles as $name => $style)
	{
		echo "insert into style values (\"" . $name . "\"," .
			$style->gu[0] . "," . $style->gu[1] . "," .
			$style->fg[0] . "," . $style->fg[1] . "," .
			$style->ibu[0] . "," . $style->ibu[1] . "," .
			$style->srm[0] . "," . $style->srm[1] . "," .
			$style->carb[0] . "," . $style->carb[1] . "," .
			$style->abv[0] . "," . $style->abv[1] . "," .
			$index++ .
			");\n";
	}
}
