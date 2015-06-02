<form id="AcceptForm"
	<% 
		sFrm_Action  = oPublisher.call("Get_PrintPageItem", iPageID, "Action")
		sFrm_Enctype = oPublisher.call("Get_PrintPageItem", iPageID, "Enctype")
		sFrm_Method  = oPublisher.call("Get_PrintPageItem", iPageID, "Method")
		sFrm_Name    = oPublisher.call("Get_PrintPageItem", iPageID, "Name")
		sFrm_Target  = oPublisher.call("Get_PrintPageItem", iPageID, "Target")

		if (sFrm_Action  <> "") then %> action="<%=sFrm_Action%>"<%end if
		if (sFrm_Enctype <> "") then %> enctype="<%=sFrm_Enctype%>"<%end if
		if (sFrm_Method  <> "") then %> method="<%=sFrm_Method%>"<%end if
		if (sFrm_Name    <> "") then %> name="<%=sFrm_Name %>"<%end if
		if (sFrm_Target  <> "") then %> target="<%=sFrm_Target%>"<%end if

	%>
>