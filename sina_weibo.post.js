Drupal.behaviors.sinaWeiboPost = function(context) {
  var syncTitle = function() {
    // syncronize the global text msg box
    $("textarea[name='social[message]']", context).val($('input[name="title"]').val());
  }

var allowUpdate;
  var checkAllowUpdate = function() {
    return $("textarea[name='social[message]']", context).val() == $('input[name="title"]').val() || !$("textarea[name='social[message]']", context).val().length;
  }
  // On title change
  $('input[name="title"]', context).keydown(function() {
    if(checkAllowUpdate()) {
      allowUpdate = true;
    }
  });
  // On title change
  $('input[name="title"]', context).keyup(function() {
    if(allowUpdate) {
      syncTitle();
    }
    allowUpdate = false;
  });
  // Check all feature
  var changeAllInProc = false;
  // on specific option change, remove check all status when not all items are checked and the way around
  var postToAnyChanged = function(){
    var allChecked = $("input.social-allowed", context).length == $("input.social-allowed:checked", context).length;
    if(!changeAllInProc) {
      if(allChecked) {
	$("input[name='social[check_all]']", context).attr("checked", true);
      }
      else {
	$("input[name='social[check_all]']", context).attr("checked", false);
      }
    }
  }
  postToAnyChanged();
  $("input.social-allowed", context).change(postToAnyChanged);
  // on check all change
  $("input[name='social[check_all]']", context).change(function() {
    changeAllInProc = true;
    if($("input[name='social[check_all]']", context).attr("checked")) {
      $("input.social-allowed", context).attr("checked", true);
    }
    else {
      $("input.social-allowed", context).attr("checked", false);
    }
    changeAllInProc = false;
  });
}