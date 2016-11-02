<?
	class FTPS_UTIL extends FTP_UTIL {

		public function __construct($host="", $user="", $pass="", $port=990, $options=array("passive_mode"=>true,"ssl"=>true)) {
			parent::__construct($host,$user,$pass,$port,$options);
		}

		public static function create($host="", $user="", $pass="", $port=990, $options=array("passive_mode"=>true,"ssl"=>true)) {
			return new FTPS_UTIL($host,$user,$pass,$port,$options);
		}

	}
