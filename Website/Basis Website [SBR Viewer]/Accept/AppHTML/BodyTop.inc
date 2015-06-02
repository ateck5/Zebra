<div id="wrapper">
  <div id="logo">
    <a href="<%=oPublisher.call("Get_PageURL", 1)%>"></a>
  </div>
  <div id="header">
    <!--ZOOMSTOP-->
    <div id="navigation">
        <ul id="navlink">
          <li Class="navlink end"></li>
            <% 
                iPrtMnLabel = "NavigateMenu" 
                iPrtMnClass = "navlink"
                iPrtMnMax = -1
            %>
            <!-- #include file='PrintMenu.inc' -->
          <li Class="navlink begin"></li>
        </ul>
        <div Class="clear"></div>
        <%
			  Dim oMenuObject

          Dim iMenuDepth
          iMenuDepth = cInt(oPublisher.Call("Get_GetWebsiteParameterValue","MenuDepth", iPageID, iProductGroupId, iProductId))

          Set oMenuObject = New ElectosMenu
          oMenuObject.SetPublisher(oPublisher)

          oMenuObject.MenuCreateTree "oMainMenu", 1, iPageID, iMenuDepth, "ALL"
          Set oMenuObject = Nothing
          %>
          <% If (oPublisher.call("Get_GetWebsiteParameterValue", "DisplaySearchBox", iPageID) = "1") Then  %>	         
            <div id="searchbutton">
                <form id="Msearch" action="MainSearch.asp" method="post">
		        <input name="MSearch__Value" id="MSearch__Value" value="Zoeken..." onFocus="if(this.value == 'Zoeken...') {this.value = '';}" onBlur="if (this.value == '') {this.value = 'Zoeken...';}" onKeyPress="return searchonenter(this,event)"></input>
                <input type="hidden" name="Structure" value="SEARCH" />
                <input type="hidden" name="SearchId" value="0" />
                <input type="hidden" name="pageid" value=""<%=iPageID%>" />
                <input type="hidden" name="sortdirection" value=""<%=iSortDirection%>" />
                <input type="hidden" name="sorton" value=""<%=sSortOn%>" />
                <a href="#" id="MSearch__button" class="button" onclick="javascript:StartSearch();"></a>
                <div class="clear"></div>
                </form>
            </div>
            <div Class="clear"></div>
        <% End If %>
    </div>
    <!--ZOOMRESTART-->
  </div>