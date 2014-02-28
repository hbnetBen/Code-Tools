<?php
// dept = 1
define("DEBUG", false);

define('DOMAIN', "http://irokopartners.com");


function crawl_page($url)
{
	static $visited = array();
	static $visitedIndex = array();
	static $position = 0;

	if(strrpos($url, '/') == strlen($url) - 1){
        $url = trim(substr_replace($url ,"",-1));
	}

	if(@!isset($visited[$url])){
    	$visitedIndex[] = $url;
	}


   	$visited[$url] = true;

   	if(DEBUG)
		print "Position: " . $position . " SizeofArraay: " . count($visited) . "\n";

    $dom = new DOMDocument('1.0');
    @$dom->loadHTMLFile($url);

    $anchors = $dom->getElementsByTagName('a');


    $staticAssets = $dom->getElementsByTagName('link');

   
   	echo "Page: " . $url . "\n";

    foreach ($staticAssets as $element) {
    	$cssjs = $element->getAttribute('href');
    	echo "\t\t" . $cssjs. "\n";
    }

    $scriptsAssets = $dom->getElementsByTagName('script');

    foreach ($scriptsAssets as $element) {
    	$cssjs = $element->getAttribute('src');
    	echo "\t\t" . $cssjs. "\n";
    }


    $imageAssets = $dom->getElementsByTagName('img');

    foreach ($imageAssets as $element) {
    	$cssjs = $element->getAttribute('src');
    	echo "\t\t" . $cssjs . "\n";
    }

    foreach ($anchors as $element) {

        $href = $element->getAttribute('href');

        if (0 !== strpos($href, 'http')) {

        	if (0 === strpos($href, 'mailto') || 0 === strpos($href, '#')){
        		continue;
        	}


        	if (0 !== strpos($href, '/')){
        		$href = "/".$href;
        	}



        	$completeurl = DOMAIN . $href;

        	$completeurl = trim(str_replace("./","/", $completeurl));

        	if(strrpos($completeurl, '/') == strlen($completeurl) - 1){
        		$completeurl = trim(substr_replace($completeurl ,"",-1));
        	}

        	if(!array_key_exists ($completeurl, $visited)){
        		//echo "\t\t".$completeurl . " ===== Doesnt Exists:======\n";
        		$visited[$completeurl] = false;
    			$visitedIndex[] = $completeurl;	
        	}
         	
        }
        
        //
    }

    if(DEBUG){
	    print "The Visited Array: \n";
	    foreach ($visited as $key => $value) {
	    	print "VURL:\t" .$key. " value:". $value . "\n";
	    }
	}

	if(DEBUG){
		print "the VisitedIndex araaay \n";
    	foreach ($visitedIndex as $key => $value) {
    		print "URL:\t" . $value . "\n";
    	}
    }

    $position++;

    if ($position == count($visited)) {
		return;
	}

	if(DEBUG)
    	print "next on List: " . $visitedIndex[$position]."\n";

    crawl_page($visitedIndex[$position]);

}

crawl_page(DOMAIN);