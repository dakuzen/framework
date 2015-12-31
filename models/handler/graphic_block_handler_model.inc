<?
	class BASE_HMODEL_GRAPHIC_BLOCK extends MODEL_HANDLER {
	
		protected $_select_fields = array("graphic_blocks.*");
		
		function get_dbq() {
			$graphic_block_dbq = new BASE_DBQ_GRAPHIC_BLOCK();
			$this->apply_constraints($graphic_block_dbq);
			return $graphic_block_dbq;
		}
		
		function get_graphic_blocks() {
			$graphic_block_dbq = $this->get_dbq();
			$graphic_blocks = $graphic_block_dbq->select($this->_select_fields);
			$graphic_block_cmodels = array();
			
			foreach($graphic_blocks as $graphic_block) {
				$graphic_block_cmodel = new BASE_CMODEL_GRAPHIC_BLOCK();
				$graphic_block_cmodel->populate($graphic_block,true,false);
				$this->apply_properties($graphic_block_cmodel,$graphic_block);
				$graphic_block_cmodels[] = $graphic_block_cmodel;
			}
			
			if(!$graphic_block_cmodels)
				return array();
			
			return $graphic_block_cmodels;
		}
		
		function get_graphic_block($graphic_block_id) {
			
			if(self::is_called_staticly(__CLASS__)) {
				$graphic_block_hmodel = new BASE_HMODEL_GRAPHIC_BLOCK();
				return $graphic_block_hmodel->get_graphic_block($graphic_block_id);
			}
			
			$this->set_graphic_block_id($graphic_block_id);
			return get_value($this->get_graphic_blocks(),0);
		}
		
		function get_count($field=null) 	{ return $this->get_dbq()->count($field); }
		function exists($field=null)		{ return $this->get_dbq()->exists($field); }
		function set_graphic_block_ids($values)	{ return $this->add_filter("graphic_blocks.graphic_block_id","IN",$values); }
		function set_graphic_block_id($value)	{ return $this->add_filter("graphic_blocks.graphic_block_id","=",$value); }
		function set_state($value)		{ return $this->add_filter("graphic_blocks.state","=",$value); }
		function set_states($values)		{ return $this->add_filter("graphic_blocks.state","IN",$values); }

	}
	