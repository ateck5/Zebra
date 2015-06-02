/* author: Data Access Europe B.V. */
/* Creation date: 4-4-2006 */
/* This file handles all javascript functions for Electos webshop*/


var cDecimalSeparator = "<%=oWebshopPublisher.Call("Get_GetWebshopSetting", "DecimalSeparator", "General")%>";


var xmlreqs = new Array();
var bUpdateShippingPayment = false;

function CXMLReq(freed) {
    this.freed = freed;
    this.xmlhttp = false;
    if (window.XMLHttpRequest)
    {
        this.xmlhttp = new XMLHttpRequest();
    }
    else if (window.ActiveXObject)
    {
        this.xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
}


function xmlhttpPostWS(sURL, bASynchronous)
{
    var pos = -1;
    
    for (var i=0; i<xmlreqs.length; i++)
    {
        if (xmlreqs[i].freed == 1)
        {
            pos = i; break;
        }
    }
    
    if (pos == -1)
    {
        pos = xmlreqs.length;
        xmlreqs[pos] = new CXMLReq(1);
    }
	
    if (xmlreqs[pos].xmlhttp)
    {
        xmlreqs[pos].freed = 0;
        xmlreqs[pos].xmlhttp.open("POST",sURL,bASynchronous);

        xmlreqs[pos].xmlhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        xmlreqs[pos].xmlhttp.send('');
        if (!bASynchronous && xmlreqs[pos].xmlhttp.status == "200") return xmlreqs[pos].xmlhttp.responseText
        else return 0
    }
}


function CalculateTax(iPrice, iTax){
	var iMultiply = 0;
	var iPrice_tax = 0;
	
	if (iTax > 10){
		iMultiply = '1.' + iTax; 	
	} else {
		iMultiply = '1.0' + iTax; 
	}
	iPrice_tax = iPrice * iMultiply;
	iPrice_tax = iPrice_tax.toFixed(2);
		
	if (isNaN(iPrice_tax)) {
		//return "Error in calculating tax price";
	}else{		
		iPrice_tax = makeNumber(iPrice_tax);
		//iPrice_tax = iPrice_tax + "";
		return makeDecimal(iPrice_tax);		
	}
}
// End CalculateTax



function CalculateTotal(){
	var nSubTotalPrice  = makeNumber(document.getElementById("sub_total").innerHTML);	    
	
	try {
		var oShippingSelect = document.getElementById("shipping_select")
		var nShippingPrice  = makeNumber(oShippingSelect.options[oShippingSelect.selectedIndex].getAttribute('price'));	
	}catch(err){
		var nShippingPrice = 0;
	}
    
	try {
		var oPaymentSelect  = document.getElementById("payment_select")
		var nPaymentPrice   = makeNumber(oPaymentSelect.options[oPaymentSelect.selectedIndex].getAttribute('price'));	
	}catch(err){
		var nPaymentPrice = 0;
	}
    
	if (isNaN(nSubTotalPrice) || isNaN(nShippingPrice) || isNaN(nPaymentPrice) ) {
		try{document.getElementById("total_field").innerHTML = makeDecimal("0")}catch(err){};
	}else{
		var nOverallTotal = nSubTotalPrice + nShippingPrice + nPaymentPrice;
		try{document.getElementById("total_field").innerHTML = makeDecimal(nOverallTotal)}catch(err){};
	}	
}

function TotalProdPrice(iLoopId, iProdId, sFormName, iCartDtlId){
	var iUnitPrice, iAmount, iTotalProdPrice, iLoop, iSubTotal, iAttributeCost;
	var oCoupon, oCartDtlRow, iLoopCoupon, sCouponType, iCouponValue, sPreference, iCount, iCouponPrice, iRowCount, iProductAmount;
	var anProductPrice = new Array();
	
	iLoop = parseInt(document.getElementById("loop").value);
	if (document.getElementById("amount" + iLoopId).value != "");
	
	iAmount = parseInt(document.getElementById("amount" + iLoopId).value); 		
	if (isNaN(iAmount) || (iAmount <=0 )) iAmount = 0;
	
	iAttributeCost = makeNumber(document.getElementById("CartDtlRow" + iLoopId).getAttribute("attributeprice"));	

	var iProdUnitPrice = xmlhttpPostWS('./WebshopProductUnitPrice.asp?productid=' + iProdId + '&amount=' + iAmount, false);
    document.getElementById("CartDtlRow" + iLoopId).setAttribute("unitprice",iProdUnitPrice);
	iUnitPrice = makeNumber(iProdUnitPrice);	
	
	if (isNaN(iAmount) || (iAmount <= 0)){
		iTotalProdPrice = 0.00;
		if (iLoop <= 1) {
			check_nextstep(false);
		}
	}else{
		iTotalProdPrice = (iUnitPrice + iAttributeCost) * iAmount;		
		check_nextstep(true);
	}
	document.getElementById("totalprodprice" + iLoopId).innerHTML = makeDecimal(iTotalProdPrice);	
	
	iSubTotal = 0.0;
	for (i=1; i<=iLoop; i++){
		iSubTotal = (iSubTotal + makeNumber(document.getElementById("totalprodprice" + i).innerHTML));
	}
	if(iSubTotal == 0.00) check_nextstep(false);
	document.getElementById("sub_total").innerHTML = makeDecimal(iSubTotal);	
		
	CalculateTotal();
	
	var sSubTotal = document.getElementById("sub_total").innerHTML;
	var sOrderTotal = document.getElementById("total_field").innerHTML;	
	
    sRemoveCouponIds = xmlhttpPostWS('./RemoveCoupon.asp?ordertotal=' + sOrderTotal + '&subtotal=' + sSubTotal + '&productid=' + iProdId + '&amount=' + iAmount + '&CartDtlId=' + iCartDtlId, false)	
	removeCouponRows(sRemoveCouponIds);	
	
	//update coupon of type free product	
	iLoopCoupon = parseInt(document.getElementById("loopcoupons").value);	
	
	for(i=1; i<=iLoopCoupon; i++)
	{
		oCoupon = document.getElementById("CouponValue" + i)
		sCouponType  = oCoupon.getAttribute('coupontype');
		iCouponValue = makeNumber(oCoupon.getAttribute('value'));
		
		if (sCouponType == "Free Product" && iCouponValue == iProdId) 
		{
			sPreference = oCoupon.getAttribute('preference');
			iCount = 0;
			
			for (iRowCount=1; iRowCount<=iLoop; iRowCount++)
			{
				oCartDtlRow = document.getElementById("CartDtlRow" + iRowCount);	
				iProductAmount = parseInt(document.getElementById("amount" + iRowCount).value);		
				
				if (oCartDtlRow.getAttribute('productid') == iProdId && iProductAmount > 0)
				{					
					anProductPrice[iCount] = makeNumber(document.getElementById("CartDtlRow" + iRowCount).getAttribute("unitprice")) + makeNumber(document.getElementById("CartDtlRow" + iRowCount).getAttribute("attributeprice"));
					iCount = iCount + 1;				
				}
			}
			
			if (anProductPrice.length > 0)
			{
				anProductPrice.sort(function(a,b){return a - b}); 
				if (sPreference == "0") iCouponPrice = anProductPrice[0]; 
				else if(sPreference =="1") iCouponPrice = anProductPrice[anProductPrice.length - 1];
				if(document.getElementById("CouponPrice" + i)) document.getElementById("CouponPrice" + i).innerHTML = makeDecimal("-" + iCouponPrice);	
			}	
			else			
			{
				if(document.getElementById("CouponPrice" + i)) document.getElementById("CouponPrice" + i).innerHTML = makeDecimal("0");		
			}
		}	
	}
	
	CalculateDiscount();	
}

function enablefields(status){

    if (document.getElementById("ship_div"))
    {
	    var ship_table = document.getElementById("ship_div");
	    //Enable field 
	    if (status == true){		
	    	ship_table.style.display = "";		
	    }else{
	    	ship_table.style.display = "none";
	    }
	}
}


function check_nextstep(status){
	var obj = document.getElementById("next")
	if (status == true){
		obj.setAttribute('href', obj.attributes['href_bak'].nodeValue);
		obj.className="button";	
	}else{
		var href = obj.getAttribute("href");
		if(href && href != "" && href != null){
			obj.setAttribute('href_bak', href);
		}
		obj.removeAttribute('href');
		obj.className="buttonDisable";	
	}
}

/*
function make_decimal(price){
	var iPrice, iPos;
	iPrice = price.toFixed(2);
	iPrice = iPrice+"";//convert to string
	iPrice = iPrice.replace(".",",");	
	iPos = (iPrice.indexOf(",") + 1);	
	if((iPrice.length - iPos) == 1) iPrice = iPrice + "0";
	else if(iPos == 0) iPrice = iPrice + ",00"; 
	return iPrice;	
}*/



//returns a number
function makeNumber(sPrice){
	var nNumber;
	sPrice = sPrice+"";
	if(cDecimalSeparator == ",") {
		if (sPrice.indexOf(",") > 0) {
			nNumber = parseFloat(sPrice.replace(",","."));			
		}
		else return (parseFloat(sPrice) + 0.0);
	}
	else  nNumber = (parseFloat(sPrice) + 0.0);
	return nNumber;
}

// this functions makesure that the price has always 2 decimals. most have a . (dot)
function makeDecimal(price){
	var iPrice, iPos;
	var sPrice;
	iPrice = parseFloat(price);		
	iPrice = iPrice.toFixed(2);
	sPrice = iPrice+"";//convert to string	
	if (cDecimalSeparator == ",") sPrice = sPrice.replace(".", cDecimalSeparator);	
	iPos = (sPrice.indexOf(cDecimalSeparator) + 1);	
	if((sPrice.length - iPos) == 1) sPrice = sPrice + "0";
	else if(iPos == 0) sPrice = sPrice + cDecimalSeparator + "00"; 
	return sPrice;	
}

function CalculateTotalDiscount()
{	
	var iLoop, iTotalDiscount, iDiscount;
	
	iLoop = parseInt(document.getElementById("loopcoupons").value);
	
	iTotalDiscount = 0.0;		
	for (i=1; i<=iLoop; i++){
		if (document.getElementById("CouponPrice" + i)) iDiscount = makeNumber(document.getElementById("CouponPrice" + i).innerHTML);
		else iDiscount = 0.0;
		
		iTotalDiscount = (iTotalDiscount + iDiscount);
	}	
	
	document.getElementById("total_discount").innerHTML = makeDecimal(iTotalDiscount);
	document.getElementById("discount_total").innerHTML = makeDecimal(iTotalDiscount);
	document.getElementById("TotalWithoutDiscount").innerHTML = document.getElementById("total_field").innerHTML;	
	
	var nOverallTotal = iTotalDiscount + makeNumber(document.getElementById("total_field").innerHTML);
	document.getElementById("OverallTotal").innerHTML = makeDecimal(nOverallTotal);	
}

function CalculateDiscount()
{
	var oCoupon, sCouponType, iCouponValue, iCouponPrice, nVat;
	
	<%
		Dim bVat, sVat, nVat
		bVat = oWebshopPublisher.call("Get_GetWebshopSetting", "IncludingVAT", "VAT")
		
		If bVat = "1" Then
			sVat = oWebshopPublisher.call("Get_GetWebshopSetting", "DefaultVAT", "VAT")
			
			If (LCase(sVat) = "high") Then 
				sVat = "HighVAT"
			ElseIf (LCase(sVat) = "low") Then 
				sVat = "LowVAT"				
			Else 
				sVat = "NullVAT"
			End If
			
			nVat = oWebshopPublisher.call("Get_GetWebshopSetting", sVat, "VAT")
		End If		
	%>	
	
	try {
		var oShippingSelect = document.getElementById("shipping_select")
		var nShippingPrice  = "-" + makeNumber(oShippingSelect.options[oShippingSelect.selectedIndex].getAttribute('price'));	
	} catch(err) {
		var nShippingPrice = 0;
	}
    
	nTotal = makeNumber(document.getElementById("total_field").innerHTML);	
	iLoop = parseInt(document.getElementById("loopcoupons").value);	
	
	for(i=1; i<=iLoop; i++)
	{
		oCoupon = document.getElementById("CouponValue" + i)
		sCouponType  = oCoupon.getAttribute('coupontype');
		iCouponValue = makeNumber(oCoupon.getAttribute('value'));
		
		switch(sCouponType)
		{	
			case "Percentage":
				iCouponPrice = ((nTotal * iCouponValue)/100);
				nVat = parseFloat('<%=nVat%>')	
				iCouponPrice = "-" + (((nVat + 100)/100) * iCouponPrice)	
				if(document.getElementById("CouponPrice" + i)) document.getElementById("CouponPrice" + i).innerHTML = makeDecimal(iCouponPrice);	
				break;
				
			case "Free Shipping":
				if(document.getElementById("CouponPrice" + i)) document.getElementById("CouponPrice" + i).innerHTML = makeDecimal(nShippingPrice);		
				break;
		}		
	}
	CalculateTotalDiscount();	
}

function removeCouponRows(sRemoveCouponIds)
{
	var oTable, oRow, oCurrentRow;
	var iCount, iCouponId;
	var asCouponRow = new Array();	
	
	asCouponRow = sRemoveCouponIds.split(",");	
	oTable = document.getElementById("CartCouponDetails");
	
	for (iCount=0; iCount<asCouponRow.length; iCount++)
	{		
		oRows = oTable.rows;		
		for (i=0; i<oRows.length;i++)
		{
			oCurrentRow = oRows[i];
			iCouponId = oCurrentRow.getAttribute('couponid');			
			var trElement = oRows[i].sectionRowIndex;
			if (asCouponRow[iCount] != "0" && asCouponRow[iCount] == iCouponId)
			{
				document.getElementById("CartCouponDetails").deleteRow(trElement);				
			}
		}	
	}	
	
	if (oTable.rows.length == 1) 
	{
		oTable.style.display = "none"; 
		document.getElementById("finaltotalOverview").style.display="none";
		document.getElementById("total_fieldrow").style.display="";		
	}
	else setBackGroundTable();
}	

function RemoveCoupons()
{
	var sSubTotal = document.getElementById("sub_total").innerHTML;		
	var sOrderTotal = document.getElementById("total_field").innerHTML;		
	var sRemoveCouponIds = xmlhttpPostWS('./RemoveCoupon.asp?ordertotal=' + sOrderTotal + '&subtotal=' + sSubTotal + '&productid=0&amount=0&CartDtlId=0', false)		
	removeCouponRows(sRemoveCouponIds);		
}

function RemoveCouponFromCart(iCouponId)
{
	var sRemoveCouponIds = xmlhttpPostWS('./RemoveCouponFromCart.asp?couponid=' + iCouponId, false)		
	removeCouponRows(sRemoveCouponIds);	
	CalculateTotalDiscount();
}

function setBackGroundTable(){
	var iCount = 0;
	
	oTable = document.getElementById("CartCouponDetails");
	oRows = oTable.rows;		
	for (i=0; i<oRows.length;i++)
	{   
    	iCount++;
		  if ((iCount%2) == 1){			
		  	oRows[i].className = "uneven";
	  	}else{
	  		oRows[i].className = "even";
	  	}
    }
}	

function SetAttributeDetails(oSelect)
{	
	var sAttributeId = oSelect.getAttribute('attributeid');	
	var sNumber  = oSelect.options[oSelect.selectedIndex].getAttribute('number');	
	var sCost = oSelect.options[oSelect.selectedIndex].getAttribute('cost');	
	
	document.getElementById("selectnumber" + sAttributeId).innerHTML = sNumber;
	document.getElementById("selectcost" + sAttributeId).innerHTML = sCost + " <%=oWebshopPublisher.call("Get_GetWebshopSetting", "Currency", "General")%>";	
	   
}

function getProductTypeValues()
{
	var iTypeValueId = "";
	var sValue = "";
	var sReturnData = "";	
	
	if (document.getElementById("ProductType"))
	{
		oTable = document.getElementById("ProductType");
		oRows = oTable.rows;		
		for (iCount=1; iCount<oRows.length; iCount++)
		{   
	    	var iAttributeId = oRows[iCount].getAttribute('attributeid');
			var sAttributeType = oRows[iCount].getAttribute('attributetype');
			
			oTypeValueTable = document.getElementById("table" + iAttributeId);
			oTypeValueRows = oTypeValueTable.rows;
			for (i=0; i<oTypeValueRows.length; i++)
			{		
				switch(sAttributeType)
				{						
					case "TEXTFIELD":
						iTypeValueId = oTypeValueRows[i].getAttribute('typevalue');
						sValue = document.getElementById("value" + iTypeValueId).value;					
						break;
						
					case "CHECKBOX":					
						if (document.getElementById("value" + oTypeValueRows[i].getAttribute('typevalue')).checked)
						{
							iTypeValueId = oTypeValueRows[i].getAttribute('typevalue');
							sValue = document.getElementById("value" + iTypeValueId).value;		
						}
						break;
					
					case "RADIOBTN":					
						if (document.getElementById("value" + oTypeValueRows[i].getAttribute('typevalue')).checked)
						{
							iTypeValueId = oTypeValueRows[i].getAttribute('typevalue');
							sValue = document.getElementById("value" + iTypeValueId).value;		
						}
						break;	
						
					case "SELECTBOX":
						var oSelect = document.getElementById("select" + iAttributeId); 	
						iTypeValueId  = oSelect.options[oSelect.selectedIndex].value;
						sValue = oSelect.options[oSelect.selectedIndex].innerHTML;	
						break;
				}	
				
				if (iTypeValueId != ""){
					if (sReturnData == "") sReturnData = iTypeValueId;
					else sReturnData = sReturnData + "," + iTypeValueId;
				}	
				
				iTypeValueId = "";
				sValue = "";			
			}
		}
	}	
	return 	sReturnData;
}

function getViewportHeight()
{
    if (window.innerHeight!=window.undefined) return window.innerHeight;
	if (document.compatMode=='CSS1Compat') return document.documentElement.clientHeight;
	if (document.body) return document.body.clientHeight; 
	return window.undefined; 
}

function getViewportWidth(){
	if (window.innerWidth!=window.undefined) return window.innerWidth; 
	if (document.compatMode=='CSS1Compat') return document.documentElement.clientWidth; 
	if (document.body) return document.body.clientWidth; 
	return window.undefined; 
}

function getAbsoluteOffsetTop(oObject) 
{
    var iReturnValue = 0;
   
    while (oObject!=null)
    {
        iReturnValue += oObject.offsetTop;	
		oObject = oObject.offsetParent;
    }
	
    return iReturnValue;
}

function getAbsoluteOffsetLeft(oObject)
{
    var iReturnValue = 0;
    
    while (oObject!=null)
	{
        iReturnValue += oObject.offsetLeft;
	    oObject = oObject.offsetParent;
    }    
	
    return iReturnValue;
}

function ShowAttributes(oElementOnOver, iCartDetailId, iProductId)
{
    var sUrl = "";	
    
    if (!window.top.document.getElementById("tooltip"))
    {
        var oPopup = window.top.document.createElement('span');
        oPopup.setAttribute('id','tooltip')
        window.top.document.body.appendChild(oPopup);
    }
    else
    {        
        var oPopup=window.top.document.getElementById("tooltip");
    }	
    
	oPopup.innerHTML = "loading...";
	oPopup.style.display="block";
	var iLeft = getViewportWidth() - oPopup.offsetWidth;
	var iTop = getViewportHeight() - oPopup.offsetHeight;
	oPopup.style.display="none";    	
	
	oPopup.style.top=Math.min(getAbsoluteOffsetTop(oElementOnOver),iTop)+"px";		
	oPopup.style.zIndex = "20000";
	oPopup.style.display = "block";	

	if (getViewportWidth() - Math.min(getAbsoluteOffsetLeft(oElementOnOver)+oElementOnOver.offsetWidth,iLeft) <= oPopup.offsetWidth)
	{	
		oPopup.style.left = getAbsoluteOffsetLeft(oElementOnOver)- oPopup.offsetWidth +"px";
	}
	else
	{
		oPopup.style.left=Math.min(getAbsoluteOffsetLeft(oElementOnOver)+oElementOnOver.offsetWidth,iLeft)+"px";
	}	
	
	if (getViewportHeight() - Math.min(getAbsoluteOffsetTop(oElementOnOver)+oElementOnOver.offsetHeight,iTop) <= oPopup.offsetHeight)
	{	
		oPopup.style.top = getAbsoluteOffsetTop(oElementOnOver)- oPopup.offsetHeight +"px";
	}
	else
	{
		oPopup.style.top=Math.min(getAbsoluteOffsetTop(oElementOnOver),iTop)+"px";
	}
	
	var sTooltip = xmlhttpPostWS('./ToolTip.asp?cartdetailid=' + iCartDetailId + '&productid=' + iProductId, false)	
	oPopup.innerHTML = unescape(sTooltip);	

}; 

function HideAttributes()
{
	window.top.document.getElementById("tooltip").style.display="none";	
}; 

function UpdateAmount(sOperation, sCurrentAmount, iLoopId, iProdId, sFormName, iCartDtlId)
{
	var iCurrentAmount;
	
	iCurrentAmount = parseInt(sCurrentAmount); 		
	if (isNaN(iCurrentAmount) || (iCurrentAmount <=0 )) iCurrentAmount = 0;
	
	if (sOperation == "+")      iCurrentAmount = iCurrentAmount + 1;
	else if (sOperation == "-") iCurrentAmount = iCurrentAmount - 1;
	
	if (iCurrentAmount <=0 ) iCurrentAmount = 0;
	
	document.getElementById("amount" + iLoopId).value = iCurrentAmount;
	//Update order total
	TotalProdPrice(iLoopId, iProdId, sFormName, iCartDtlId);
};