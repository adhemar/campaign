var exist_form_attempt = false;
var user_id = jQuery.cookie("user_id"); 
var url = window.location.href;

function form_attempt(field){
  if(!exist_form_attempt){
    var event_info = '{"field":"' + field +'"}';
         
    var text = 'event_type=form_attepmt'
    + '&user_id=' + user_id
    + '&url=' + url
    + '&event_info=' + event_info;
            
    jQuery.ajax({ 
      data: text, 
      type: "GET", 
      url: "campaign-capture", 
      success: function(data){}
    });
    exist_form_attempt = true;
  }
}
function form_submit(form){

    var event_info = jQuery.toJSON(form.serializeObject());
    
    var text = 'event_type=form_submit'
    + '&user_id=' + user_id
    + '&url=' + url
    + '&event_info=' + event_info;
            
    jQuery.ajax({ 
      data: text, 
      type: "GET", 
      url: "campaign-capture", 
      success: function(data){}
    });
}


jQuery(document).ready(function() {
  
  var user_agent = '';
  jQuery.each(jQuery.browser, function(i, val) {
    user_agent = user_agent + ' ' + i + ':' + val;
  });
  
  
  jQuery.getJSON("http://jsonip.appspot.com?callback=?",function(data){
    var ip = data.ip;
    
    if (user_id == null){
      user_id = jQuery.md5(ip + user_agent);
      jQuery.cookie("user_id", user_id);
    }
  
    var referrer =  encodeURIComponent(document.referrer);
        
    var text = 'page_load=true'
    + '&user_agent=' + user_agent
    + '&user_ip=' + ip
    + '&user_id=' + user_id
    + '&referrer=' + referrer;
    
    //Capturing user    
    jQuery.ajax({ 
      data: text, 
      type: "GET", 
      url: "campaign-capture", 
      success: function(data){}
    });
    
    text = 'event_type=impresion'
    + '&user_id=' + user_id
    + '&url=' + url;
    
    //Capturing impresion event 
    jQuery.ajax({ 
      data: text, 
      type: "GET", 
      url: "campaign-capture", 
      success: function(data){}
    });
   
  });
  
  //Form attempt
  jQuery(':input[type=text]').keyup(function() {
    form_attempt(jQuery(this).attr('name'));
  });
  jQuery('select').change(function() {
    form_attempt(jQuery(this).attr('name'));
  });
  
  //Form Submit
  jQuery('form').submit(function() {
    var form = jQuery(this);
    form_submit(form);
  });
  
  
  
});  