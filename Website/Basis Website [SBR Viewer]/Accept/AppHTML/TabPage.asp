<!-- #include file='Include/ElectosPageBegin.inc' -->
<!-- #include file='Include/ElectosRestriction.inc' -->
<!-- #include file='ElectosDates.inc' -->
<!-- #include file='PageTop.inc' -->

</head>

<%  bCheckEmbedded=1

    Dim iTabPageId, sTabPageTitle
    '------------------------------------------------------------------------
    ' URL-Argumenten
    '------------------------------------------------------------------------
    iTabPageId = request("tabpageid")
    sTabPageTitle = request("tabpagetitle")
    '------------------------------------------------------------------------

    Dim iParentID, iChildItems, iTabLevel, iPrevTabLevel, bLi, bUl, iLev0, iSubOpen, iSubOpenAll, bLiSub, iNextTabLevel, iCnt, iCldPID, iRecurringItems, iSildeShow, sLevelTabTitle, iFormulier, sDefaultTitle
    Dim iSpCnt, iCntDone, iPageCnt, iTabCorr, iLastLevel0_ID
    Dim ArraySpID() 
    Dim ArrayChildPageID()
    Dim ArrayChildItemsID()
    Dim ArrayTabLevel()
                               

    if (iTabPageId > 0) then
        iParentID = oPublisher.Call ("Get_LevelPageID", iTabPageId, (oPublisher.Call ("Get_PageLevel", iTabPageId)-1))
        iBcPageIdRedirect=iTabPageId
    else
        iParentID = oPublisher.Call ("Get_LevelPageID", iPageID, (oPublisher.Call ("Get_PageLevel", iPageID)-1))
    end if
    iChildItems = oPublisher.call ("GET_CountChildren", iParentID)
    ReDim Preserve ArrayChildPageID(iChildItems+iTabPageId) 
    ReDim Preserve ArrayTabLevel(iChildItems+iTabPageId) 

    iTabCorr = 0
    For iCounter = 1 to iChildItems
        if iCntDone = iCounter then
            iChildPageID = ArrayChildPageID(iCounter+iTabCorr)
        else
            iChildPageID = oPublisher.call ("GET_ChildPageID", iParentID, iCounter)
            iPageCnt = iPageCnt + 1
            ArrayChildPageID(iPageCnt) = iChildPageID
            iSidePanel = oPublisher.call ("Get_GetWebsiteParameterValue", "SidePanel", iChildPageID, 0, 0) 
            if iSidePanel = 0 then
                iTabLevel = oPublisher.call ("Get_GetWebsiteParameterValue", "LevelTab", iChildPageID, 0, 0) 
                ArrayTabLevel(iPageCnt) = iTabLevel
            elseif iSidePanel = 1 then
                iSpCnt = iSpCnt + 1
                ReDim Preserve ArraySpID(iSPCnt)  
                ArraySpID(iSPCnt) = iChildPageID
            end if
        end if
        if (iTabLevel = 0) then iLastLevel0_ID = iChildPageID
        if ((iCounter = 1) and (iChildPageID <> iPageID)) then
		    iRecurringItems = oPublisher.call ("GET_CountChildren", iChildPageID)
            For iCnt = 1 to iRecurringItems
                iCldPID = oPublisher.call ("GET_ChildPageID", iChildPageID, iCnt)
                iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iCldPID, 0, 0) 
                if (iSildeShow=1) then 
                    if  (oPublisher.call ("Get_GetWebsiteParameterValue", "NoBanner", iPageID, 0, 0) = "0") then iSS_BannerID = iCldPID
                else
                    iSidePanel = oPublisher.call ("Get_GetWebsiteParameterValue", "SidePanel", iCldPID, 0, 0) 
                    if iSidePanel = 1 then
                        iSpCnt = iSpCnt + 1
                        ReDim Preserve ArraySpID(iSpCnt) 
                        ArraySpID(iSpCnt) = iCldPID
                    end if
                end if
		    next 
            iChildPageID = ArrayChildPageID(iCounter+iTabCorr)
        end if
        if (iTabLevel = 0) then iLev0=iCounter
        if ((iChildPageID = iPageID) or (iChildPageID = iTabPageId))  then 
            if (iTabPageId > 0 ) then 
                if (iTabLevel = 0) then iLev0=iCounter+1
                iPageCnt = iPageCnt + 1
                iTabCorr = 1
                ArrayChildPageID(iPageCnt) = iPageID
                ArrayTabLevel(iPageCnt) = iTabLevel
            end if
            iSubOpen = iLev0
            if (iTabLevel > 0) then 
                bLiSub = iLev0
                iBcInsertParentLink = iLastLevel0_ID
            else
                if (cInt(iCounter) < cInt(iChildItems)) then
                iCntDone = iCounter + 1
                iChildPageID = oPublisher.call ("GET_ChildPageID", iParentID, iCntDone)
                iPageCnt = iPageCnt + 1
                ArrayChildPageID(iPageCnt) = iChildPageID
                iSidePanel = oPublisher.call ("Get_GetWebsiteParameterValue", "SidePanel", iChildPageID, 0, 0) 
                if iSidePanel = 0 then
                    iTabLevel = oPublisher.call ("Get_GetWebsiteParameterValue", "LevelTab", iChildPageID, 0, 0) 
                    ArrayTabLevel(iPageCnt) = iTabLevel
                elseif iSidePanel = 1 then
                    iSpCnt = iSpCnt + 1
                    ReDim Preserve ArraySpID(iSPCnt)  
                    ArraySpID(iSPCnt) = iChildPageID
                end if
                if (iTabLevel > 0) then bLiSub = iLev0
                end if
            end if
        end if
    Next  
    iSubOpenAll =  oPublisher.call ("Get_GetWebsiteParameterValue", "LevelTabShowAll", ArrayChildPageID(1), 0, 0) 
%>

<body>
     <div id="bghome">
        <!-- #include file='FormulierMelding.inc' -->
         <div id="bgpattern">
            <!-- #include file='BodyTop.inc' -->
            <!-- #include file='BreadCrumb.inc' -->
            <div id="pagepanel">
               <div id="contentContainer">
                  <div class="tabContainer">
                    <%
                        Dim bNoSitePanel
                        bNoSitePanel= oPublisher.call ("Get_GetWebsiteParameterValue", "NoSitePanels", iPageID, 0, 0) 
                     %>
                     <div class= "tabdialog <% if (bNoSitePanel=1) then %>tabdialogNoSitePanel<% end if %>">
                        <% 
                            '------------------------------------------------------------------------
                            'TABBLOCK
                            '------------------------------------------------------------------------ 
                        %>
                        <!--ZOOMSTOP-->
                        <div class="tabblock">
                           <ul class="tb-menu"><% 
                              iPrevTabLevel = 0
                              For iCounter = 1 to iPageCnt
                                 iChildPageID = ArrayChildPageID(iCounter)
                                 iTabLevel = ArrayTabLevel(iCounter)
                                 if ((iCounter<iPageCnt) and (iSubOpenAll=1)) then iNextTabLevel = ArrayTabLevel(iCounter+1)
                                 if ((iTabLevel = 0) and (iPrevTabLevel = 1) and bUl) then
                                    %> </ul> <%
                                    bUl=false
                                 End If
                                 if ((iTabLevel = 1) and (iPrevTabLevel = 0) and ((iSubOpen = iCounter - 1) or (iSubOpenAll = 1))) then
                                    %> <ul> <%
                                    bUl=true
                                 Else
                                    if (bLi = true) then
                                        %> </li> <% 
                                        bLi=false
                                    End If
                                 End If
                                 
                                 sLevelTabTitle = oPublisher.call ("Get_GetWebsiteParameterValue", "LevelTabTitle", iChildPageID, 0, 0) 
                                 if (sLevelTabTitle="") then sLevelTabTitle=oPublisher.call("Get_PrintPageItem", iChildPageID, "Title")

                                 if (iChildPageID = iPageID) Then 
                                    sDefaultTitle = sLevelTabTitle
                                    %> <li class="rootcurrent<% if (iCounter = 1) or ((iTabLevel = 1) and (iPrevTabLevel = 0) and ((iSubOpen = iCounter - 1) or (iSubOpenAll = 1))) then %> first<% End If 
                                    if (bLiSub=iCounter) then %> submenu<% end if 
                                    %>"><% 
                                    if (iTabPageId > 0) then
                                        if (sTabPageTitle <> "") then
                                            %> 
                                            <div class="embedded"><%=sTabPageTitle%></div>
                                            <% 
                                         else
                                            %> 
                                            <div class="embedded"><%=sLevelTabTitle%></div>
                                            <% 
                                         end if
                                      else %>
                                      <%=sLevelTabTitle%>
                                      <% 
                                    end if 
                                    bLi=true 
                                 Else 
                                    if ((iTabLevel = 0) or bUl) then
                                        dim sDocType, sTarget, sUrl, sId
                                        sDocType = oPublisher.call ("GET_ObjectType", iChildPageID)
                                        If (sDocType = "LNK") Then
                                            sId = oPublisher.call ("GET_ObjectUrl", iChildPageID)
                                            sId = replace(replace(Ucase(sId),"HTTP://@PAGE_",""),"@","")
                                            sUrl = oPublisher.Call ("Get_PageURL", sId)
                                        Else
                                            sUrl = oPublisher.Call ("Get_PageURL", iChildPageID)
                                        End If
                                        %> <li class="<% if (iCounter = 1) or ((iTabLevel = 1) and (iPrevTabLevel = 0) and ((iSubOpen = iCounter - 1) or (iSubOpenAll = 1))) then %>first<% End If 
                                        if ((bLiSub=iCounter) or (iNextTabLevel>iTabLevel)) then %> submenu<% end if
                                        %>"><a href="<%=sUrl%>"><%=sLevelTabTitle %></a> <% 
                                        bLi=true
                                    End If
                                 End if
                                 iPrevTabLevel = iTabLevel
                              Next  
                              if ((iTabLevel = 0) and (iPrevTabLevel = 1)) then
                                    %> </ul> <%
                                    bUl=false
                              End If
                              if (bLi = true) then
                                    %> </li> <% 
                              End If
                              %>
                            </ul>
                        </div>
                        <!--ZOOMRESTART-->
                        <% 
                            '------------------------------------------------------------------------
                            'TABCOLLUMNBLOCK
                            '------------------------------------------------------------------------ 
                        %>
                        <div class="tabcollumnblock <% if (bNoSitePanel=1) then %>tabcollumnblockNoSitePanel<% end if %>" style="min-height: <%=sPageMinHeight %>px;">
                           <%
                               Dim iSidePanel, iShowHide, bShH, iSH, iIsGallary, bSHOut
                               Dim iLevel,bItGrp, bStart, sSHTitle, i, bEmpty, bDivForm
                           

                               iRecurringItems = oPublisher.call ("GET_CountChildren", iPageID)
                               ReDim Preserve ArrayChildItemsID(iRecurringItems)  

                               iLevel = 0
                               bDivForm = 0
                               bShH = oPublisher.call ("Get_GetWebsiteParameterValue", "DoAllHide", iPageID, 0, 0) 
                           
                               iFormulier = oPublisher.call ("Get_GetWebsiteParameterValue", "IsFormulier", iPageID, 0, 0) 
                               if (iFormulier=1) then
                                    %>
                                    <!-- #include file='FormulierStart.inc' -->
                                    <%
                               end if
                               if (iRecurringItems=0) then
                                    if oPublisher.call ("Get_GetWebsiteParameterValue", "IsGallery", iPageID, 0, 0) then
                                        bGlly_boxgrid = 1
                                        bGlly_Zoom = 1
                                        bGlly_Download = 0
                                        %>
                                        <!-- #include file='Gallery.inc' -->
                                        <%
                                    elseif oPublisher.call("Get_GetWebsiteParameterValue","IsSitemap",iPageID,0,0) then
                                        %>
                                        <!-- #include file='SiteMap.inc' -->
                                        <%
                                    end if
                               end if

                               For iCounter = 1 to iRecurringItems
                                   if ((iSS_BannerID) and (iCounter = 2)) then
                                        iSS_DefaultWidth= 465
                                        iSS_DefaultHeight=250
                                        sSS_DefaultTitle = sDefaultTitle 

                                        %>
                                        <!-- #include file='SlideShow.inc' -->
                                        <%
                                        sSS_DefaultTitle = ""
                                   end if

                                   iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                                   ArrayChildItemsID(iCounter) = iChildPageID

                                   if (oPublisher.call("Get_GetWebsiteParameterValue","IsSildeShow",iChildPageID,0,0) = 1) then
                                        if (bDivForm = 1) then
                                            bDivForm=0
                                            %>
                                            </div>
                                            <%
                                        end if
                                        iSS_DefaultWidth= 465
                                        iSS_DefaultHeight=250
                                        %>
                                        <!-- #include file='SlideShow.inc' -->
                                        <%
                                   elseif (oPublisher.call("Get_ASPTemplate", iChildPageID) = cnstFrmASP) then
                                        if (bDivForm = 0) then
                                            bDivForm=1
                                            %>
                                            <div class="formdiv">
                                            <%
                                        end if
                                        %>
                                        <!-- #include file='formulier_input.inc' -->
                                        <%
                                   elseif (oPublisher.call("Get_GetWebsiteParameterValue","GoogleMap",iChildPageID,0,0) = 1) then
                                        %>
                                        <!-- #include file='GoogleMap.inc' -->
                                        <%
                                   else
                                        if (bDivForm = 1) then
                                            bDivForm=0
                                            %>
                                            </div>
                                            <%
                                        end if
                                        iSidePanel = oPublisher.call ("Get_GetWebsiteParameterValue", "SidePanel", iChildPageID, 0, 0) 
                                        if iSidePanel = 0 then
                                            iShowHide = oPublisher.call ("Get_GetWebsiteParameterValue", "NewShowHide", iChildPageID, 0, 0) 
                                            if ((iShowHide = 1) and (bSHOut <> 1)) then
                                                %>
                                                <a href="javascript:{}" onclick="javascript:toggleAllShowHide('ToggleSH')"  target="_self" id="ToggleSH">Alles tonen</a>
                                                <%
                                                bSHOut=1
                                            end if
                                            Do While (cInt(iLevel) > cInt(iShowHide)) 
                                                %></div></div><div class="clear endlev"></div><%
                                                iLevel = iLevel - 1
                                            Loop
                                            if (iShowHide = 0) then
                                                if bStart then 
                                                    %></div><% 
                                                else 
                                                    bStart=true
                                                end if
                                            end if
                                            if (iShowHide > 0) then
                                                if (bSH_Init=0) then bSH_Init=1
                                                if cInt(iLevel) = cInt(iShowHide) then
                                                    %></div></div><%
                                                end if
                                                for i=1 to  (cint(iShowHide) - cInt(iLevel) - 1)
                                                    iSH = iSH + 1
                                                        %>
                                                        <div class="itemgroup w100 showhide<% if (not bShH) then %> open<% end if %>">
                                                        <a href="javascript:{}" onclick="javascript:ReverseContentDisplay('SH<%=iSH%>','SSH<%=iSH%>','<%=i%>')" target="_self" id="SSH<%=iSH%>"
                                                            <%if (bShH="0") then %>style="background-image: url(images/zoomminus.gif);"<% end if%>>
                                                            <h2>#ERROR - Level ontbreekt! -</h2>
                                                        </a>
                                                        </div>
                                                        <div class="showhidegroup" id="SH<%=iSH%>" level="<%=i%>">
                                                            <div class="itemgroup w100">
                                                        <% 
                                                    bShH=true
                                                next
                                                iSH = iSH + 1
                                                %>
                                                <div class="itemgroup w100 showhide<% if (not bShH) then %> open<% end if %>">
                                                    <a href="javascript:{}" onclick="javascript:ReverseContentDisplay('SH<%=iSH%>','SSH<%=iSH%>','<%=iShowHide%>')" target="_self" id="SSH<%=iSH%>"
                                                        <%if (bShH="0") then %>style="background-image: url(images/zoomminus.gif);"<% end if
                                            
                                                        sSHTitle=oPublisher.call ("Get_GetWebsiteParameterValue", "TitleShowHide", iChildPageID, 0, 0)
                                                        if (sSHTitle="") then sSHTitle=oPublisher.call ("Get_PrintPageItem", iChildPageID, "Title")
                                                            %>>
                                                        <h2><%=sSHTitle %></h2>
                                                    </a>
                                                </div>
                                                <div class="showhidegroup" id="SH<%=iSH%>" level="<%=iShowHide%>"<% 
                                                    if (bShH) then %>style="display:none;"<% end if %>><%
                                                bShH=true
                                            end if
                                            %>
                                                <div class="itemgroup w100">
                                            <%
                                            bItGrp=true
                                            bEmpty = (oPublisher.call("Get_PrintPageItem", iChildPageID, "Text")="") and (oPublisher.call("Get_GetWebsiteParameterValue", "TitleTab", iChildPageID, 0, 0)=0) and (iCounter>1)
                                            %>
                                                <div class="tabitemtitle <% if (iCounter=1) then%> first<% end if %> <% if (bEmpty) then%> empty<% end if %>">
                                                    <% if ((iCounter=1) or (oPublisher.call("Get_GetWebsiteParameterValue", "TitleTab", iChildPageID, 0, 0)=1)) then %>                                    
                                                        <h2><% oPublisher.call "Msg_PrintPageItem", iChildPageID, "Title" %></h2>
                                                    <% end if %>                                    
                                                    <% 
                                                        oPublisher.call "Msg_PrintPageItem", iChildPageID, "Text"
                                                        iIsGallary = oPublisher.call ("Get_GetWebsiteParameterValue", "IsGallery", iChildPageID, 0, 0)
                                                        if (iIsGallary = "1") then
                                                            bGlly_boxgrid = 1
                                                            bGlly_Zoom = 1
                                                            bGlly_Download = 0
                                                            %>
                                                            <!-- #include file='Gallery.inc' -->
                                                            <%
                                                        elseif (iIsGallary = "2") then
                                                            %>
                                                            <!-- #include file='FileGallery.inc' -->
                                                            <%
                                                        end if
                                                    %>
                                                </div>
                                            <% 
                                        Else
                                            iSpCnt = iSpCnt + 1
                                            ReDim Preserve ArraySpID(iSPCnt)  
                                            ArraySpID(iSPCnt) = iChildPageID
                                        End if
                                        iLevel = iShowHide
                                    end if
                               Next
                               if (bDivForm = 1) then
                                    bDivForm=0
                                    %>
                                    </div>
                                    <%
                               end if
                               if (bShH = true) then
                                 Do While (cInt(iLevel) >= 1)
                                    %></div></div><%
                                    iLevel = iLevel - 1
                                 Loop
                                 bShH=false
                               end if
                               if bItGrp then%></div><%end if
                           %>
                        </div>
                        <div class="clear"></div>
                        <%
                        if (iFormulier=1) then
                            %>
                            <!-- #include file='FormulierStop.inc' -->
                            <%
                        end if
                        %>
                     </div>

                     <% 
                        '------------------------------------------------------------------------
                        'SITECONTAINER
                        '------------------------------------------------------------------------ 
                        if (bNoSitePanel = 0) then
                         %>
                         <!--ZOOMSTOP-->
                         <!--ZOOMSTOPFOLLOW--> 
                         <div class="siteContainer">
                            <%
                            For iCounter = 1 to iSpCnt
                                iChildPageID=ArraySpID(iCounter) 
                                %>
                                <div class="siteitemgroup w100">
                                <%
                                    %>
                                    <div class="tabitemtitle">
                                        <!-- #include file='PageItem.inc' -->
                                    </div>
                                </div>
                                <%
                            Next
                            %>
                         </div>
                         <!--ZOOMRESTARTFOLLOW-->
                         <!--ZOOMRESTART-->
                         <div class="clear"></div>
                      </div>
                     <% end if %>
                  <div class="clear"></div>
               </div>
            </div>
            <div class="clear"></div>
         </div>
      </div>

   </div>
   </div>
    <!-- #include file='BodyBottom.inc' -->
</body>
<!-- #include file='PageEnd.inc' -->