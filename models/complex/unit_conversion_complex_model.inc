<?php
        /**
        * Class BASE_CMODEL_UNIT_CONVERSION
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_UNIT_CONVERSION extends MODEL_COMPLEX {

		const CM_TO_IN = 0.393700787;
		const IN_TO_CM = 2.54;
		const KG_TO_LB = 2.20462262185;
		const LB_TO_KG = 0.45359237;
		
                /**
                 * Returns in from cm
                 * @param integer $cm
                 * @return integer 
                 */
		static function get_cm_to_in($cm) { return $cm * self::CM_TO_IN; }
                
                /**
                 * Returns cm from in
                 * @param integer $in
                 * @return integer 
                 */
		static function get_in_to_cm($in) { return $in * self::IN_TO_CM; }
                
                /**
                 * Returns lb from kg
                 * @param integer $kg
                 * @return integer 
                 */
		static function get_kg_to_lb($kg) { return $kg * self::KG_TO_LB; }
                
                /**
                 * Returns kg from lb
                 * @param integer $lb
                 * @return integer 
                 */
		static function get_lb_to_kg($lb) { return $lb * self::LB_TO_KG; }
		
		
	}