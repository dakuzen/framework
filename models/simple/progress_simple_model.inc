<?
	class BASE_MODEL_PROGRESS {

		static function get_html($form_action,$content_id="",$form_name="") {
			return "";
			$str ='
				<script type="text/javascript" src="/lib/js/jquery/jquery.js"></script>
				<script type="text/javascript" src="/lib/js/livepipe/src/livepipe.js"></script>
				<script type="text/javascript" src="/lib/js/livepipe/src/progressbar.js"></script>

				<script>

					var container = null;
					var progress_bar = null;
	
					function progress_content(content) {
						progress_complete();
						
						var obj = $("'.$content_id.'");
						if(obj)
							obj.innerHTML = content;
					}

					function progress_update(index, total) {
						progress_bar.setProgress((index / total)*100);
					}

					function progress_start(form_action,form_name) {
					
						var statusFrame = $("statusFrame");
						
						if(!statusFrame) {
							statusFrame = new Element("iframe", { id: "statusFrame", name: "statusFrame" }).hide();
							$("body").append(statusFrame);
						
							statusForm = new Element("form", { action: "", method: "post", target: "statusFrame", id : "statusForm" });
							$("body").append(statusForm);
						}
						
						statusForm = $("statusForm");
						
						var form_action = form_action ? form_action : "'.$form_action.'";
						var form_name = form_name ? form_name : "'.$form_name.'";
						
						var url  = form_action;
						
						var form = $(form_name);
						
						if(form)
							url = url + "?" + form.serialize();
						
						statusForm.setAttribute("action",url);
						statusForm.submit();
						container.show();
						progress_bar.reset();
						progress_bar.setProgress(1);
					}	

					function progress_complete() {
						container.hide();
					}

					function create_progress() {

						container = $("<div>",{ id : "progress-bar-container", style : "display:inline-table" });

						var inner_container = $("<div>", { style : "display:inline-table" });

						var progress_status_div = $("<div>",{ id : "progress-status" });
						var progress_bar_div = $("<div>",{ id : "progress-bar" });

						progress_status_div.html("Processing...");

						inner_container.append(progress_status_div);
						inner_container.append(progress_bar_div);
						container.append(inner_container);

						$("body").append(container);

						progress_bar = new Control.ProgressBar("progress-bar", { interval: 0.15 } );

						var progressBarDims 	= container.getDimensions();
						var browserDims 	= document.body.getDimensions();

						var y 			= (browserDims.height - progressBarDims.height) / 2;
						var x 			= (browserDims.width - progressBarDims.width) / 2;

						container.setStyle({ position : "absolute", top : y + "px", left : x + "px" });
						container.hide();
					}

					$(function() {
						create_progress();
					});	

				</script>
				
				<style>

					#progress-bar-container {
						background-color: #999999; 
						margin: 0px;
					}

					#progress-bar-container>div {
						padding:17px;
						border:5px solid #CCCCCC;
						margin: 0px;
					}

					#progress-status {
						font-size: 10px;	
					}

					#progress-bar {  
						width:100px;  
						height:7px;  
						border:1px solid #ccc;  
						padding:0;  
						margin:0;  
						position:relative;  
						background-image:url("/lib/js/livepipe/images/progress_bar.gif");  
						background-repeat:repeat-x;  
					} 

					#progress-bar div {  
						background-color:#fff;  
					}  


				</style>';
				
			return $str;
		}
		
		static function get_progress_update($index,$count) {
			return "";
			return '<script type="text/javascript">parent.progress_update('.$index.', '.$count.');</script>'."\n";
		}
		
		static function get_progress_content($html) {			
			return "";
			$html = preg_replace(array("/\\\\{1}/","/\r{1}/","/\n{1}/",'/"{1}/'),array("\\\\\\","","",'\\"'),$html);
			return '<script type="text/javascript">parent.progress_content("'.$html.'");</script>'."\n";
		}		

		static function render_progress_update($index,$count) {
			return "";
			echo self::get_progress_update($index,$count);
		}
		
		static function render_progress_content($html) {
			return "";
			echo self::get_progress_content($html);
		}		
	}