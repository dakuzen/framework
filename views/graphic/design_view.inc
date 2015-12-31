<?php

	class BASE_VIEW_GRAPHIC_DESIGN extends VIEW {
	
		protected $_id_function 		= null;
		protected $_preview_url 		= null;
		protected $_design_file 		= null;
		protected $_graphic_blocks 		= array();
		protected $_delete_url	 		= null;
		protected $_block_url	 		= null;
		protected $_update_url	 		= null;
		protected $_window_width	 	= 750;
		protected $_window_height 		= 250;
		protected $_width 				= 250;
		protected $_height 				= 250;
		protected $_preview_width 		= 600;
		protected $_options				= array();

		function __construct() {
			$this->set_route("graphic","design");
			$this->disable_authorization();
		}

		function init() {		

			self::include_web_assets($this->get_web_asset_manager());

			$image_width_limit = $image_height_limit = 0;
		
			if(is_file($this->_design_file)) {
			
				$dimensions = IMAGE_UTIL::get_size($this->_design_file);
				
				$this->_width 	= get_value($dimensions,"width");
				$this->_height 	= get_value($dimensions,"height");
			}
			
			$image_width_limit 		= $this->_width; 
			$image_height_limit 	= $this->_height;
			$ratio					= 1;
			
			if($this->_width>$this->_preview_width) {
				$image_width_limit 	= $this->_preview_width;
				$image_height_limit 	= $this->_height - ((($this->_width - $this->_preview_width) / $this->_width) * $this->_height);

				$ratio = $this->_width / $image_width_limit;
			}
			
			$image_width_limit 	= round($image_width_limit); 
			$image_height_limit 	= round($image_height_limit);

			$blocks = array();
			foreach($this->_graphic_blocks as $graphic_block_cmodel) { 
			
				$x2 = $graphic_block_cmodel->get_x2();
				$y2 = $graphic_block_cmodel->get_y2();
		
				if($graphic_block_cmodel->get_width()<10)
					$x2 += 10;
					
				if($graphic_block_cmodel->get_height()<10)
					$y2 += 5;
				
				$block["x1"] 		= round($graphic_block_cmodel->get_x1() / $ratio);
				$block["x2"] 		= round($x2 / $ratio);
				$block["y1"] 		= round($graphic_block_cmodel->get_y1() / $ratio);
				$block["y2"] 		= round($y2 / $ratio);
				$block["default"] 	= addslashes($graphic_block_cmodel->get_value());
				$block["id"]		= call_user_func(array($graphic_block_cmodel,$this->_id_function));
				$block["index"] 	= $graphic_block_cmodel->get_index();
				
				$blocks[] = $block;
			}				
			
			$this->_options["height"] 		= $image_height_limit;
			$this->_options["width"] 		= $image_width_limit;
			$this->_options["block_url"] 	= $this->_block_url;
			$this->_options["update_url"] 	= $this->_update_url;
			$this->_options["delete_url"] 	= $this->_delete_url;
			$this->_options["preview_url"]	= $this->_preview_url;
			$this->_options["ratio"] 		= $ratio;
			$this->_options["blocks"] 		= $blocks;

			$this->set_var("options",$this->_options);
			$this->set_var("height",$image_height_limit);
			$this->set_var("width",$image_width_limit);
			$this->set_var("image_width_limit",$image_width_limit);
			$this->set_var("image_height_limit",$image_height_limit);
			$this->set_var("blocks",$this->_graphic_blocks);
			$this->set_var("id_function",$this->_id_function);
			$this->set_var("preview_url",$this->_preview_url);
			$this->set_var("ratio",$ratio);
			$this->set_var("block_url",$this->_block_url);
			$this->set_var("delete_url",$this->_delete_url);
			$this->set_var("update_url",$this->_update_url);
			$this->set_var("window_width",$this->_window_width);
			$this->set_var("window_height",$this->_window_height);			
		}
		
		function set_site($site_cmodel) 				{ $this->_site_cmodel = $site_cmodel; }
		function set_preview_url($preview_url) 			{ $this->_preview_url = $preview_url; }
		function set_id_function($id_function) 			{ $this->_id_function = $id_function; }
		function set_design_file($design_file) 			{ $this->_design_file = $design_file; }
		function set_update_url($update_url) 			{ $this->_update_url = $update_url; }
		function set_block_url($block_url) 				{ $this->_block_url = $block_url; }
		function set_graphic_blocks($graphic_blocks) 	{ $this->_graphic_blocks = $graphic_blocks; }
		function set_window_height($window_height) 		{ $this->_window_height = $window_height; }
		function set_window_width($window_width) 		{ $this->_window_width = $window_width; }
		function set_width($width) 						{ $this->_width = $width; }
		function set_height($height) 					{ $this->_height = $height; }

		function disable_move()		 					{ $this->_options["move"] = false; }
		function disable_add()		 					{ $this->_options["add"] = false; }
		function disable_remove()		 				{ $this->_options["remove"] = false; }

		function include_web_assets($web_asset_manager_cmodel) {

			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/spin/spin.js");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/colorpicker/css/colorpicker.css");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/colorpicker/js/colorpicker.js");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/jcrop/js/jquery.Jcrop.js");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/jcrop/css/jquery.Jcrop.css");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/minicolors/jquery.miniColors.css");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/minicolors/jquery.miniColors.js");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/designer/jquery.designer.css");
			$web_asset_manager_cmodel->add_js_lib("jquery/plugins/designer/jquery.designer.js");
			$web_asset_manager_cmodel->add_js_lib("upload/upload.js");			
		}
		
	}