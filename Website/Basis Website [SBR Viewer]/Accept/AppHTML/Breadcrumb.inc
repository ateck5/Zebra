<!--ZOOMSTOP-->
<!--ZOOMSTOPFOLLOW-->
<div id="breadcrumb">
    <ul>
        <li><a href="<%=oPublisher.call("get_PageUrl", oPublisher.call("get_PageId", 0)) %>" class="first">Home</a></li>
        <%
            Dim bBcInWS, iBcLevel, iBcLevelTo, iBcPageId, iBcProductGrpId, iBcExlude, sBcName, iBcOrgPageId

            function MakeText(sName)
                sName = replace(sName, " [hidden]", "")
                sName = replace(sName, "@", "")
                MakeText = sName
            end function

            if (iBcPageIdRedirect > 0) then
            iBcOrgPageId=iPageId
            iPageId=iBcPageIdRedirect
            end if

            '   Determine the target level
            iBcLevelTo = CInt(oPublisher.call("get_PageLevel", iPageId, iProductGroupId))
            iBcProductGrpId = 0
            bBcInWS = false

            '   Loop up to the target level
            For iBcLevel = 1 to iBcLevelTo

            If (bBcInWS) Then
            '   When we are inside a webshop the get_LevelPageId returns an encoded string '{iPageId}9{iProdGrpId}999' and we need this product group id
                    iBcProductGrpId = oPublisher.call("get_LevelPageId", iPageId, iBcLevel, iProductGroupId)
                iBcProductGrpId = Mid(iBcProductGrpId, Len(iBcPageId & "9") + 1, Len(iBcProductGrpId) - Len(iBcPageId & "9") - 3)
                Else    
                    '   Do the regular stuff for pages we only need to determine if the next level will be inside a webshop
                    iBcPageId = oPublisher.call("get_LevelPageId", iPageId, iBcLevel, iProductGroupId)
                    bBcInWS = (oPublisher.call("GET_ObjectType", iBcPageId) = "WSC")
                End If

                if (iBcLevel = (iBcLevelTo-1)) then 
                    iBcExlude = oPublisher.call ("Get_GetWebsiteParameterValue", "ExcludeBreadcrum", iBcPageId, iBcProductGrpId, 0) 
                    if (iBcExlude = "1") then iBcExlude = (iPageID = oPublisher.call ("GET_ChildPageID", iBcPageId, 1))
                else 
                    iBcExlude = 0
                end if
                if (iBcExlude=0) then 
                  If (iBcLevel = iBcLevelTo) and (iBcPageIdRedirect = 0) Then
                      if (iBcInsertParentLink > 0) then
                          sBcName =  MakeText(oPublisher.call("get_PageName", iBcInsertParentLink, iBcProductGrpId))
                          Response.write("<li><a href=""" & oPublisher.call("get_PageUrl", iBcInsertParentLink, iBcProductGrpId) & """>" & sBcName & "</a></li>")
                      end if
                      sBcName = MakeText(oPublisher.call("get_PageName", iBcPageId, iBcProductGrpId))
                      Response.write("<li class=""last"">" & sBcName & "</li>")
                  Else
                      sBcName =  MakeText(oPublisher.call("get_PageName", iBcPageId, iBcProductGrpId))
                      Response.write("<li><a href=""" & oPublisher.call("get_PageUrl", iBcPageId, iBcProductGrpId) & """>" & sBcName & "</a></li>")
                  End If
                end if
            Next
            if (iBcPageIdRedirect > 0) then 
                if (iBcInsertParentLink > 0) then
                    sBcName =  MakeText(oPublisher.call("get_PageName", iBcInsertParentLink, iBcProductGrpId))
                    Response.write("<li><a href=""" & oPublisher.call("get_PageUrl", iBcInsertParentLink, iBcProductGrpId) & """>" & sBcName & "</a></li>")
                end if
                sBcName = MakeText(oPublisher.call("get_PageName", iBcOrgPageId, iBcProductGrpId))
                Response.write("<li class=""last"">" & sBcName & "</li>")
                iPageId=iBcOrgPageId
            end if
        %>
	</ul>
</div>
<!--ZOOMRESTARTFOLLOW-->
<!--ZOOMRESTART-->