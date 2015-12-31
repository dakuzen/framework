<?php

	class OFC_Elements_Value {		
		
		const VALUE_PARM = "#val#";
		
		public $type = "dot";
		public $value = 0;

		/*

			  "type": "dot",
			  "value": 1.88941834231,
			  "colour": "#D02020",
			  "tip": "#val#<br>Your text here"
		     */

		function __construct($value,$tip="") {
		$this->value = $value;

		if($tip)
			$this->tip = $tip;
		}

		function set_colour( $colour ) {
		$this->colour = $colour;
		}

		function set_tooltip( $tip )
		{
		$this->tip = $tip;
		}

		function get_value() { return $this->value; }
	}
