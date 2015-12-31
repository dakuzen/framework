<?php
        /**
        * Class BASE_CMODEL_LINKEDIN
        *
        * @package Framework
        * @subpackage Cmodels
        */
 	
 	class BASE_CMODEL_LINKEDIN {
		
                /**
                 * The array
                 * @var array $array 
                 */
		protected $_array = array();
		
                /**
                 * Constructor
                 * @param string $consumer_key
                 * @param string $consumer_secret 
                 */
		function __construct($consumer_key,$consumer_secret) {
			
			define('CONNECTION_COUNT', 20);
			define('UPDATE_COUNT', 10);
			
			$API_CONFIG = array(
				'appKey'       => '<your application key here>',
				'appSecret'    => '<your application secret here>',
				'callbackUrl'  => NULL 
			);	
			
			$this->_linkedin = new LinkedIn($API_CONFIG);
		}
		
                /**
                 * Return the request token
                 * @param string $callback_url 
                 */
		function get_request_token($callback_url) {

			$response = $this->_linkedin->retrieveTokenRequest();
			
			p($response,$this->_linkedin);
			
			die;
			
			if($response['success'] === TRUE) {
			
				//$_SESSION['oauth']['linkedin']['request'] = $response['linkedin'];

				//Header('Location: ' . LINKEDIN::_URL_AUTH . $_SESSION['oauth']['linkedin']['request']['oauth_token']);
			}
			
			/*
			// LinkedIn has sent a response, user has granted permission, take the temp access token, the user's secret and the verifier to request the user's real secret key
			$response = $this->_linkedin->retrieveTokenAccess($_GET['oauth_token'], $_SESSION['oauth']['linkedin']['request']['oauth_token_secret'], $_GET['oauth_verifier']);
			
			if($response['success'] === TRUE) {
				$_SESSION['oauth']['linkedin']['access'] = $response['linkedin'];

				// set the user as authorized for future quick reference
				$_SESSION['oauth']['linkedin']['authorized'] = TRUE;
			}
			*/
		}

                /**
                 * Share 
                 */
		function share() {
			// process a status update action
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);

			// prepare content for sharing
			$content = array();
			if(!empty($_POST['scomment'])) {
			$content['comment'] = $_POST['scomment'];
			}
			if(!empty($_POST['stitle'])) {
			$content['title'] = $_POST['stitle'];
			}
			if(!empty($_POST['surl'])) {
			$content['submitted-url'] = $_POST['surl'];
			}
			if(!empty($_POST['simgurl'])) {
			$content['submitted-image-url'] = $_POST['simgurl'];
			}
			if(!empty($_POST['sdescription'])) {
			$content['description'] = $_POST['sdescription'];
			}
			if(!empty($_POST['sprivate'])) {
			$private = TRUE;
			} else {
			$private = FALSE;
			}

			// share content
			$response = $this->_linkedin->share('new', $content, $private);
			if($response['success'] === TRUE) {
			// status has been updated
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error sharing content:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
		}

		/**
                 * Comment 
                 */
		function comment() {

			/*
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			if(!empty($_POST['nkey'])) {
			$response = $this->_linkedin->comment($_POST['nkey'], $_POST['scomment']);
			
			if($response['success'] === TRUE) {
			// comment posted

			} else {
				echo "Error commenting on update:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			} else {
				echo "You must supply a network update key to comment on an update.";
			}
			*/
		}

                /** 
                 * Invite 
                 */
		function invite() {
			/*
			// invitation messaging
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			if(!empty($_POST['invite_to_id'])) {
			// send invite via LinkedIn ID
			$response = $this->_linkedin->invite('id', $_POST['invite_to_id'], $_POST['invite_subject'], $_POST['invite_body']);
			if($response['success'] === TRUE) {
			// message has been sent
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error sending invite:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			} elseif(!empty($_POST['invite_to_email'])) {
			// send invite via email
			$recipient = array('email' => $_POST['invite_to_email'], 'first-name' => $_POST['invite_to_firstname'], 'last-name' => $_POST['invite_to_lastname']);
			$response = $this->_linkedin->invite('email', $recipient, $_POST['invite_subject'], $_POST['invite_body']);
			if($response['success'] === TRUE) {
			// message has been sent
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error sending invite:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			} else {
			// no email or id supplied
			echo "You must supply an email address or LinkedIn ID to send out the invitation to connect.";
			}
			*/
		}

                /**
                 * Like 
                 */
		function like() {
			/*
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			if(!empty($_GET['nKey'])) {
			$response = $this->_linkedin->like($_GET['nKey']);
			if($response['success'] === TRUE) {
			// update 'liked'
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// problem with 'like'
			echo "Error 'liking' update:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			} else {
			echo "You must supply a network update key to 'like' an update.";
			}




			case 'message':
			// connection messaging
			if(!empty($_POST['connections'])) {
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);

			if(!empty($_POST['message_copy'])) {
			$copy = TRUE;
			} else {
			$copy = FALSE;
			}
			$response = $this->_linkedin->message($_POST['connections'], $_POST['message_subject'], $_POST['message_body'], $copy);
			if($response['success'] === TRUE) {
			// message has been sent
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error sending message:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			} else {
			echo "You must select at least one recipient.";
			}
			*/
		}
		
		/**
                 * Reshare 
                 */
		function reshare() { 
			/*
			// process a status update action
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);

			// prepare content for sharing
			$content = array();
			if(!empty($_POST['scomment'])) {
			$content['comment'] = $_POST['scomment'];
			}
			if(!empty($_POST['sid'])) {
			$content['id'] = $_POST['sid'];
			}
			if(!empty($_POST['sprivate'])) {
			$private = TRUE;
			} else {
			$private = FALSE;
			}

			// re-share content
			$response = $this->_linkedin->share('reshare', $content, $private);
			if($response['success'] === TRUE) {
			// status has been updated
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error re-sharing content:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			*/
		}

                /**
                 * Revoke 
                 */
		function revoke() {
			/*
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			$response = $this->_linkedin->revoke();
			if($response['success'] === TRUE) {
			// revocation successful, clear session
			session_unset();
			$_SESSION = array();
			if(session_destroy()) {
			// session destroyed
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// session not destroyed
			echo "Error clearing user's session";
			}
			} else {
			// revocation failed
			echo "Error revoking user's token:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			*/
		}

                /**
                 * Unlike 
                 */
		function unlike() {
			/*
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			if(!empty($_GET['nKey'])) {
			$response = $this->_linkedin->unlike($_GET['nKey']);
			if($response['success'] === TRUE) {
			// update 'unliked'
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// problem with 'unlike'
			echo "Error 'unliking' update:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			} else {
			echo "You must supply a network update key to 'unlike' an update.";
			}
			*/
		}

                /**
                 * Update Network 
                 */
		function updateNetwork() {
			/*
			// process a network update action
			$this->_linkedin  = new LinkedIn($API_CONFIG);
			$this->_linkedin->setTokenAccess($_SESSION['oauth']['linkedin']['access']);
			$response = $this->_linkedin->updateNetwork($_POST['updateNetwork']);
			if($response['success'] === TRUE) {
			// status has been updated
			header('Location: ' . $_SERVER['PHP_SELF']);
			} else {
			// an error occured
			echo "Error posting network update:<br /><br />RESPONSE:<br /><br /><pre>" . print_r($response, TRUE) . "</pre><br /><br />LINKEDIN OBJ:<br /><br /><pre>" . print_r($this->_linkedin , TRUE) . "</pre>";
			}
			*/
		}
	}
        
        require_once(MODEL_PATH::get_framework_packages_directory()."linkedin/linkedin.class.php");        