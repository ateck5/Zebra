<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <%
      Dim sTitle, sDisplayPageName, sSeparator, sTxtH2, sUrlH2, sTxtP, sPageMinHeight,bCheckEmbedded , bPageEmbedded, sItemTitleClass, sBronMap, bSH_Init, iBcPageIdRedirect, iBcInsertParentLink
      Dim iPrtMnId, iPrtMnChildId, iPrtMnItems, iPrtMnLinkId, iPrtMnUrl, iPrtMnName, iPrtMnDocType, iPrtMnLabel, iPrtMnClass, sPrtMnTarget, iPrtMnCtr, iPrtMnMax, bPrtMnNews		' PrintMenu.inc - Output menu van iPrtMnLabel

	  sPageMinHeight=650
	  bCheckEmbedded=0
	  sBronMap="Downloads/"
	  bSH_Init=0
	  iBcPageIdRedirect=0
	  bPrtMnNews=0
        
      sTitle = oPublisher.call("Get_GetWebsiteParameterValue", "Title", iPageID, iProductGroupId, iProductId)
        
      sDisplayPageName = oPublisher.call("Get_GetWebsiteParameterValue", "PageNameInTitle", iPageID, iProductGroupId, iProductId)
      If (sDisplayPageName <> "0") Then
         sSeparator = oPublisher.call("Get_GetWebsiteParameterCustomValue", "PageNameInTitle", iPageID, iProductGroupId, iProductId)
         sTitle = sTitle & " "  & sSeparator & " " & oPublisher.call ("Get_PageName", iPageID)
      End If        
   %>
    
   <title><%=sTitle%></title>

   <meta http-equiv="Content-type" content="application/xhtml+xml; charset=iso-8859-1" />
   <meta name="description" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaDescription", iPageID, iProductGroupId, iProductId)%>" />
   <meta name="abstract" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaAbstract", iPageID, iProductGroupId, iProductId)%>" />
   <meta name="copyright" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaCopyright", iPageID, iProductGroupId, iProductId)%>" />
   <meta name="keywords" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaKeywords", iPageID, iProductGroupId, iProductId)%>" />
   <meta name="author" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaAuthor", iPageID, iProductGroupId, iProductId)%>" />
   <meta name="robots" content="<%=oPublisher.call("Get_GetWebsiteParameterValue", "MetaRobots", iPageID, iProductGroupId, iProductId)%>" />
    
   <link rel="stylesheet" type="text/css" href="css/css-reset.css" />    
   <link rel="stylesheet" type="text/css" href="css/AcceptStyles.css" />
   <link rel="stylesheet" type="text/css" href="css/MagaDropDowwnMenu.css" />
   <link rel="stylesheet" type="text/css" href="css/Forms.css" />
   <link rel="stylesheet" type="text/css" href="css/ElectosStyles.css" />
   <link rel="stylesheet" type="text/css" href="css/lightbox.css" media="screen" />

   <link rel="shortcut icon" href="favicon.ico" />
   <!--	<script type="text/javascript" src="js/jquery-1.4.2.min.js"></script>				-->
   <!-- <script src="http://cdn.jquerytools.org/1.2.6/full/jquery.tools.min.js"></script>	-->
   <script src="js/jquery.tools.min.js"></script>                        
   <script type='text/javascript' src='js/pagetop.js'></script>    
     
   <!-- NAVIGATION -->
   <!-- #include file='ElectosMenu.inc' -->
   <script type='text/javascript' src='js/jquery.hoverIntent.minified.js'></script>
   <script type='text/javascript' src='js/jquery.dcmegamenu.1.3.4.min.js'></script>    
   <script type="text/javascript"> 
   $(document).ready(function(){ 
      jQuery('#nav').dcMegaMenu({
         rowItems: '2',
         speed: 'fast',
         effect: 'slide',
         outline: 'left',
         fixedWidth: 400,
		 hoverTimeout: 0
         });
      });    
   </script>
   <!-- LIGHTBOX -->
      <script type="text/javascript" src="js/jquery.lightbox-0.5.min.js"></script>
      <script type="text/javascript">
         $(function() {
            $('.zoom').lightBox();
		});
      </script>
   <!-- PHOTO GALLERY BOXCAPTION --> 
      <script type="text/javascript" src="js/boxcaption.js"></script>
	  <script type='text/javascript' src='js/Update.js'></script>
	  <% 
		Dim iGlyLoop, iGlyTotal, sGlyImageUrl, sGlyImageAlt, sGlyImageThumb, iGlyRowWidth, sGlyMap, sGlyClass
		Dim bGlly_boxgrid, bGlly_Zoom, bGlly_Download, sGlyTarget
		Dim sAFWVersie, sAFWBuild, sRAPVersie, sRAPRevisie
		%>
		<!-- #include file='FilesAsp.inc' -->
		<%
		ListFolder "Updates", -1, 0, "", ""
	  %>
   <!-- COMMON FUNCTIONS -->       
      <script type="text/javascript" src="js/function.js"></script>
   <!-- FORMS, INPUT -->
		<script>
			function textAreaAdjust(o) {
				o.style.height = "1px";
				o.style.height = (25+o.scrollHeight)+"px";
			}
			function reloadPage(sRemove) {
				var temp="";
				var newAdditionalURL = "";
				var currentUrl = window.location.href;

				var tempArray = currentUrl.split("?");
				var baseURL = tempArray[0];
				var aditionalURL =tempArray[1];
				if (aditionalURL ) {
					var tempArray = aditionalURL .split("&");
					for ( var i in tempArray ){
						if(tempArray[i].indexOf(sRemove) == -1){
							newAdditionalURL += temp+tempArray[i];
							temp = "&";
						}
					}
					if (newAdditionalURL!="") {newAdditionalURL="?"+newAdditionalURL}
					window.location.assign(baseURL+newAdditionalURL);
				}
			}
		</script>
		<% 
   		Dim sFrm_Action	,sFrm_Enctype ,sFrm_Method ,sFrm_Name ,sFrm_Target
		Dim cnstFrmASP, cnstFrmGlly
		Dim sInput_class, sInput_name, sInput_type, sInput_value, sInput_valueItems, bInput_checked, bInput_disabled, bInput_readonly, iInput_maxlength, iInput_tabindex, sInput_accept, iInputSameRow, bInSameRow, iSelectOption
		Dim sInput_accesskey, sInput_align, iInput_border, iInput_height, iInput_hspace, bInput_ismap, iInput_size, sInput_src, sInput_alt, sInput_usemap, iInput_vspace, iInput_width, bInput_Required, bCheckboxGrp_required, sNameChbGrp_required
		cnstFrmASP="formulier_input.inc"
		cnstFrmGlly="Gallery.asp"
		 %>    
   <!-- SLIDESHOW -->  
		<% 
		   Dim iSS_DefaultWidth, iSS_DefaultHeight, iSS_CorrHeight, iSS_DefaultRibbon, sSS_DefaultTitle, iSS_BannerID
		   Dim iSS_Total,iSS_Width, iSS_height, iSS,  iSS_Loop,  sSS_ImageUrl, sSS_ImageAlt, sSS_ImageThumb, iSS_noribbon
		 %>    
		<script type="text/javascript" src="js/jquery.cycle.all.js"></script>
		<script type="text/javascript">
			$.fn.cycle.defaults.timeout = 1000;
			$(function () {
				if (getInternetExplorerVersion()  <= 8.0) {
					$('#slideshow img').css('behavior', 'none'); 
					$('#static_slideshow img').css('behavior', 'none'); 
				}
				$('#slideshow').cycle({
					fx: 'fade',
					speed: 3000,
					timeout: 5000,
					pause: 1,
					slideResize: 0,
					// callback fn that creates a thumbnail to use as pager anchor 
					pagerAnchorBuilder: function (idx, slide) {
						return '<li><a href="#">' + (idx + 1) + '</a></li>'; 
					},
					// override Cycle's default updateActivePagerLink function     
					updateActivePagerLink: function (pager, currSlide, clsName) {
						$(pager).each(function () {
							$(this).children().removeClass(clsName).eq(currSlide).addClass(clsName);
						});

					},
					before: function (currSlideElement, nextSlideElement, options, forwardFlag) {
						var index = -1;
						var ver = getInternetExplorerVersion();
						if ((ver > -1) && (ver <= 8.0)) {index = 0;}
						$('#slideshowtxt').children().each(
							function () {
								//access to form element via $(this)
								var a = $(this)[0].nodeName;
								
								var nsl=$(nextSlideElement).index();
								if (ver <= 8.0) { nsl=(parseInt(nsl/2))+1; }
								
								if ($(this)[0].nodeName == 'H2') {
									index = index + 1;
								}
								//if (index == $(nextSlideElement).index()) {
								if (index == nsl) {
									$(this).css("display", "block");
								}
								else {
									$(this).css("display", "none");
								}
							}
						);
					}
				});
				if (getInternetExplorerVersion()  <= 8.0) {
					$('#slideshow img').css('behavior', 'url(PIE.htc)'); 
					$('#static_slideshow img').css('behavior', 'url(PIE.htc)'); 
				}
				$(function(){
					$.each(document.images, function(){
							   var this_image = this;
							   var src = $(this_image).attr('src') || '' ;
							   if(!src.length > 0){
								   //this_image.src = options.loading; // show loading
								   var lsrc = $(this_image).attr('lsrc') || '' ;
								   if(lsrc.length > 0){
									   var img = new Image();
									   img.src = lsrc;
									   $(img).load(function() {
										   this_image.src = this.src;
									   });
								   }
							   }
						   });
				});
				$('#togglebutton').click(function (event) {
					event.preventDefault();
					$('#slideshow').cycle('toggle');
					$("#togglebutton span").toggle();
				});
			});
		</script>
   <!-- SHOWHIDE -->  
		<script type='text/javascript' src='js/SlideShowHide.js'></script>
   <!-- SEARCHBOX -->  
		<style type="text/css">.highlight { background: #FFFF40; }</style>
		<script type="text/javascript" src="js/highlight.js"></script>
		<script type="text/javascript">

			function searchonenter(myfield,e) {
				var keycode;
				if (window.event) keycode = window.event.keyCode;
				else if (e) keycode = e.which;
				else return true;

				if (keycode == 13)    {
					StartSearch();
					return false;
				}
				else
					return true;
			}


			function StartSearch(s) {
				var searchStr = document.getElementById('MSearch__Value');
				if (searchStr!="" && searchStr!="Zoeken...") {
					document.forms['Msearch'].action = "MainSearch.asp?zoom_sort=0&zoom_query=" + searchStr.value + "&zoom_and=0";
					document.getElementById('Msearch').submit();
				}
				return false;
			}
     </script>