<?
    /**
    * Class BASE_CMODEL_SITE_CONTENT_WIDGET
    *
    * @package Framework
    * @subpackage Cmodels
    */

	class BASE_CMODEL_SITE_CONTENT_WIDGET extends BASE_CMODEL_FF_CONTENT_WIDGET {

        public function create_site_content_widget($tag,$name,$default="",$is_tag_readonly=false,$vars=array()) {
        	return self::create($tag,$name,$default,$is_tag_readonly,$vars);
        }

	/*	public static function get($tag) {

			$content_widget_cmodel = MODEL_SITE_CONTENT_WIDGET::get_site_content_widget($tag);

			if($site_content_widget_cmodel)
				return $site_content_widget_cmodel->content();
		}*/
	}