<?
    /**
    * Class XPATH_UTIL
    *
    * @package Framework
    * @subpackage Utilities
    */
 
	/*
	class DOMNodeRecursiveIterator extends ArrayIterator implements RecursiveIterator {
	  
	  public function __construct (DOMNodeList $node_list,$xpath_util) {
	    
	    $nodes = array();
	    foreach($node_list as $node) {
	      $nodes[] = new XPATH_UTIL($xpath_util->get_handler(),$node);
	    }

	    parent::__construct($nodes);	    
	  }

	  public function item($index) {
		for($i=0;$i<$index;$i++)
			$this->next();

		return $this->current();
	  }  

	  public function getRecursiveIterator(){
	    return new RecursiveIteratorIterator($this, RecursiveIteratorIterator::SELF_FIRST);
	  }
	  
	  public function hasChildren () {
	    return $this->current()->hasChildNodes();
	  }

	  
	  public function getChildren() {
	    return new self($this->current()->childNodes);
	  }
	  
	}
	*/

	class XPATH_UTIL {
			
		const TYPE_DOM 		= "D";
		const TYPE_SIMPLE 	= "S";


        /**
         * Dom path
         * @var string
         */
		protected $_handler = null;
        
		protected $_node	= null;

		protected $_type	= null;

        /**
         * Construct
         * 
         * @param object $object the xpath object
         * @throws Exception
         */
		function __construct($object,$node=null) {

			$this->_node = $node;

			if(is_object($object)) {

				if(get_class($object)=="DOMDocument") {
					$this->_handler = new DOMXPath($object);
					$this->_type = self::TYPE_DOM;
				}

				if(get_class($object)=="DOMXPath") {
					$this->_handler = $object;
					$this->_type = self::TYPE_DOM;
				}
			
			} elseif(is_string($object)) {
				
				$this->_type = self::TYPE_SIMPLE;

				$object = preg_replace("/xmlns=/","dummy-xmlns=",$object);
				
				$previous_handler = set_error_handler(array($this,"internal_error_handler"));

				$exception = null;

				try {
					$this->_handler = new SimpleXMLElement($object);
				} catch(Exception $exception) {}

				set_error_handler($previous_handler);

				if($exception)
					throw $exception;					
			}			
		}

		public static function create($object) {
			return new XPATH_UTIL($object);
		}		
        
        /**
         * Handle the error from the SimpleXMLElement and throw an exception
         * @param string $errno
         * @param string $errmsg
         * @param string $filename
         * @param string $linenum
         * @throws Exception 
         */
		function internal_error_handler($errno, $errmsg, $filename, $linenum) {				
			throw new Exception($errmsg);
		}

        /**
         * Check whether is dom
         * @return boolean 
         */
		function node() 		{ return $this->_node; }
		function is_dom() 		{ return $this->_type==self::TYPE_DOM; }
		function get_handler() 	{ return $this->_handler; }
		
        /**
         * Select a path
         * @param string $path
         * @param string $node
         * @return string 
         */
		function select($path,$node=null) {			

			$elements = array();

			$node = $node ? $node : $this->node();

			if($node && is_a($node,"XPATH_UTIL"))
				$node = $node->node();
			
			if($this->is_dom()) {

				$items = $node ? $this->_handler->query($path,$node) : $this->_handler->query($path);

				foreach($items as $item)
					$elements[] = new XPATH_UTIL($this->_handler,$item);
				
			} else
				$elements = $this->_handler->xpath($path);

			return $elements;
		}

	    /**
	     * Select one 
	     * @param string $path
	     * @param string $node
	     * @return string 
	     */
		function one($path,$node=null) {
			
			$elements = $this->select($path,$node);

			$value = null;

			//if($this->is_dom())
			//	$value = $elements->item(0) ? $elements->item(0) : null;
			//else
				$value = value($elements,0);

			return $value;
		}

		function select_one($path,$node=null) {
			return self::one($path,$node);
		}	
		
        /**
         * Select value
         * @param string $path
         * @param string $node
         * @return string 
         */
		function value($path,$node=null) {

			$node = $node ? $node : $this->_node;
			
			$element = $this->one($path,$node);
	
			$value = "";

			if($this->is_dom())
				$value = $element ? $element->node()->nodeValue : "";
			else
				$value = $element;

			return (string)$value;
		}

		function name($node=null) {

			$node = $node ? $node : $this->_node;
			
			return $node->nodeName;
		}


		function values($path,$node=null) {

			$node = $node ? $node : $this->_node;
			
			$elements = $this->select($path,$node);
	
			$values = array();

			foreach($elements as $element) {
				if($this->is_dom())
					$values[] = $element ? $element->node()->nodeValue : "";
				else
					$values[] = $element;
			}

			return $values;
		}

		function select_value($path,$node=null) {
			return self::value($path,$node);
		}
		
		/**
		 * Checks if a value exists
		 * @param string $path
                 * @param string $node
		 * @return boolean 
		 */
		function exists($path,$node=null) {
			$element = $this->select_one($path,$node);			
			return !is_empty($element);
		}
		
        /**
         * Select attribute
         * @param string $path
         * @param string $attrbiute
         * @param string $node
         * @return null 
         */

        function select_attribute($path,$attrbiute,$node=null) {
        	return $this->attribute($path,$attrbiute,$node);
        }

		function attribute($path,$attrbiute,$node=null) {

			$element = $this->select_one($path,$node);
			
			if($element) {

				if($element && is_a($element,"XPATH_UTIL"))
					$element = $element->node();

				$attribute_element = $element->attributes->getNamedItem($attrbiute);
				if($attribute_element)	
					return $attribute_element->nodeValue; 
			}
			
			return null;
		}

		function __toString() {
			if($this->is_dom())
				return $this->node() ? $this->node()->nodeValue : "";
			
			return $this->node();
		}

		function name_space($name,$uri) {
			$this->_handler->registerNamespace($name,$uri);
			return $this;
		}	
	}

	class XDOMElement {

		protected $_DOMElement = null;

		public function __construct(DOMElement $DOMElement) {
			$this->_DOMElement = $DOMElement;
		}

		public function __call($function,$args) {
			p($function,$args);
			
		}

		public function domelement() {
			return $this->_DOMElement;
		}
	}	