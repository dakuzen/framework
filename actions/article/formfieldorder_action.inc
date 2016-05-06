<?
	class BASE_ACTION_ARTICLE_FORMFIELDORDER extends BASE_ACTION_ARTICLE_BASE {
		
		function process() {
		
			$form_field_ids	= (array)$this->post("ffid");
				
			DB::start_transaction();
			
			foreach($form_field_ids as $index=>$form_field_id) {
				DBQ_FF_FORM_FIELD::create()
					->where("form_id","=",$this->get("fid"))
					->where("form_field_id","=",$form_field_id)
					->update(array("priority"=>$index));
			}
			
			$this->success(DB::complete_transaction());
		}
	}
	

