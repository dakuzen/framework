(function() {
    var saveCmd =
    {
        modes: { wysiwyg: 1, source: 1 },
        exec: function(editor) {
       		onsave = editor.config.customsave.onclick;
       		onsave(editor);
        }
    };
    var pluginName = 'customsave';
    CKEDITOR.plugins.add(pluginName,
    {
      init: function(editor) {
          var command = editor.addCommand(pluginName, saveCmd);
          editor.ui.addButton('customsave',
         {
             label: editor.lang.save,
             command: pluginName,
             icon: "/lib/js/uncom/ckeditor/plugins/customsave/save.png"
         });
      }
  });
})();