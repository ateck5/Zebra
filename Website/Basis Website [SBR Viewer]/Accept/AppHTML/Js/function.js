function OpenWin(psurl) {
    win = window.open(psurl);
}

function getElementAbsPosX(el)
{
    var dx = 0;
    if (el.offsetParent) {
		dx = el.offsetLeft + 8;
		while (el = el.offsetParent) {
		    dx += el.offsetLeft;
		}
    }
    return dx;
}

function getElementAbsPosY(el, foo)
{
    var dy = 0;
    if (el.offsetParent) {
		dy = el.offsetTop + el.offsetHeight / 2;
		while (el = el.offsetParent) {
		    if (foo == 0)
			dy += el.offsetTop;
		}
    }
    return dy;
}

// when an embedded label is clicked, force focus to input field.
function ChangeFocus(el)
{	
	while (el.tagName != 'INPUT') 
	{		
		el = el.previousSibling;	
		if (el.tagName == 'SPAN') el = el.firstChild;			
	}  
	el.focus();
}

// OnChange of an input field, set the label's visibility based on value of the input field.
function UpdateHelpText(el){
  var label = document.getElementById("label_"+el.name);
  if (label) {
    if (el.value == '') {
      label.style.visibility = 'visible';
    }
    else {
      label.style.visibility = 'hidden';
    }
  }
}

// page may not have these objects if user is logged in or login ability is disabled.
function showEmbLabel(){
  var oLogin = document.getElementById("member_loginid");
  var oPassword = document.getElementById("member_password");
  if (oLogin){ UpdateHelpText(oLogin); }
  if (oPassword){ UpdateHelpText(oPassword); }
}

function doResize()
{
	document.getElementById('content').style.width=(document.body.offsetWidth>400)?document.body.offsetWidth-400+'px':'0px';
	document.getElementById('column1').style.height=document.getElementById('content').offsetHeight+50+'px'
}

function doSubmit(event, theForm)
{		
	for (i=0; i<theForm.elements.length; i++){
		if (theForm.elements[i].type == "text" || theForm.elements[i].type == "textarea" || theForm.elements[i].type == "password" ){
			if (theForm.elements[i].getAttribute('required') == "required" && theForm.elements[i].value == ""){
				alert("Please fill in all required fields.");
				return false;
			}
		}
	}
	theForm.submit();
}    

function toggleShipping(bState)
{
	bState=!bState;
	if (document.getElementById('oShipping_Name')){
		document.getElementById('oShipping_Name').disabled = bState;
	}
	if (document.getElementById('oShip_Street1')){
		document.getElementById('oShip_Street1').disabled = bState;
	}
	if (document.getElementById('oShip_Street2')){
		document.getElementById('oShip_Street2').disabled = bState;
	}
	if (document.getElementById('oShip_City')){
		document.getElementById('oShip_City').disabled = bState;
	}
	if (document.getElementById('oShip_State')){
		document.getElementById('oShip_State').disabled = bState;
	}
	if (document.getElementById('oShip_Zip')){
		document.getElementById('oShip_Zip').disabled = bState;
	}
	if (document.getElementById('oShip_Country')){
		document.getElementById('oShip_Country').disabled = bState;
	}
	if (document.getElementById('oShip_Phone')){
		document.getElementById('oShip_Phone').disabled = bState;
	}
}

function sitemap()
{
	var oSitemap = document.getElementById("sitemap")
	if(oSitemap){
		
		this.listItem = function(li){
			if(li.getElementsByTagName("ul").length > 0){
				var ul = li.getElementsByTagName("ul")[0];
				ul.style.display = "none";
				var span = document.createElement("span");
				span.className = "collapsed";
				span.onclick = function(){
					ul.style.display = (ul.style.display == "none") ? "block" : "none";
					this.className = (ul.style.display == "none") ? "collapsed" : "expanded";					
					doResize();
				};
				li.appendChild(span);
			};
		};
		
		var aItems = oSitemap.getElementsByTagName("li");
		for(var i=0;i<aItems.length;i++){
			listItem(aItems[i]);
		};
		
	};	
};

function insertSmiley(text)
{
	var textarea = document.forms['globalForm'].comments;
	text = ' ' + text + ' ';
	
	//check if text length limit for textarea is not exceeding 5000 characters
	if ((text.length + textarea.value.length) <= 5000)
	{
		// Attempt to create a text range (IE).
		if ((textarea.caretPos != null) && (textarea.createTextRange != null))
		{
			var caretPos = textarea.caretPos;

			caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text + ' ' : text;
			caretPos.select();
		}	
		// Firefox text range replace.
		else if (textarea.selectionStart != null)
		{	
			var begin = textarea.value.substr(0, textarea.selectionStart);
			var end = textarea.value.substr(textarea.selectionEnd);
			var scrollPos = textarea.scrollTop;        
			
			textarea.value = begin + text + end;

			if (textarea.setSelectionRange)
			{
				textarea.focus();
				textarea.setSelectionRange(begin.length + text.length, begin.length + text.length);
			}
			
			textarea.scrollTop = scrollPos;		
		}	
		else
		{		
			textarea.value += text;
			textarea.focus(textarea.value.length - 1);	
		}
	}
	
	var iCharsRemaining = 5000 - textarea.value.length;
	document.getElementById('countdown').innerHTML = "[" + iCharsRemaining + "/" + 5000 + "]";	
} 

// 
$(function(){
	$('#MoreSmilies').hide();
	
	$('#ExpandSmilies').toggle(function(){
		$(this).addClass('expanded');
		$('#MoreSmilies').slideDown('fast', 'swing');
	}, function(){
		$(this).removeClass('expanded');
		$('#MoreSmilies').slideUp('fast', 'swing');
	});

});

function RefreshImage(valImageId) 
{
	var objImage = document.images[valImageId];
	if (objImage == undefined) {
		return;
	}
	var now = new Date();
	objImage.src = objImage.src.split('?')[0] + '?x=' + now.toUTCString();
}
	
function limitText(limitField, limitCount, limitNum) {	
	if (limitField.value.length > limitNum) {
		limitField.value = limitField.value.substring(0, limitNum);
	} else {
		var iCharsRemaining = limitNum - limitField.value.length;
		limitCount.innerHTML = "[" + iCharsRemaining + "/" + limitNum + "]";
	}
}

// Read a page's GET URL variables and return them as an associative array.
function getUrlVars() {
    var vars = [], hash;
    var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');
    for (var i = 0; i < hashes.length; i++) {
        hash = hashes[i].split('=');
        vars.push(hash[0]);
        vars[hash[0]] = hash[1];
    }
    return vars;
}
//function init() {
//    var findStr = getUrlVars()["findStr"];
//    if (findStr != null && findStr != "" && findStr != "Zoeken...") {
//        var findinput = document.getElementById('searchbox');
//        findinput.value = findStr;
//        document.getElementById('searchform').submit();
//    }
//}
//window.onload = init;

function getInternetExplorerVersion() {
    // Returns the version of Windows Internet Explorer or a -1
    // (indicating the use of another browser).
    var rv = -1; // Return value assumes failure.
    if (navigator.appName == 'Microsoft Internet Explorer') {
        var ua = navigator.userAgent;
        var re = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
        if (re.exec(ua) != null)
            rv = parseFloat(RegExp.$1);
    }
    return rv;
}