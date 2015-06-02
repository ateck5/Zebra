<% 
	Dim AccDoUse
	sInput_type = oPublisher.call("Get_PrintPageItem", iChildPageID, "type")
	sInput_name		 = oPublisher.call("Get_PrintPageItem", iChildPageID, "name")
	if (bInSameRow=0) then 
		if (uCase(sInput_type) = "HIDDEN") then
			%> <div class="formrowhidden"> <%	
		else
			%> <div class="formrow"> <%	
		end if
	end if 

	if ((sNameChbGrp_required <> "") and (sNameChbGrp_required <> sInput_name)) then
		sNameChbGrp_required=""
		%></div> <%
	end if

	bInput_required=0
	if (uCase(sInput_type) = "CHECKBOX") then 
		bCheckboxGrp_required = oPublisher.call("Get_PrintPageItem", iChildPageID, "required")
		if ((bCheckboxGrp_required = "1") and (sNameChbGrp_required = "")) then
			%>
				<div id="<%=sInput_name%>" class="requiredchkgrp">
				<span  id="_err<%=sInput_name%>" class="error" style="display: none; position:absolute; top:-35px; right:0px; ">U moet hier een keuze maken<em/></span>
			<%
		end if
		%><div class="checkboxitem"><%
	elseif (uCase(sInput_type) = "SUBMIT") then 
	elseif (uCase(sInput_type) = "RESET") then 
	else
		bInput_required = oPublisher.call("Get_PrintPageItem", iChildPageID, "required")
	end if
	%>
	<label class="formlabel<%if(uCase(sInput_type)="CHECKBOX") then%>checkbox<% end if %>">
		<%oPublisher.call "Msg_PrintPageItem", iChildPageID, "label" %>
	</label>
	<%	if (uCase(sInput_type) <> "CHECKBOX") and (uCase(sInput_type) <> "TEXTAREA")  then 
			%>
			<span class="required"><% if (bInput_required = "1") then %>*<% end if %></span>
			<% 
		end if
		If (uCase(sInput_type) = "SELECT") then
			%> <select <%
		elseIf (uCase(sInput_type) = "TEXTAREA") then
			%> <textarea <%
		else
			%> <input <%
		end if

		iInputSameRow    = oPublisher.call("Get_PrintPageItem", iChildPageID, "SameRow")
		sInput_value	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "value")
		bInput_checked	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "checked")
		sInput_class   	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "class")
		iInput_size	     = oPublisher.call("Get_PrintPageItem", iChildPageID, "size")
		If (AccDoUse = "1") then
			bInput_disabled	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "disabled")
			bInput_readonly	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "readonly")
			iInput_maxlength = oPublisher.call("Get_PrintPageItem", iChildPageID, "maxlength")
			iInput_tabindex	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "tabindex")
			sInput_accept	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "accept")
			sInput_accesskey = oPublisher.call("Get_PrintPageItem", iChildPageID, "accesskey")
			sInput_align	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "align")
			iInput_border	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "border")
			iInput_height	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "height")
			iInput_hspace	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "hspace")
			bInput_ismap	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "ismap")
			sInput_src	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "src")
			sInput_alt	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "alt")
			sInput_usemap	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "usemap")
			iInput_vspace	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "vspace")
			iInput_width	 = oPublisher.call("Get_PrintPageItem", iChildPageID, "width")
		end if

		if (sInput_class = "") then sInput_class="forminput" 

		if (sInput_name <> "") then 
			%> name="<%=sInput_name%>"<% 
			if (bCheckboxGrp_required = "1") then sNameChbGrp_required = sInput_name
		end if

		If (uCase(sInput_type) <> "SELECT") then
			If (uCase(sInput_type) <> "TEXTAREA") then
				if (sInput_type <> "") then %> type="<%=sInput_type%>"<% end if
				if (sInput_value <> "") then %> value="<%=sInput_value%>"<% end if
			end if
			if (bInput_checked = "1") then %> CHECKED<% end if
			If (AccDoUse = "1") then	
				if (bInput_disabled=  "1") then %> DISABLED<% end if
				if (bInput_readonly = "1") then %> READONLY<% end if
				if (iInput_maxlength <> "") then %> maxlength="<%=iInput_maxlength%>"<% end if
				if (iInput_tabindex <> "") then %> tabindex="<%=iInput_tabindex%>"<% end if
				if (sInput_accept <> "") then %> accept="<%=sInput_accept%>"<% end if
				if (sInput_accesskey <> "") then %> accesskey="<%=sInput_accesskey%>"<% end if
				if (sInput_align <> "") then %> align="<%=sInput_align%>"<% end if
				if (iInput_border <> "") then %> border="<%=iInput_border%>"<% end if
				if (iInput_height <> "") then %> height="<%=iInput_height%>"<% end if
				if (iInput_hspace <> "") then %> hspace="<%=iInput_hspace%>"<% end if
				if (bInput_ismap = "1") then %> ismap="<%=bInput_ismap%>"<% end if
				if (sInput_src <> "") then %> src="<%=sInput_src%>"<% end if
				if (sInput_alt <> "") then %> alt="<%=sInput_alt%>"<% end if
				if (sInput_usemap <> "") then %> usemap="<%=sInput_usemap%>"<% end if
				if (iInput_vspace <> "") then %> vspace="<%=iInput_vspace%>"<% end if
				if (iInput_width <> "") then %> width="<%=iInput_width%>"<% end if	
			end if
		end if
		if (bInput_required=  "1") then %> REQUIRED="REQUIRED"<% end if
		


		If (uCase(sInput_type) <> "TEXTAREA") then
			if (iInput_size <> "") then %> style="width:<%=iInput_size%>px;"<% end if
		end if
		if ((uCase(sInput_type) = "SUBMIT") or (uCase(sInput_type) = "RESET")) then sInput_class = sInput_class & " formbutton"
		%> class="<%=sInput_class%>"<%
	
	If (uCase(sInput_type) = "SELECT") then
		%> > <% 
		sInput_valueItems = split(sInput_value,"|")
		
		for iSelectOption = 0 to ubound(sInput_valueItems)
			%><option value="<%=sInput_valueItems(iSelectOption)%>"> <%=sInput_valueItems(iSelectOption)%> </option><%
		next		
		%> </select> <%
		%> <input  name="<%=sInput_name%>$" type="HIDDEN" value="<%=sInput_value%>" class="forminput"/> <%
	elseIf (uCase(sInput_type) = "TEXTAREA") then
		if (iInput_size = "auto") then
			%> placeholder="<%=sInput_value%>" onkeyup="textAreaAdjust(this)" style="overflow:hidden"<%
		else		
			iInput_size=split(iInput_size,"|")
			if (ubound(iInput_size) > 0) then
				%> cols="<%=iInput_size(0)%>"<% 
				%> rows="<%=iInput_size(1)%>"<% 
			end if
		end if
		%> ></textarea> <%
	else
		%>/><% 
	end if
	if (uCase(sInput_type) = "CHECKBOX") then 
		%></div><% 
	end if
	if (iInputSameRow="") then 
		bInSameRow=0
		%></div><% 
	else 
		bInSameRow=1
	end if 
%>