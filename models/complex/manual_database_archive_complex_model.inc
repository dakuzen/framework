<?
	/**
	* Class BASE_CMODEL_MANUAL_DATABASE_ARCHIVE
	*
	* @package Framework
	* @subpackage Cmodels
	*/

	class BASE_CMODEL_MANUAL_DATABASE_ARCHIVE extends BASE_CMODEL_DATABASE_ARCHIVE {

		public function get_basename()				{ return $this->_db_name."_".date("Y-m-d-H-i-s")."-manual"; }
	
	}
	