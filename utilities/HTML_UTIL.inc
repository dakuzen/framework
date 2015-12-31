<?
   /**
    * Class HTML_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */

	class HTML_UTIL {

        /**
         * Defaults
         * @var array $DEFAULTS
         */
		static protected $DEFAULTS = array();

        /**
         * Set defaults
         * @param array $defaults
         */
		static function set_defaults($defaults) { self::$DEFAULTS = $defaults; }


		/**
		 * Returns string html of a drop down box
		 *
		 * <code>
		 * $animallist = array("dog","cat","mouse");
		 * echo $HTML_UTIL::get_dropdown("animals",$animallist);
		 * </code>
		 * <pre>
		 * <select id="animals" name="animals">
		 * <option value="0">dog</option>
		 * <option value="1">cat</option>
		 * <option value="2">mouse</option>
		 * </select>
		 * </pre>
		 *
		 * @param string $name string name of the dropdown
		 * @param array $list array list which the box contains
		 * @param array $selected array of selected items
		 * @param array $attribs array of attributes which may apply to the drop down
		 * @param int $display_row_count integer value displaying the number of rows
		 * @param boolean $multi_select enables multi select
		 * @param boolean $option_groups boolean contains group options
		 *
		 * @return string
	 	 */

        static function get_dropdown($name,$list,$selected=null,$attribs=array(),$display_row_count=1,$multi_select=false,$option_groups=true) {
        	return self::dropdown($name,$list,$selected,$attribs,$display_row_count,$multi_select,$option_groups);
		}

		static function dropdown($name,$list,$selected=null,$attribs=array(),$display_row_count=1,$multi_select=false,$option_groups=true) {

			if(!is_array($attribs))
				return user_error("Invalid attributes array");

			$attributes["id"] = $name;

			if($multi_select) {
				$attributes["name"] = $name."[]";
				$attributes["multiple"] = true;
			} else
				$attributes["name"] = $name;

			if($display_row_count>1)
				$attributes["size"] = $display_row_count;

			foreach($attribs as $key=>$value)
				$attributes[$key] = $value;

			$classes = array();
			if($class=get_value(self::$DEFAULTS,array("dropdown","class")))
				$classes[] = $class;

			if($class=get_value($attribs,"class"))
				$classes[] = $class;

			$attributes["class"] = implode(" ",$classes);

			HTML_UTIL::apply_name_standard($attributes);

			foreach($attributes as $name=>$value)
				$select_attrib[] = $name.'="'.$value.'"';

			$str = "\t".'<select '.implode(" ",$select_attrib).'>'."\n";

			if (!is_array($selected))
				$selected = array($selected);


			foreach($list as $key=>$value) {

				if($option_groups && is_array($value)) {

					$str .= '<optgroup label="'.$key.'">';

					foreach($value as $option_key=>$option_value)
						$str .= self::get_option($option_key,$option_value,$selected);

					$str .= '</optgroup>';

				} elseif(!$option_groups && is_array($value)) {
					$str .= self::get_option($key,get_value($value,"label"),$selected,$value);

				} else
					$str .= self::get_option($key,$value,$selected);
			}

			$str .= "\t</select>\n";
			return $str;
		}

		/**
		 * Returns string option with a key value
		 *
		 * <code>
		 * $selected = array("dog","cat","mouse");
		 * echo $HTML_UTIL::get_option("key","value",$selected);
		 * </code>
		 * <pre>
		 * <option value="key">value</option>
		 * </pre>
 		 *
         * @param string $key
         * @param array $value
         * @param array $selected
         *
		 * @return string
	 	 */
		static function get_option($key,$value,$selected,$attribs=array()) {

			$data = "";
			if(is_array($value)) {

				foreach($value as $ary_key=>$ary_value) {
					if($ary_key=="data")
						$data = $ary_value;
					else
						$attribs[$ary_key] = $ary_value;
				}
			} else
				$data = $value;

			foreach($selected as $test)
				if((string)$test===(string)$key)
					$attribs["selected"] = "selected";

			$attribs["value"] = $key;

			return "\t\t".self::get_tag("option",$attribs,$data);
		}

		/**
		 * Returns string option with a key value
		 *
		 * <code>
		 * $attribs["type"] 		= "password";
		 * $attribs["value"] 		= self::escape_quotes($value);
		 * $attribs["name"] 		= $name;
		 * $attribs["id"] 		= $name;
		 * $attribs["class"] 		= "textfield";
		 * $attribs["autocomplete"] 	= "off";
		 * echo HTML_UTIL::apply_name_standard($attribs);
		 * </code>
		 *
		 * @param array $attribs array of attributes applied to the name
		 *
		 * @return string
	 	 */

		static function apply_name_standard(&$attribs) {

			$name 	= get_value($attribs,"name");
			$id 	= get_value($attribs,"id");

			if($name && $id===null)
				$attribs["id"] = $name;
			if($id && $name===null)
				$attribs["name"] = $id;

			if(!get_value($attribs,"id"))
				unset($attribs["id"]);
		}

		/**
		 * Returns a inputbox
		 *
		 * <code>
		 * echo HTML_UTIL::get_input("name");
		 * </code>
		 * <pre>
		 * <input type="text" value="" name="name" id="name" class="textfield" autocomplete="off" size="35" />
		 * </pre>
		 *
		 * @param string $name string name of the input box
		 * @param string $value string value of the input box
		 * @param array $attribs array attributes that can be applied
		 *
		 * @return string
	 	 */

		static public function get_input($name,$value="",$attribs=array()) {

			$classes = array("textfield");
			if($class=get_value(self::$DEFAULTS,array("input","class")))
				$classes[] = $class;

			if($class=get_value($attribs,"class"))
				$classes[] = $class;			

			$attribs["type"] 			= get_value($attribs,"type","text");
			$attribs["value"] 			= str_replace('"',"&quot;",$value);
			$attribs["name"] 			= get_value($attribs,"name",$name);
			$attribs["id"] 				= get_value($attribs,"id",$name);
			$attribs["class"] 			= implode(" ",$classes);
			$attribs["autocomplete"] 	= get_value($attribs,"autocomplete","off");

			HTML_UTIL::apply_name_standard($attribs);

			return HTML_UTIL::get_inline_tag("input",$attribs);
		}

		static public function input($name,$value="",$attribs=array()) {
			return self::get_input($name,$value,$attribs);
		}

		/**
		 * Returns a inputbox
		 *
		 * <code>
		 * echo HTML_UTIL::get_password_field("password");
		 * </code>
		 * <pre>
		 * <input type="password" value="" name="password" id="password" class="textfield" autocomplete="off" />
		 * </pre>
		 *
		 * @param string $name string name of the field
		 * @param string $value string value
		 * @param array $attributes array attributes that can be applied
		 *
		 * @return string
	 	 */

		static function get_password($name,$value="",$attributes=array()) {
			return self::password($name,$value,$attributes);
		}

		static function password($name,$value="",$attributes=array()) {

			$classes = array("textfield");
			if($class=get_value(self::$DEFAULTS,array("input","class")))
				$classes[] = $class;

			if($class=get_value($attributes,"class"))
				$classes[] = $class;				

			$attribs["type"] 		= "password";
			$attribs["value"] 		= self::escape_quotes($value);
			$attribs["name"] 		= $name;
			$attribs["id"] 			= $name;
			$attribs["class"] 		= implode(" ",$classes);
			$attribs["autocomplete"] 	= "off";

			foreach($attributes as $key=>$value)
				$attribs[$key] = $value;

			HTML_UTIL::apply_name_standard($attribs);

			return HTML_UTIL::get_inline_tag("input",$attribs);
		}

        /**
         * Return password field
         * @param string $name
         * @param string $value
         * @param array $attributes
         * @return string
         */
		static function get_password_field($name,$value="",$attributes=array()) {
			return self::get_password_field($name,$value,$attributes);
		}

		/**
		 * Returns a file field
		 *
		 * <code>
		 * echo HTML_UTIL::get_filefield("file");
		 * </code>
		 * <pre>
		 * <input type="file" value="" id="file" class="filefield" name="file" /> (Max size 8M)
		 * </pre>
		 *
		 * @param string $name string name of the field
		 * @param string $value string value
		 * @param array $attributes array attributes that can be applied
		 * @param boolean $enable_max_size_label
		 * @param string $content string content
		 *
		 * @return string
	 	 */

		static function get_filefield($name,$value="",$attributes=array(),$enable_max_size_label=null,$content="") {

			$enable_max_size_label = $enable_max_size_label===null ? '<div class="filefield-label">(Max size {max_upload_size})</div>' : $enable_max_size_label;

			$attribs["type"] 	= "file";
			$attribs["value"] 	= $value;
			$attribs["id"] 		= $name;
			$attribs["class"] 	= "filefield";

			foreach($attributes as $key=>$value)
				$attribs[$key] = $value;

			HTML_UTIL::apply_name_standard($attribs);
			$max_size_str = str_replace("{max_upload_size}",SERVER_UTIL::get_max_filesize(),$enable_max_size_label);

			return HTML_UTIL::get_inline_tag("input",$attribs).$content.$max_size_str;
		}

		const VAR_MAX_FILE_SIZE = "{max_upload_size}";

		/**
		 * Returns the string max size upload
		 *
		 * <code>
		 * echo HTML_UTIL::get_max_upload_size_label();
		 * </code>
		 * <pre>
		 * Max size 8M
		 * </pre>
		 *
		 * @return string
	 	 */

		static function get_max_upload_size_label() {
			return "Max size ".SERVER_UTIL::get_max_filesize();
		}

		/**
		 * Returns a hidden field
		 *
		 * <code>
		 * echo HTML_UTIL::get_hidden("hidden");
		 * </code>
		 * <pre>
		 * <input type="hidden" value="" name="hidden" id="hidden" />
		 * </pre>
		 *
                 * @param string $name
                 * @param string $value
                 * @param integer $id
                 * @param array $attribs
                 *
		 * @return string
	 	 */
		static function get_hidden($name,$value="",$id=null,$attribs=array()) {

			$attribs["type"] 	= "hidden";
			$attribs["value"] 	= self::escape_quotes($value);
			$attribs["name"] 	= $name;

			if(!is_null($id))
				$attribs["id"] = $id;

			HTML_UTIL::apply_name_standard($attribs);

			return HTML_UTIL::get_inline_tag("input",$attribs);
		}

		static function hidden($name,$value="",$id=null,$attribs=array()) {
			return self::get_hidden($name,$value,$id,$attribs);
		}


                /**
                 * Escape quotes
                 *
                 * @param string $value
                 * @return mixed
                 */
		static function escape_quotes($value) { return str_replace('"',"&#34;",$value); }

                /**
                 * Return hidden field
                 * @param string $name
                 * @param string $value
                 * @param string $id
                 * @return string
                 */
		static function get_hidden_field($name,$value="",$id=null) {
			return self::get_hidden($name,$value,$id);
		}


	 	/**
                 * Returns a bloekn input box
                 * @param string $name
                 * @param string $field_structure
                 * @param string $defaults
                 * @param integer $default_field_count
                 * @param array $events
                 * @return string
                 */
		static function get_broken_inputbox($name,$field_structure,$defaults,$default_field_count=1,$events=array()) {

			if(!is_array($defaults)) $defaults = array_fill(0,$default_field_count,"");
			$defaults = count($defaults)<$default_field_count ? array_fill(1,$default_field_count,"") : $defaults;

			$str = "";
			for($i=0;$i<count($defaults) || $i<$default_field_count;$i++) {

				$attribs = array();
				$field_name = $name.'['.$field_structure[$i][0].']';
				$value = (isset($defaults[$i]) ? $defaults[$i] : "");
				$attribs["size"] = $field_structure[$i][1];
				$attribs["maxlength"] = $field_structure[$i][1];
				$attribs["id"] = $name."_".$i;
				$attribs["onFocus"] = "this.select();";

				if($i<(count($defaults)-1)) {
					$next_name = "'".$name."_".($i+1)."'";
					$attribs["onKeyUp"] = "inputbox_navigation(this,".$next_name.",event);";
				}

				foreach($events as $event=>$action)
					if(!isset($attribs[$event]))
						$attribs[$event] = $action;
					else $attribs[$event] = $action.$attribs[$event];

				$str .= $field_structure[$i][2]."".HTML_UTIL::get_input($field_name,$value,$attribs);
			}
			return $str;
		}

		/**
		 * Returns a text area
		 *
		 * <code>
		 * echo HTML_UTIL::get_textarea("textarea");
		 * </code>
		 * <pre>
		 * <textarea name="broken" id="broken" class="textfield"></textarea>
		 * </pre>
		 *
		 * @param string $name string name
		 * @param string $value string value
		 * @param array $attributes array of attributes that can be applied
		 *
		 * @return string
	 	 */

		static function get_textarea($name,$value="",$attributes=array()) {
			return self::textarea($name,$value,$attributes);
		}

		static function textarea($name,$value="",$attributes=array()) {

			$attribs["name"] 	= $name;
			$attribs["id"] 		= $name;			
			$value			= str_replace(array(">","<"),array("&gt;","&lt;"),$value);

			foreach($attributes as $key=>$v)
				$attribs[$key] = $v;

			$classes = array("textfield");
			if($class=get_value(self::$DEFAULTS,array("textarea","class")))
				$classes[] = $class;

			if($class=get_value($attributes,"class"))
				$classes[] = $class;

			$attribs["class"] 	= implode(" ",$classes);			

			HTML_UTIL::apply_name_standard($attribs);

			return HTML_UTIL::get_tag("textarea",$attribs,$value);
		}

		/**
		 * Returns a hidden field
		 *
		 * <code>
		 * echo HTML_UTIL::get_redirect_button("textarea");
		 * </code>
		 * <pre>
		 * <input type="button" value="redirect" name="cmd_redirect" class="button" onClick="window.location='url';" />
		 * </pre>
		 *
		 * @param string $label string label attached
		 * @param string $url string url contained
                 * @param array $attributes
		 *
		 * @return string
	 	 */

		static function get_redirect_button($label,$url,$attributes=array()) {

			$attributes["type"] = "button";
			$attributes["onClick"] = "window.location='".$url."'";

			return HTML_UTIL::get_button("cmd_redirect",$label,$attributes);
		}

		/**
		 * Returns a button on click
		 *
		 * <code>
		 * echo HTML_UTIL::get_button_onclick("button","onclick");
		 * </code>
		 * <pre>
		 * <input type="button" value="button" name="cmd_button" class="button" onClick="onclick" />
		 * </pre>
		 *
		 * @param string $label string label attached
		 * @param string $on_click
		 *
		 * @return string
	 	 */

		static function get_button_onclick($label,$on_click) {
			return HTML_UTIL::get_button("cmd_button",$label,array("onClick"=>$on_click,"type"=>"button"));
		}

		/**
		 * Returns a button
		 *
		 * <code>
		 * echo HTML_UTIL::get_button("button","value");
		 * </code>
		 * <pre>
		 * <input type="submit" value="value" name="button" class="button" />
		 * </pre>
		 *
		 * @param string $name string name
		 * @param string $value string value
		 * @param array $attributes array attributes that can be applied
                 * @param boolean $disable_on_click
		 *
		 * @return string
	 	 */

	 	static function get_button($name,$value,$attributes=array(),$disable_on_click=true) {
			return self::button($name,$value,$attributes,$disable_on_click);
		}

		static function button($name,$value,$attributes=array(),$disable_on_click=true) {

			$classes = array();
			if($class=get_value(self::$DEFAULTS,array("button","class"),"button"))
				$classes[] = $class;

			if($class=get_value($attributes,"class"))
				$classes[] = $class;

			$attribs["type"] 	= get_value($attributes,"type","submit");
			$attribs["value"] 	= self::escape_quotes($value);
			$attribs["name"] 	= $name;
			$attribs["id"] 		= $name;

			foreach($attributes as $key=>$value)
				$attribs[$key] = $value;

			$attribs["class"] 	= implode(" ",$classes);

			return HTML_UTIL::get_inline_tag("input",$attribs);
		}

		/**
		 * Returns an image
		 *
		 * <code>
		 * echo HTML_UTIL::get_image("c:\mydocuments");
		 * </code>
		 *
		 * @param string $src string path of file
		 * @param array $attributes array attributes that can be applied
		 *
		 * @return string
	 	 */

		static function get_image($src,$attributes=array()) {
			return self::image($src,$attributes);
		}

		static function image($src,$attribs=array()) {

			$attribs["src"] = $src;

			if($title=get_value($attribs,"title"))
				$attribs["title"] = htmlentities($title);

			if($alt=get_value($attribs,"alt"))
				$attribs["title"] = htmlentities($alt);

			return HTML_UTIL::get_inline_tag("img",$attribs);
		}

		/**
		 * Returns a link
		 *
		 * <code>
		 * echo HTML_UTIL::get_link("link","text");
		 * </code>
		 * <pre>
		 * <a href="link">text</a>
		 * </pre>
		 *
		 * @param string $link string url link
		 * @param string $text string text showing
		 * @param array $attributes array attributes that can apply
		 *
		 * @return string
	 	 */
		static function a($link,$text,$attributes=array()) {
			return self::link($link,$text,$attributes);	
		}

		static function link($link,$text,$attributes=array()) {

			$attributes = array_merge(array("href"=>$link),$attributes);

			return HTML_UTIL::get_tag("a",$attributes,$text);
		}

		static function get_link($link,$text,$attributes=array()) { return self::link($link,$text,$attributes); }


		/**
		 * Returns a anchor
		 *
		 * <code>
		 * echo HTML_UTIL::get_anchor("name","text");
		 * </code>
		 * <pre>
		 * <a name="name">text</a>
		 * </pre>
		 *
		 * @param string $name string url name
		 * @param string $text string text showing
		 * @param array $attributes array attributes that can apply
		 *
		 * @return string
	 	 */

		static function get_anchor($name,$text,$attributes=array()) {

			$attributes = array_merge(array("name"=>$name),$attributes);

			return HTML_UTIL::get_tag("a",$attributes,$text);
		}

		/**
		 * Returns a checkbox
		 *
		 * <code>
		 * echo HTML_UTIL::get_checkbox("name","value");
		 * </code>
		 * <pre>
		 * <input type="checkbox" value="value" name="name" class="checkbox" id="checkbox_a7d22c85919bc0eac86f482e332b27cb" />
		 * </pre>
		 *
		 * @param string $name string name of check box
		 * @param string $value string value
		 * @param boolean $checked boolean enables checked
		 * @param array $attributes array attributes that can apply
		 * @param string $label string label
		 *
		 * @return string
	 	 */

		static function checkbox($name,$value,$checked=false,$attributes=array(),$label=null,$label_attributes=array(),$wrap_attribs=array()) {

			$attribs = array("type"=>"checkbox","value"=>self::escape_quotes($value),"name"=>$name,"class"=>"checkbox");

			if($checked)
				$attribs["checked"] = "checked";

			$id = value($attribs,"id");

			if(!$id)
				$id = preg_match("/\[\]$/",$name) ? MISC_UTIL::guid(10) : $name;

			$attribs["id"] = value($attribs,"id",$id);

			foreach($attributes as $key=>$value)
				$attribs[$key] = $value;

			$checkbox = self::get_inline_tag("input",$attribs);
			
			$nested_checkbox = value(self::$DEFAULTS,array("checkbox","nested"),false);

			$wrap_class = value($wrap_attribs,"class");

			$wrap_attribs["class"] = value(self::$DEFAULTS,array("checkbox","wrap-class"),"checkbox-wrap").($wrap_class ? " ".$wrap_class : "");

			if(!is_null($label)) {

				$label_attributes["for"] = value($attribs,"id");
				
				if($nested_checkbox)
					$checkbox = HTML_UTIL::span(self::get_open_tag("label",$label_attributes).$checkbox." ".$label."</label>",$wrap_attribs);
				else
					$checkbox = HTML_UTIL::span($checkbox." ".self::get_open_tag("label",$label_attributes).$label."</label>",$wrap_attribs);
			}

			return $checkbox;
		}

		static function get_checkbox($name,$value,$checked=false,$attributes=array(),$label="",$label_attributes=array()) { return self::checkbox($name,$value,$checked,$attributes,$label,$label_attributes); }


		/**
		 * Returns checkboxes
		 *
		 * <code>
		 * $animals2 = array("dog","cat","mouse");
		 * echo HTML_UTIL::get_checkboxes("name",$animals2);
		 * </code>
		 * <pre>
		 * <input type="checkbox" value="dog" name="name[0]" class="checkbox" id="checkbox_72c0523b22a37cff7466b820a3ca5c77" /> <label for="checkbox_72c0523b22a37cff7466b820a3ca5c77">dog</label><br/>
		 * <input type="checkbox" value="cat" name="name[1]" class="checkbox" id="checkbox_2e5a5445579efed4d72cf17618d57711" /> <label for="checkbox_2e5a5445579efed4d72cf17618d57711">cat</label><br/>
		 * <input type="checkbox" value="mouse" name="name[2]" class="checkbox" id="checkbox_d4945af50415641ec619cf7159db3547" /> <label for="checkbox_d4945af50415641ec619cf7159db3547">mouse</label>
		 * </pre>
		 *
		 * @param string $name string name of check box
		 * @param array $list array list of items
		 * @param array $default array default
		 * @param array $attribs array attributes that can apply
                 * @param string $delimiter
		 *
		 * @return string
	 	 */
	 	static function get_checkboxes($name,$list,$default=array(),$attribs=array(),$delimiter="<br/>") {
	 		return self::checkboxes($name,$list,$default,$attribs,$delimiter);
	 	}

		static function checkboxes($name,$list,$default=array(),$attribs=array(),$delimiter="<br/>",$label_attribs=array(),$wrap_attribs=array()) {

			if(count($list)==0)
				return "";

			$wrap_class = value($wrap_attribs,"class");

			$wrap_attribs["class"] = value(self::$DEFAULTS,array("checkboxes","wrap-class"),"checkbox-wrap").($wrap_class ? " ".$wrap_class : "");
			$default = (array)$default;

			$checkboxes = array();
			foreach($list as $value=>$item) {
				
				$label = $item;

				$tmp_attribs 		= $attribs;
				$tmp_label_attribs 	= $label_attribs;
				$tmp_wrap_attribs 	= $wrap_attribs;

				if(is_array($item)) {
				
					foreach($item as $item_key=>$item_value) {
						if($item_key=="label")
							$label = $item_value;
						else {
							$tmp_attribs[$item_key] 		= trim(value($tmp_attribs,$item_key)." ".$item_value);
							$tmp_label_attribs[$item_key] 	= trim(value($tmp_label_attribs,$item_key)." ".$item_value);
							$tmp_wrap_attribs[$item_key] 	= trim(value($tmp_wrap_attribs,$item_key)." ".$item_value);
						}
					}
				}

				$attribs["id"] = "checkbox_".MISC_UTIL::get_random_uid();

				$selected = in_array($value,$default);

				$checkboxes[] = HTML_UTIL::span(HTML_UTIL::get_checkbox($name."[]",$value,$selected,$tmp_attribs,$label,$tmp_label_attribs),$tmp_wrap_attribs);
				//The $key index was removed because the naming could not be grouped with HTML form naming/grouping
				//$checkboxes[] = HTML_UTIL::get_checkbox($name."[".$key."]",$key,in_array($key,$default),$attribs,$item);
			}

			return implode($delimiter,$checkboxes);
		}

		/**
		 * Returns radio buttons
		 *
		 * <code>
		 * $animals2 = array("dog","cat","mouse");
		 * echo HTML_UTIL::get_radiobuttons("name",$animals2);
		 * </code>
		 * <pre>
		 * <input name="name" type="radio" value="0" class="radiobutton" id="radiobutton_da35f7fc831da4eada2cadd936b7dace" checked="checked" /> <label for="radiobutton_da35f7fc831da4eada2cadd936b7dace">dog</label><br/>
		 * <input name="name" type="radio" value="1" class="radiobutton" id="radiobutton_c96c2f470a4224a5663cc5ad5f24f15f" /> <label for="radiobutton_c96c2f470a4224a5663cc5ad5f24f15f">cat</label><br/>
		 * <input name="name" type="radio" value="2" class="radiobutton" id="radiobutton_d93d3f88a8b758bef136e9def5768e70" /> <label for="radiobutton_d93d3f88a8b758bef136e9def5768e70">mouse</label>
		 * </pre>
		 *
		 * @param string $name string name
		 * @param array $list array list of button items
		 * @param array $default array default
		 * @param array $attribs array attributes that can apply
		 * @param boolean $force_select boolean enables force select
                 * @param string $delimiter
                 * @param array $label_attribs
		 *
		 * @return string
	 	 */

		static function get_radiobuttons($name,$list=array(),$default=null,$attribs=array(),$force_select=true,$delimiter="<br/>",$label_attribs=array()) {
			return self::radiobuttons($name,$list,$default,$attribs,$force_select,$delimiter,$label_attribs);
		}

		static function radiobuttons($name,$list=array(),$default=null,$attribs=array(),$force_select=true,$delimiter="<br/>",$label_attribs=array(),$wrap_attribs=array("class"=>"radiobutton-wrap")) {

			if(count($list)==0)
				return "";

			$default = is_array($default) ? array_shift($default) : $default;
			
			if($force_select && count($list)>0) {
				if(!array_key_exists($default,$list)) {
					$key_ary = array_keys($list);
					$default = $key_ary[0];
				}
			}

			$index = 0;
			$options = array();
			foreach($list as $value=>$item) {

				/*
				if($index==0) {
					$tmp_label_attribs["class"] = ($class=value($tmp_label_attribs,"class")) ? $class." first" : "first";
					$tmp_attribs["class"] 		= ($class=value($tmp_attribs,"class")) ? $class." first" : "first";
				}

				if(($index+1)==count($list)) {
					$tmp_label_attribs["class"] = ($class=value($tmp_label_attribs,"class")) ? $class." last" : "last";
					$tmp_attribs["class"] 		= ($class=value($tmp_attribs,"class")) ? $class." last" : "last";
				}
				*/
				$label = $item;

				$tmp_attribs 		= $attribs;
				$tmp_label_attribs 	= $label_attribs;
				$tmp_wrap_attribs 	= $wrap_attribs;

				if(is_array($item)) {
				
					foreach($item as $item_key=>$item_value) {
						if($item_key=="label")
							$label = $item_value;
						else {
							$tmp_attribs[$item_key] 		= trim(value($tmp_attribs,$item_key)." ".$item_value);
							$tmp_label_attribs[$item_key] 	= trim(value($tmp_label_attribs,$item_key)." ".$item_value);
							$tmp_wrap_attribs[$item_key] 	= trim(value($tmp_wrap_attribs,$item_key)." ".$item_value);
						}
					}
				}

				$selected = is_null($default) ? false : $default==$value;

				$options[] = self::radiobutton($name,self::escape_quotes($value),$label,$selected,$tmp_attribs,$tmp_label_attribs,$tmp_wrap_attribs);

				$index++;
			}

			return implode($delimiter,$options);
		}

		/**
		 * Returns radio button
		 *
		 * <code>
		 * $animals2 = array("dog","cat","mouse");
		 * echo HTML_UTIL::get_radiobutton("name",$animals2);
		 * </code>
		 * <pre>
		 * <input name="name" type="radio" value="0" class="radiobutton" id="radiobutton_da35f7fc831da4eada2cadd936b7dace" checked="checked" /> <label for="radiobutton_da35f7fc831da4eada2cadd936b7dace">dog</label><br/>
		 * <input name="name" type="radio" value="1" class="radiobutton" id="radiobutton_c96c2f470a4224a5663cc5ad5f24f15f" /> <label for="radiobutton_c96c2f470a4224a5663cc5ad5f24f15f">cat</label><br/>
		 * <input name="name" type="radio" value="2" class="radiobutton" id="radiobutton_d93d3f88a8b758bef136e9def5768e70" /> <label for="radiobutton_d93d3f88a8b758bef136e9def5768e70">mouse</label>
		 * </pre>
		 *
		 * @param string $name string name
		 * @param array $value array list of button items
		 * @param string $label string label appearing
		 * @param boolean $selected boolean enables selection
		 * @param array $attributes array attributes that can apply
                 * @param array $label_attribs
		 *
		 * @return string
	 	 */

		static function get_radiobutton($name,$value,$label=null,$selected=false,$attributes=array(),$label_attribs=array()) {
			return self::radiobutton($name,$value,$label,$selected,$attributes,$label_attribs);
		}

		static function radiobutton($name,$value,$label=null,$selected=false,$attributes=array(),$label_attribs=array()) {

			$attribs = array("name"=>$name,"type"=>"radio","value"=>$value,"class"=>"radiobutton");

			foreach($attributes as $key=>$value)
				$attribs[$key] = $value;

			$attribs["id"] = get_value($attribs,"id","radiobutton_".MISC_UTIL::get_random_uid());

			if($selected)
				$attribs["checked"] = "checked";

			$radiobutton = HTML_UTIL::get_inline_tag("input",$attribs);

			if(!is_null($label)) {

				$nested = get_value(self::$DEFAULTS,array("radiobutton","nested"),false);

				$label_attribs["class"] = ($class=get_value($label_attribs,"class")) ? "radiolabel ".$class : "radiolabel";

				if($nested)
					$radiobutton = self::get_open_tag("label",$label_attribs).$radiobutton." ".$label."</label>";
				else {
					$label_attribs["for"] 	= $attribs["id"];
					$radiobutton = $radiobutton." ".self::get_open_tag("label",$label_attribs).$label."</label>";
				}
			}

			return $radiobutton;



			//return HTML_UTIL::get_inline_tag("input",$attribs).$label;
		}

		/**
		 * Returns a frame
		 *
		 * <code>
		 * echo HTML_UTIL::get_iframe("/site/pageframe/","873","1000");
		 * </code>
		 *
		 * @param string $src string source file
		 * @param int $width int weight
		 * @param int $height int height
		 * @param array $attributes array attributes that can apply
                 * @param string $content
		 *
		 * @return string
	 	 */

		static function get_iframe($src,$width,$height,$attributes=array(),$content="") {

            if(is_empty($src))
                    $src = "/blank";

			$default_attribs["width"] 		= $width;
			$default_attribs["height"] 		= $height;
			$default_attribs["src"] 		= $src;
			$default_attribs["scrolling"] 		= get_value($attributes,"scrolling","yes");
			$default_attribs["frameborder"] 	= 0;
			$default_attribs["marginwidth"] 	= 0;
			$default_attribs["marginheight"] 	= 0;
			$default_attribs["align"] 		= "middle";

			foreach($attributes as $key=>$value)
				$default_attribs[$key] = $value;

			HTML_UTIL::apply_name_standard($attributes);

			return HTML_UTIL::get_tag("iframe",$default_attribs,$content);
		}

		/**
		 * Returns form start tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_open_form();
		 * </code>
		 * <pre>
		 * <form action="" method="POST">
		 * </pre>
		 *
		 * @param string $action string action
		 * @param string $id string id
		 * @param string $name string name
		 * @param boolean $data boolean enables data
		 * @param string $method string method
		 * @param array $attribs array attributes that can apply
		 *
		 * @return string
	 	 */

		static function get_open_form($action=null,$id=null,$name=null,$data=false,$method="POST",$attribs=array()) {

			$default_attribs["action"]		= $action;

			if($data) $default_attribs["enctype"]	= "multipart/form-data";
			if($id!=null) $default_attribs["id"]	= $id;
			if($name!=null) $default_attribs["name"]= $name;

			if($method) $default_attribs["method"] 		= $method;

			foreach($attribs as $key=>$value)
				$default_attribs[$key] = $value;

			HTML_UTIL::apply_name_standard($default_attribs);

			return self::get_open_tag("form",$default_attribs);
		}

		/**
		 * Returns form end tag
		 *
		 * @return string
	 	 */

		static function get_close_form() { return "</form>"; }

		/**
		 * Returns image button
		 *
		 * @param string $name string name
		 * @param string $src string image source
		 * @param array $attribs array attributes that can apply
		 *
		 * @return string
	 	 */

		static function get_image_button($name,$src,$attribs=array()) {
			$default_attribs["name"] 	= $name;
			$default_attribs["type"] 	= "image";
			$default_attribs["src"]  	= $src;
			$default_attribs["class"] 	= "input-image";
			$default_attribs["border"] 	= "0";

			foreach($attribs as $key=>$value)
				$default_attribs[$key] = $value;

			if(isset($attribs["alt"]) && !isset($attribs["title"]))
				$default_attribs["title"] = $attribs["alt"];
			if(isset($attribs["title"]) && !isset($attribs["alt"]))
				$default_attribs["alt"] = $attribs["title"];

			HTML_UTIL::apply_name_standard($default_attribs);

			return HTML_UTIL::get_inline_tag("input",$default_attribs);
		}

		/**
		 * Returns image button
		 *
		 * <code>
		 * $animals2 = array("dog","cat","mouse");
		 * echo HTML_UTIL::get_list($animals2,array("style"=>"margin:5px;margin-left:30px;"));
		 * </code>
		 * <pre>
		 * <ul style="margin:5px;margin-left:30px;"><li>dog</li><li>cat</li><li>mouse</li></ul>
		 * </pre>
		 *
		 * @param array $list array list
		 * @param array $ul_attribs array attributes applied to ul
		 * @param array $li_attribs array attributes applied to li
		 *
		 * @return string
	 	 */

		static function get_list($list,$ul_attribs=array(),$li_attribs=array()) {
			return self::listing($list,$ul_attribs,$li_attribs);
		}

		static function listing($list,$ul_attribs=array(),$li_attribs=array()) {

			if($list) {

				$items = array();
				foreach($list as $value) {

					$attribs = $li_attribs;

					if(is_array($value)) {
						$attribs += $value;
						$value = get_value($value,"data");
						unset($attribs["data"]);
					}

					$items[] = HTML_UTIL::get_tag("li",$attribs,$value);
				}

				return HTML_UTIL::get_tag("ul",$ul_attribs,implode("",$items));
			}
		}

		/**
		 * Returns div tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_div("name");
		 * </code>
		 * <pre>
		 * <div>name</div>
		 * </pre>
		 *
		 * @param string $value string
		 * @param array $default_attribs array attributes applied
		 *
		 * @return string
	 	 */

	 	static function get_div($value,$default_attribs=array()) {
	 		return self::div($value,$default_attribs);
	 	}

		static function div($value,$default_attribs=array()) {
			return HTML_UTIL::get_tag("div",$default_attribs,$value);
		}

		/**
		 * Returns span tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_span("name");
		 * </code>
		 * <pre>
		 * <span>name</span>
		 * </pre>
		 *
		 * @param string $value string
		 * @param array $default_attribs array attributes applied
		 *
		 * @return string
	 	 */

		static function span($value,$default_attribs=array()) {
			return HTML_UTIL::get_tag("span",$default_attribs,$value);
		}

		static function get_span($value,$default_attribs=array()) {
			return self::span($value,$default_attribs);
		}

		/**
		 * Returns label tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_label("name","for");
		 * </code>
		 * <pre>
		 * <label for="for">name</label>
		 * </pre>
		 *
		 * @param string $text string name appearing
		 * @param string $for string for
                 * @param array $attribs
		 *
		 * @return string
	 	 */

		static function get_label($text,$for,$attribs=array()) {
			return self::label($text,$for,$attribs);
		}

		static function label($text,$for,$attribs=array()) {
			$attribs["for"] = $for;
			return HTML_UTIL::get_tag("label",$attribs,$text);
		}

		/**
		 * Returns a strong tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_bold_string("bold");
		 * </code>
		 * <pre>
		 * <strong>bold</strong>
		 * </pre>
		 *
		 * @param string $string
		 *
		 * @return string
	 	 */

		static function get_bold_string($string) {
			return "<strong>".$string."</strong>";
		}

		static function b($value) { return HTML_UTIL::tag("b",array(),$value); }

		/**
		 * Returns italic tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_italic_string("italic");
		 * </code>
		 * <pre>
		 * <i>italic</i>
		 * </pre>
		 *
		 * @param string $string
		 * @return string
	 	 */

		static function get_italic_string($string) {
			return "<i>".$string."</i>";
		}

		/**
		 * Returns a horizontal ruler
		 *
		 * <code>
		 * echo HTML_UTIL::get_horizontal_ruler();
		 * </code>
		 * <pre>
		 * <hr />
		 * </pre>
		 *
		 * @param array $attribs array attributes applied
		 *
		 * @return string
	 	 */

		static public function get_horizontal_ruler($attribs=array()){
			return HTML_UTIL::get_inline_tag("hr",$attribs);
		}

		/**
		 * Returns a paragraph tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_paragraph();
		 * </code>
		 * <pre>
		 * <p>paragraph</p>
		 * </pre>
		 *
		 * @param string $value string value
		 * @param array $attribs array attributes applied
		 *
		 * @return string
	 	 */

		static public function get_paragraph($value,$attribs=array()){
			return HTML_UTIL::get_tag("p",$attribs,$value);
		}

		/**
		 * Returns a h1 heading tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_heading1("heading1");
		 * </code>
		 * <pre>
		 * <h1>heading</h1>
		 * </pre>
		 *
		 * @param string $value string value
		 * @param array $attribs array attributes applied
		 *
		 * @return string
	 	 */

		static public function heading1($value,$attribs=array()){
			return HTML_UTIL::get_tag("h1",$attribs,$value);
		}


		static public function h1($value,$attribs=array()){
			return HTML_UTIL::get_tag("h1",$attribs,$value);
		}

		static public function h2($value,$attribs=array()){
			return HTML_UTIL::get_tag("h2",$attribs,$value);
		}

		static public function h3($value,$attribs=array()){
			return HTML_UTIL::get_tag("h3",$attribs,$value);
		}

		static public function h4($value,$attribs=array()){
			return HTML_UTIL::get_tag("h4",$attribs,$value);
		}

		static public function get_heading1($value,$attribs=array()){
			return self::heading1($value,$attribs);
		}	

		static public function li($value,$attribs=array()){
			return self::get_tag("li",$attribs,$value);
		}

		static public function ul($value,$attribs=array()){
			return self::get_tag("ul",$attribs,$value);
		}

		static public function img($value,$attribs=array()){
			$attribs["src"] = $value;
			return self::get_tag("img",$attribs);
		}

		/**
		 * Returns a h2 heading tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_heading2("heading2");
		 * </code>
		 * <pre>
		 * <h2>heading</h2>
		 * </pre>
		 *
		 * @param string $value string value
		 * @param array $attribs array attributes applied
		 *
		 * @return string
	 	 */

		static public function get_heading2($value,$attribs=array()){
			return HTML_UTIL::get_tag("h2",$attribs,$value);
		}

		/**
		 * Returns a h3 heading tag
		 *
		 * <code>
		 * echo HTML_UTIL::get_heading3("heading3");
		 * </code>
		 * <pre>
		 * <h3>heading</h3>
		 * </pre>
		 *
		 * @param string $value string value
		 * @param array $attribs array attributes applied
		 *
		 * @return string
	 	 */

		static public function get_heading3($value,$attribs=array()){
			return HTML_UTIL::get_tag("h3",$attribs,$value);
		}

		/**
         * Return css
         * @param string $href
         * @param array $attributes
         * @return string
         */
		static function get_css($href,$attributes=array()) {

			$attributes["href"]	= $href;
			$attributes["rel"] 	= "stylesheet";
			$attributes["type"] 	= "text/css";

			return self::get_inline_tag("link",$attributes);
		}

		static function meta($name,$attribs) {

			$attribs = array_merge(array("name"=>$name),$attribs);

			return self::get_inline_tag("meta",$attribs);
		}

		static function i($value,$attribs=array()) {
			return self::get_tag("i",$attribs,$value);
		}
        /**
         * Return meta keywords
         * @param string $keywords
         * @return array
         */
		static function get_meta_keywords($keywords) {

			$attributes["name"]	= "keywords";
			$attributes["content"] 	= $keywords;

			return self::get_inline_tag("meta",$attributes);
		}

        /**
         * Return meta descriptio
         * @param string $description
         * @return string
         */
		static function get_meta_description($description) {

			$attributes["name"]	= "description";
			$attributes["content"] 	= $description;

			return self::get_inline_tag("meta",$attributes);
		}


        /**
         * Return script tag
         * @param string $src
         * @return string
         */
		static function get_script($src,$content="",$attributes=array()) {

			if($src)
				$attributes["src"] = $src;

			return self::get_tag("script",$attributes,$content);
		}

        /**
         * Obfuscate
         * @param obfuscate $address
         * @return obfuscate
         */
		static function obfuscate($address) {

			include_once(DIR_FRAMEWORK."packages/jsmin/jsmin-1.1.1.php");

			$address = strtolower($address);
			$coded = "";
			$unmixedkey = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.@'();";
			$inprogresskey = $unmixedkey;
			$mixedkey="";
			$unshuffled = strlen($unmixedkey);

			for ($i = 0; $i <= strlen($unmixedkey); $i++) {
				$ranpos = rand(0,$unshuffled-1);
				$nextchar = @$inprogresskey[$ranpos];
				$mixedkey .= $nextchar;
				$before = substr($inprogresskey,0,$ranpos);
				$after = substr($inprogresskey,$ranpos+1,$unshuffled-($ranpos+1));
				$inprogresskey = $before.''.$after;
				$unshuffled -= 1;
			}

			$cipher = $mixedkey;

			$shift = strlen($address);

			for ($j=0; $j<strlen($address); $j++) {
				if(strpos($cipher,$address{$j}) == -1) {
					$chr = $address{$j};
					$coded .= $address{$j};
				} else {
					$chr = (strpos($cipher,$address{$j}) + $shift) % strlen($cipher);
					$coded .= $cipher{$chr};
				}
			}

			$code = '<script>
					coded = "'.$coded.'";
					key = "'.$cipher.'";
					shift = coded.length;
					s="";
					for (i=0; i<coded.length; i++) {
						if (key.indexOf(coded.charAt(i))==-1) {
						      ltr = coded.charAt(i);
						      s += (ltr);
						} else {
						      ltr = (key.indexOf(coded.charAt(i))-shift+key.length) % key.length;
						      s += (key.charAt(ltr));
						}
					}

					document.write(s);

				</script>';

			$code = JSMin::minify($code);

			return $code;
		}



		/**
		 * Returns a h3 heading tag
		 *
		 * <code>
		 * $attribs["type"] 		= "text";
		 * $attribs["value"] 		= self::escape_quotes($value);
		 * $attribs["name"] 		= get_value($attribs,"name",$name);
		 * $attribs["id"] 		= get_value($attribs,"id",$name);
		 * $attribs["class"] 		= get_value($attribs,"class","textfield");
		 * $attribs["autocomplete"] 	= get_value($attribs,"autocomplete","off");
		 * $attribs["size"] 		= get_value($attribs,"size","35");
		 * HTML_UTIL::apply_name_standard($attribs);
		 * HTML_UTIL::get_inline_tag("input",$attribs);
		 * </code>
		 *
		 * @param string $type string type
		 * @param array $attributes array attributes applied
		 *
		 * @return string
	 	 */

		static function get_inline_tag($type,$attributes) {
			$attributes = self::sanitize_attributes($attributes);
			$attributes = $attributes ? " ".ARRAY_UTIL:: get_imploded_string($attributes," ",'"',"=") : "";
			return '<'.$type.$attributes.' />';
		}

		/**
		 * Returns a tag
		 *
		 * <code>
		 * $data = "";
		 * $attribs = array();
		 * if(!is_array($value)) {
		 * 	foreach($selected as $test)
		 * if((string)$test===(string)$key)
		 * 	$attribs["selected"] = "selected";
		 * $data = $value;
		 * } else {
		 * foreach($value as $ary_key=>$ary_value) {
		 * if($ary_key=="data")
		 * $data = $ary_value;
		 * else
		 * 	$attribs[$ary_key] = $ary_value;
		 * }
		 * }
		 * $attribs["value"] = $key;
		 * HTML_UTIL::get_tag("option",$attribs);
		 * </code>
		 *
		 * @param string $type string type
		 * @param array $attributes array attributes applied
		 * @param string $value string value
		 *
		 * @return string
	 	 */

		static function get_tag($type,$attributes,$value="") {
			return self::tag($type,$attributes,$value);
		}

		static function tag($type,$attributes,$value="") {
			$attributes = self::sanitize_attributes($attributes);
			$attributes = $attributes ? " ".ARRAY_UTIL:: get_imploded_string($attributes," ",'"',"=") : "";
			return '<'.$type.$attributes.'>'.$value.'</'.$type.'>';
		}

		/**
		 * Returns a tag
		 *
		 * <code>
		 * $default_attribs["action"] = "POST";
		 * $default_attribs["id"] = "id";
		 * echo HTML_UTIL::get_open_tag("form",$default_attribs);
		 * </code>
		 * <pre>
		 * <form action="POST" id="id">
		 * </pre>
		 *
		 * @param string $type string type
		 * @param array $attributes array attributes applied
		 * @param string $value string value
		 *
		 * @return string
	 	 */

		static function get_open_tag($type,$attributes,$value="") {
			$attributes = self::sanitize_attributes($attributes);
			$attributes = $attributes ? " ".ARRAY_UTIL:: get_imploded_string($attributes," ",'"',"=") : "";
			return '<'.$type.$attributes.'>'.$value;
		}

        /**
         * Sanitize attributes
         * @param array $attributes
         * @return array
         * @todo review the function
         */
		static function sanitize_attributes($attributes) {

			foreach($attributes as &$attribute)
				$attribute = preg_replace('/"/','&quot;',$attribute);

			return $attributes;
		}

		static function date($name,$default="",$attribs=array()) {
			return APPLICATION::get_base_view_instance("components","calendar")
								->set_clear(true)
								->set_attribs($attribs)
								->get_html($name,$default);
		}

		static function date_time($name,$default="") {
			return APPLICATION::get_base_view_instance("components","calendar")
							->set_clear(true)
							->get_html($name,$default);
		}

		static function date_of_birth($name,$default="",$years=100,$attribs=array()) {

			$min = date("Y") - $years;
			$max = date("Y");

			return APPLICATION::get_base_view_instance("components","calendar")
						->set_clear(true)
						->set_year_range($min,$max)
						->set_attribs($attribs)
						->get_html($name,$default);
		}

		static function parse($tag) {

			$tag = trim($tag);
			
			preg_match_all("/(\S+)=[\"']?((?:.(?![\"']?\s+(?:\S+)=|[>\"']))+.)[\"']?/",$tag,$matches);

			$attributes = array();
			for($i=0;$i<(count($matches,0) - 1);$i++)
				$attributes[value($matches,array(1,$i))] = value($matches,array(2,$i));
			preg_match("/^<\s*([^\s]+)/",$tag,$match);

			return array("name"=>value($match,1),"attributes"=>$attributes);
		}
	}