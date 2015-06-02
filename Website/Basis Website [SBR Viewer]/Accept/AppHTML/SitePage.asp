<!-- #include file='Include/ElectosPageBegin.inc' -->
<!-- #include file='Include/ElectosRestriction.inc' -->
<!-- #include file='ElectosDates.inc' -->
<!-- #include file='PageTop.inc' -->
<script type='text/javascript' src='js/SlideShowHide.js'></script>

</head>

<body>
     <div id="bghome">
         <!-- #include file='FormulierMelding.inc' -->
         <div id="bgpattern">
            <!-- #include file='BodyTop.inc' -->
            <!-- #include file='BreadCrumb.inc' -->
            <div id="pagepanel">
               <div id="contentContainer">
                  <div class="mainContainer">
                     <!--ZOOMSTOP-->
                     <!--ZOOMSTOPFOLLOW-->
                     <div class="siteContainer leftsite">
                        <%
                        Dim iRecurringItems, iSidePanel, iShowHide, bShH, iSH, iTitleTab, sText, bSHOut
                        Dim iLevel,bItGrp, bStart, sSHTitle, i, bEmpty, iSildeShow, bFirstDone
                        Dim iSPCnt, iPageCnt
                        Dim ArrayChildItemsID()
                        Dim ArraySiteItemsID()
                           
                        iRecurringItems = oPublisher.call ("GET_CountChildren", iPageID)
                        For iCounter = 1 to iRecurringItems
                           iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                           
                           iSidePanel = oPublisher.call ("Get_GetWebsiteParameterValue", "SidePanel", iChildPageID, 0, 0) 
                           if iSidePanel = 0 then
                                iPageCnt = iPageCnt + 1
                                ReDim Preserve ArrayChildItemsID(iPageCnt)  
                                ArrayChildItemsID(iPageCnt) = iChildPageID
                           elseif iSidePanel = 1 then
                                iSPCnt = iSPCnt + 1
                                ReDim Preserve ArraySiteItemsID(iSPCnt)  
                                ArraySiteItemsID(iSPCnt) = iChildPageID
                           elseif iSidePanel = 2 then
                              %>
                              <div class="siteitemgroup w100">
                              <%
                                 %>
                                 <div class="tabitemtitle">
                                    <!-- #include file='PageItem.inc' -->
                                 </div>
                              </div>
                              <%
                           End If
                        Next
                        %>
                     </div>
                    <!--ZOOMRESTARTFOLLOW-->
                    <!--ZOOMRESTART-->
                     <div class= "sitedialog">
                        <div class="tabcollumnblock rightsite" style="min-height: <%=sPageMinHeight %>px;">
                           <%
                           iLevel = 0
                           bFirstDone = false
                           For iCounter = 1 to iPageCnt
                              iChildPageID = ArrayChildItemsID(iCounter)
                              iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iChildPageID, 0, 0) 
                              if (iSildeShow=1) then
                                iSS_DefaultWidth= 420
                                iSS_DefaultHeight=250
                                %>
                                <!-- #include file='SlideShow.inc' -->
                                <%
                              else
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
                                    if cInt(iLevel) = cInt(iShowHide) then
                                        %></div></div><%
                                    end if
                                    for i=1 to  (cint(iShowHide) - cInt(iLevel) - 1)
                                        iSH = iSH + 1
                                            %>
                                            <div class="itemgroup w100 showhide<% if (not bShH) then %> open<% end if %>">
                                            <a href="javascript:{}" onclick="javascript:ReverseContentDisplay('SH<%=iSH%>','SSH<%=iSH%>','<%=i%>')" target="_self" id="SSH<%=iSH%>"
                                                <%if (not bShH) then %>style="background-image: url(images/zoomminus.gif);"<% end if%>>
                                                <h2>#ERROR - Level ontbreekt! -</h2>
                                            </a>
                                            </div>
                                            <div class="showhidegroup" id="SH<%=iSH%>">
                                            <div class="itemgroup w100">
                                            <% 
                                        bShH=true
                                    next
                                    iSH = iSH + 1
                                    %>
                                    <div class="itemgroup w100 showhide<% if (not bShH) then %> open<% end if %>">
                                        <a href="javascript:{}" onclick="javascript:ReverseContentDisplay('SH<%=iSH%>','SSH<%=iSH%>','<%=iShowHide%>')" target="_self" id="SSH<%=iSH%>"
                                            <%if (not bShH) then %>style="background-image: url(images/zoomminus.gif);"<% end if
                                            
                                            sSHTitle=oPublisher.call ("Get_GetWebsiteParameterValue", "TitleShowHide", iChildPageID, 0, 0)
                                            if (sSHTitle="") then sSHTitle=oPublisher.call ("Get_PrintPageItem", iChildPageID, "Title")
                                                %>>
                                            <h2><%=sSHTitle %></h2>
                                        </a>
                                    </div>
                                    <div class="showhidegroup" id="SH<%=iSH%>"<% 
                                        if (bShH) then %>style="display:none;"<% end if %>><%
                                    bShH=true
                                end if
                                    %>
                                    <div class="itemgroup w100">
                                    <%
                                    bItGrp=true
                                    iTitleTab = oPublisher.call("Get_GetWebsiteParameterValue", "TitleTab", iChildPageID, 0, 0)
                                    sText = oPublisher.call("Get_PrintPageItem", iChildPageID, "Text")
                                    bEmpty = (sText="") and (iTitleTab=0) and (iCounter>1)
                                    %>
                                    <div class="tabitemtitle <%if (bFirstDone=false) then%> first<% end if %> <% if (bEmpty) then%> empty<% end if %>">
                                        <% if ((bFirstDone=false) or (iTitleTab=1)) then %>                                    
                                            <h2><% oPublisher.call "Msg_PrintPageItem", iChildPageID, "Title" %></h2>
                                        <% end if %>                                    
                                        <%=sText %>
                                    </div>
                                    <% 
                                    bFirstDone=true
                                  iLevel = iShowHide
                              end if
                           Next
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
                     </div>
                     <!--ZOOMSTOP-->
                     <!--ZOOMSTOPFOLLOW-->
                     <div class="siteContainer">
                        <%
                        For iCounter = 1 to iSPCnt
                           iChildPageID = ArraySiteItemsID(iCounter)
                            %>
                            <div class="siteitemgroup w100">
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