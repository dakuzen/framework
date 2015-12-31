
<div id="graphic-designer"></div>

<script>
        $(function() {     	
        	$("#graphic-designer").designer(<?=JSON_UTIL::encode($options)?>);  
        });
</script>

<?=HTML_UTIL::hidden("id","");?>