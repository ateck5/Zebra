<%
	sItemTitleClass = oPublisher.call("Get_GetWebsiteParameterValue", "ItemTitleClass", iChildPageID, 0, 0) 
	if (sItemTitleClass <> "") then %> <div class="color <%=sItemTitleClass %>"><% end if
	sTxtH2 = oPublisher.call("Get_PrintPageItem", iChildPageID, "Title")
	sUrlH2 = oPublisher.call("Get_PrintPageItem", iChildPageID, "URL")
	if sUrlH2 <> "" then
		bPageEmbedded = oPublisher.call("Get_GetWebsiteParameterValue", "EmbeddedTab", iChildPageID, 0, 0) 
		%> <a href="<%=sUrlH2%><% if (bPageEmbedded) then %>&tabpageid=<%if(iTabPageId) then%><%=iTabPageId%><%else%><%=iPageId%><%end if%>&tabpagetitle=<%=sTxtH2%><% end if %>"><% 	
	end if 
	if (sTxtH2 <> "") then
		%>
		<h2><%=sTxtH2 %>
		<%if sUrlH2<>"" then %>    &#187;<% end if %></h2>
		<%
	end if
	if sUrlH2<>"" then %></a><% end if %>
	<% oPublisher.call "Msg_PrintPageItem", iChildPageID, "Text"
	if (sItemTitleClass <> "") then %> </div> <% end if
%>