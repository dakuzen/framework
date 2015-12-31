<?

	class BASE_HMODEL_BACKUP extends MODEL_HANDLER {

		protected $_select_fields = array("backups.*");

		function get_dbq() {

			$backup_dbq = new BASE_DBQ_BACKUP();

			$this->apply_constraints($backup_dbq);

			return $backup_dbq;
		}

		function get_backups() {

			$backup_dbq = $this->get_dbq();

			$backups = $backup_dbq->select($this->_select_fields);

			$backup_cmodels = array();

			foreach($backups as $backup) {

				$backup_cmodel = new BASE_CMODEL_BACKUP();
				$backup_cmodel->populate($backup,true);
				$this->apply_properties($backup_cmodel,$backup);
				$backup_cmodels[] = $backup_cmodel;
			}

			return $backup_cmodels;

		}

		function get_count() {
			return $this->get_dbq()->count();
		}

		function get_backup($backup_id) { 

			$backup_handler = new BASE_HMODEL_BACKUP();
			$backup_handler->set_backup_ids(array($backup_id));
			return get_value($backup_handler->get_backups(),0);
		} 

		function set_backup_ids($backup_ids) 	{ $this->add_filter("backup_id","IN",$backup_ids); }
		function set_backup_id($backup_id) 	{ $this->add_filter("backup_id","=",$backup_id); }

	}