	<% 
	if ((sNameChbGrp_required <> "") and (sNameChbGrp_required <> sInput_name)) then
		sNameChbGrp_required=""
		%></div> <%
	end if
	if (bInSameRow=1) then 
		%></div><% 
	end if 
	%>
</form>
<script>
	// http://flowplayer.org/tools/validator/index.html
	// supply the language
	$.tools.validator.localize("nl", {
		'*'		: 'Vul een correcte waarde in',
		':email'  	: 'Vul een juist e-mail adres in',
		':number' 	: 'Vul een numerieke waarde in',
		':url' 		: 'Vul een juiste URL in',
		'[max]'	 	: 'De waarde moet kleiner zijn dan $1',
		'[min]'		: 'De waarde moet groter zijn dan $1',
		'[required]'	: 'Vul dit veld in'
	});

	var ChkbxGrp	= "_keuze";
	var ChkbxItm	= "_keuze";
	var ChkbxErrGrp = "_err_keuze";

	$("#AcceptForm").bind("onFail", function(e, errors)  {

		// we are only doing stuff when the form is submitted
		if (e.originalEvent.type == 'submit') {

			// loop through Error objects and add the border color
			$.each(errors, function()  {
				var input = this.input;
				input.css({borderColor: 'red'}).focus(function()  {
					input.css({borderColor: '#444'});
				});
				input.css({borderStyle: 'solid'}).focus(function(){});
				input.css({borderWidth: '2px'}).focus(function(){});
			});

			$("div.requiredchkgrp").each(function(c){
				var sId = this.id;	

				var Chk = false;
				$("input[name='"+sId+"']").each(function(i){
					if (this.checked == true) {
						Chk = true;
						return false;
					}
				});

				var itm = document.getElementById(sId);
				var err = document.getElementById("_err"+sId);
				if (Chk) {  
					itm.style.border = 'none';
					err.style.display = 'none';
				}
				else {
					itm.style.border = 'solid 2px red';
					err.style.display = 'block';
				}
			});
		}
	});
	$("#AcceptForm").bind("onSuccess", function(e, fields)  {
		// we are only doing stuff when the form is submitted
		if (e.originalEvent.type == 'submit') {
			$(this).licOption(true);
		}
	});
	$("#AcceptForm").bind("reset", function() {
		$("div.requiredchkgrp").each(function(c){
			var err = document.getElementById("_err"+this.id);
			err.style.display = 'none';
			$("input[type='checkbox']").each(function(index) {this.disabled=false;});
		});
	});	


	$("input[type='checkbox']").bind('click', function() {
		var sname = this.name;	
		var itm = document.getElementById(sname);
		if (itm != null) {if (itm.style.borderColor == 'red') { itm.style.borderColor = '#005900';}; };
	});

	// Licentieformulier - Aanvraag demo
	$.fn.licOption = function(bSubmit) { 
		var _bas = 0;	var _uur = 1;	var _sal = 2;	var _rap = 3;	var _act = 4;	var _ea  = 5;	
		var _fac = 6;	var _dec = 7;	var _kpl = 8;	var _tel = 9;	var _kol = 10;	var _inc = 11;
		var _ond = "ond"; var _acc = "acc"
		var _start = "sta";  var _basis = "bas"; var _extra = "ext"; var _plus = "plu"; var _totaal = "tot"; var _rapportage = "rap"; var _uren = "uur"; var _salaris = "loon";

		var brch = "";
		$("select[name='_branchSelectie'] option:selected").each(function () {brch += $(this).text();});
		brch = $.trim(brch).toLowerCase().substring(0, 3);

		var pak = "" ;
		$("select[name='_licentieSelectie'] option:selected").each(function () {pak += $(this).text();});
		pak = ($.trim(pak).substring(0,1)=="-" ? "" : $.trim(pak).toLowerCase().substring(0, 3));

		if(bSubmit) {
			var list = [] ; var extraopt = "";
			$("input[type='checkbox']").each(function(index,item) {list.push(item.checked);});
			if (pak=="") {
				if (brch==_ond) {
					if (list[_ea]||list[_act]||list[_kol]||list[_inc]){if(list[_fac]){pak=_totaal;}else{ pak=_plus;}}
					else if (list[_kpl]||list[_tel]) {if(list[_fac]){pak=_extra;}else{ pak=_basis;}}
					else if (list[_bas]){pak=_start;}
					else if (list[_rap]){pak=_rapportage;};
					
				}
				else if (brch==_acc) {
					if (list[_dec]||list[_tel]||list[_inc]){if(list[_rap]){pak=_totaal;}else{ pak=_plus;}}
					else if (list[_ea]||list[_act]||list[_kol]||list[_kpl]) {if(list[_rap]){pak=_extra;}else{ pak=_basis;}}
					else if (list[_bas]) {if(list[_rap]){pak=_extra;}else{ pak=_start;}}
					else if (list[_rap]){pak=_rapportage;};
				}
				$("select[name='_licentieSelectie'] option").each(function () {
					if ($(this).val().toLowerCase().substring(0, 3)==pak) {
						$("select[name='_licentieSelectie']").val($(this).val());
						$(this).licOption(false);
					};
				});
			}
			extraopt+=list[_uur]?(extraopt!=""?"|":"")+_uren:"";extraopt+=list[_sal]?(extraopt!=""?"|":"")+_salaris:"";extraopt+=(brch==_ond)&&(pak!=_rapportage)&&list[_rap]?(extraopt!=""?"|":"")+_rapportage:"";
			$("input[name='_licentieExtra']").val(extraopt);
		}
		else {
			if (pak!="") {

				$("input[type='checkbox']").each(function(index) {
					this.disabled=(index==_sal)||(index==_uur)||((brch==_ond)&&(pak!=_rapportage)&&(index==_rap))||((brch==_acc)&&(index==_fac))?false:true;
					if(this.disabled==true){this.checked=false;}
					if (pak==_start) {			
						if(index==_bas){this.checked=true;}																			
					}
					else if (pak==_basis) {		
						if (brch==_ond) {if((index==_bas)||(index==_kpl)||(index==_tel)){this.checked=true;}}											
						else if (brch==_acc) {if((index==_bas)||(index==_act)||(index==_ea)||(index==_kpl)||(index==_kol)){this.checked=true;}}		
					}
					else if (pak==_extra) {		
						if (brch==_ond) {if((index==_bas)||(index==_kpl)||(index==_tel)||(index==_fac)){this.checked=true;}}									
						else if (brch==_acc) {if((index==_bas)||(index==_act)||(index==_ea)||(index==_kpl)||(index==_kol)||(index==_rap)){this.checked=true;}}	
					}
					else if (pak==_plus) {		
						if (brch==_ond)      {if((index==_bas)||(index==_kpl)||(index==_tel)||(index==_act)||(index==_ea)||(index==_kol)||(index==11)){this.checked=true;}}					
						else if (brch==_acc) {if((index==_bas)||(index==_act)||(index==_ea)||(index==_kpl)||(index==_kol)||(index==_tel)||(index==_dec)){this.checked=true;}}	
					}
					else if (pak==_totaal) {		
						if (brch==_ond) {if((index==_bas)||(index==_kpl)||(index==_tel)||(index==_fac)||(index==_act)||(index==_ea)||(index==_kol)||(index==_inc)){this.checked=true;}}					
						else if (brch==_acc) {if((index==_bas)||(index==_kpl)||(index==_tel)||(index==_dec)||(index==_act)||(index==_ea)||(index==_kol)||(index==_inc)||(index==_rap)){this.checked=true;}}
					}
					else if (pak==_rapportage) {		
						if(index==_rap){this.checked=true;}																									
					}
				});
			}
			else {
				$("input[type='checkbox']").each(function(index) {this.disabled=false;});
			}
		}
	};
	// LET OP!, indien pijl op|neer in Firefox, zal de event alleen getriggerd worden door een enter!
	$("select[name='_licentieSelectie']").bind('change', function() {$(this).licOption(false);});
	$("select[name='_branchSelectie']").bind('change', function() {$(this).licOption(false);});

	// use the dutch language in the validator
	$("#AcceptForm").validator({ 
		lang: 'nl',
		position: 'top right', 
		offset: [-12, 0],
		message: '<div><em/></div>' // em element is the arrow
	 });


</script>