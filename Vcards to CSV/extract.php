<?php

$vv = null;

for ($i=1; $i < 472; $i++) {

	$handle = file_get_contents($i.".vcf");

	$aa = explode("\n", $handle);

	$bb = explode(":", $aa[3]);

	$vv .= trim($bb[1]).",";
}
	echo $vv;

?>
