<div id="thumbs">
	<% 
	iGlyTotal = oPublisher.call("GET_GalleryCount", iPageID, "Gallery")
	iGlyRowWidth = oPublisher.call("GET_GalleryRowWidth")
		
	For iGlyLoop = 0 To (iGlyTotal - 1)
		sGlyImageUrl    = oPublisher.call("GET_GalleryImageURL", iPageID, "Gallery", iGlyLoop)
		sGlyImageAlt    = oPublisher.call("GET_GalleryAltText", iPageID, "Gallery", iGlyLoop)
		If (Len(sGlyImageAlt) > 18) Then sGlyImageAlt = Left(sGlyImageAlt, 18)
						
						
		sGlyImageThumb  = oPublisher.call("GET_GalleryThumbURL", iPageID, "Gallery", iGlyLoop, 162, 122, 2)
		If sGlyImageThumb = "" Then sGlyImageThumb = "Images/ThumbNAV.jpg"

		if (bGlly_boxgrid = 1) then						
		    %>
			<div class="boxgrid captionfull">
				<img src="<%=sGlyImageThumb%>" alt="<%=sGlyImageAlt%>"/>
                    <div class="cover boxcaption">
					<h3><%=sGlyImageAlt%></h3>
					<% if (bGlly_Zoom=1) then %>
						<a class="zoom" href="<%=sGlyImageUrl%>" title="<%=sGlyImageAlt%>">Vergroten</a>
					<% end if %>
					<% if (bGlly_Download=1) then %>
						<a class="visit" href="<%=sGlyImageUrl%>">Download original</a>
					<% end if %>

				</div>		
			</div>
		    <% 
		end if
	Next
	%>
</div>
<div class="clear"></div>	