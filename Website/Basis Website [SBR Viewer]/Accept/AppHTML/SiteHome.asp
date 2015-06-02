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
                Dim  iSildeShow, iChildItems, iTotPag
                iChildItems = oPublisher.call ("GET_CountChildren", iPageID)
             %>
            <div id="featuresitepanel">
                <%
                    For iCounter = 1 to iChildItems
                        iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                        iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iChildPageID, 0, 0) 
                        if (iSildeShow=1) then
                            iSS_DefaultWidth= 297
                            iSS_DefaultHeight= 1580
                            iSS_CorrHeight=-70
                            %>
                            <!--ZOOMSTOP-->
                            <!--ZOOMSTOPFOLLOW-->
                            <!-- #include file='SlideShow.inc' -->
                            <!--ZOOMRESTARTFOLLOW-->
                            <!--ZOOMRESTART-->
                            <%
                        else
                            iTotPag = iTotPag + 1
                        end if
                    Next
                    %>
             </div>
            
            <div id="contentsiteContainer">
               <div class="collumnblock">
                  <%
                  Dim iRecurringItems, iFileId, iNewblock, iRow, iItem
                                
                  iRecurringItems = oPublisher.call ("GET_CountChildren", iPageID)
                
                  iRow = 0
                
                  For iCounter = 1 to iRecurringItems
                     iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
                     iSildeShow = oPublisher.call ("Get_GetWebsiteParameterValue", "IsSildeShow", iChildPageID, 0, 0) 
                     if (iSildeShow = 0) then
                         iItem = iItem + 1
                         if (iItem = 2 or iItem = 3) Then
                            iRow = 1
                            %>
                            </div>
                            <div class="collumnblock rowblock<%=iRow %>">
                            <%
                         End If
                    
                            %>
                            <div class="itemgroup<%
                                if (iItem>2) then 
                                    if (iTotPag=4) then 
                                        %> w50<%
                                        elseif (iTotPag=3) then 
                                            %> w100x <% 
                                        else 
                                            %> w33 <% 
                                        end if
                                else
                                    %> w100<%
                                end if%><%if (iItem=1) then%> first<%end if %> " style="min-height: <%if (iItem>2) then%>215<% else %>250<% end if%>px;">
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