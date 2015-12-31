<?
	class MODEL_GAME extends MODEL {
		
		static function get_title($baby_cmodel,$baby_game_guess_cmodel) {
			return LANG_UTIL::get_posessive($baby_cmodel->get_name())." ".($baby_game_guess_cmodel->get_game() ? $baby_game_guess_cmodel->get_game()->get_name() : "");
		}
	}