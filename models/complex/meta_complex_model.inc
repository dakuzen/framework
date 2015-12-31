<?php
        /**
        * Class BASE_CMODEL_META
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_META {

                protected  $_metas = array();
		
                public function add_meta($property,$value) {
                        $this->_metas[$property] = $value;
                }

                /**
                 * Returns meta data
                 * @param string $prepend
                 * @return string 
                 */
                function get_meta($prepend="") {
                        
                        $metas = array();
                        foreach($this->_metas as $property=>$value)
                                $metas[] = self::get_meta_tag($property,$value);

                        if($prepend)
                                foreach($metas as $index=>$item)
                                        $metas[$index] = $prepend.$item;

                        return implode("\n",$metas);
                }

		/**
                 * Returns the graph meta data
                 * @param string $meta
                 * @param string $value
                 * @return string 
                 */
		static function get_meta_tag($meta,$value) { 

                        $value = trim(preg_replace(array("/[\n]/","/[\s]{2,}/"),array(" "," "),strip_tags($value)));

                        return '<meta property="'.$meta.'" content="'.$value.'"></meta>'; 
                }
	}
	
	
	
	