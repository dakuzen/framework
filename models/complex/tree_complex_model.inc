<?php
        /**
        * Class BASE_CMODEL_TREE
        *
        * @package Framework
        * @subpackage Cmodels
        */

	class BASE_CMODEL_TREE {

                /**
                 * The table
                 * @var string $table
                 */
		protected $_table		= "";
                
                /**
                 * Enable locks
                 * @var boolean $enable_locks 
                 */
		protected $_enable_locks	= true;
                
                /**
                 * The fields
                 * @var array $fields
                 */
		protected $_fields		= array("id"		=> "id",
							"parent_id"	=> "parent_id",
							"position"	=> "position",
							"left"		=> "left",
							"right"		=> "right",
							"level"		=> "level");

                /**
                 * Contructor
                 * @param string $table
                 * @param array $fields
                 * @param boolean $enable_locks 
                 */
		function __construct($table="tree",$fields=array(),$enable_locks=true) {
			
			foreach($fields as $key => $field) 
				$this->_fields[$key] = $field;
			
			$this->_table 		= $table;
			$this->db		= DB::get_instance();
			$this->_enable_locks 	= $enable_locks;
		}
	
                /**
                 * Return the root node
                 * @return mixed
                 */
		function get_root_node() {
			
			if($this->is_debug())
				p("function: get_root_node()");
			
			$row = $this->db->_select_row("SELECT `".implode("` , `", $this->_fields)."` FROM `".$this->_table."` WHERE `".$this->get_field_parent_id()."` IS null");
			
			return $row ? $row : false;
		}		
		
                /**
                 * Return the nodes
                 * @param integer $id
                 * @param array $fields
                 * @return mixed 
                 */
		function get_node($id,$fields=array()) {
			
			if($this->is_debug())
				p("function: get_node(".$id.")");
			
			$fields = array_merge($fields,$this->_fields);
			
			$row = $this->db->_select_row("SELECT `".implode("` , `", $fields)."` FROM `".$this->_table."` WHERE `".$this->get_field_id()."` = ".(int) $id);
			
			return $row ? $row : false;
		}
		
                /**
                 * Return the childred
                 * @param integer $id
                 * @param boolean $recursive
                 * @param array $fields
                 * @return array 
                 */
		function get_children($id,$recursive=false,$fields=array()) {
			
			if($this->is_debug())
				p("function: get_children(".$id.",".($recursive ? "true" : "false").")");
			
			$rows = $children = array();
			
			$fields = array_merge($fields,$this->_fields);
			
			if($recursive) {
				$node = $this->get_node($id);
				$rows = $this->db->_select("SELECT `".implode("` , `", $fields)."` FROM `".$this->_table."` WHERE `".$this->get_field_left()."` >= ".(int) $node[$this->get_field_left()]." AND `".$this->_fields["right"]."` <= ".(int) $node[$this->_fields["right"]]." ORDER BY `".$this->get_field_left()."` ASC");
			} else 
				$rows = $this->db->_select("SELECT `".implode("` , `", $fields)."` FROM `".$this->_table."` WHERE `".$this->get_field_parent_id()."` = ".(int) $id." ORDER BY `".$this->get_field_position()."` ASC");
				
			foreach($rows as $row)				
				$children[get_value($row,$this->get_field_id())] = $row;
					
			return $children;
		}
		
                /**
                 * Return the path
                 * @param integer $id
                 * @return string 
                 */
		function get_path($id) {
			
			if($this->is_debug())
				p("function: get_path(".$id.")");
			
			$node = $this->get_node($id);
			$path = array();
			
			if($node) {

				$rows = $this->db->_select("SELECT `".implode("` , `", $this->_fields)."` FROM `".$this->_table."` WHERE `".$this->get_field_left()."` <= ".(int) $node[$this->get_field_left()]." AND `".$this->_fields["right"]."` >= ".(int) $node[$this->_fields["right"]]);

				foreach($rows as $row)
					$path[get_value($row,$this->get_field_id())] = $row;
			}
			
			return $path;
		}
		
                /**
                 * Returns the subordinate nodes
                 * @param string $depth
                 * @param string $primary_key_value
                 * @param string $fields
                 * @return object
                 * @todo review types 
                 */
		public function get_subordinate_nodes($depth=null,$primary_key_value=null,$fields=null) {
			
			$fields = $fields ? $fields : "node.*";
					
			$sql = 	"SELECT ".$fields.", (COUNT(parent.".$this->get_field_parent_id().") - (sub_tree.depth + 1)) AS depth ".
				"FROM ".$this->get_table()." AS node, ".
					$this->get_table()." AS parent, ".
					$this->get_table()." AS sub_parent, ".
					"(".
					"SELECT node.".$this->get_field_parent_id().", (COUNT(parent.".$this->get_field_parent_id().") - 1) AS depth ".
						"FROM ".$this->get_table()." AS node, ".
						$this->get_table()." AS parent ".
						"WHERE node.".$this->get_field_left()." BETWEEN parent.".$this->get_field_left()." AND parent.".$this->get_field_right()." ";
			if($primary_key_value)
				$sql .= "AND node.".$this->get_field_parent_id()." = '".$primary_key_value."' ";
						
			$sql .=			"GROUP BY node.".$this->get_field_parent_id()." ".
						"ORDER BY node.".$this->get_field_left()." ".
					")AS sub_tree ".
					
				"WHERE node.".$this->get_field_left()." BETWEEN parent.".$this->get_field_left()." AND parent.".$this->get_field_right()." ".
					"AND node.".$this->get_field_left()." BETWEEN sub_parent.".$this->get_field_left()." AND sub_parent.".$this->get_field_right()." ".
					"AND sub_parent.".$this->get_field_parent_id()." = sub_tree.".$this->get_field_parent_id()." ".
				"GROUP BY node.".$this->get_field_parent_id()." ";
			
			if($depth)
				$sql .= "HAVING depth = ".$depth." ";
			
			$sql .= "ORDER BY node.".$this->get_field_left()."";
	
			return $this->db->_select($sql);			
		}		

                /**
                 * Creates
                 * @param integer $parent_id
                 * @param integer $position
                 * @param array $data
                 * @return integer 
                 */
		function create($parent_id, $position, $data=array()) {
			
			if($this->is_debug())
				p("function: create(".$parent.",".$position.")");
			
			if($this->move(null, $parent_id, $position, false, $data))
				return $this->db->get_insert_id();
		}
		
                /**
                 * Removes
                 * @param integer $id
                 * @return boolean 
                 */
		function remove($id) {
			
			if((int)$id === 1)
				return false;
			
			$this->enable_lock_table_write();
			
			$this->db->start_transaction();
			
			$data = $this->get_node($id);
			$lft = (int)$data[$this->get_field_left()];
			$rgt = (int)$data[$this->_fields["right"]];
			$dif = $rgt - $lft + 1;

			// deleting node and its children
			$this->db->query("DELETE FROM `".$this->_table."` " . 
					"WHERE `".$this->get_field_left()."` >= ".$lft." AND `".$this->_fields["right"]."` <= ".$rgt);

			// shift left indexes of nodes right of the node
			$this->db->query("UPDATE `".$this->_table."` " . 
					"SET `".$this->get_field_left()."` = `".$this->get_field_left()."` - ".$dif." " . 
					"WHERE `".$this->get_field_left()."` > ".$rgt);
				
			// shift right indexes of nodes right of the node and the node's parents
			$this->db->query("UPDATE `".$this->_table."` " . 
					"SET `".$this->_fields["right"]."` = `".$this->_fields["right"]."` - ".$dif." " . 
					"WHERE `".$this->_fields["right"]."` > ".$lft);

			$pid = (int)$data[$this->get_field_parent_id()];
			$pos = (int)$data[$this->get_field_position()];

			// Update position of siblings below the deleted node
			$this->db->query("UPDATE `".$this->_table."` " . 
					"SET `".$this->get_field_position()."` = `".$this->get_field_position()."` - 1 " . 
					"WHERE `".$this->get_field_parent_id()."` = ".$pid." AND `".$this->get_field_position()."` > ".$pos
			);
			
			$has_success = $this->db->has_transaction_success();
			$this->db->complete_transaction();
			
			$this->disable_lock_table();
			
			return $has_success;	
		}
		
                /**
                 * Create a DBQ
                 * @return object DBQ 
                 */
		function create_dbq() {
			return new DBQ($this->get_table());
		}
                
		/**
                 * Move
                 * @param integer $id
                 * @param integer $parent_id
                 * @param integer $position
                 * @param boolean $is_copy
                 * @param array $data
                 * @return boolean 
                 */
		function move($id, $parent_id, $position = 0, $is_copy=false, $data=array()) {
			
			if(!$parent_id)
				return false;
			
			$node		= $this->get_node($id);			// Node data
			$ref_node	= $this->get_node($parent_id);		// Ref node data
			$rchildren	= $this->get_children($parent_id);	// Ref node children
			$ndif 		= 2;
			$node_ids 	= array(-1);
			
			if($node) {
				$node_ids = array_keys($this->get_children($id, true));
				
				// TODO: should be !$is_copy && , but if copied to self - screws some right indexes
				if(in_array($parent_id, $node_ids)) 
					return false;
				
				$ndif = $node[$this->get_field_right()] - $node[$this->get_field_left()] + 1;
			}
			
			if($position >= count($rchildren)) 
				$position = count($rchildren);
			
			$this->enable_lock_table_write();
			
			$this->db->start_transaction();
			
			// Not creating or copying - old parent is cleaned
			if($node && $is_copy == false) {
				
				$this->db->query("UPDATE `".$this->_table."` SET `".$this->get_field_position()."` = `".$this->get_field_position()."` - 1 " . 
					"WHERE `".$this->get_field_parent_id()."` = ".$node[$this->get_field_parent_id()]." AND `".$this->get_field_position()."` > ".$node[$this->get_field_position()]);
				
				$this->db->query("UPDATE `".$this->_table."`  SET `".$this->get_field_left()."` = `".$this->get_field_left()."` - ".$ndif." " . 
						"WHERE `".$this->get_field_left()."` > ".$node[$this->get_field_right()]);
				
				$this->db->query("UPDATE `".$this->_table."` " . 
						"SET `".$this->get_field_right()."` = `".$this->get_field_right()."` - ".$ndif." " . 
					"WHERE " . 
						"`".$this->get_field_right()."` > ".$node[$this->get_field_left()]." AND " . 
						"`".$this->get_field_id()."` NOT IN (".implode(",", $node_ids).") ");
			}
			
			// Preparing new parent
			
			$dbq = $this->create_dbq();
			$dbq->add_where($this->get_field_parent_id(),"=",$parent_id);
			$dbq->add_where($this->get_field_position(),">=",$position);
			
			if(!$is_copy)
				$dbq->add_where($this->get_field_id(),"NOT IN",$node_ids);
			
			$dbq->update(array($this->get_field_position()=>array("parm"=>$this->get_field_position()." + 1")));
			
			
			$ref_ind = $parent_id ? (int)$ref_node[$this->get_field_right()] : (int)$rchildren[count($rchildren) - 1][$this->get_field_right()] + 1;
			$ref_ind = max($ref_ind, 1);

			$self = ($node !== false && !$is_copy && (int)$node[$this->get_field_parent_id()] == $parent_id && $position > $node[$this->get_field_position()]) ? 1 : 0;
			
			foreach($rchildren as $k => $v) {
				if($v[$this->get_field_position()] - $self == $position) {
					$ref_ind = (int)$v[$this->get_field_left()];
					break;
				}
			}
			if($node && !$is_copy && $node[$this->get_field_left()] < $ref_ind) 
				$ref_ind -= $ndif;
			
			$this->db->query("UPDATE `".$this->_table."` " . 
					"SET `".$this->get_field_left()."` = `".$this->get_field_left()."` + ".$ndif." " . 
					"WHERE " . 
					"`".$this->get_field_left()."` >= ".$ref_ind." " . 
					( $is_copy ? "" : " AND `".$this->get_field_id()."` NOT IN (".implode(",", $node_ids).") "));
			
			$this->db->query("UPDATE `".$this->_table."` " . 
					"SET `".$this->get_field_right()."` = `".$this->get_field_right()."` + ".$ndif." " . 
					"WHERE " . 
					"`".$this->get_field_right()."` >= ".$ref_ind." " . 
					( $is_copy ? "" : " AND `".$this->get_field_id()."` NOT IN (".implode(",", $node_ids).") "));

			$ldif = $parent_id == 0 ? 0 : $ref_node[$this->get_field_level()] + 1;
			$idif = $ref_ind;
			
			if($node) {
				$ldif = $node[$this->get_field_level()] - ($ref_node[$this->get_field_level()] + 1);
				$idif = $node[$this->get_field_left()] - $ref_ind;
				
				if($is_copy) {
					$this->db->query("INSERT INTO `".$this->_table."` (" . 
							"`".$this->get_field_parent_id()."`, " . 
							"`".$this->get_field_position()."`, " . 
							"`".$this->get_field_left()."`, " . 
							"`".$this->get_field_right()."`, " . 
							"`".$this->get_field_level()."`" . 
						") " . 
							"SELECT " . 
								"".$parent_id.", " . 
								"`".$this->get_field_position()."`, " . 
								"`".$this->get_field_left()."` - (".($idif + ($node[$this->get_field_left()] >= $ref_ind ? $ndif : 0))."), " . 
								"`".$this->get_field_right()."` - (".($idif + ($node[$this->get_field_left()] >= $ref_ind ? $ndif : 0))."), " . 
								"`".$this->get_field_level()."` - (".$ldif.") " . 
							"FROM `".$this->_table."` " . 
							"WHERE " . 
								"`".$this->get_field_id()."` IN (".implode(",", $node_ids).") " . 
							"ORDER BY `".$this->get_field_level()."` ASC");
				} else {
					$this->db->query("UPDATE `".$this->_table."` SET " . 
							"`".$this->get_field_parent_id()."` = ".$parent_id.", " . 
							"`".$this->get_field_position()."` = ".$position." " . 
							"WHERE " . 
							"`".$this->get_field_id()."` = ".$id);
					
					$this->db->query("UPDATE `".$this->_table."` SET " . 
							"`".$this->get_field_left()."` = `".$this->get_field_left()."` - (".$idif."), " . 
							"`".$this->get_field_right()."` = `".$this->get_field_right()."` - (".$idif."), " . 
							"`".$this->get_field_level()."` = `".$this->get_field_level()."` - (".$ldif.") " . 
							"WHERE " . 
							"`".$this->get_field_id()."` IN (".implode(",", $node_ids).") ");
				}
				
			} else {
			
				$data[$this->get_field_parent_id()] 	= $parent_id;
				$data[$this->get_field_position()] 	= $position;
				$data[$this->get_field_left()] 		= $idif;
				$data[$this->get_field_right()] 	= $idif + 1;
				$data[$this->get_field_level()] 		= $ldif;
				
				$dbq = new DBQ($this->_table);
				$dbq->insert($data);
			}
			
			
			
			if($is_copy) {
				$ind = $this->db->get_insert_id();
				$has_success = $this->fix_copy($ind, $position);
			} else {
			
				$has_success = $node === false;
			}
			
			//if(!$has_success)
			//	$this->db->fail_transaction();
			
			$has_success = $this->db->has_transaction_success();
			$this->db->complete_transaction();
			
			$this->disable_lock_table();
			
			return $has_success;
		}
		
                /**
                 * Updates
                 * @param integer $parent_id
                 * @param integer $id
                 * @return boolean 
                 */
		function update($parent_id, $id) {
			
			if(!$parent_id || !$id)
				return false;
			
			$parent_node = $this->get_node($parent_id);
			
			if(!$parent_node)
				return false;
			
			$this->enable_lock_table_write();
			
			$this->db->start_transaction();
			
			$parent_right = $parent_node[$this->get_field_right()];
			$parent_level = $parent_node[$this->get_field_level()];

			$dbq = $this->create_dbq();
			$dbq->add_where($this->get_field_right(true),">",($parent_right - 1));
			$dbq->update(array($this->get_field_right()=>array("parm"=>$this->get_field_right(true)." + 2")));

			$dbq = $this->create_dbq();
			$dbq->add_where($this->get_field_left(true),">",($parent_right - 1));
			$dbq->update(array($this->get_field_left()=>array("parm"=>$this->get_field_left(true)." + 2")));

			$data[$this->get_field_left()] 	= $parent_right;
			$data[$this->get_field_right()]	= $parent_right + 1;
			$data[$this->get_field_level()] = $parent_level + 1;
			p($data);
			$dbq = $this->create_dbq();
			$dbq->add_where($this->get_field_id(true),"=",$id);
			$dbq->update($data);
			
			
			$has_success = $this->db->has_transaction_success();
			$this->db->complete_transaction();
			
			$this->disable_lock_table();
			
			return $has_success;
		}		
		
                /**
                 * Fix copy
                 * @param integer $id
                 * @param integer $position
                 * @return boolean 
                 */
		function fix_copy($id, $position) {
			
			$this->db->start_transaction();
			
			$node 		= $this->get_node($id);
			$children 	= $this->get_children($id, true);

			$map = array();
			for($i = $node[$this->get_field_left()] + 1; $i < $node[$this->_fields["right"]]; $i++) {
				$map[$i] = $id;
			}
			
			foreach($children as $cid => $child) {
				if((int)$cid == (int)$id) {
					$this->db->query("UPDATE `".$this->_table."` SET `".$this->get_field_position()."` = ".$position." WHERE `".$this->get_field_id()."` = ".$cid);
					continue;
				}
				$this->db->query("UPDATE `".$this->_table."` SET `".$this->get_field_parent_id()."` = ".$map[(int)$child[$this->get_field_left()]]." WHERE `".$this->get_field_id()."` = ".$cid);
				for($i = $child[$this->get_field_left()] + 1; $i < $child[$this->_fields["right"]]; $i++) {
					$map[$i] = $cid;
				}
			}
			
			$has_success = $this->db->has_transaction_success();
			$this->db->complete_transaction();
			
			return $has_success;				
		}

                /**
                 * Reconstruct
                 * @return boolean 
                 */
		function reconstruct() {
			
			if(!$this->get_root_node())
				return false;
		
			$this->db->start_transaction();
			
			DB::disable_foreign_keys();
			
			$table_fields = $this->db->get_db_utility()->get_table_fields($this->get_table());
			
       			$select_table_fields = $create_table_fields = array();
			
			foreach($table_fields as $table_field) {
				$field = get_value($table_field,"Field");
				
				$create_table_fields[$field] = "`".$field."` ".get_value($table_field,"Type");
				$select_table_fields[$field] = "`".$field."`";
			}
				
			$this->db->query("CREATE TEMPORARY TABLE `temp_tree` (".implode(",",$create_table_fields).") ENGINE=MEMORY");
			
			$this->db->query("INSERT INTO `temp_tree` SELECT ".implode(", ",$select_table_fields)." FROM `".$this->get_table()."`");
		
						
			$temp_stack_fields = array_merge($create_table_fields,array("`stack_top` INTEGER NOT NULL"));
						
			$this->db->query("CREATE TEMPORARY TABLE `temp_stack` (".implode(",",$temp_stack_fields).") ENGINE=MEMORY");
			
			$counter = 2;
			
			$maxcounter = $this->db->_select_one("SELECT COUNT(*) FROM temp_tree");
			$maxcounter = (int)$maxcounter * 2;
						
			$temp_stack_fields = $select_table_fields;
			
			$temp_stack_fields[$this->get_field_left()] 	= 1;
			$temp_stack_fields[$this->get_field_right()] 	= "null";
			$temp_stack_fields[$this->get_field_level()] 	= 0;
			$temp_stack_fields["stack_top"] 		= 1;
			
			$currenttop = 1;
			$this->db->query("INSERT INTO `temp_stack` SELECT ".implode(", ",$temp_stack_fields)." FROM `temp_tree` WHERE `".$this->get_field_parent_id()."` IS null");
			
			$this->db->query("DELETE FROM `temp_tree` WHERE `".$this->get_field_parent_id()."` IS null");

			while ($counter <= $maxcounter) {
				
				$row = $this->db->_select_row("SELECT `temp_tree`.* FROM `temp_stack`, `temp_tree` " . 
							"WHERE " . 
								"`temp_stack`.`".$this->get_field_id()."` = `temp_tree`.`".$this->get_field_parent_id()."` AND " . 
								"`temp_stack`.`stack_top` = ".$currenttop." " . 
							"ORDER BY `temp_tree`.`".$this->get_field_position()."` ASC LIMIT 1");

				if($row) {
					
					$id = get_value($row,$this->get_field_id());
					
					$data = $row;
					$data["stack_top"] 			= ($currenttop + 1);
					$data[$this->get_field_left()] 		= $counter;
					$data[$this->_fields["right"]] 		= null;
					$data[$this->get_field_level()] 		= $currenttop;
					
					$dbq = new DBQ("temp_stack");
					$dbq->insert($data);
					
					$this->db->query("DELETE FROM `temp_tree` WHERE `".$this->get_field_id()."` = ".$id);
					
					$counter++;
					$currenttop++;
				} else {
					$this->db->query("UPDATE temp_stack SET  `".$this->_fields["right"]."` = ".$counter.",  `stack_top` = -`stack_top` WHERE `stack_top` = ".$currenttop);					
					$counter++;
					$currenttop--;
				}
			}

			$this->db->query("CREATE TEMPORARY TABLE `temp_tree2` SELECT ".implode(", ", $select_table_fields)." FROM `".$this->get_table()."` ");
			
			$this->db->query("TRUNCATE TABLE `".$this->get_table()."`");
			
			$this->db->query("INSERT INTO ".$this->get_table()." (" .implode(", ", $select_table_fields).") " . 
					"SELECT " .implode(", ", $select_table_fields)." ".
					"FROM temp_stack " . 
					"ORDER BY `".$this->get_field_id()."`");
			
			DB::enable_foreign_keys();
			
			$has_success = $this->db->has_transaction_success();
			$this->db->complete_transaction();
			
			return $has_success;
		}

                /**
                 * Analyze
                 * @return array 
                 */
		function analyze() {
			
			$report = array();

			$r = $this->db->_select("SELECT `".$this->get_field_left()."` FROM `".$this->_table."` s " . 
					"WHERE `".$this->get_field_parent_id()."` is null");
			
			if(!$r) {
				$report[] = "[FAIL]\tNo root node.";
			}
			else {
				$report[] = (count($r)>1) ? "[FAIL]\tMore than one root node." : "[OK]\tJust one root node.";
			}
			
			$report[] = (get_value($r,array(0,"left")) != 1) ? "[FAIL]\tRoot node's left index is not 1." : "[OK]\tRoot node's left index is 1.";

			$c = $this->db->_select_one("SELECT " . 
							"COUNT(*) FROM `".$this->_table."` s " . 
							"WHERE " . 
							"`".$this->get_field_parent_id()."` is not null AND " . 
							"(SELECT COUNT(*) FROM `".$this->_table."` WHERE `".$this->get_field_id()."` = s.`".$this->get_field_parent_id()."`) = 0 ");
			
			$report[] = ($c > 0) ? "[FAIL]\tMissing parents." : "[OK]\tNo missing parents.";

			$n = $this->db->_select_one("SELECT MAX(`".$this->_fields["right"]."`) FROM `".$this->_table."`");
			
			$c = $this->db->_select_one("SELECT COUNT(*) FROM `".$this->_table."`");
			
			$report[] = ($n/2 != $c) ? "[FAIL]\tRight index does not match node count." : "[OK]\tRight index matches count.";

			$c = $this->db->_select_one("SELECT COUNT(`".$this->get_field_id()."`) FROM `".$this->_table."` s " . 
							"WHERE " . 
								"(SELECT COUNT(*) FROM `".$this->_table."` WHERE " . 
									"`".$this->_fields["right"]."` < s.`".$this->_fields["right"]."` AND " . 
									"`".$this->get_field_left()."` > s.`".$this->get_field_left()."` AND " . 
									"`".$this->get_field_level()."` = s.`".$this->get_field_level()."` + 1" . 
								") != " .
								"(SELECT COUNT(*) FROM `".$this->_table."` WHERE " . 
									"`".$this->get_field_parent_id()."` = s.`".$this->get_field_id()."`" . 
								") ");
			
			$report[] = ($c > 0) ? "[FAIL]\tAdjacency and nested set do not match." : "[OK]\tNS and AJ match";

			return $report;
		}

                /**
                 * Returns a dump
                 * @return string 
                 */
		function get_dump() {
			
			$nodes = $this->db->_select("SELECT * FROM ".$this->_table." ORDER BY `".$this->get_field_left()."`");
			
			$dump = array();
				
			foreach($nodes as $node) {
				$row = str_repeat("-",(int)($node[$this->get_field_level()] + 1) * 1).">";
				
				$data = array();
				$data[] = "Left: ".$node[$this->get_field_left()];
				$data[] = "Right: ".$node[$this->_fields["right"]];
				$data[] = "Level: ".$node[$this->get_field_level()];
				$data[] = "Parent ID: ".$node[$this->get_field_parent_id()];
				$data[] = "Position: ".$node[$this->get_field_position()];
				
				$row .= $node[$this->get_field_id()]." ".implode(", ",$data);
				$dump[] = $row;
			}

			return $dump;
		}
		
                /**
                 * Drops 
                 */
		function drop() {
			
			if($this->is_debug())
				p("function: _drop()");
			
			$this->db->query("TRUNCATE TABLE `".$this->_table."`");
		}
		
                /**
                 * Init 
                 */
		function init() {
			
			if($this->is_debug())
				p("function: _init()");
			
			$count = $this->db->_select_one("SELECT COUNT(*) FROM `".$this->_table."`");
			
			if($count==0) {

				$data[$this->get_field_id()] 		= 1;
				$data[$this->get_field_parent_id()]	= null;
				$data[$this->get_field_position()] 	= 0;
				$data[$this->get_field_left()] 		= 1;
				$data[$this->get_field_right()] 	= 2;
				$data[$this->get_field_level()] 	= 0;
				$data[$this->get_field_id()] 		= self::ROOT_TAG;
				
				$dbq = new DBQ($this->get_table());
				$dbq->insert($data);
			}
		}
		
                /**
                 * Enables lock table write 
                 */
		function enable_lock_table_write() {
			if($this->_enable_locks)
				$this->db->enable_lock_table_write($this->get_table());
		}
		
                /**
                 * Disables lock table 
                 */
		function disable_lock_table() {
			if($this->_enable_locks)
				$this->db->disable_lock_table();
		}
		
                /**
                 * Returns the field id
                 * @param boolean $quote
                 * @return string 
                 */
		function get_field_id($quote=false) 		{ return $this->get_quote(get_value($this->_fields,"id"),$quote); }
                
                /**
                 * Returns the field parent id
                 * @param boolean $quote
                 * @return integer 
                 */
		function get_field_parent_id($quote=false) 	{ return $this->get_quote(get_value($this->_fields,"parent_id"),$quote); }
                
                /**
                 *Returns the filed position
                 * @param boolean $quote
                 * @return integer
                 */
		function get_field_position($quote=false) 	{ return $this->get_quote(get_value($this->_fields,"position"),$quote); }
                
                /**
                 * Returns the field left
                 * @param boolean $quote
                 * @return string 
                 */
		function get_field_left($quote=false) 		{ return $this->get_quote(get_value($this->_fields,"left"),$quote); }
                
                /**
                 * Returns the field right
                 * @param boolean $quote
                 * @return string 
                 */
		function get_field_right($quote=false) 		{ return $this->get_quote(get_value($this->_fields,"right"),$quote); }
                
                /**
                 * Returns the field level
                 * @param boolean $quote
                 * @return string 
                 */
		function get_field_level($quote=false) 		{ return $this->get_quote(get_value($this->_fields,"level"),$quote); }
                
                /**
                 * Returns the table
                 * @return string 
                 */
		function get_table() 				{ return $this->_table; }
                
                /**
                 * Returns quote
                 * @param string $field
                 * @param boolean $quote
                 * @return string|integer
                 */
		function get_quote($field,$quote) 		{ return $quote ? "`".$field."`" : $field; }	
                
                /**
                 * Returns the fields
                 * @return array 
                 */
		function get_fields() 				{ return $this->_fields; }
                
                /**
                 * Checks whether debugs
                 * @return boolean 
                 */
		function is_debug() 				{ return $this->db->is_debug(); }
                
                /**
                 * Debug
                 * @param boolean $debug 
                 */
		function debug($debug=true) 			{ $this->db->debug(); }
		
	}
