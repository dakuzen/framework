

<div>Search</div>
<a href="javascript:;" class="btn btn-default btn-small fr" id="preview-toggle">Toggle Full Message</a>
<?=HTML_UTIL::input("message-search","",["class"=>"w500"])?>

<div id="message-list" class="mt10"></div>

<script>

	$(function() {

		$("#message-list").bind("load",function() {
			$(this).load("<?=$current?>",{ action: 'messages', search: $("input[name='message-search']").val() },function() {
				
				$(this).find(".iframe-body").each(function() {
					co = $(this).html();

					iframe = $("<iframe>",{ href: "about:blank", width: "100%", height: "300", frameborder: 0 }).data("co",co).load(function() {
						$(this).contents().find("html").html($(this).data("co"));
					});
					
					$(this).empty().append(iframe);		
				});

			});
		}).trigger("load");

		$("input[name='message-search']").keyup(debounce(function() {
			$("#message-list").trigger("load");
		},300));

		$("#preview-toggle").click(function() {
			$(".preview").toggle();
		});
	});

</script>