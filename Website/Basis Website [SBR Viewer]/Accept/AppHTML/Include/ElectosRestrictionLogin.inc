<%
'variables needed for member registraion
Dim sUserName, sPassword, sPassword2, sLastName, sFirstName, sEmail, sRedirectUrl
Dim sLastURL
Dim iGroupId, iDisplayLoginBox

If (request("logout") <> "") Then
	iErr = oPublisher.call("get_AccountSessionLogout", sSessionKey)
	response.cookies(sWebsiteName)("mid") = ""
	response.cookies(sWebsiteName).path = "/"
    Session("BuildMenu") = "1"
	Session("MenuHTML") = ""
	
    If (iPageId <> "") Then	
		sRedirectUrl = oPublisher.Call ("Get_PageURL", iPageId, iProductGroupId, iProductId)	
		response.redirect (sRedirectUrl)			       
    Else
        Response.redirect ("default.asp")
    End If

ElseIf (request("login") <> "") Then
    iMemberId = oPublisher.call ("get_AccountSessionLogin", request("member_loginid"), request("member_password"), sSessionKey)

    If ( (iMemberId > 0) AND (sSessionKey <> "") ) then
	    response.cookies(sWebsiteName)("mid") = iMemberId
      	response.cookies(sWebsiteName).path = "/"
        Session("BuildMenu") = "1"
		Session("MenuHTML") = ""
      
        If (sOperation = "checkout") Then
            response.redirect ("WebshopCheckout.asp?pageid=" + iPageId)
        Else        
            If (iPageId <> "") Then
                sRedirectUrl = oPublisher.Call ("Get_PageURL", iPageId, iProductGroupId, iProductId)	
                response.redirect (sRedirectUrl)				    
            Else
                response.redirect ("default.asp")
            End If          
	    End If          
	Else
        If (sOperation <> "" ) Then 
            response.redirect ("login.asp?operation=" + sOperation + "&pageid=" + iPageId + "&wrong=120")		
        Else 
            response.redirect ("login.asp?pageid=" + iPageId + "&wrong=120") 
        End If		
 	End If
ElseIf (request("signup") = "addnew") Then
	sUserName       = request("oLoginId")
	sPassword       = request("oPassword")
	sPassword2      = request("oPassword2")
	sLastName       = request("oLastName")
	sFirstName      = request("oFirstName")
	sEmail          = request("oEmail")
    iGroupId        = "2"
    
    iErr = oPublisher.call ("get_MemberRegistration", sUserName, sPassword, sPassword2, sLastName, sFirstName, sEmail, iGroupId)
	
	If (iErr = "0") Then        
		iDisplayLoginBox = oPublisher.call ("Get_GetWebsiteParameterValue", "DisplayLoginBox", iPageID)
		
		If (iDisplayLoginBox = "1") Then
			iMemberId = oPublisher.call ("get_AccountSessionLogin", sUserName, sPassword, sSessionKey)
							
			iErr = oPublisher.Call ("Get_ChangeMemberGroups", iMemberId, iGroupId )
        
			If (iErr = "0") Then
				iMemberId = oPublisher.call ("get_AccountSessionLogin", sUserName, sPassword, sSessionKey)
			End If
		End If	
    End If
	
    If ( (iErr = 0) and (iMemberId <> "0") and (sSessionKey <> "") ) then
	    response.cookies(sWebsiteName)("mid") = iMemberId
      	response.cookies(sWebsiteName).path = "/"
        Session("BuildMenu") = "1"
		Session("MenuHTML") = ""
		      	
		If (sOperation = "checkout") Then
            response.redirect ("WebshopCheckout.asp?pageid=" + iPageId)
        Else        
            If (iPageId <> "") then
                sRedirectUrl = oPublisher.Call ("Get_PageURL", iPageId, iProductGroupId, iProductId)	
                response.redirect (sRedirectUrl)		    
            Else
                response.redirect ("default.asp")
            End If         
	    End If    
    End If	
ElseIf (request("removeaccount") <> "") Then	
	iMemberId       = request.cookies(sWebsiteName)("mid")
	iErr = oPublisher.call("get_RemoveAccount", iMemberId)
	response.cookies(sWebsiteName)("mid") = ""
	response.cookies(sWebsiteName).path = "/"
    Session("BuildMenu") = "1"
	Session("MenuHTML") = ""
	
	If (iPageId <> "") then
		sRedirectUrl = oPublisher.Call ("Get_PageURL", iPageId, iProductGroupId, iProductId)	
		response.redirect (sRedirectUrl)		    
	Else
		response.redirect ("default.asp")
	End If    
End If
%>