<%
	Option Explicit ' ASP command that forces you to declare variable before use. This is a a good programming practise.
	Dim iPageID, iProductGroupId, iProductId, sASPTemplate, sPageName, sPageIds, asPageIds ' Needed variables

	Response.Expires = -1 ' Prevent the browser from caching the pages.

	' We need to determine the PageID we want to show with the PageID function
	' The PageID function is used to resolve a PageID to a PageID that can be used to print a page. When 0 is used as parameter, the first activated pageID will be returned  
	iPageID = oPublisher.call ("GET_PageID", request("pageid"))
	sPageName = Request("page")
	
	If (sPageName <> "") Then		
		sPageIds = oPublisher.call ("GET_PageByName", sPageName)		
		asPageIds = split(sPageIds, " ")		
		
		If (UBound(asPageIds) >= 2) Then 
			iPageID = asPageIds(0)
			iProductGroupId = asPageIds(1)
			iProductId = asPageIds(2)
		End If	
		
		If ((UBound(asPageIds) < 2) or (iPageID = 0)) Then
			iPageID = oPublisher.call ("GET_PageIDByName", sPageName, "/", 1)
		End If 		
		
		If (iProductId > 0) Then sASPTemplate = "webshopDetail.asp"
	Else
		iPageID = oPublisher.call ("GET_PageID", iPageID)
	End If
    
    if (iPageID <= 0) then 
        iPageID = oPublisher.call ("GET_PageIDByLabel", "ErrorPage")			' LET OP! == LABEL
        if (iPageID > 0) then 
            Response.Redirect(oPublisher.call("Get_PageURL", iPageID))
        else
            Response.Write ("<html><body>Helaas, de door u gezochte pagina is niet gevonden.</body></html>")
        end if
    else
        If (iPageID > 0 or iProductID > 0) Then
            ' Based on the PageID we determine the ASPTemplate we will use to show the first page. If no layout is given an error will be shown.
            ' The function "ASPTemplate" can be used to determine the filename of the ASP template that should be used to represent a certain page, category or item.
            If (sASPTemplate = "") Then sASPTemplate = oPublisher.call ("GET_ASPTemplate", iPageID)
        
            If (sASPTemplate <> "") Then
                Server.Transfer(sASPTemplate)
            Else
                Response.Write ("<html><body>Geen ASP template gevonden! </body></html>")
            End If
        Else
            Response.Write ("<html><body>Helaas, de door u gezochte pagina is niet gevonden.</body></html>")
        End If
    end if    
%>

