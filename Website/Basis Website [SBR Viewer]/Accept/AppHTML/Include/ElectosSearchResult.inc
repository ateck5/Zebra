<%
    ' This include file can be used to print the resuls of search action
    
    ' We determine if any errors occures and, if so, we print these
    ' After a search has been initiated by the "SearchCreateID" function the "SearchErrorText" function can be used to retrieve possible errors. Not finding any results is consider an error as well!
    sError = oPublisher.call("get_SearchErrorText",iSearchId)

	If sError="" then

        'get the total amount of results
        iResultCount = oPublisher.Call("Get_SearchResultCount", iSearchId)

		' The "SearchHeader" function can be used to retrieve a string containing the header of a search result. It will tell you how many items were found and which items are shown at the moment.
    	Response.Write(oPublisher.call("get_SearchHeader", iSearchId, iSearchPage, "Result {BEGIN} to {END} of {TOTAL}") + "<br />")
		
		' The SearchRunReport function prints the results of a given search result within a HTML-table. The table-tag will be given the classname "SearchTable", the result rows will be given the
		' classnames "SearchRowEven" and "SearchRowOdd" alternately. These classnames can be used to change the appearance of the result table.
    	%>
    	<table class="SearchTable" >
    	<%
    	
    	'calculate the amount of items to be displayed (max per page set to 10)
    	iDisplayCount = (iResultCount - ((iSearchPage-1) * 10))
    	If (iDisplayCount > 10) Then iDisplayCount = 10
    	
    	For iCounter = 0 to iDisplayCount-1
    	    sResultType = oPublisher.Call ("Get_SearchResultItemType", iSearchId, (iCounter + ((iSearchPage-1) * 10)))
    	    iResultItemID = oPublisher.Call ("Get_SearchResultItemID", iSearchId, (iCounter + ((iSearchPage-1) * 10)))
    	    If (sResultType = "doc") Then
    	        sResultName = oPublisher.Call ("Get_PageName", iResultItemID)
    	        sTemplate = oPublisher.Call ("Get_ASPTemplate", iResultItemID)
    	        sVariableName = "pageid"
    	    Else
    	        sResultName = oWebshopPublisher.Call ("Get_ProductName", iResultItemID)
    	        sTemplate = "webshopdetail.asp"
    	        sVariableName = "productid"
    	    End If
    	    
    	    %>
	        <tr style="text-align: left" class="SearchRow<%If iCounter mod 2 = 1 Then%>Odd<% Else %>Even<% End If%>">
	        	<td style="text-align: left">
	        		<a href="<%=sTemplate%>?<%=sVariableName%>=<%=iResultItemID%>"><%=sResultName%></a>
	        	</td>
	        </tr>
    	    <%     
        Next
		
		%>
		</table>
		<%
		
		' If the result is split over multiple page we create a way to navigate between these pages
		If iSearchPage > 1 then 
			%><a href="search.asp?SearchId=<%=iSearchId%>&SearchPage=<%=iSearchPage-1%>">Previous</a> <%
		end if
		If iResultCount > 10 and iDisplayCount = 10 then
			%><a href="search.asp?SearchId=<%=iSearchId%>&SearchPage=<%=iSearchPage+1%>">Next</a> <%
		end if
	else
		Response.Write(sError)
	end if
%>