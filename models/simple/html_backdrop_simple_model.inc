<?
	class BASE_MODEL_HTML_BACKDROP extends MODEL {
		
		function render_top($class="",$padding="") {
			
			$class = $class ? " ".$class : "";
			$padding = $padding ? ' style="padding:'.$padding.'px"' : "";
			
			echo '<div class="backdrop'.$class.'">

				<div class="backdrop-lb">
				<div class="backdrop-rb">
				<div class="backdrop-bb"><div class="backdrop-blc"><div class="backdrop-brc">
				<div class="backdrop-tb"><div class="backdrop-tlc"><div class="backdrop-trc">

				<div class="backdrop-content"'.$padding.'>';
	
		}
		
		function render_bottom() {
	
				echo' </div>

					</div></div></div></div>
					</div></div></div></div>
				</div>';
		}
	}
	
/*



.backdrop-tlc, .backdrop-trc { zoom: 1 }

.backdrop {
	position: relative;
	margin-left: auto;
	margin-right: auto;	
	padding: 0;
	text-align: left;
	background-color: #FFFFFF;
}
 	
.backdrop-content {
	padding: 20px;
}

.backdrop-tlc, .backdrop-trc, .backdrop-blc, .backdrop-brc {
	background-color: transparent;
	background-repeat: no-repeat;
}

.backdrop-tlc {
	background-image:url(/images/theme/backdrop/tlc.gif);
	background-position: 0% 0%;
}

.backdrop-trc{
	background-image:url(/images/theme/backdrop/trc.gif);
	background-position: 100% 0%;
}
	
.backdrop-blc {
	background-image:url(/images/theme/backdrop/blc.gif);
	background-position: 0% 100%;
}

.backdrop-brc {
	background-image:url(/images/theme/backdrop/brc.gif);
	background-position: 100% 100%;
}

.backdrop-tb, .backdrop-bb {
	background-color: transparent;
	background-repeat: repeat-x;
}
			
.backdrop-tb {
	background-image:url(/images/theme/backdrop/tb.gif);
	background-position: 0% 0%;
}

.backdrop-bb {
	background-image:url(/images/theme/backdrop/bb.gif);
	background-position: 50% 100%;
}
	
.backdrop-rb {
	background-image:url(/images/theme/backdrop/br.gif);
	background-position: 100% 0%;
	background-repeat: repeat-y;
}

.backdrop-lb {
	background-image:url(/images/theme/backdrop/bl.gif);
	background-position: 0% 100%;
	background-repeat: repeat-y;
}

*/