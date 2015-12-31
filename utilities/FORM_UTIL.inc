<?
 	/**
	 * Class FORM_UTIL 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */

	class FORM_UTIL extends HTML_FORM_UTIL {

		public static function create($form_atributes=array()) { return new FORM_UTIL($form_atributes); }

		function radiobuttons($fieldname,$label,$list,$default="",$attribs=array(),$options=array()) {
			$delimiter 		= value($options,"delimiter","<br>");
			$force_select 	= value($options,"force_select",true);
			$required 		= value($options,"required",false);
			return $this->add_field(new HTML_ROW_RADIOBUTTONS($fieldname,$label,$list,$default,$required,$attribs,$force_select,$delimiter));
		}

		function input($fieldname,$label,$default="",$attribs=array(),$options=array()) {

			$required 	= value($options,"required",false);
			$maxlength 	= value($options,"maxlength",null);
			$size 		= value($options,"size",null);
			$lines 		= value($options,"lines",1);
			$info 		= value($options,"info",null);

			return $this->add_field(new HTML_ROW_INPUT($fieldname,$label,$default,$required,$size,$maxlength,$lines,$attribs,$info));
		}
		 
		function textarea($fieldname,$label,$default="",$attribs=array(),$options=array()) {	
			$required 	= value($options,"required",false);
			$size 		= value($options,"size",60);
			$lines 		= value($options,"lines",5);
			$info 		= value($options,"info",null);
			return $this->add_field(new HTML_ROW_INPUT($fieldname,$label,$default,$required,$size,0,$lines,$attribs,$info));
		}

		function checkbox($fieldname,$label,$default="",$attribs=array(),$options=array()) {	
			$required 	= value($options,"required",false);
			$info 		= value($options,"info",null);
			return $this->add_field(new HTML_ROW_CHECKBOX($fieldname,$label,$default,$required,$attribs,$info));
		}

		function checkboxes($fieldname,$label,$list,$default="",$attribs=array(),$options=array()) {
			$required 	= value($options,"required",false);
			$info 		= value($options,"info",null);
			return $this->add_field(new HTML_ROW_CHECKBOXES($fieldname,$label,$list,$default,$required,$attribs,$info));
		}

		function date($fieldname,$label,$default="",$attribs=array(),$options=array()) {	
			$required 	= value($options,"required",false);
			return $this->add_field(new HTML_ROW_DATE($fieldname,$label,$default,$required));
		}

		function datetime($fieldname,$label,$default="",$options=array()) {
			$required 	= value($options,"required",false);
			return $this->add_field(new HTML_ROW_DATE($fieldname,$label,$default,$required,true));
		}

		function dropdown($fieldname,$label,$list,$default="",$attribs=array(),$options=array()) {
			$multiple 		= value($options,"multiple",false);
			$display 		= value($options,"display",1);
			$info 			= value($options,"info",null);
			$required 		= value($options,"required",false);
			return $this->add_field(new HTML_ROW_DROPDOWN($fieldname,$label,$list,$default,$required,$attribs,$display,$multiple,$info));
		}

		function submit($fieldname,$label,$attribs=array(),$options=array()) {
			return $this->add_field(new HTML_ROW_BUTTON($fieldname,$label,$attribs));
		}

		function button($fieldname,$label,$attribs=array(),$options=array()) {
			return $this->add_field(new HTML_ROW_BUTTON($fieldname,$label,$attribs));
		}

		function phone($fieldname,$label,$default="",$options=array()) {
			$required 		= value($options,"required",false);
			$fields 		= value($options,"fields",3);
			return $this->add_field(new HTML_ROW_PHONE($fieldname,$label,$default,$required,$fields));
		}

        function password($fieldname,$label,$default="",$options=array()) {
			$required 		= value($options,"required",false);
			$info 			= value($options,"info",null);			
			return $this->add_field(new HTML_ROW_PASSWORD($fieldname,$label,$required,$default,$options));
		}

		function custom($value,$content_attribs=array(),$row_attribs=array(),$label_attribs=array()) {
			return $this->add_field(new HTML_ROW_CUSTOM($value),$content_attribs,$row_attribs,$label_attribs);
		}

		function text($label,$text,$options=array()) {
			$required = value($options,"required",false);
			return $this->add_field(new HTML_ROW_STATIC($label,$text,$required));
		}

		function h1($text,$attribs=array()) {
			return $this->custom(HTML_UTIL::h1($text,$attribs));
		}

		function h2($text,$attribs=array()) {
			return $this->custom(HTML_UTIL::h2($text,$attribs));
		}

		function h3($text,$attribs=array()) {
			return $this->custom(HTML_UTIL::h3($text,$attribs));
		}

		function h4($text,$attribs=array()) {
			return $this->custom(HTML_UTIL::h4($text,$attribs));
		}		
	}
