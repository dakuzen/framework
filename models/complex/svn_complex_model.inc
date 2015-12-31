<?php
        /**
        * Class BASE_CMODEL_SVN
        *
        * @package Framework
        * @subpackage Cmodels
        */
		
	class BASE_CMODEL_SVN extends MODEL {
		
		/**
		 * The host
		 * @var string $host 
		 */
		private $_host			= "";
		
		/**
		 * The username
		 * @var string $username
		 */
		private $_username		= "";
		
		/**
		 * The password
		 * @var string $password
		 */
		private $_password		= "";
		
		/**
		 * The verbose mode
		 * @var integer $is_verbose_mode 
		 */
		private $_is_verbose_mode 	= 0;
		
		/**
		 * The debug mode
		 * @var boolean $debug 
		 */
		private $_debug		 	= false;
		
		/**
		 * The protocol
		 * @var string $protocol
		 */
		private $_protocol	 	= "https";
		
		/**
		 * Constructor
		 * @param string $host
		 * @param string $username
		 * @param string $password
		 * @param integer $is_verbose_mode 
		 */
		function __construct($host="",$username="",$password="",$is_verbose_mode=false) {
			$this->_host 			= $host;
			$this->_username 		= $username;
			$this->_password 		= $password;
			$this->_is_verbose_mode		= $is_verbose_mode;
		}

		/**
		 * Verobose 
		 */
		function verbose() 		{ $this->_is_verbose_mode = true; }
		
		/**
		 * Debug
		 * @param boolean $debug 
		 */
		function debug($debug=true) 	{ $this->_debug = $debug; }
		
		/**
		 * is debug
		 * @return boolean 
		 */
		function is_debug() 		{ return $this->_debug; }
	
		/**
		 * set protocol
		 * @param string $protocol 
		 */
		function set_protocol($protocol) { $this->_protocol = $protocol; }
		
		 /**
		  * Returns info output
		  * @param string $repo_dir
		  * @param array $output
		  * @return boolean 
		  */
		function get_info_output($repo_dir,&$output) {
			$cmd = $this->get_svn_command()." info %s";
			$cmd = sprintf($cmd,$repo_dir);	
			return $this->exec($cmd,$output);		
		}
		 
		/**
		 * Returns info
		 * @param string $repo_dir
		 * @return array 
		 */
		function get_info($repo_dir) {
				
			$has_success = $this->get_info_output($repo_dir,$output);
		
			
			$info = array();
			
			if($has_success) {
			
				foreach($output as $line) {
					
					if(($break_point=strpos($line,":"))!==false) {
					
						$key 	= str_replace(" ","_",strtolower(substr($line,0,$break_point)));
						$value	= substr($line,$break_point + 1);					
						$info[$key] = $value;
					}
				}
			}
					
			return $info;
		}

		/**
		 * Return repo url
		 * @return string 
		 */
		function get_repo_url() {
			return $this->_protocol."://".$this->_host;
		}
		
		/**
		 * Returns the list
		 * @param string $repo_path
		 * @return array 
		 */
		function get_list($repo_path) {
	
			$cmd = $this->get_svn_command()." list ".$this->_protocol."://%s%s";
			$cmd = sprintf($cmd,$this->_host,$repo_path);	
			
			$this->exec($cmd,$output);
			
			return $output;
		}
		
		/**
		 * Repo exists
		 * @param string $repo_path
		 * @return boolean 
		 */
		function repo_exists($repo_path) {
	
			$cmd = $this->get_svn_command()." list ".$this->_protocol."://%s%s";
			$cmd = sprintf($cmd,$this->_host,$repo_path);	
			
			return $this->exec($cmd);
		}		
		
		/**
		 * Revert
		 * @param string $repo_dir
		 * @return boolean 
		 */
		function revert($repo_dir) {
	
			$cmd = $this->get_svn_command()." revert --recursive %s";
			$cmd = sprintf($cmd,$repo_dir);	
			
			return $this->exec($cmd);		
		}
		
		/**
		 * Cleanup
		 * @param string $repo_dir
		 * @return boolean 
		 */
		function cleanup($repo_dir) {
	
			$cmd = $this->get_svn_command()." cleanup %s";
			$cmd = sprintf($cmd,$repo_dir);	
			
			return $this->exec($cmd);		
		}		
		
		/**
		 * Update
		 * @param string $repo_dir
		 * @param string $revert
		 * @return boolean 
		 */
		function update($repo_dir,$revert=false) {
			$has_success = true;
			
			if($revert)
				$has_success = $this->revert($repo_dir);	
			
			if($has_success) {
			
				$cmd = $this->get_svn_command()." update ".$this->get_credentials()."%s -r HEAD";
				$cmd = sprintf($cmd,$repo_dir);	
				$has_success = $this->exec($cmd);		
			}
			
			return $has_success;			
		}
		
		/**
		 * Checkout
		 * @param string $checkout_directory
		 * @param string $repo_path
		 * @return boolean 
		 */
		function checkout($checkout_directory,$repo_path,$options=array()) {

			$options = $options ? ARRAY_UTIL::get_imploded($options," ",""," ")." " : "";

			$cmd = $this->get_svn_command()." checkout ".$options.$this->_protocol."://%s%s %s"; 			
			$cmd = sprintf($cmd,$this->_host,$repo_path,$checkout_directory);	

			return $this->exec($cmd);
		}

		/**
		 * Returns the credentials
		 * @return string
		 */
		function get_credentials() {
			$credentials = "";

			if($this->_username)
				$credentials .= "--username ".$this->_username." ";

			if($this->_password)
				$credentials .= "--password ".$this->_password." ";

			return $credentials;
		}
		
		/**
		 * Commit
		 * @param array $paths
		 * @param string $message
		 * @return boolean 
		 */
		function commit($paths,$message="") {
			
			$paths = is_array($paths) ? $paths : array($paths);
			
			$message = $message ? ' -m "'.str_replace('"','\\"',$message).'"' : "";

			$cmd = $this->get_svn_command()." commit ".$this->get_credentials()."%s%s"; 			
			$cmd = sprintf($cmd,implode(" ",$paths),$message);	

			return $this->exec($cmd);
		}
		
		/**
		 * Adds
		 * @param array $paths
		 * @param boolean $recursive
		 * @return boolean 
		 */
		function add($paths,$recursive=true) {
				
			$paths = is_array($paths) ? $paths : array($paths);
						
			$has_success = true;
			
			foreach($paths as $path) {

				$cmd = $this->get_svn_command()." add --force ".($recursive ? "" : "--non-recursive")." %s"; 			
				$cmd = sprintf($cmd,$path);	
				$has_success &= $this->exec($cmd);
			}
			
			return $has_success;
		}		
		
		/**
		 * Point
		 * @param string $repo_dir
		 * @param string $repo_url
		 * @return boolean 
		 */
		function point($repo_dir,$repo_url) {

			$repo_url = URL_UTIL::has_protocol($repo_url) ? $repo_url : $this->get_repo_url().$repo_url;
			
			$cmd = $this->get_svn_command()." switch %s %s";
			$cmd = sprintf($cmd,$repo_url,$repo_dir);	
			
			return $this->exec($cmd);		
		}
		
		/**
		 * Copy command
		 * @param string $from_repo_path
		 * @param string $to_repo_path
		 * @param string $comments
		 * @return boolean 
		 */
		function copy($from_repo_path,$to_repo_path,$comments="") {
			
			$comments = $comments ? ' -m "'.$comments.'"' : "";
			
			$cmd = $this->get_svn_command()." copy ".$this->_protocol."://%s%s ".$this->_protocol."://%s%s%s";
			$cmd = sprintf($cmd,$this->_host,$from_repo_path,$this->_host,$to_repo_path,$comments);	
			
			return $this->exec($cmd);		
		}
			
		/**
		 * Make a new directory
		 * @param string $repo_path
		 * @param string $comments
		 * @return boolean 
		 */
		function mkdir($repo_path,$comments="") {
			
			$comments = $comments ? ' -m "'.$comments.'"' : "";
			
			$cmd = $this->get_svn_command()." mkdir ".$this->_protocol."://%s%s%s";
			$cmd = sprintf($cmd,$this->_host,$repo_path,$comments);	
			
			return $this->exec($cmd);			
		}
		
		/**
		 * Delete
		 * @param string $repo_path
		 * @param string $comments
		 * @return boolean 
		 */
		function delete($repo_path,$comments="") {
			
			$comments = $comments ? ' -m "'.$comments.'"' : "";
			
			$cmd = $this->get_svn_command()." delete ".$this->_protocol."://%s%s%s";
			$cmd = sprintf($cmd,$this->_host,$repo_path,$comments);	
			
			return $this->exec($cmd);			
		}		
		
		/**
		 * Notify message callback
		 * @param string $message 
		 */
		function notify_message_callback($message) {}
		
		/**
		 * Return the svn command
		 * @return string 
		 */	
		function get_svn_command() {			

			$cmd = SERVER_UTIL::is_os_windows() ? "svn.exe" : "svn";
			$cmd .= " --non-interactive ";

			if($this->_username && $this->_password) { 
				$cmd .= " --username %s --password %s ";
				$cmd = sprintf($cmd,$this->_username,$this->_password);
			}

			return $cmd;
		}
		
		/**
		 * Exec
		 * @param string $cmd
		 * @param array $output
		 * @return boolean
		 * @throws Exception 
		 */
		function exec($cmd,&$output=array()) {

			if($this->is_debug())
				p($cmd);
			
			if($this->_is_verbose_mode)
				APPLICATION::display_notify_message($cmd);
				
			$this->notify_message_callback($cmd);
			
			$has_success = SERVER_UTIL::exec($cmd,$output,true);
						
			if(!$has_success) {
				
				$message = implode("\n",$output);

				if($this->_is_verbose_mode)
					APPLICATION::display_error_message($message);

				throw new Exception($message);				
			}
			
			return $has_success;	
		}		
	}