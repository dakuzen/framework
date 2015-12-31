<?
	require_once DIR_FRAMEWORK."packages/constantcontact/ConstantContact.php"; 
	
	class BASE_CMODEL_CONSTANT_CONTACT extends ConstantContact {

		function __construct($api_key,$user_name,$password) {
			parent::__construct("basic", $api_key, $user_name, $password);
		}
		
		function getLists() { return @parent::getLists(); }
	}
	