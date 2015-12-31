<?
	class BASE_DBO_MESSAGE_QUEUE_ATTACHMENT extends DBO {

		function __construct() {
			$this->_tablename = "message_queue_attachments";
			$this->_columns["message_queue_attachment_id"] = new DBO_COLUMN("int",11,false,true);
			$this->_columns["message_queue_id"] = new DBO_COLUMN("int",11,false,false);
			$this->_columns["state"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["guid"] = new DBO_COLUMN("varchar",40,true,false);
			$this->_columns["type"] = new DBO_COLUMN("char",1,true,false);
			$this->_columns["filesize"] = new DBO_COLUMN("int",11,true,false);
			$this->_columns["filename"] = new DBO_COLUMN("varchar",100,true,false);
		}
	}