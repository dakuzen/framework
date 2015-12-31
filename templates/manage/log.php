<? if($log) { ?>

	<? if($next_id) { ?>
		<a href="/manage/log/lid:<?=$next_id?>" class="btn btn-small fr">Next</a>
	<? } ?>

	<? if($back_id) { ?>
		<a href="/manage/log/lid:<?=$back_id?>" class="btn btn-small fr mr5">Back</a>
	<? } ?>

	<h2 class="heading"> Log</h2>

	<div class="cb"></div>

	<div id="log-tabs">

		<div id="overview">

			<form action="javascript:;" method="post" id="form-log">	
				<?
					$create_time = CMODEL_TIME::create($log->get_create_date());

					$html_form = HTML_FORM_UTIL::create()	
									->set_label_attribute("class","vat")
									->text("Message",$log->get_message())
									->text("URL",HTML_UTIL::div($log->get_url(),array("class"=>"")))
									->text("Date",$create_time->format()->medium_date_time()." (".$create_time->format()->age().")");

					if($backtrace=JSON_UTIL::decode($log->get_backtrace()))
						$html_form->text("Backtrace","<pre>".print_r($backtrace,true)."</pre>");
				
					if($post=JSON_UTIL::decode($log->get_post()))
						$html_form->text("\$_POST","<pre>".print_r($post,true)."</pre>");

					if($get=JSON_UTIL::decode($log->get_get()))
						$html_form->text("\$_GET","<pre>".print_r($get,true)."</pre>");

					if($server=JSON_UTIL::decode($log->get_server()))
						$html_form->text("\$_SERVER","<pre>".print_r($server,true)."</pre>");

					/*
					if($log->get_response()) {

						$response = trim($log->get_response());

						if(strpos($response,"<")===0)
							$html_form->text("Response","<pre>".htmlentities(XML_READER_UTIL::get_pretty_xml($response))."</pre>");

						elseif(JSON_UTIL::is_encoded($response))
							$html_form->text("Response","<pre>".print_r(JSON_UTIL::decode($response),true)."</pre>");

						else
							$html_form->text("Response","<pre>".$response."</pre>");
					}
					*/

					$html_form->render();
				?>

			</form>

		</div>
	</div>
<? } ?>