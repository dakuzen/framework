<?php
        /**
        * Class BASE_CMODEL_HMENU
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_HMENU extends MODEL {
                
                /**
                 * The menu
                 * @var string $menu 
                 */
		private $_menu			= null;
                
                /**
                 * The image path
                 * @var string $image_path
                 */
		private $_image_path		= "/lib/js/purecssmenu/images/";
                
                /**
                 * The arrow hover image
                 * @var string $arrow_hover_image
                 */
		private $_arrow_hover_image	= "arr_grey.gif";
                
                /**
                 * The arrow image
                 * @var string
                 */
		private $_arrow_image		= "arr_black.gif";
                
                /**
                 * The styles
                 * @var array $styles 
                 */
		private $_styles 		= array();
                
                /**
                 * The roles
                 * @var array $roles 
                 */
		private $_roles 		= array();

                /**
                 *Render 
                 */
		function render() 						{ echo $this->get_html(); }
	
                /**
                 * Returns the style value
                 * @param string $name
                 * @param string $default
                 * @return string 
                 */
		function get_style_value($name,$default) 			{ return get_value($this->_styles,$name,$default); }
		
                /**
                 * Returns the roles
                 * @return array 
                 */
                function get_roles() 						{ return $this->_roles; }
                
                /**
                 * Returns the menu border width
                 * @return string 
                 */
		function get_menu_border_width() 				{ return $this->get_style_value("menu_border_width","0"); }
		
                /**
                 * Sets the menu
                 * @param string $menu 
                 */
		function set_menu($menu) 					{ $this->_menu = $menu; }
                
                /**
                 * Sets the style value
                 * @param string $name
                 * @param string $value 
                 */
		function set_style_value($name,$value) 				{ $this->_styles[$name] = $value; }
                
                /**
                 * Sets the menu border width
                 * @param string $value 
                 */
		function set_menu_border_width($value) 				{ $this->set_style_value("menu_border_width",$value); }
		
                /**
                 * Sets the menu border color
                 * @param string $value 
                 */
                function set_menu_border_color($value) 				{ $this->set_style_value("menu_border_color",$value); }
		
                /**
                 * Sets the menu background color
                 * @param string $value 
                 */
                function set_menu_background_color($value) 			{ $this->set_style_value("menu_background_color",$value); }
		
                /**
                 * Sets the menu font color
                 * @param string $value 
                 */
                function set_menu_font_color($value) 				{ $this->set_style_value("menu_font_color",$value); }		
		
                /**
                 * Sets the hover menu font color
                 * @param string $value 
                 */
                function set_hover_menu_font_color($value) 			{ $this->set_style_value("hover_menu_font_color",$value); }		
		
                /**
                 * Sets the hover menu background color
                 * @param string $value 
                 */
                function set_hover_menu_background_color($value) 		{ $this->set_style_value("hover_menu_background_color",$value); }
		
                /**
                 * Sets the image path
                 * @param string $value 
                 */
                function set_image_path($value) 				{ $this->_image_path = $value; }
		
                /**
                 * Sets the arrow image
                 * @param string $value 
                 */
                function set_arrow_image($value) 				{ $this->_arrow_image = $value; }
		
                /**
                 * Sets the arrow hover image
                 * @param string $value 
                 */
                function set_arrow_hover_image($value) 				{ $this->_arrow_hover_image = $value; }
		
                /**
                 * Sets the roles
                 * @param array $value 
                 */
                function set_roles($value) 					{ $this->_roles = $value; }
		
                /**
                 * Cretae the menu
                 * @param array $menu_items
                 * @param array $roles
                 * @return object HMENU 
                 */
		function create_menu($menu_items=array(),$roles=array())	{ return new HMENU($menu_items,$roles); }

                /**
                 * Returns the html
                 * @return string 
                 */
		function get_html() { 	
			
			if(!$this->_menu)
				return "";

			$menu_items = $this->_menu->has_menu_items() ? $this->_menu->get_menu_items() : array();
				
			return '<div id="cmenu">'.$this->get_menus($menu_items,"cmenu").'</div>'; 
		}

                /**
                 * Creates the menu time
                 * @param string $link
                 * @param string $title
                 * @param array $menu_items
                 * @param array $attributes
                 * @return object HMENU_ITEM 
                 */
		function create_menu_item($link,$title,$menu_items=array(),$attributes=array()) { 
			return new HMENU_ITEM($link,$title,$menu_items,$attributes);			
		}
		
                /**
                 * Returns the menus
                 * @param array $menu_items
                 * @param string $class
                 * @return string 
                 */
		function get_menus($menu_items,$class) {

			$html = '<ul class="'.$class.'">';
			
			foreach($menu_items as $index=>$menu_item) {

					if(!$menu_item->has_access($this->get_roles()))
						continue;

					if($menu_item->get_menu_items()) 
						$menu_html = '<span class="cmenu-parent">'.$menu_item->get_text().'</span>';
					else
						$menu_html = '<span>'.$menu_item->get_text().'</span>';
					
					$class = "";
					
					if($index==0)
						$class = "first";
					elseif($index==(count($menu_items) - 1))
						$class = "last";
						
					if($menu_item->get_menu_items())
						$menu_item->add_class("cmenu-parent");
					
					$html .= '<li'.($class ? ' class="'.$class.'"' : "").'>'.HTML_UTIL::get_link($menu_item->get_link(),$menu_html,$menu_item->get_attributes());

					if($menu_item->get_menu_items())
						$html = $html.$this->get_menus($menu_item->get_menu_items(),$menu_item->get_class());
						
					$html .= '</li>';
				
			}	
			$html .= '</ul>';
			return $html;		
		}

                /**
                 * Renders the behavior
                 * @param string $htc_url 
                 */
		static function render_behavior($htc_url) {
			echo '	<!--[if lt IE 7]> 
				<style> 
					body { 
						behavior:url('.$htc_url.'); 
					}
				</style>
				<![endif]-->';
		}

                /**
                 * Renders the framework  
                 */
		function render_framework() {

			$html = '			
				<style>
					ul.cmenu ul { 
						position: 		absolute;
						z-index:		999999;
						top:			100%; 
					}

					ul.cmenu ul ul { 
						position: 		absolute;
						left:			100%;
						top:			-'.$this->get_menu_border_width().'px;
					}					

					ul.cmenu,
					ul.cmenu ul {
						margin:			0px;
						list-style:		none;
						padding:		0px;
						background-color:	'.$this->get_style_value("menu_background_color","#ffffff").';
						background-repeat:	repeat;
						border-color:		'.$this->get_style_value("menu_border_color","#AAAAAA").';
						border-width:		'.$this->get_menu_border_width().'px;
						border-style:		solid;
					}

					ul.cmenu {
						width:			100%;
					}

					ul.cmenu a:active,
					ul.cmenu a:focus {
						outline-style:		none;
					}

					ul.cmenu a:link,
					ul.cmenu a:visited,
					ul.cmenu a:hover,
					ul.cmenu a:active {
						text-decoration:	none;
						color:			'.$this->get_style_value("menu_font_color","#333333").';
					}

					ul.cmenu a {
						padding:		0px;
						margin:			0px;
						display:		block;
						vertical-align:		middle;
						border-width:		0px;
						text-align:		left;
						padding-left:		10px;
						padding-right:		10px;
						padding-top:		5px;
						padding-bottom:		5px;
					}

					ul.cmenu li {
						float:			left;
						margin:			0px;
						position:		relative;
					}

					ul.cmenu li > li {
						float:			left;
						margin:			0px;
						background-color:	'.$this->get_style_value("menu_background_color","#ffffff").';
						position:		relative;
					}

					ul.cmenu ul li {
						float:			none;
					}

					ul.cmenu ul a {
						text-align:		left;
						white-space:		nowrap;
					}

					ul.cmenu ul, 
					ul.cmenu li:hover ul ul,
					ul.cmenu li:hover li:hover ul ul,
					ul.cmenu li:hover li:hover li:hover ul ul { 	
						display:none; 
					}

					ul.cmenu li:hover ul, 
					ul.cmenu li:hover li:hover ul,
					ul.cmenu li:hover li:hover li:hover ul,
					ul.cmenu li:hover li:hover li:hover li:hover ul { 
						display:block; 
					} 					

					ul.cmenu li:hover > a {
						background-color:	'.$this->get_style_value("hover_menu_background_color","#4792e6").';
						color: 			'.$this->get_style_value("hover_menu_font_color","#ffffff").';
						text-decoration:	none;
					}

					ul.cmenu span.cmenu-parent {
						background-image:	url('.$this->_image_path.$this->_arrow_image.');
						background-position:	right center;
						background-repeat: 	no-repeat;
						padding-right:		25px;
					}

					ul.cmenu a:hover span.cmenu-parent { 
						background-image:	url('.$this->_image_path.$this->_arrow_hover_image.');
					}

					ul.cmenu ul li:hover > a span.cmenu-parent {
						background-image:	url('.$this->_image_path.$this->_arrow_hover_image.'); 
					}

				</style>

				<!--[if lt IE 7]> 
				<style>					
					ul.cmenu li {
						padding:		1px;
					}		
				</style>
				<![endif]-->
				';
			echo $html;
		}
		
                /**
                 *Init 
                 */
		function init() {}
	}

        /**
        * Class HMENU
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class HMENU {
            
                /**
                 * Menu items
                 * @var array $menu_items 
                 */
		protected $_menu_items	= array();
                
                /**
                 * the roles
                 * @var array $roles 
                 */
		protected $_roles		= array();

                /**
                 * constructor
                 * @param array $menu_items
                 * @param array $roles 
                 */
		function __construct($menu_items=array(),$roles=array()) {
			$this->_menu_items 	= $menu_items;
			$this->_roles 		= $roles;
		}

                /**
                 * Returns menu items
                 * @return array
                 */
		function get_menu_items() 		{ return $this->_menu_items; }
                
                /**
                 * Returns the roles
                 * @return array $roles
                 */
		function get_roles() 			{ return $this->_roles; }
                
                /**
                 * Checks whether has menu items
                 * @return boolean
                 */
		function has_menu_items() 		{ return !is_empty($this->_menu_items); }
		
                /**
                 * Adds menu item
                 * @param string $menu_item 
                 */
		function add_menu_item($menu_item) 	{ $this->_menu_items[] = $menu_item; }

                /**
                 * Populates global role
                 * @param array|string $roles 
                 */
		function populate_global_role($roles) { 

			$roles = is_array($roles) ? $roles : array($roles);
			
			$this->_roles = array_merge($this->_roles,$roles);

			foreach($this->get_menu_items() as $menu_item) 
				$menu_item->populate_global_role($roles);
		}
	}
        
        /**
        * Class HMENU_ITEM
        *
        * @package Framework
        * @subpackage Cmodels
        */
        
	class HMENU_ITEM extends HMENU {
		
                /**
                 * the link
                 * @var string $link 
                 */
		protected $_link 		= "";
                
                /**
                 * the text
                 * @var string $text
                 */
		protected $_text		= "";
                
                /**
                 * the classes
                 * @var array $classes
                 */
		protected $_classes		= array("cmenu-item");
                
                /**
                 * The attributes
                 * @var array $attributes
                 */
		protected $_attributes	= array();
		
                /**
                 * Constructor
                 * @param string $link
                 * @param string $text
                 * @param array $menu_items
                 * @param array $roles
                 * @param array $attributes 
                 */
		function __construct($link,$text,$menu_items=array(),$roles=array(),$attributes=array()) {
			parent::__construct($menu_items);
			$this->_link = $link;
			$this->_text = $text;
			$this->_roles = $roles;
			$this->_attributes = $attributes;
		}

                /**
                 * Returns text
                 * @return string 
                 */
		function get_text() 				{ return $this->_text; }
                
                /**
                 * Returns text
                 * @return string 
                 */
		function get_link() 				{ return $this->_link; }
		
                /**
                 * Returns roles
                 * @return array 
                 */
                function get_roles() 				{ return $this->_roles; }
		
                /**
                 * Returns attribute
                 * @param string $name
                 * @return string 
                 */
                function get_attribute($name)		{ return get_value($this->_attributes,$name); }
		
                /**
                 * Returns class
                 * @return string 
                 */
                function get_class() 				{ return implode(" ",$this->_classes); }
		
                /**
                 * Sets the link
                 * @param string $link 
                 */
		function set_link($link) 			{ $this->_link = $link; }

                /**
                 * Adds a class
                 * @param string $class 
                 */
		function add_class($class) 			{ $this->_classes[] = $class; }
		
                /**
                 * Checks whether user has access
                 * @param array $user_roles
                 * @return boolean 
                 */
		function has_access($user_roles) {

			if($this->_roles) {

				foreach($user_roles as $user_role)
					if(in_array($user_role,$this->_roles))
						return true;

				return false;
			}

			return true; 
		}
		
                /**
                 * Returns the attributes
                 * @return array 
                 */
		function get_attributes() { 
			
			$attributes = $this->_attributes;
			
			if($class=$this->get_class()) {
				$classes = $this->_classes;
				
				if($class=get_value($this->_attributes,"class"))
					$classes[] = $class;
				
				$attributes["class"] = implode(" ",$classes);
			}
			
			return $attributes;
		}

	}




