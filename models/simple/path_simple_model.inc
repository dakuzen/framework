<?
	class BASE_MODEL_PATH extends MODEL {

		static function get_instance_directory_name()			{ return basename(DIR_INSTANCE); }
		static function get_application_directory()				{ return self::get_application_dir(); }
		static function get_instance_directory()				{ return self::get_instance_dir(); }
		static function get_framework_directory()				{ return self::get_framework_dir(); }
		static function get_framework_assets_directory()		{ return self::get_framework_assets_dir(); }
		static function get_framework_packages_directory()		{ return self::get_framework_packages_dir(); }
		static function get_framework_libraries_directory()		{ return self::get_framework_libraries_dir(); }
		static function get_application_packages_directory()	{ return self::get_application_packages_dir(); }
		static function get_data_directory()					{ return self::get_data_dir(); }
		static function get_logs_directory()					{ return self::get_logs_dir(); }
		static function get_public_directory()					{ return self::get_public_dir(); }
		static function get_temp_directory()					{ return self::get_temp_dir(); }
		static function get_process_directory()					{ return self::get_process_dir(); }
		static function get_temporary_directory()				{ return self::get_temporary_dir(); }
		static function get_content_directory()					{ return self::get_content_dir(); }
		static function get_backups_directory()					{ return self::get_backups_dir(); }
		static function get_web_directory()						{ return self::get_application_dir(); }
		static function get_langs_directory()					{ return self::get_langs_dir(); }
		static function get_public_url($time=null)				{ return "/public/".($time ? $time."/" : ""); }

		static function get_application_dir()					{ return DIR_APPLICATION; }
		static function get_backend_dir()						{ return DIR_APPLICATION; }
		static function get_instance_dir()						{ return DIR_INSTANCE; }
		static function get_framework_dir()						{ return DIR_FRAMEWORK; }
		static function get_framework_assets_dir()				{ return self::get_framework_dir()."assets/"; }
		static function get_framework_packages_dir()			{ return self::get_framework_dir()."packages/"; }
		static function get_framework_libraries_dir()			{ return self::get_framework_dir()."libraries/"; }
		static function get_application_packages_dir()			{ return self::get_application_dir()."packages/"; }
		static function get_data_dir()							{ return self::get_instance_dir()."data/"; }
		static function get_logs_dir()							{ return self::get_instance_dir()."logs/"; }
		static function get_public_dir()						{ return self::get_data_dir()."public/"; }
		static function get_temp_dir()							{ return self::get_data_dir()."temp/"; }
		static function get_process_dir()						{ return self::get_data_dir()."process/"; }
		static function get_temporary_dir()						{ return self::get_temp_dir().MISC_UTIL::get_random_uid()."/"; }
		static function get_content_dir()						{ return self::get_public_dir()."content/"; }
		static function get_backups_dir()						{ return self::get_instance_dir()."backups/"; }
		static function get_web_dir()							{ return self::get_application_dir()."web/"; }
		static function get_langs_dir()							{ return self::get_application_dir()."langs/"; }
	}