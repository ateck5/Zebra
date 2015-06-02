<%
'variables needed for member registration
Dim sLastName, sFirstName, sEmail
Dim iGroupId
Dim sLastURL

If (request("myaccount") = "myaccount") Then    
    sLastName       = request("oLastName")
    sFirstName      = request("oFirstName")
    sEmail          = request("oEmail")
    iGroupId        = "1"
    
    iErr = oPublisher.call ("Get_ChangeMemberProperties", iMemberId, sFirstName, sLastName, sEmail, iGroupId)
	If (iErr = 0) Then
		iErr = oPublisher.call ("get_AccountSessionUpdateCart", iMemberId, sSessionKey)
	End If
End If
%>