<?php
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" lang="<?php print $language->language; ?>" dir="<?php print $language->dir; ?>">

<head>
  <title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
</head>
<body class="<?php print $body_classes; ?>">
	<div id="bodwrap">
	    <div id="header">
	
			<div id="topstrip">
				<div class="page">
					<?php print $topstrip; ?>
				</div> <!-- end page -->
			</div> <!-- end topstrip -->
	    
		    <div class="page" id="hdr-main">
		          <h1 id="site-name"><a href="/"><?php print $site_name; ?></a></h1>
		      
		      <?php if ($header): ?>
		          <?php print $header; ?>
		      <?php endif; ?>
		
		    </div><!-- end .page -->
		    
		    
		    
	    </div><!-- end #header -->
		
		   <div id="nav">
		    	<div class="page">
		    		<?php print $nav; ?>
		    	</div>
		    </div>


		    <div id="notices" class="strip">
		    	<div class="page">
                          <?php print $notices; ?>
		    	</div>
		    </div>

		    
		    <div id="search" class="strip">
		    	<div class="page">
		    		<h3>Find</h3>
		    		<div id="searchtabs" class="hp-section">
		    			
		    			<?php print $search; ?>
		    		</div> <!-- end searchtabs -->
		    	</div>
		    
		    </div>

		  
	    <div id="main" class="strip">

	    </div> <!-- end #main -->

  			<div class="push"></div>
  	  </div> <!-- end #bodwrap -->
	  <div id="footer">
		<div class="page">
	    <?php print $footer; ?>
		</div> <!-- end .page -->
	  </div> <!-- end #footer -->
	  
	<?php print $closure; ?>
</body>
</html>