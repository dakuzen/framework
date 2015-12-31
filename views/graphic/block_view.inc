<?
	class BASE_VIEW_GRAPHIC_BLOCK extends VIEW {
		
		protected $_graphic_block_cmodel 	= null;
		protected $_is_save					= false;
		protected $_font_list 				= array("helvetica"=>"Helvetica","times"=>"Times");
		protected $_font_sizes				= array(5,6,7,8,9,10,12,13,14,15,16,17,18,19,20,25,30,40,50,70,80,100);
		protected $_types					= array();
		protected $_font_directory			= array();
		
		function __construct() {
			$this->set_route("graphic","block");
			$this->enable_type_component();
		
			$this->_graphic_block_cmodel	= new BASE_CMODEL_GRAPHIC_BLOCK();
			$this->_types 					= BASE_DBQ_GRAPHIC_BLOCK::get_type_list();
		}

		function init() {
			
			$registry = REGISTRY::get_instance();
					
			if(!$this->_graphic_block_cmodel->get_font_size())
				$this->_graphic_block_cmodel->set_font_size(25);
				
			if(!$this->_graphic_block_cmodel->get_font_color())
				$this->_graphic_block_cmodel->set_font_color("000000");			
			
			$font_sizes = array_combine($this->_font_sizes,$this->_font_sizes);
		
			$update_label = $this->_graphic_block_cmodel->get_graphic_block_id() ? "Save" : "Add";

			$x1 		= $this->_graphic_block_cmodel->get_x1();
			$y1 		= $this->_graphic_block_cmodel->get_y1();
			$x2 		= $this->_graphic_block_cmodel->get_x2();
			$y2 		= $this->_graphic_block_cmodel->get_y2();
			
			if($this->_font_directory)
				$this->_font_list = BASE_CMODEL_GRAPHIC_RENDERER::get_font_list($this->_font_directory);
			
			$this->set_var("x1",$x1);
			$this->set_var("y1",$y1);
			$this->set_var("x2",$x2);
			$this->set_var("y2",$y2);
			$this->set_var("halign_list",BASE_DBQ_GRAPHIC_BLOCK::get_halign_list());
			$this->set_var("update_label",$update_label);
			$this->set_var("graphic_block",$this->_graphic_block_cmodel);
			$this->set_var("font_list",$this->_font_list);
			$this->set_var("font_sizes",$font_sizes);
			$this->set_var("types",$this->_types);
			$this->set_var("is_save",$this->_is_save);
		}
		
		function set_graphic_block($value) 				{ $this->_graphic_block_cmodel = $value; }
		function set_font_list($value) 					{ $this->_font_list = $value; }
		function set_is_save($is_save) 					{ $this->_is_save = $is_save; }
		function add_font($font_key,$font_name) 		{ $this->_font_list[$font_key] = $font_name; }
		function set_font_directory($font_directory) 	{ $this->_font_directory = $font_directory; }
		function get_id()								{ return REGISTRY::get_instance()->get_request_parm("id"); }
		function get_types()							{ return $this->_types; }

		function set_types($types)						{ $this->_types = $types; }

		function disable_type_image()					{ unset($this->_types[BASE_DBQ_GRAPHIC_BLOCK::TYPE_IMAGE]); }

	}