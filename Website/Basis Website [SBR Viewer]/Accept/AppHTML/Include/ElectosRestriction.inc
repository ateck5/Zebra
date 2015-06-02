<%
iRestricted = oPublisher.call ("GET_PageRestricted", iPageID)

If iRestricted = 3 Then Response.redirect ("login.asp?wrong=130") End If
If iRestricted = 4 Then Response.redirect ("login.asp?pageid="+iPageID+"&wrong=140") End If
%>