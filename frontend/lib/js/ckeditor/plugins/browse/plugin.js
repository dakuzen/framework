(function() {
    var cmd =
    {
        modes: { wysiwyg: 1, source: 1 },
        exec: function(editor) {
       		browse_assets = editor.config.browse.onclick;
       		browse_assets(editor);
        }
    };
    var pluginName = 'browse';
    CKEDITOR.plugins.add(pluginName,
    {
      init: function(editor) {
          var command = editor.addCommand(pluginName, cmd);
          editor.ui.addButton('browse',
         {
             label: "Browse Assets",
             command: pluginName,
             icon: "/lib/js/uncom/ckeditor/plugins/browse/browse.png"
         });
      }
  });
})();