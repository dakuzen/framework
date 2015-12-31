<?
	class BASE_DBQ_GRAPHIC_BLOCK extends DBQ {

		const STATE_ACTIVE	= "A";
		const STATE_DELETED	= "D";
		
		const TYPE_TEXT		= "T";
		const TYPE_IMAGE	= "I";
		const TYPE_BARCODE	= "B";

		const HALIGN_LEFT	= "L";
		const HALIGN_RIGHT	= "R";
		const HALIGN_CENTER	= "C";
		const HALIGN_JUSTIFY	= "J";

		static function get_halign_list()	{ return array(self::HALIGN_LEFT=>"Left",self::HALIGN_CENTER=>"Center",self::HALIGN_RIGHT=>"Right"); }

		static function get_state_list()	{ return array(	self::STATE_ACTIVE=>"Active",self::STATE_DELETED=>"Deleted"); }
		
		static function get_type_list()		{ return array(	self::TYPE_TEXT=>"Text",self::TYPE_IMAGE=>"Image",self::TYPE_BARCODE=>"Barcode"); }

		public function __construct() {
			parent::__construct("graphic_blocks", "graphic_block_id");
		}
	}