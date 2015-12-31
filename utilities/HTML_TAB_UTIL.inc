<?php
        /**
        * Class HTML_TAB_UTIL
        *
        * @package Framework
        * @subpackage Utilities
        */
	 
	class HTML_TAB_UTIL {
	
                /**
                 * The html tabs.
                 * @var array $html_tabs
                 */
		private $_html_tabs 		= array();
                
                /**
                 * The selected index.
                 * @var integer $selected_index 
                 */
		private $_selected_index	= 0;
		
		/**
		 * Initiattes tabs 
		 *
		 * @param array $tabs array tabs
		 */
		function __construct($tabs=array()) {
			$this->set_tabs($tabs);
		}
		
		/**
		 * Initiattes tabs
		 *
		 * <code>
		 * $tabs = array();
		 * $tab_util = new HTML_TAB_UTIL();
		 * $myarray[] = new HTML_TAB("dogs","animal");
		 * $myarray[] = new HTML_TAB("cats","kitten");
		 * $tab_util->set_tabs($myarray);
		 * </code>
		 *
		 * @param array $tabs array tabs
		 */
		function set_tabs($tabs) {
			$this->_html_tabs = $tabs;
		}
		
		/**
		 * Sets the selected index of the tabs
		 *
		 * <code>
		 * $tabs = array();
		 * $tab_util = new HTML_TAB_UTIL();
		 * $myarray[] = new HTML_TAB("dogs","animal");
		 * $myarray[] = new HTML_TAB("cats","kitten");
		 * $tab_util->set_tabs($myarray);
		 * $tab_util->set_selected_index(1);
		 * </code>
		 * 
		 * @param int $selected_index array tabs
		 */
		function set_selected_index($selected_index) {
			$this->_selected_index = $selected_index;
		}
		
		/**
		 * Renders css style sheet
		 *
		 */
		function render_css() { 
		?>
			<style>
				
				ul.tabbernav {
					margin:0;
					padding: 3px 0;
					border-bottom: 1px solid #778;
					font-weight: bold;
				}
				
				ul.tabbernav li {					
					list-style: none;
					margin: 0;
					display: inline;					
				}
				
				ul.tabbernav li a {
					padding: 3px 0.5em;
					margin-left: 3px;
					border: 1px solid #778;
					border-bottom: none;
					background: #DDE;
					text-decoration: none;					
				}

				ul.tabbernav li a:visited { color: #448; }
				ul.tabbernav li a:link { color: #448; }
				

				ul.tabbernav li a:hover {
					color: #000;
					background: #AAE;
					border-color: #227;
				}

				ul.tabbernav li.tabberactive a {
					background-color: #fff;
					border-bottom: 1px solid #fff;
				}

				ul.tabbernav li.tabberactive a:hover {
					color: #000;
					background: white;
					border-bottom: 1px solid white;
				}

				ul.tabbernav li.tabberactive a:visited {
					color: #667;
				}				

				.tabbertab {
					padding:15px;
					border:1px solid #778;
					border-top:0;
					background-color: #fff;
				}

		
			</style>
		<? 
		}	
		
		/**
		 * Renders html tabbed list
		 * 
		 * <code>
		 * $tabs = array();
		 * $tab_util = new HTML_TAB_UTIL();
		 * $myarray[] = new HTML_TAB("tab1","link1");
		 * $myarray[] = new HTML_TAB("cats","kitten");
		 * $tab_util->set_tabs($myarray);
		 * echo $tab_util->get_top();
		 * </code>
		 * <pre>
		 * <ul class="tabbernav"><li class="tabberactive"><a href="animal">dogs</a></li>
		 * <li><a href="kitten">cats</a></li></ul><div class="tabbertab">
		 * </pre>
		 * 
		 * @return html
		 */
		function get_top() {
			
			$html = '<div class="tabbernav-container"><ul class="tabbernav">';
			
			foreach($this->_html_tabs as $index=>$html_tab) {
				
				$selected = $this->_selected_index!==null && $this->_selected_index==$index;
				
				$html .= $html_tab->get_html($selected);
			}
			
			$html .= '</ul></div><div class="tabbertab">';
			
			return $html;
		}
		
		/**
		 * Renders start tags 
		 */
		function render_top() 		{ echo $this->get_top(); }
		
		/**
		 * Prints the end tags
		 */
		function render_bottom() 	{ echo $this->get_bottom(); }
		
		/**
		 * Prints the end div tag
		 */
		function get_bottom() {
			return "</div>";
		}
		
		/**
		 * Adds a html tab
		 *
		 * <code>
		 * $tab_util = new HTML_TAB_UTIL();
		 * $tab_util->add_html_tab("tab","link");
		 * </code>
		 * 
		 * @param string $label string label for the tab
		 * @param string $link string link of the tab
		 * @param boolean $active enables active 
		 * @param array $link_attribs array attributes set for the tabs
		 */
		
		function add_html_tab($label,$link,$active=false,$link_attribs=array()) {
			$this->_html_tabs[] = self::create_html_tab($label,$link,$active,$link_attribs);
		}
		
		/**
		 * Creates a html tab
		 *
		 * <code>
		 * $tab_util = new HTML_TAB_UTIL();
		 * $tab_util->create_html_tab("tab","link");
		 * </code>
		 * 
		 * @param string $label string label for the tab
		 * @param string $link string link of the tab
		 * @param boolean $active enables active 
		 * @param array $link_attribs array attributes set for the tabs
		 */
		
		static function create_html_tab($label,$link,$active=false,$link_attribs=array()) {
			return new HTML_TAB($label,$link,$active,$link_attribs);	
		}
	}
	
	/**
	 * Class HTML_TAB 
	 *
	 * @package Framework
	 * @subpackage Utilities
	 */
	class HTML_TAB {
            
                /**
                 * The html tab.
                 * @var array $tab 
                 */
		private $_tab = array();
		
		/**
		 * Initiates the date for  html tab
		 *
		 * @param string $label string label attached to tab
		 * @param string $link string link
		 * @param boolean $active enables active tab
		 * @param array $link_attribs array attributes applied to tabs
		 */
                function __construct($label,$link,$active=false,$link_attribs=array()) {
			$this->_tab = array($label,$link,$active,$link_attribs);
		}
		
		/**
		 * Returns one html list element
		 *
                 * @param boolean $active active
		 * @return string
		 */
		function get_html($active) {
			$label 		= get_value($this->_tab,0);
			$link 		= get_value($this->_tab,1);
			$link_attribs 	= get_value($this->_tab,3);
			
			$class = $active ? ' class="tabberactive"' : "";
			
			return "<li".$class.">".HTML_UTIL::get_link($link,$label,$link_attribs)."</li>";
		}
	}