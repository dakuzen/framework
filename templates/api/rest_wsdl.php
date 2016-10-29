<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>WSDL</title>
    <link href='https://fonts.googleapis.com/css?family=Roboto:400,100,700,500' rel='stylesheet' type='text/css'>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <script src="//maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
    <link href="//maxcdn.bootstrapcdn.com/bootstrap/3.3.1/css/bootstrap.min.css" type="text/css" rel="stylesheet" />
  </head>
  <body>

<style>
		* {
			font-family: 'Roboto';
		}

		a:hover, a:focus {
		    text-decoration: none;
		}

		.method {
			float:left;
			width: 100px;
		}

		.method.get,
		.parameter-method.get {
			color: #5CB85C;
		}

		.method.post,
		.parameter-method.post {
			color: #5BC0DE;
		}

		.method.delete,
		.parameter-method.delete {
			color: #D9534F;
		}

		.method.put,
		.parameter-method.put {
			color: #F0AD4E;
		}

		.content {
			overflow: hidden;
		}

		.paths {
			margin: 0;
		}

		.path {
			display: block;
		}

		.path:hover {
			text-decoration: none;
		}

		h2 {
			margin-top: 10px;
			font-size: 25px;
		}

		h1 {
			position: relative;
			vertical-align: middle;
			font-size: 30px;
		}

		h1 a {
			color: #555555;
		}

		h1 i {
			color: #555555;
		}

		h1 .glyphicon {
			font-size: 18px;
		}

		h1 .glyphicon-record {
			margin-left: 2px;
			margin-right: 5px;
			display: block;
		    float: left;
		    margin-top: 10px;
		}

		h1 .glyphicon-link {
			position: absolute;
			left: -25px;
			top: 10px;
			display: none;
		}

		h1 .name:hover .glyphicon-link {
			display: block;
		}

		h3 {
			font-size: 20px;
		}

		h4 {
			margin-top: 40px;
		}

		h5 {
			margin: 0 0 10px 0;
			font-size: 14px;
			font-weight: normal;
			color: #555555;
		}

		h1 {
			padding-top: 0px;
			margin-top: 55px;
			margin-bottom: 0;
			font-weight: bold;
		}

		h1 .description {
			padding-top: 6px;
			padding-bottom: 14px;
			font-size: 14px;
			font-weight: normal;
		}

		.glyphicon-cloud span {
			position: absolute;
			left: 0;
			right: 0;
			text-align: center;
			bottom: 0;
			top: 0;
			vertical-align: middle;
			font-size: 11px;
			color: #fff;
			display: block;
			line-height: 35px;
			font-weight: bold;
		}

		.fl {
			float: left;
		}

		.p0 {
			padding: 0px !important;
		}

		.w150 {
			width: 150px;
		}

		.table tr > td:first-child {
			width: 250px;
		}

		.wa {
			width: auto !important;
		}

		.table-nested td {
			border-top: none !important;
		}

		.table-values .description {
			width: 99%;
		}

		.table-values .value {
			white-space: nowrap;
		}

		.table-values .default {
			font-size: smaller;
		}

		.parameter-values {
		}

		.parameter-values .heading {

		}

		.parameter-values-table {
			margin-top: 10px;
			border-left: 1px solid #ccc;
			padding-left: 10px;
		}

		.parameter-values tr td {
			border: none !important;
		}

		.parameter-name {
			font-style: italic;
		}

		header {
			margin-top: 30px;
			margin-bottom: 20px;
			font-weight: 500;
			font-size: 38px;
		}

		header .title {
			line-height: 38px;
			display: inline;
		}

		header .title a {
			font-size: 12px;
			line-height: 18px;
			font-weight: normal;
		}

		header .title .links {
			display: inline-block;
		}

		header .host {
			display: inline;
			color: #bbb;
			font-size: 32px;
			font-weight: 100;
			margin-bottom: 25px;
		}

		header .host-title {

		}

		header .links {
			float: right;
		}

		header .links a {
			font-size: 10px;
			color: #FFF;
			display: block;
			width: 30px;
			height: 30px;
			border-radius: 15px;
			background-color: #7ABAF9;
			text-align: center;
			line-height: 30px;
			float: right;
			margin-left: 5px;
		}

		header i {
			color: #7ABAF9;
			vertical-align: middle;
		}

		.description {
			font-size: 14px;
		}

		.endpoints {
			overflow: hidden;
			padding-left: 30px;
		}

		.api:first-child h1 {
			margin-top: 0;
		}

		.api-body {

		}

		nav {
			float: left;
			width: 200px;
			position: relative;
			height: 10px;
		}

		nav #menu {
		    position:absolute;
		    top:0;
		    width:100%;
		    padding-right: 20px;
		}

		nav #menu ul {
		    margin:0;
		    list-style: none;
		    padding: 0;
		}

		nav #menu ul li a {
		    display:block;
		    border-left:6px solid #ccc;
		    text-decoration:none;
		    padding:5px 5px 5px 25px;
		    list-style: none;
		}

		nav #menu ul li a:hover {
		    background-color: #F7F7F7;
		}

		nav #menu ul li a.active {
			font-weight: bold;
			background-color: #ccc;
			color: #fff;
		}

		.api {
			margin-bottom: 40px
		}

		.required {
			color: red;
			font-size: smaller;
		}

		.optional {
			color: #5BC0DE;
			font-size: smaller;
		}

    </style>

  	<div class="container">

  		<header>

			<div class="links">
				<a href="/api?wadl">wadl</a>
				<a href="/api?wsdl">wsdl</a>
			</div>
	    	<div class="host-title">
	    		<i class="glyphicon glyphicon-cloud"><span>API</span></i>
	    		<!--<div class="title">Documentation</div>-->
	    		<span class="host"><?=SERVER_UTIL::get_server_host()?></span>
	    	</div>
	    </header>

	    <nav>
	    	<div id="menu">
	    	<ul>
	    	<? foreach($config as $api) { ?>

	  			<? $paths = (array)value($api,array("endpoints",0,"path"),array()) ?>

  				<li>
  					<a href="#<?=value($paths,0)?>"><?=value($api,"name")?></a>
  				</li>

		  	<? } ?>
		  	</ul>
		  	</div>

  		</nav>
  		<div class="endpoints">
		    <? foreach($config as $api) { ?>
		    	<?
		    		$path = value((array)value($api,array("endpoints",0,"path")),0);
		    	?>
				<div class="api" data-api="<?=$path?>">

			    	<h1 id="<?=$path?>">

			    		<span class="name">
							<a href="/api/?wsdl#<?=$path?>"><span class="glyphicon glyphicon-link"></span>
								<?=value($api,"name")?>
  							</a>
  						</span>

			    		<? if(value($api,"require_api_token")) { ?>
			    			<i class="glyphicon glyphicon-lock" data-toggle="tooltip" data-placement="top" title="This endpoint requires an Api-Key passed in the header"></i>
			    		<? } ?>
			    		<? if(value($api,"require_api_token")) { ?>
			    			<i class="glyphicon glyphicon-user" data-toggle="tooltip" data-placement="top" title="This endpoint requires an Api-Key passed in the header for user authentication"></i>
			    		<? } ?>

		    			<? if($model=value($api,"model")) { ?>
			    			<a href="/api/<?=$model?>?wsdl">
			    				<span class="glyphicon glyphicon glyphicon-list-alt" data-toggle="tooltip" data-placement="top" title="Object description for a <?=value($api,"name")?>"></span>
			    			</a>
			    		<? } ?>

			    		<? if($description=value($api,"description")) { ?>
				    		<div class="description"><?=$description?></div>
				    	<? } ?>
			    	</h1>
			    	<div class="api-body">

					    <? foreach(value($api,"endpoints",array()) as $endpoint) { ?>
					    	<h2>
				    			<div class="method <?=strtolower(value($endpoint,"method"))?>"><?=strtoupper(value($endpoint,"method"))?></div>
					    	</h2>
					    	<div class="content">

					    		<h3 class="paths">
						    		<? foreach((array)value($endpoint,"path") as $path) { ?>
						    			<? $id = strtolower(value($endpoint,"method")).trim(preg_replace(array("/[^a-z0-9]/i"),array(""),$path)); ?>
						    			<a href="#<?=$id?>" id="<?=$id?>" class="path">/api/<?=$path?></a>
						    		<? } ?>
					    		</h3>
					    		<? if($description=value($endpoint,"description")) { ?>
					    			<h5><?=$description?></h5>
					    		<? } ?>

					    		<? if($parms=value($endpoint,"parms",array())) { ?>
					    			<h4><span class="parameter-method <?=strtolower(value($endpoint,"method"))?>"><?=strtoupper(value($endpoint,"method"))?></span> Parameters</h4>

					    			<table class="table">

					    				<? foreach($parms as $name=>$value) { ?>

					    					<? if(is_array($value)) { ?>

						    					<?
						    						$description 	= value($value,"description");
						    						$default 		= value($value,"default");
						    						$values 		= value($value,"values",[]);
						    						$object 		= value($value,"object");

						    						if(!array_key_exists("object",$value) && !array_key_exists("description",$value) && !array_key_exists("default",$value) && !array_key_exists("values",$value)) {
						    							$values = $value;
						    						} else {
							    						if(value($value,"name")) {
							    							$name = value($value,"name");
							    						}
						    						}
						    					?>

				    							<tr>
							    					<td>
							    						<span class="parameter-name"><?=$name?></span>
							    					</td>
													<td width="80%">
														<? if($description) { ?>
															<div class="description"><?=$description?></div>
														<? } ?>

														<? if($description && $values) { ?>
															<br>
														<? } ?>

							    						<? if($values && is_array($values)) { ?>

							    							<div class="parameter-values">
							    								<h5 class="heading">Acceptable Values</h5>
								    							<div class="parameter-values-table">
									    							<table class="table table-values">
											    						<?	foreach($values as $name=>$item) { ?>
											    							<tr>
									    										<td class="value">
									    											<span class="parameter-name"><?=$name?></span>
									    										</td>

									    										<td class="description">
														    						<?=$item?> <? if($default==$name) { ?><span class="default">(default)</span><? } ?>
													    						</td>
													    					</tr>
											    						<? } ?>
											    					</table>
											    				</div>
											    			</div>
								    					<? } ?>

								    					<? if($object && is_array($object)) { ?>

							    							<div class="parameter-values">
								    							<div class="parameter-values-table">
									    							<table class="table table-values">
											    						<?	foreach($object as $name=>$item) { ?>
											    							<tr>
									    										<? /* If the array is just a number indexed array then display just the value */?>
										    									<? if(is_numeric($name)) { ?>
										    										<td><span class="parameter-name"><?=$item?></span> <? if($default==$item) { ?><span class="default">(default)</span><? } ?></td>
										    									<? } else { ?>

										    										<td class="value">
										    											<span class="parameter-name"><?=$name?></span>
										    										</td>

										    										<td class="description">
															    						<?=$item?> <? if($default==$name) { ?><span class="default">(default)</span><? } ?>
														    						</td>
														    					<? } ?>
													    					</tr>
											    						<? } ?>
											    					</table>
											    				</div>
											    			</div>
								    					<? } ?>
							    					</td>
						    					</tr>

				    						<? } else { ?>
					    						<?

					    							if(is_numeric($name)) {
					    								$name = $value;
					    								$value = "";
					    							}

					    							$required 	= preg_match("/^\*/",$name);
					    							$optional 	= preg_match("/^\?/",$name);
					    							$name 		= preg_replace("/^[\?\*]\s?/","",$name);
					    						?>
						    					<tr>
							    					<td>
							    						<span class="parameter-name">
							    							<?=$name?>

									    					<? if($required) { ?>
									    						<?=HTML_UTIL::span("required",array("class"=>"required"))?>
									    					<? } ?>

									    					<? if($optional) { ?>
																<?=HTML_UTIL::span("optional",array("class"=>"optional"))?>
															<? } ?>
							    						</span>
							    					</td>
							    					<td>
							    						<div class="description">
							    						<?
								    						if(strpos($value,"|")===false) {
								    							echo $value;
								    						} else {
								    							echo "<ul><li>".str_replace("|","</li><li>",$value)."</li></ul>";
								    						}
								    					?>
							    						</div>

							    					</td>
						    					</tr>
						    				<? } ?>
					    				<? } ?>
					    			</table>
					    		<? } ?>


					    		<? if($response=value($endpoint,"response",array())) { ?>
					    			<h4>Response</h4>

					    			<table class="table">

					    				<? foreach($response as $name=>$value) { ?>

					    					<? if(is_numeric($name)) { ?>
					    						<tr>
						    					<td><?=$value?></td>
						    					<td></td>
					    					</tr>
					    					<? } else { ?>
						    					<tr>
							    					<td><?=$name?></td>
							    					<td><?=$value?></td>
						    					</tr>
						    				<? } ?>
					    				<? } ?>
					    			</table>
					    		<? } ?>
					    	</div>
					    <? } ?>
					</div>
		    	</div>
		    <? } ?>
		</div>
	</div>

	<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>
	<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>

	<script>

		$(function () {
			$('[data-toggle="tooltip"]').tooltip();

			var menu = $("#menu");
		    var header = $("header").outerHeight(true);

		    $(window).scroll(function() {
		    	var stop = $(window).scrollTop();
		    	if(stop>=header)
		    		menu.finish().css("top",stop - header);
		    	else
		    		menu.finish().css("top",0);

		    	$($(".api").get().reverse()).each(function() {
		    		var offset = $(this).offset();

		    		if(offset.top<=stop) {
		    			$("#menu a").removeClass("active");
		    			$("#menu a[href='#" + $(this).data("api") + "']").addClass("active");
		    			return false;
		    		}
		    	});
		    });
		});

	</script>

  </body>
</html>