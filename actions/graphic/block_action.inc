<?php

	class BASE_ACTION_GRAPHIC_BLOCK extends ACTION_JSON {
		 
		function get_type() 				{ return REGISTRY::get_instance()->get_request_parm("type"); }
		function get_ratio() 				{ return REGISTRY::get_instance()->get_request_parm("ratio"); }
		function get_value() 				{ return REGISTRY::get_instance()->get_request_parm("value"); }
		function get_id() 					{ return REGISTRY::get_instance()->get_request_parm("id"); }
		function get_font_size() 			{ return REGISTRY::get_instance()->get_request_parm("font_size"); }
		function get_font_name() 			{ return REGISTRY::get_instance()->get_request_parm("font_name"); }
		function get_halign()				{ return REGISTRY::get_instance()->get_request_parm("halign"); }
		function get_font_color()			{ return REGISTRY::get_instance()->get_request_parm("font_color"); }
		function get_x1() 					{ return REGISTRY::get_instance()->get_request_parm("x1"); }
		function get_y1() 					{ return REGISTRY::get_instance()->get_request_parm("y1"); }
		function get_x2() 					{ return REGISTRY::get_instance()->get_request_parm("x2"); }
		function get_y2() 					{ return REGISTRY::get_instance()->get_request_parm("y2"); }
		function get_action() 				{ return REGISTRY::get_instance()->get_request_parm("action"); }
		function is_action_move()			{ return $this->get_action()=="move"; }
		function is_action_delete()			{ return $this->get_action()=="delete"; }
		function is_action_update()			{ return $this->get_action()=="update"; }
		function is_action_upload()			{ return $this->get_action()=="upload"; }
		function is_action_back()			{ return $this->get_action()=="back"; }
		function is_action_front()			{ return $this->get_action()=="front"; }
		function is_action_image_remove()	{ return $this->get_action()=="image-remove"; }

		public function populate($graphic_block_cmodel) {

			if($this->get_action()=="move" || $this->get_action()=="update") {
				
				if($this->get_action()=="move" || !$graphic_block_cmodel->get_graphic_block_id()) {

					if(is_numeric($this->get_x1()))
						$graphic_block_cmodel->set_x1($this->get_x1() * $this->get_ratio());

					if(is_numeric($this->get_y1()))
						$graphic_block_cmodel->set_y1($this->get_y1() * $this->get_ratio());

					if(is_numeric($this->get_x2()))
						$graphic_block_cmodel->set_x2($this->get_x2() * $this->get_ratio());

					if(is_numeric($this->get_y2()))
						$graphic_block_cmodel->set_y2($this->get_y2() * $this->get_ratio());
				}
				
				if($this->get_type()!==null)
					$graphic_block_cmodel->set_type($this->get_type());

				if($this->get_value()!==null)
					$graphic_block_cmodel->set_value($this->get_value());

				if($this->get_font_size()!==null)
					$graphic_block_cmodel->set_font_size($this->get_font_size());

				if($this->get_font_name()!==null)
					$graphic_block_cmodel->set_font_name($this->get_font_name());

				if($this->get_font_color()!==null)
					$graphic_block_cmodel->set_font_color($this->get_font_color());

				if($this->get_halign()!==null)
					$graphic_block_cmodel->set_halign($this->get_halign());	
			}
		}

	}		
