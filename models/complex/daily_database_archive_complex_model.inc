<?
	/**
	* Class BASE_CMODEL_DAILY_DATABASE_ARCHIVE
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	class BASE_CMODEL_DAILY_DATABASE_ARCHIVE extends BASE_CMODEL_DATABASE_ARCHIVE {

		function __construct($db_host,$db_name,$db_user,$db_pass,$backup_directory="",$options=array()) {
			parent::__construct($db_host,$db_name,$db_user,$db_pass,$backup_directory,$options);
			$this->set_basename($this->get_basename()."-daily");
		
		}
		
		public function can_purge($filename,$modified_time,$retention_time) {
			return parent::can_purge($filename,$modified_time,$retention_time) && !preg_match("/-manual/i",$filename);
		}
	}
	