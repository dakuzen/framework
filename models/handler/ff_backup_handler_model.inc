<?
	class BHMODEL_FF_BACKUP extends HMODEL {
	
		protected $_select_fields = array("ff_backups.*");
		
		public function get_dbq() {
			return $this->apply_constraints(DBQ_FF_BACKUP::create());
		}
		
		public function gets() {			
			$ff_backups = $this->get_dbq()->select($this->_select_fields);

			$ff_backup_cmodels = array();			
			foreach($ff_backups as $ff_backup) {
				$ff_backup_cmodel = CMODEL_FF_BACKUP::create()->populate($ff_backup,true,false);
				$this->apply_properties($ff_backup_cmodel,$ff_backup);
				$ff_backup_cmodels[] = $ff_backup_cmodel;
			}
			
			if(!$ff_backup_cmodels)
				return array();
		
			return $ff_backup_cmodels;
		}
		
		public function set_ff_backup_ids($values)	{ return $this->where("ff_backups.ff_backup_id","IN",$values); }
		public function set_ff_backup_id($value)	{ return $this->where("ff_backups.ff_backup_id","=",$value); }
		public function set_state($value)				{ return $this->where("ff_backups.state","=",$value); }
		public function set_states($values)				{ return $this->where("ff_backups.state","IN",$values); }				
	}