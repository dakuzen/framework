<?

	class BASE_MODEL_IMAGE_ICON extends MODEL {
		
		public static function get_image_icon($filename,$alt="",$attributes=array()) {
			$attributes["alt"] = $alt;
			return HTML_UTIL::image("/lib/images/icons/admin/".$filename,$attributes);
		}
			
		public static function get_view($alt="View",$attributes=array()) 				{ return self::get_image_icon("view.png",$alt,$attributes); }	
		public static function get_add($alt="Add",$attributes=array()) 					{ return self::get_image_icon("add.png",$alt,$attributes); }
		public static function get_edit($alt="Edit",$attributes=array()) 				{ return self::get_image_icon("edit.png",$alt,$attributes); }
		public static function get_download($alt="Download",$attributes=array()) 		{ return self::get_image_icon("download.png",$alt,$attributes); }
		public static function get_upload($alt="Upload",$attributes=array()) 			{ return self::get_image_icon("upload.png",$alt,$attributes); }
		public static function get_up($alt="Up",$attributes=array()) 					{ return self::get_image_icon("uparrow.png",$alt,$attributes); }
		public static function get_down($alt="Down",$attributes=array()) 				{ return self::get_image_icon("downarrow.png",$alt,$attributes); }
		public static function get_delete($alt="Delete",$attributes=array()) 			{ return self::get_image_icon("delete.png",$alt,$attributes); }
		public static function get_blank() 												{ return self::get_image_icon("blank.png"); }
		public static function get_preview($alt="",$attributes=array()) 				{ return self::get_image_icon("preview.png",$alt,$attributes); }
		public static function get_generate($alt="Generate",$attributes=array()) 		{ return self::get_image_icon("generate.png",$alt,$attributes); }
		public static function get_process($alt="Process",$attributes=array()) 			{ return self::get_image_icon("generate.png",$alt,$attributes); }
		public static function get_impresonate($alt="Impersonate",$attributes=array())	{ return self::get_image_icon("impresonate.png",$alt,$attributes); }
		public static function get_reset($alt="Reset",$attributes=array()) 				{ return self::get_image_icon("reset.png",$alt,$attributes); }
		public static function get_check($alt="",$attributes=array()) 					{ return self::get_image_icon("check.png",$alt,$attributes); }
		public static function get_star($alt="",$attributes=array()) 					{ return self::get_image_icon("star.png",$alt,$attributes); }
		public static function get_print($alt="Print",$attributes=array()) 				{ return self::get_image_icon("print.png",$alt,$attributes); }
		public static function get_report($alt="Report",$attributes=array()) 			{ return self::get_image_icon("report.png",$alt,$attributes); }
		public static function get_calendar($alt="",$attributes=array()) 				{ return self::get_image_icon("calendar.png",$alt,$attributes); }
		public static function get_archive($alt="",$attributes=array()) 				{ return self::get_image_icon("archive.png",$alt,$attributes); }		

		public static function get_default($alt="Default",$attributes=array()) 			{ return self::get_image_icon("default.png",$alt,$attributes); }
		public static function get_drag($alt="Drag",$attributes=array()) 				{ return self::get_image_icon("drag.png",$alt,$attributes); }
		public static function get_template($alt="Template",$attributes=array()) 		{ return self::get_image_icon("template.png",$alt,$attributes); }
		public static function get_plus($alt="",$attributes=array()) 					{ return self::get_image_icon("plus.png",$alt,$attributes); }
		public static function get_critical($alt="",$attributes=array()) 				{ return self::get_image_icon("critical.png",$alt,$attributes); }
		public static function get_go($alt="",$attributes=array()) 						{ return self::get_image_icon("go.png",$alt,$attributes); }
		public static function get_debug($alt="",$attributes=array()) 					{ return self::get_image_icon("debug.png",$alt,$attributes); }				
		public static function get_blocks($alt="Blocks",$attributes=array()) 			{ return self::get_image_icon("blocks.png",$alt,$attributes); }
		public static function get_caution($alt="Caution",$attributes=array()) 			{ return self::get_image_icon("caution.png",$alt,$attributes); }
		public static function get_refresh($alt="Refresh",$attributes=array()) 			{ return self::get_image_icon("refresh.png",$alt,$attributes); }
		public static function get_start($alt="Start",$attributes=array()) 				{ return self::get_image_icon("start.png",$alt,$attributes); }
		public static function get_restart($alt="Restart",$attributes=array()) 			{ return self::get_image_icon("restart.png",$alt,$attributes); }
		public static function get_rss($alt="RSS",$attributes=array()) 					{ return self::get_image_icon("rss.png",$alt,$attributes); }
		public static function get_link($alt="",$attributes=array()) 					{ return self::get_image_icon("link.png",$alt,$attributes); }
		public static function get_play($alt="Play",$attributes=array()) 				{ return self::get_image_icon("play.png",$alt,$attributes); }
	
		public static function get_file_url($extension)									{ return "/lib/images/icons/files/".$extension.".png"; }
		public static function get_close_folder_url()									{ return "/lib/images/icons/files/folder-close.png"; }		
		public static function get_excel($alt="",$attributes=array()) 					{ return HTML_UTIL::image(self::get_file_url("xls"),$attributes); }
		public static function get_word($alt="",$attributes=array()) 					{ return HTML_UTIL::image(self::get_file_url("doc"),$attributes); }		
		public static function get_file($extension,$alt="")								{ return HTML_UTIL::image(self::get_file_url($extension),array("title"=>$alt)); }
		public static function get_file_generic($alt="")								{ return HTML_UTIL::image("/lib/images/icons/files/generic.png",array("title"=>$alt)); }
		public static function get_open_folder()										{ return HTML_UTIL::image("/lib/images/icons/files/folder-open.png"); }
		public static function get_close_folder()										{ return HTML_UTIL::image("/lib/images/icons/files/folder-close.png"); }
		
		public static function get_pdf($alt="")											{ return self::get_file("pdf",$alt); }
		
	}	