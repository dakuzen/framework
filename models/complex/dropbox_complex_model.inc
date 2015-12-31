<?
	SERVER_UTIL::add_include_path_pear();

	include 'Dropbox/autoload.php';

	class BASE_CMODEL_DROPBOX extends Dropbox_API {

		public static function create_oauth($consumer_key,$consumer_secret) {
			return new Dropbox_OAuth_PEAR($consumer_key,$consumer_secret);
		}
		
		public function put($local_file,$remote_file) {
			return $this->putFile($remote_file,$local_file);
		}
	}