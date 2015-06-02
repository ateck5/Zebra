<!-- #include file='Include/ElectosPageBegin.inc' -->
<!-- #include file='Include/ElectosRestriction.inc' -->
<!-- #include file='ElectosDates.inc' -->
<!-- #include file='PageTop.inc' -->

</head>
<%
    Dim sZoom_query, iZoom_page, iZoom_per_page, iZoomitem

    '------------------------------------------------------------------------
    ' URL-Argumenten
    '------------------------------------------------------------------------
    sZoom_query     = request("zoom_query")
    iZoom_page      = request("zoom_page")
    iZoom_per_page  = request("zoom_per_page")
    iZoomitem       = request("zoomitem")
    '------------------------------------------------------------------------
    Dim iRecurringItems, Items, iPag, sBetreft, iStart
    iRecurringItems = oPublisher.call ("GET_CountChildren", iPageID)
    iPag = 0
    if not(iZoom_per_page = 10 or iZoom_per_page = 20) then iZoom_per_page = 5
    if cInt(iZoom_page) = 0 then iZoom_page = 1
    iStart = ((iZoom_page-1) * iZoom_per_page) + 1 
    if iStart <= 0  then iStart = 1

    Function RipAllTags(sHTML, iLength)
        sHTML = RipTags(sHTML,"table")
        sHTML = RipTags(sHTML,"tr")
        sHTML = RipTags(sHTML,"td")
        sHTML = RipTags(sHTML,"th")
        sHTML = RipTags(sHTML,"ul")
        sHTML = RipTags(sHTML,"li")
        sHTML = RipTags(sHTML,"p")
        sHTML = RipTags(sHTML,"a")
        sHTML = RipTags(sHTML,"span")
        sHTML = RipTags(sHTML,"strong")
        sHTML = RipTags(sHTML,"br")
        if sHTML > iLength then sHTML = left(sHTML, iLength) & "<b>...</b>"
        RipAllTags = sHTML
    End Function
    Function RipTags(sHTML,sTagType)
	    Dim iStart, sFront, sBack
	    Do While inStr(1,sHTML,"<" & sTagType,1) > 0
		    iStart = instr(1,sHTML,"<" & sTagType,1)
		    If iStart > 0 Then
			    sFront = left(sHTML,iStart-1)
	
			    iStart = instr(iStart,sHTML,">",1)
			    sBack = right(sHTML,len(sHTML)-iStart)
	
			    sHTML = sFront & sBack
		    End If
	
		    iStart = instr(1,sHTML,"<!--" & sTagType & "-->",1)
		    If iStart > 0 Then 
			    sFront = left(sHTML,iStart-1)
			    iStart = instr(iStart,sHTML,">",1)
			    sBack = right(sHTML,len(sHTML)-iStart)
			    sHTML = sFront & sBack
		    End If
	    Loop
	    RipTags = replace(sHTML,"</" & sTagType & ">","")
    End Function
    Function ResultPages(iStart, iStop)
        Dim sHtml, i, sUrl, sZoomStr, iNr
        if (iStart > 1) or (iStop > iZoom_per_page) then 
            sUrl =  oPublisher.call("Get_PageURL",iPageID)
            sZoomStr = "&zoom_per_page=" & iZoom_per_page & "&zoom_query=" & sZoom_query 
            sHtml = "<div class=""result_pages"">"
            if iZoomitem > 0 then
                sHtml = sHtml & "Andere nieuwsitems: "
                if iZoomitem > 1 then sHtml = sHtml  & "<a href=""" & sUrl & "&zoomitem=" & (iZoomitem-1) & sZoomStr & """>&#171; Vorige</a>    "
                if iZoomitem < iRecurringItems then sHtml = sHtml  & "<a href=""" & sUrl & "&zoomitem=" & (iZoomitem+1) & sZoomStr & """>Volgende &#187;</a>"
                sHtml = sHtml  & "<a class=""newslist"" href=""" & sUrl & "&zoom_page=" & (cInt(iZoomitem/iZoom_per_page)+1) & sZoomStr & """>Nieuwslijst</a>"
            else
                sHtml = sHtml & "Pagina's met nieuwsitems: "
                if iStart > 1 then sHtml = sHtml  & "<a href=""" & sUrl & "&zoom_page=" & (iZoom_page-1) & sZoomStr & """>&#171; Vorige</a>    "
                iNr = 1
                For i = 1 to iStop step iZoom_per_page
                    if iNr = cInt(iZoom_page) then
                        sHtml = sHtml  & iNr & "  "
                    else
                        sHtml = sHtml  & "<a href=""" & sUrl & "&zoom_page=" & iNr & sZoomStr & """>" & iNr & "</a>  "
                    end if
                    iNr = iNr + 1
                Next 
                if cInt(iStop) > cInt(iStart + iZoom_per_page - 1) then sHtml = sHtml  & "<a href=""" & sUrl & "&zoom_page=" & (iZoom_page+1)  & sZoomStr & """>Volgende &#187;</a>"
            end if
            sHtml = sHtml  & "</div>"
        end if
        ResultPages = sHtml
    End Function

%>

<body>
   <div id="bghome">
      <!-- #include file='FormulierMelding.inc' -->
      <div id="bgpattern">
         <!-- #include file='BodyTop.inc' -->
         <!-- #include file='BreadCrumb.inc' -->
         <div id="pagepanel">
            
            <div id="contentContainer">
                <div id="contentLarge">
                    <h2><%=oPublisher.call("Get_PrintPageItem", iPageId, "Title") %></h2>
                    
                    <div id="newswrapper">
                    
                        <!--
                        <form class="zoom_newsform" action="News.asp" method="get">
                            <span  class="zoom_searchbox" >
                                Filter op: 
                                <select name"zoom_query"id="zoom_searchbox" >
                                    <optgroup>
                                        <option selected="selected">Toon alle nieuwsitems</option>
                                        <option>Financieel</option>
                                        <option>Rapportage</option>
                                        <option>Electronische aangifte</option>
                                    </optgroup>
                                </select>
                            </span>
                            <input class="zoom_button" type="submit" value="Verzend" />

                            <span class="zoom_results_per_page">
                                Resultaten per pagina
                                <select name="zoom_per_page" >
                                    <optgroup>
                                        <option<%if iZoom_per_page = 5 then %> selected="selected"<%end if %>>5</option>
                                        <option<%if iZoom_per_page = 10 then %> selected="selected"<%end if %>>10</option>
                                        <option<%if iZoom_per_page = 20 then %> selected="selected"<%end if %>>20</option>
                                    </optgroup>
                                </select>
                            </span>

                        </form>
                        -->
                        <%=oPublisher.call("Get_PrintPageItem", iPageId, "Text") %>
                    </div>
                    
                </div>
                <div id="newsnav_begin">
                    <%=ResultPages(iStart,iRecurringItems) %>
                </div>
                <div id="newsresults">
                    <div class="results">
                        <%
                        if cInt(iZoomitem) > 0 then
                            iStart = iZoomitem
                            Items = iZoomitem
                        else 
                            Items = iRecurringItems
                        end if
                        For iCounter = iStart to Items
                            iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)

                            sBetreft=oPublisher.call("Get_PrintPageItem", iChildPageID, "Betreft")
                            if sZoom_query = "" or uCase(sZoom_query) = uCase(sBetreft) then
                                iPag = iPag + 1
                                %>
                                <div class="<%if ((iPag mod 2) = 0) and cInt(iZoomitem) = 0 then %>result_altblock<% else %>result_block<%end if%>">
                                    <div class="result_title">
                                        <% if cInt(iZoomitem) > 0 then %>
                                            <h2><%=oPublisher.call("Get_PrintPageItem", iChildPageID, "Title")%></h2>
                                            <b style="float:right;"><%=oPublisher.call("Get_PrintPageItem", iChildPageID, "Date")%></b>
                                        <% else %>
                                            <b><%=oPublisher.call("Get_PrintPageItem", iChildPageID, "Date")%></b>
                                            <a href="<%=oPublisher.call("get_PageUrl", iPageID, 0)%>&zoomitem=<%=iCounter%>"><%=oPublisher.call("Get_PrintPageItem", iChildPageID, "Title")%></a>
                                        <% end if %>
                                    </div>
                                    <div class="context <% if cInt(iZoomitem) > 0 then %> newsItem<% end if %>">
                                        <% if cInt(iZoomitem) > 0 then %>
                                            <%=oPublisher.call("Get_PrintPageItem", iChildPageID, "Text")%>
                                        <% else %>
                                            <%=RipAllTags(oPublisher.call("Get_PrintPageItem", iChildPageID, "Text"),300)%>
                                        <% end if %>

                                    </div>
                                    <div class="infoline">
                                        nieuwitem heeft betrekking op <%=sBetreft%> - URL:<%=oPublisher.call("get_PageUrl", iPageID, 0)%>&zoomitem=<%=iCounter %>
                                    </div>
                                </div>
                                <%
                            end if
                            if iPag >= iZoom_per_page then Exit For
                        Next
                        %>
                    </div>
                </div>
                <div id="newsnav_end">
                    <%=ResultPages(iStart,iRecurringItems) %>
                </div>
            </div>
         </div>
      </div>
   </div>
    <!-- #include file='BodyBottom.inc' -->
</body>
<!-- #include file='PageEnd.inc' -->