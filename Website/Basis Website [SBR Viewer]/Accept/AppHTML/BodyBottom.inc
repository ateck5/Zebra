<!--ZOOMSTOP-->
<!--ZOOMSTOPFOLLOW-->
<div id="footer">
    <div id="bg_bottom">
        <div id="bg_btmTitle">
            <div id="bg_Versie">
                <%=oPublisher.call("Get_GetWebsiteParameterValue", "CompanyTagline", iPageID="", iProductGroupId="", iProductId="") %>
            </div>
            <div id="bg_Title">
                <%=oPublisher.call("Get_GetWebsiteParameterValue", "Title", iPageID="", iProductGroupId="", iProductId="") %>
            </div>
        </div>
    </div>
    <div id="footerwrapper">
     <div id="footerblock">
       <%
          Dim iFootItems, ifCtr, iFoot, iFootPageID, ifFileID, ifP, ifChildItems, ifChildPageID, ifWidhtFP, ifNieuwsGrp

          iFootItems = oPublisher.call ("GET_CountChildren", 1)
          ifWidhtFP = "46"
          For ifCtr = 1 to iFootItems 
            iFootPageID = oPublisher.call ("GET_ChildPageID", 1, ifCtr)
            iFoot = oPublisher.call ("Get_GetWebsiteParameterValue", "FooterPanel ", iFootPageID, 0, 0)
            if (cBool(iFoot)) then 
              ifChildItems = oPublisher.call ("GET_CountChildren",iFootPageID)
              For ifP = 1 to ifChildItems 
                ifChildPageID = oPublisher.call ("GET_ChildPageID", iFootPageID, ifP) %> 
                <div class="itemgroup" style="width:<%if (ifP=1) then %><%=ifWidhtFP %>%;"><% else %> <%=(cInt((100-ifWidhtFP)/(ifChildItems-1)))%>%;"><% end if %> 
                  <div class="footitemtitle<% if (ifP=cInt(ifChildItems)) then %> last<% end if%>">
                      <% 
                          iChildPageID = ifChildPageID 
                          ifNieuwsGrp = oPublisher.call ("Get_GetWebsiteParameterValue", "IsNieuwsGroup ", ifChildPageID, 0, 0)
                          if (ifNieuwsGrp ="1") then
                          	    sTxtH2 = oPublisher.call("Get_PrintPageItem", iChildPageID, "Title")
                                sUrlH2 = oPublisher.call("Get_PrintPageItem", iChildPageID, "URL")
	                            if (sUrlH2 <> "") then %> <a href="<%=sUrlH2%>"><% end if %>
		                        <h2><%=sTxtH2 %>
		                        <%if sUrlH2<>"" then %>    &#187;<% end if %></h2>
                                <%if sUrlH2<>"" then %></a><% end if %>
                                <ul id="nieuws">
                                    <% 
                                        iPrtMnLabel = "Nieuws" 
                                        iPrtMnClass = "Nieuwslink"
                                        iPrtMnMax = 5
                                        bPrtMnNews=1
                                    %>
                                    <!-- #include file='PrintMenu.inc' -->
                                </ul>
                          <%      
                          else
                                %>
                                <!-- #include file='PageItem.inc' -->
                                <%
                          end if
                      %>
                  </div>
                </div> <% 
              Next
            end if
          Next
       %>
    </div>

     <div id="credits">
   		<div id="copyright">
   			<p><%=oPublisher.call("Get_GetWebsiteParameterValue", "Copyright", iPageID)%></p>
   		</div>
   		<div id="shortcuts">
            <ul>
                <% 
                    iPrtMnLabel = "FooterMenu" 
                    iPrtMnClass = "footlink"
                    iPrtMnMax = -1
                %>
                <!-- #include file='PrintMenu.inc' -->
            </ul>
        </div>
	        <div id="topofpage">
			<a href="javascript:void(0)" onclick="goto_top()">Terug naar boven</a>
	        </div>
   	</div>
   </div>
</div>

<% if (sAFWVersie <>"") then %>
    <script>
        //call after page loaded
        window.onload=InitPage(<%=bSH_Init%>,"@AFVersie@","@AFBuild@","@RPVersie@","@RPRevisie@","<%=sAFWVersie %>","<%=sAFWBuild %>","<%=sRAPVersie %>","<% if ((sRAPRevisie<>"0") and (sRAPRevisie<>"1")) then %> (<%=sRAPRevisie %>)<% end if%>");
    </script>
<% end if %>

<!--ZOOMRESTARTFOLLOW-->
<!--ZOOMRESTART-->