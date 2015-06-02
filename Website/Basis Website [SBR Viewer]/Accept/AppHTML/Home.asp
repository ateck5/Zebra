<!-- #include file='Include/ElectosPageBegin.inc' -->
<!-- #include file='Include/ElectosRestriction.inc' -->
<!-- #include file='ElectosDates.inc' -->
<!-- #include file='PageTop.inc' -->

</head>

<body>
   <div id="bghome">
      <!-- #include file='FormulierMelding.inc' -->
      <div id="bgpattern">
         <!-- #include file='BodyTop.inc' -->
         <!-- #include file='BreadCrumb.inc' -->
         <div id="pagepanel">
            <%
                Dim iNoBanner
                iNoBanner = oPublisher.call ("Get_GetWebsiteParameterValue", "NoBanner", iPageID, 0, 0)
             %>
            <div id="featurepanel<% if (iNoBanner = "1") then %>_nobanner<% end if %>">
                <%
                    Dim  iSildeShow, iChildItems, iTotPag, iTopPag
                    
                    iChildItems = oPublisher.call ("GET_CountChildren", iPageID)
                    For iCounter = 1 to iChildItems
                        iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                        iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iChildPageID, 0, 0) 
                        if ((iSildeShow=1) and (iNoBanner = "0")) then
                            iSS_DefaultWidth= 930
                            iSS_DefaultHeight=310
                            iSS_DefaultRibbon=1
                            iSS_CorrHeight=-10
                            %>
                            <!--ZOOMSTOP-->
                            <!--ZOOMSTOPFOLLOW-->
                            <!-- #include file='SlideShow.inc' -->
                            <!--ZOOMRESTARTFOLLOW-->
                            <!--ZOOMRESTART-->
                            <%
                        else
                            iTotPag = iTotPag + 1
                            if (iNewblock = 0) then
                                iNewblock = oPublisher.call ("Get_GetWebsiteParameterValue", "NewBlock", iChildPageID, 0, 0) 
                                if (iNewblock = 0) then iTopPag = iTopPag + 1
                            end if
                        end if
                    Next
                    %>
             </div>
            
            <div id="contentContainer">
               <div class="collumnblock">
                  <%
                  Dim iRecurringItems, iNewblock, iRow, iHeight, iTotHeight, iPag
                                
                  iRecurringItems = oPublisher.call ("GET_CountChildren", iPageID)
                  iRow = 0
                  iPag = 0
                
                  For iCounter = 1 to iRecurringItems
                     iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                     iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iChildPageID, 0, 0) 
                     if (iSildeShow = 0) then
                         iNewblock = oPublisher.call ("Get_GetWebsiteParameterValue", "NewBlock", iChildPageID, 0, 0) 
                         if (iNewblock = 1) Then
                            iRow = 1
                            %>
                            </div>
                            <div class="collumnblock rowblock<%=iRow %>">
                            <%
                         End If
                        
                         if (iRow = 0) then
                            if iTotPag <= 2 then iHeight = (sPageMinHeight-285) else iHeight = (sPageMinHeight-420)
                            if (iNoBanner = "1") then iHeight = iHeight + 310
                            iTotHeight = iHeight

                            %>
                            <div class="itemgroup<% if (iTopPag=1) then%> w100<% else %> w50<% end if %>" style="min-height:<%=iHeight%>px;">
                            <% 
                         Else
                            if (iTotHeight=0) then iHeight = (sPageMinHeight-285) else iHeight =  (sPageMinHeight-515) 
                            if (iNoBanner = "1") then iHeight = iHeight + 445
                            iPag = iPag + 1
                            %>
                            <div class="itemgroup w33 item<%=iPag %>" style="min-height: <%=iHeight %>px;">
                            <% 
                         End If
                            %>
                            <div class="itemtitle">
                                <!-- #include file='PageItem.inc' -->
                            </div>
                         </div>
                         <%
         
                        end if                
                     Next
                     %>
                  </div>
                  <div class="clear"></div>
               </div>
            </div>
         </div>
      </div>
   </div>
    <!-- #include file='BodyBottom.inc' -->
</body>
<!-- #include file='PageEnd.inc' -->