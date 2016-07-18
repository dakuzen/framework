<?
	class BASE_MODEL_COUNTRY extends MODEL {

		const COUNTRY_UNITED_STATES 	= "US";
		const COUNTRY_CANADA			= "CA";

		const COUNTRY_US 				= "US";
		const COUNTRY_CA				= "CA";

		function get_canada_us_list() { return array(self::COUNTRY_UNITED_STATES=>"United States",self::COUNTRY_CANADA=>"Canada"); }

		function get_regions() { return self::get_canadian_provinces() + self::get_us_states(); }

		function get_canadian_provinces() {
			$list["AB"] = "Alberta";
			$list["BC"] = "British Columbia";
			$list["MB"] = "Manitoba";
			$list["NB"] = "New Brunswick";
			$list["NL"] = "Newfoundland and Labrador";
			$list["NT"] = "Northwest Territories";
			$list["NS"] = "Nova Scotia";
			$list["NU"] = "Nunavut";
			$list["ON"] = "Ontario";
			$list["PE"] = "Prince Edward Island";
			$list["QC"] = "Quebec";
			$list["SK"] = "Saskatchewan";
			$list["YT"] = "Yukon Territory";
			return $list;
		}

		function get_us_states() {

			$list["AL"] = "Alabama";
			$list["AK"] = "Alaska";
			$list["AS"] = "American Samoa";
			$list["AZ"] = "Arizona";
			$list["AR"] = "Arkansas";
			$list["CA"] = "California";
			$list["CO"] = "Colorado";
			$list["CT"] = "Connecticut";
			$list["DE"] = "Delaware";
			$list["DC"] = "District of Columbia";
			$list["FM"] = "Federated States of Micronesia";
			$list["FL"] = "Florida";
			$list["GA"] = "Georgia";
			$list["GU"] = "Guam";
			$list["HI"] = "Hawaii";
			$list["ID"] = "Idaho";
			$list["IL"] = "Illinois";
			$list["IN"] = "Indiana";
			$list["IA"] = "Iowa";
			$list["KS"] = "Kansas";
			$list["KY"] = "Kentucky";
			$list["LA"] = "Louisiana";
			$list["ME"] = "Maine";
			$list["MH"] = "Marshall Islands";
			$list["MD"] = "Maryland";
			$list["MA"] = "Massachusetts";
			$list["MI"] = "Michigan";
			$list["MN"] = "Minnesota";
			$list["MS"] = "Mississippi";
			$list["MO"] = "Missouri";
			$list["MT"] = "Montana";
			$list["NE"] = "Nebraska";
			$list["NV"] = "Nevada";
			$list["NH"] = "New Hampshire";
			$list["NJ"] = "New Jersey";
			$list["NM"] = "New Mexico";
			$list["NY"] = "New York";
			$list["NC"] = "North Carolina";
			$list["ND"] = "North Dakota";
			$list["MP"] = "Northern Mariana Islands";
			$list["OH"] = "Ohio";
			$list["OK"] = "Oklahoma";
			$list["OR"] = "Oregon";
			$list["PW"] = "Palau";
			$list["PA"] = "Pennsylvania";
			$list["PR"] = "Puerto Rico";
			$list["RI"] = "Rhode Island";
			$list["SC"] = "South Carolina";
			$list["SD"] = "South Dakota";
			$list["TN"] = "Tennessee";
			$list["TX"] = "Texas";
			$list["UT"] = "Utah";
			$list["VT"] = "Vermont";
			$list["VI"] = "Virgin Island";
			$list["VA"] = "Virginia";
			$list["WA"] = "Washington";
			$list["WV"] = "West Virginia";
			$list["WI"] = "Wisconsin";
			$list["WY"] = "Wyoming";

			return $list;
		}

		public static function regions($country,$options=array()) {

			$options["capitals"] 	= value($options,"capitals",false);
			$options["code"] 		= value($options,"code",false);
			$options["keys"] 		= value($options,"keys",true);
			$options["lng"] 		= value($options,"lng",true);
			$options["lat"] 		= value($options,"lat",true);
			$options["value_keys"] 	= value($options,"value_keys",true);

			$list = array();
			if($country==self::COUNTRY_CA) {
				$list["ON"] = array("Ontario","Ottawa",45.3999995,-75.6833333);
				$list["PE"] = array("Prince Edward Island","Charlottetown",46.2333328,-63.1166667);
				$list["AB"] = array("Alberta","Edmonton",53.5333328,-113.4833333);
				$list["NL"] = array("Newfoundland and Labrador","Fredericton",45.9499994,-66.6333333);
				$list["NS"] = array("Nova Scotia","Halifax",44.6333328,-63.5666667);
				$list["NU"] = array("Nunavut","Iqaluit",63.7333315,-68.5166667);
				$list["QC"] = array("Quebec","Montreal",45.4999994,-73.55);
				$list["SK"] = array("Saskatchewan","Regina",50.4499994,-104.6);
				$list["ON"] = array("Ontario","Toronto",43.6499994,-79.3666667);
				$list["BC"] = array("British Columbia","Vancouver",49.2333328,-123.1);
				$list["YT"] = array("Yukon Territory","Whitehorse",60.7166662,-135.05);
				$list["MB"] = array("Manitoba","Winnipeg",49.8833328,-97.15);
				$list["NT"] = array("Northwest Territories","Yellowknife",62.4499995,-114.3666667);
			}

			if($country==self::COUNTRY_US) {
				$list["AK"] = array("Alaska","",61.385,-152.2683);
				$list["AL"] = array("Alabama","",32.799,-86.8073);
				$list["AR"] = array("Arkansas","",34.9513,-92.3809);
				$list["AS"] = array("American Samoa","",14.2417,-170.7197);
				$list["AZ"] = array("Arizona","",33.7712,-111.3877);
				$list["CA"] = array("California","",36.17,-119.7462);
				$list["CO"] = array("Colorado","",39.0646,-105.3272);
				$list["CT"] = array("Connecticut","",41.5834,-72.7622);
				$list["DE"] = array("Delaware","",38.8964,-77.0262);
				$list["FL"] = array("Florida","",27.8333,-81.717);
				$list["GA"] = array("Georgia","",32.9866,-83.6487);
				$list["HI"] = array("Hawaii","",21.1098,-157.5311);
				$list["IA"] = array("Iowa","",42.0046,-93.214);
				$list["ID"] = array("Idaho","",44.2394,-114.5103);
				$list["IL"] = array("Illinois","",40.3363,-89.0022);
				$list["IN"] = array("Indiana","",39.8647,-86.2604);
				$list["KS"] = array("Kansas","",38.5111,-96.8005);
				$list["KY"] = array("Kentucky","",37.669,-84.6514);
				$list["LA"] = array("Louisiana","",31.1801,-91.8749);
				$list["MA"] = array("Massachusetts","",42.2373,-71.5314);
				$list["MD"] = array("Maryland","",39.0724,-76.7902);
				$list["ME"] = array("Maine","",44.6074,-69.3977);
				$list["MI"] = array("Michigan","",43.3504,-84.5603);
				$list["MN"] = array("Minnesota","",45.7326,-93.9196);
				$list["MO"] = array("Missouri","",38.4623,-92.302);
				$list["MP"] = array("Northern Mariana Islands","",14.8058,145.5505);
				$list["MS"] = array("Mississippi","",32.7673,-89.6812);
				$list["MT"] = array("Montana","",46.9048,-110.3261);
				$list["NC"] = array("North Carolina","",35.6411,-79.8431);
				$list["ND"] = array("North Dakota","",47.5362,-99.793);
				$list["NE"] = array("Nebraska","",41.1289,-98.2883);
				$list["NH"] = array("New Hampshire","",43.4108,-71.5653);
				$list["NJ"] = array("New Jersey","",40.314,-74.5089);
				$list["NM"] = array("New Mexico","",34.8375,-106.2371);
				$list["NV"] = array("Nevada","",38.4199,-117.1219);
				$list["NY"] = array("New York","",42.1497,-74.9384);
				$list["OH"] = array("Ohio","",40.3736,-82.7755);
				$list["OK"] = array("Oklahoma","",35.5376,-96.9247);
				$list["OR"] = array("Oregon","",44.5672,-122.1269);
				$list["PA"] = array("Pennsylvania","",40.5773,-77.264);
				$list["PR"] = array("Puerto Rico","",18.2766,-66.335);
				$list["RI"] = array("Rhode Island","",41.6772,-71.5101);
				$list["SC"] = array("South Carolina","",33.8191,-80.9066);
				$list["SD"] = array("South Dakota","",44.2853,-99.4632);
				$list["TN"] = array("Tennessee","",35.7449,-86.7489);
				$list["TX"] = array("Texas","",31.106,-97.6475);
				$list["UT"] = array("Utah","",40.1135,-111.8535);
				$list["VA"] = array("Virginia","",37.768,-78.2057);
				$list["VI"] = array("Virgin Islands","",18.0001,-64.8199);
				$list["VT"] = array("Vermont","",44.0407,-72.7093);
				$list["WA"] = array("Washington","",47.3917,-121.5708);
				$list["WI"] = array("Wisconsin","",44.2563,-89.6385);
				$list["WV"] = array("West Virginia","",38.468,-80.9696);
				$list["WY"] = array("Wyoming","",42.7475,-107.2085);
			}

			$keys 		= value($options,"keys");
			$value_keys = value($options,"value_keys");

			$regions = array();
			foreach($list as $code=>$value) {

				$item = array();

				$value_keys ? $item["code"] = $code : $item[] = $code;
				$value_keys ? $item["name"] = value($value,0) : $item[] = value($value,0);

				if(value($options,"capitals"))
					$value_keys ? $item["capital"] = value($value,1) : $item[] = value($value,1);

				if(value($options,"lat"))
					$value_keys ? $item["lat"] = value($value,2) : $item[] = value($value,2);

				if(value($options,"lng"))
					$value_keys ? $item["lng"] = value($value,3) : $item[] = value($value,3);

				$keys ? $regions[$code] = $item : $regions[] = $item;
			}

			return $regions;
		}

		public static function json_regions($country,$options=array()) {

			p($options);
			return JSON_UTIL::encode(self::regions($country,$options));
		}

		public static function json_countries($country_options=array(),$region_options=array()) {

			$countries = array();

			if(value($country_options,"only_name"))
				$countries = self::get_countries();
			else
				foreach(self::get_countries() as $code=>$name) {

					$country = array();

					if(value($country_options,"code"))
						$country["code"] = $code;

					if(value($country_options,"name"))
						$country["name"] = $name;

					if(value($country_options,"regions"))
						$country["regions"] = self::regions($code,$region_options);

					if(value($country_options,"key_code"))
						$countries[$code] = $country;
					else
						$countries[] = $country;
				}

			return JSON_UTIL::encode($countries);
		}

		function get_countries() {

			return array(	"AF" => "Afghanistan",
							"AL" => "Albania",
							"DZ" => "Algeria",
							"AS" => "American Samoa",
							"AD" => "Andorra",
							"AO" => "Angola",
							"AI" => "Anguilla",
							"AQ" => "Antarctica",
							"AG" => "Antigua and Barbuda",
							"AR" => "Argentina",
							"AM" => "Armenia",
							"AW" => "Aruba",
							"AU" => "Australia",
							"AT" => "Austria",
							"AZ" => "Azerbaijan",
							"BS" => "Bahamas",
							"BH" => "Bahrain",
							"BD" => "Bangladesh",
							"BB" => "Barbados",
							"BY" => "Belarus",
							"BE" => "Belgium",
							"BZ" => "Belize",
							"BJ" => "Benin",
							"BM" => "Bermuda",
							"BT" => "Bhutan",
							"BO" => "Bolivia",
							"BA" => "Bosnia and Herzegovina",
							"BW" => "Botswana",
							"BV" => "Bouvet Island",
							"BR" => "Brazil",
							"IO" => "British Indian Ocean Territory",
							"BN" => "Brunei Darussalam",
							"BG" => "Bulgaria",
							"BF" => "Burkina Faso",
							"BI" => "Burundi",
							"KH" => "Cambodia",
							"CM" => "Cameroon",
							"CA" => "Canada",
							"CV" => "Cape Verde",
							"KY" => "Cayman Islands",
							"CF" => "Central African Republic",
							"TD" => "Chad",
							"CL" => "Chile",
							"CN" => "China",
							"CX" => "Christmas Island",
							"CC" => "Cocos (Keeling) Islands",
							"CO" => "Colombia",
							"KM" => "Comoros",
							"CG" => "Congo",
							"CD" => "Congo, the Democratic Republic of the",
							"CK" => "Cook Islands",
							"CR" => "Costa Rica",
							"CI" => "Cote D'Ivoire",
							"HR" => "Croatia",
							"CU" => "Cuba",
							"CY" => "Cyprus",
							"CZ" => "Czech Republic",
							"DK" => "Denmark",
							"DJ" => "Djibouti",
							"DM" => "Dominica",
							"DO" => "Dominican Republic",
							"EC" => "Ecuador",
							"EG" => "Egypt",
							"SV" => "El Salvador",
							"GQ" => "Equatorial Guinea",
							"ER" => "Eritrea",
							"EE" => "Estonia",
							"ET" => "Ethiopia",
							"FK" => "Falkland Islands (Malvinas)",
							"FO" => "Faroe Islands",
							"FJ" => "Fiji",
							"FI" => "Finland",
							"FR" => "France",
							"GF" => "French Guiana",
							"PF" => "French Polynesia",
							"TF" => "French Southern Territories",
							"GA" => "Gabon",
							"GM" => "Gambia",
							"GE" => "Georgia",
							"DE" => "Germany",
							"GH" => "Ghana",
							"GI" => "Gibraltar",
							"GR" => "Greece",
							"GL" => "Greenland",
							"GD" => "Grenada",
							"GP" => "Guadeloupe",
							"GU" => "Guam",
							"GT" => "Guatemala",
							"GN" => "Guinea",
							"GW" => "Guinea-Bissau",
							"GY" => "Guyana",
							"HT" => "Haiti",
							"HM" => "Heard Island and Mcdonald Islands",
							"VA" => "Holy See (Vatican City State)",
							"HN" => "Honduras",
							"HK" => "Hong Kong",
							"HU" => "Hungary",
							"IS" => "Iceland",
							"IN" => "India",
							"ID" => "Indonesia",
							"IR" => "Iran, Islamic Republic of",
							"IQ" => "Iraq",
							"IE" => "Ireland",
							"IL" => "Israel",
							"IT" => "Italy",
							"JM" => "Jamaica",
							"JP" => "Japan",
							"JO" => "Jordan",
							"KZ" => "Kazakhstan",
							"KE" => "Kenya",
							"KI" => "Kiribati",
							"KP" => "Korea, Democratic People's Republic of",
							"KR" => "Korea, Republic of",
							"KW" => "Kuwait",
							"KG" => "Kyrgyzstan",
							"LA" => "Lao People's Democratic Republic",
							"LV" => "Latvia",
							"LB" => "Lebanon",
							"LS" => "Lesotho",
							"LR" => "Liberia",
							"LY" => "Libyan Arab Jamahiriya",
							"LI" => "Liechtenstein",
							"LT" => "Lithuania",
							"LU" => "Luxembourg",
							"MO" => "Macao",
							"MK" => "Macedonia",
							"MG" => "Madagascar",
							"MW" => "Malawi",
							"MY" => "Malaysia",
							"MV" => "Maldives",
							"ML" => "Mali",
							"MT" => "Malta",
							"MH" => "Marshall Islands",
							"MQ" => "Martinique",
							"MR" => "Mauritania",
							"MU" => "Mauritius",
							"YT" => "Mayotte",
							"MX" => "Mexico",
							"FM" => "Micronesia, Federated States of",
							"MD" => "Moldova, Republic of",
							"MC" => "Monaco",
							"MN" => "Mongolia",
							"MS" => "Montserrat",
							"MA" => "Morocco",
							"MZ" => "Mozambique",
							"MM" => "Myanmar",
							"NA" => "Namibia",
							"NR" => "Nauru",
							"NP" => "Nepal",
							"NL" => "Netherlands",
							"AN" => "Netherlands Antilles",
							"NC" => "New Caledonia",
							"NZ" => "New Zealand",
							"NI" => "Nicaragua",
							"NE" => "Niger",
							"NG" => "Nigeria",
							"NU" => "Niue",
							"NF" => "Norfolk Island",
							"MP" => "Northern Mariana Islands",
							"NO" => "Norway",
							"OM" => "Oman",
							"PK" => "Pakistan",
							"PW" => "Palau",
							"PS" => "Palestinian Territory, Occupied",
							"PA" => "Panama",
							"PG" => "Papua New Guinea",
							"PY" => "Paraguay",
							"PE" => "Peru",
							"PH" => "Philippines",
							"PN" => "Pitcairn",
							"PL" => "Poland",
							"PT" => "Portugal",
							"PR" => "Puerto Rico",
							"QA" => "Qatar",
							"RE" => "Reunion",
							"RO" => "Romania",
							"RU" => "Russian Federation",
							"RW" => "Rwanda",
							"SH" => "Saint Helena",
							"KN" => "Saint Kitts and Nevis",
							"LC" => "Saint Lucia",
							"PM" => "Saint Pierre and Miquelon",
							"VC" => "Saint Vincent and the Grenadines",
							"WS" => "Samoa",
							"SM" => "San Marino",
							"ST" => "Sao Tome and Principe",
							"SA" => "Saudi Arabia",
							"SN" => "Senegal",
							"CS" => "Serbia and Montenegro",
							"SC" => "Seychelles",
							"SL" => "Sierra Leone",
							"SG" => "Singapore",
							"SK" => "Slovakia",
							"SI" => "Slovenia",
							"SB" => "Solomon Islands",
							"SO" => "Somalia",
							"ZA" => "South Africa",
							"GS" => "South Georgia and Sandwich Isles",
							"ES" => "Spain",
							"LK" => "Sri Lanka",
							"SD" => "Sudan",
							"SR" => "Suriname",
							"SJ" => "Svalbard and Jan Mayen",
							"SZ" => "Swaziland",
							"SE" => "Sweden",
							"CH" => "Switzerland",
							"SY" => "Syrian Arab Republic",
							"TW" => "Taiwan, Province of China",
							"TJ" => "Tajikistan",
							"TZ" => "Tanzania, United Republic of",
							"TH" => "Thailand",
							"TL" => "Timor-Leste",
							"TG" => "Togo",
							"TK" => "Tokelau",
							"TO" => "Tonga",
							"TT" => "Trinidad and Tobago",
							"TN" => "Tunisia",
							"TR" => "Turkey",
							"TM" => "Turkmenistan",
							"TC" => "Turks and Caicos Islands",
							"TV" => "Tuvalu",
							"UG" => "Uganda",
							"UA" => "Ukraine",
							"AE" => "United Arab Emirates",
							"GB" => "United Kingdom",
							"US" => "United States",
							"UM" => "United States Minor Outlying Islands",
							"UY" => "Uruguay",
							"UZ" => "Uzbekistan",
							"VU" => "Vanuatu",
							"VE" => "Venezuela",
							"VN" => "Viet Nam",
							"VG" => "Virgin Islands, British",
							"VI" => "Virgin Islands, U.S.",
							"WF" => "Wallis and Futuna",
							"EH" => "Western Sahara",
							"YE" => "Yemen",
							"ZM" => "Zambia",
							"ZW" => "Zimbabwe");
		}

	}
