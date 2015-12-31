<?
      /**
        * Class HMENU_ITEM
        *
        * @package Framework
        * @subpackage Cmodels
        */
        
	class BASE_CMODEL_MENU {

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
		function __construct($link,$text,$menu_items=array(),$attributes=array(),$roles=array()) {
			$this->_menu_items = $menu_items;
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




