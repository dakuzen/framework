<?php
        /**
        * Class MATH_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */

	class MATH_UTIL {

		/**
		 * Returns the rotated dimensions
		 *
		 * @param integer $width int width value
		 * @param integer $height int height value
		 * @param integer $degrees int degree value
                 *
		 * @return array
		 */
		function get_rotated_dimensions($width,$height,$degrees) {

			$new_width = $width;
			$new_height = $height;

			if($degrees==90 || $degrees==270) {
				$new_width 	= $height;
				$new_height 	= $width;
			}

			return array("width"=>$new_width,"height"=>$new_height);
		}


		function km_between_lat_long($lat1, $lon1, $lat2, $lon2) {

			$pi80 = M_PI / 180;
			$lat1 *= $pi80;
			$lon1 *= $pi80;
			$lat2 *= $pi80;
			$lon2 *= $pi80;

			$r = 6372.797; // mean radius of Earth in km
			$dlat = $lat2 - $lat1;
			$dlon = $lon2 - $lon1;
			$a = sin($dlat / 2) * sin($dlat / 2) + cos($lat1) * cos($lat2) * sin($dlon / 2) * sin($dlon / 2);
			$c = 2 * atan2(sqrt($a), sqrt(1 - $a));
			$km = $r * $c;

			return $km;
		}
	}