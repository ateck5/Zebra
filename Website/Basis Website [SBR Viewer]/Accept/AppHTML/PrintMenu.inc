<%
   	iPrtMnId = oPublisher.call ("GET_PageIDByLabel", iPrtMnLabel)			' LET OP! == LABEL
	iPrtMnItems = oPublisher.call ("GET_CountChildren", iPrtMnId)

	For iPrtMnCtr = 1 to iPrtMnItems 
		iPrtMnChildId = oPublisher.call("GET_ChildPageID", iPrtMnId, iPrtMnCtr)
		iPrtMnDocType = oPublisher.call ("GET_ObjectType", iPrtMnChildId)
		If (iPrtMnDocType = "LNK") Then
			iPrtMnLinkId = oPublisher.call ("GET_ObjectUrl", iPrtMnChildId)
			if (InStrRev(uCase(iPrtMnLinkId),"@PAGE_") > 0) then
				iPrtMnLinkId = replace(replace(Ucase(iPrtMnLinkId),"HTTP://@PAGE_",""),"@","")
				iPrtMnUrl = oPublisher.Call ("Get_PageURL", iPrtMnLinkId)
			else
				'sPrtMnTarget = oPublisher.Call("Get_ObjectTarget", iPrtMnChildId, "")
                'If (sPrtMnTarget = "0") then
                '    iPrtMnUrl = "javascript:OpenWin("""& iPrtMnLinkId & """);"	
                'Else
                '    iPrtMnUrl = "javascript:window.top.location.href="""& iPrtMnLinkId & """;"	
                'End If
				iPrtMnUrl = iPrtMnLinkId		' == Download (b.v. Teamviewer)
			end if
		Else
			if (bPrtMnNews = 0) then
				iPrtMnUrl = oPublisher.Call ("Get_PageURL", iPrtMnChildId)
			else
				iPrtMnUrl = oPublisher.call("get_PageUrl", iPrtMnId, 0) & "&zoomitem=" & iPrtMnCtr
			end if
		End If
		iPrtMnName = oPublisher.call("Get_PrintPageItem", iPrtMnChildId, "Title")
		%><li <%if (iPrtMnClass<>"") then %>class="<%=iPrtMnClass%>"<% end if %>><a href="<%=iPrtMnUrl %>"><%=iPrtMnName%></a></li><%

		if ((iPrtMnMax > -1) and (iPrtMnCtr >= iPrtMnMax)) then exit for
	next
%>	
