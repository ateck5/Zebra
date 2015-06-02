<%
	if (iSS_BannerID<>0) then iChildPageID = iSS_BannerID

	iSS_Total = oPublisher.call("GET_GalleryCount", iChildPageID, "Gallery")

    iSS_Width=oPublisher.call ("Get_GetWebsiteParameterValue", "ss_width", iChildPageID, 0, 0) 
    iSS_height=oPublisher.call ("Get_GetWebsiteParameterValue", "ss_height", iChildPageID, 0, 0) 
    iSS_noribbon=oPublisher.call ("Get_GetWebsiteParameterValue", "ss_noribbon", iChildPageID, 0, 0) 

	if (iSS_Width<=0) then iSS_Width = iSS_DefaultWidth
	if (iSS_height=0) then iSS_height = iSS_DefaultHeight
	
	%>

	<% if ((iSS_DefaultRibbon = 1) and (iSS_noribbon <> 1)) then %>
		<div id="slideshowband" class="ss_Band" style="width:<%=iSS_Width-2 %>px;height:100px;top:175px;">
		</div>
		<div id="slideshowtxt" class="ss_Title" style="width:<%=iSS_Width-2 %>px;height:100px;top:175px;">
			<% oPublisher.call "Msg_PrintPageItem", iChildPageID, "Text"%>
		</div>
	<%end if%> 

	<%if (sSS_DefaultTitle<>"") then%>
		<div id="slideshowTitle" >
			<h2><%=sSS_DefaultTitle %></h2>
		</div>
	<%end if %>
    <div id="<% if (iSS_Total=1) then%>static_<%end if%>slideshow" style="height: auto !important;">
	<%
		For iSS_Loop = 0 To (iSS_Total - 1)
			'sSS_ImageUrl    = oPublisher.call("GET_GalleryImageURL", iChildPageID, "Gallery", iSS_Loop)
			'sSS_ImageAlt    = oPublisher.call("GET_GalleryAltText", iChildPageID, "Gallery", iSS_Loop)

			sSS_ImageThumb  = oPublisher.call("GET_GalleryThumbURL", iChildPageID, "Gallery", iSS_Loop, 950,iSS_height, 2)
			'If sSS_ImageThumb = "" Then sSS_ImageThumb = "Images/ThumbNAV.jpg"
            sSS_ImageThumb = Left(sSS_ImageThumb,InStr(sSS_ImageThumb,"Random"))
			if (iSS_Loop = 0) then
				%>
				<img src="<%=sSS_ImageThumb%>" alt="<%=sSS_ImageAlt%>" class="imgageslide" <%if (iSS_Width>0) then %> width="<%=iSS_Width %>px"<% end if %> />
				<% 
			else
				%>
				<img lsrc="<%=sSS_ImageThumb%>" alt="<%=sSS_ImageAlt%>" class="imgageslide" <%if (iSS_Width>0) then %> width="<%=iSS_Width %>px"<% end if %> />
				<% 
			end if
		Next
    %>
    </div>
    <div id="pager" <% if (iSS_Total<=1) then %>style="display:none;"<% else %>style="top:<%=iSS_height-10%>px;"<% end if %> >
        <div id="toggle">
            <a id="togglebutton" class="togglebuton" href="#">
                <span>pause</span>
                <span style="display: none">start</span>
            </a>
        </div>
    </div>