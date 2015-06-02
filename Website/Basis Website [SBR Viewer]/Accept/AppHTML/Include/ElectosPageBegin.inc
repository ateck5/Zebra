<%
	Option Explicit ' ASP command that forces you to declare variable before use. This is a a good programming practise.

  	Dim iPageID, iOriginalPageId, iMenuItemID, iHasItems, iPageLevel, iRestricted, iOrrigionalPageID, iProductGroupId, iProductId, sPageName ' Variables needed generic use
	Dim iSearchId, sSearchValue, lsSearchValue, iSearchPage, sError, iMaxPage, iResultCount, iResultItemID, iDisplayCount, iLevelPageID, sResultType, sResultName, sTemplate, sVariableName, sSortOn, iSortDirection, sSortImageSrc ' Variables needed for the search functionality
  	Dim iNumberOfItems, iCounter, iChildPageID, sChildASPTemplate   ' Variables needed for muli-item pages
	Dim sWebsiteName, iPageIdRef, sSessionKey, iMemberId, sASPTemplate, iErr, sMemberName 'Variables needed for login functionality
    Dim sCurrency, bVat 'Variable needed by the webshop	
	Dim iPos, sHttp, sEnterUrl, iURLCharCount, asPageIds, sPageIds, iGuestbookPage	
	Dim iCommentParentId, sAuthorName, sAuthorEmail, sAuthorURL, sAuthorIP, sComments, sCommentPageType, sWebsiteUrl, sCurrentPageUrl, sTestCaptcha 'variables needed for adding comments to guestbook
	Dim sGuestBookComments, iCommentID, sCommentAuthorURL, sCommentText, iNoOfCommentsPerPage, iTotalCommentsCount, sDisableSmileys, iCommentCount, iCntr, sRecaptchaEnabled 'variables needed for guestbook
	Dim aiCommentIDs

    ' Prevent the browser from caching the pages.
	Response.Expires = -1 
	
	'Clear the error queue
    oPublisher.call "MSG_ClearErrorReport"
    
    ' We start a session for this visit.
	sWebsiteName    = oPublisher.call("get_websitename")
	sSessionKey     = request.cookies(sWebsiteName)("skey")
	iMemberId       = request.cookies(sWebsiteName)("mid")

	'determine the enter url to be used for absolute pathing
	sEnterUrl = Request.ServerVariables("PATH_INFO")

	For iURLCharCount=1 to Len(sEnterUrl) step 1
		If (Mid(sEnterUrl,iURLCharCount, 1) = "/") Then
			iPos = iURLCharCount
		End If
		
		If (Mid(iURLCharCount, 1) = "?") Then
			Exit For
		End If
	Next	
	
	If Request.ServerVariables("HTTPS") = "off" Then 
		sHttp = "http://"
	Else
		sHttp = "https://" 
	End If	
	
  	sEnterUrl = (sHttp + Request.ServerVariables("HTTP_HOST") + (Left(Request.ServerVariables("PATH_INFO"), iPos)))
	
    'If the session cookie is not empty, we validate the session. If there is no valid session, we create one.
    If sSessionKey <> "" Then iErr = oPublisher.call ("get_SessionValidate", sSessionKey, iMemberId) 
    
    If sSessionKey = "" OR iErr <> 0 Then    
	    sSessionKey = oPublisher.call ("get_SessionCreate", Request.ServerVariables("REMOTE_ADDR"))
        iMemberId   = ""
        
        response.cookies(sWebsiteName)("skey") = sSessionKey
	    response.cookies(sWebsiteName)("mid")  = iMemberId
        response.cookies(sWebsiteName).path = "/"
	End If
    
    
  	' We need to determine the PageID we want to show with the PageID function
	' The PageID function is used to resolve a PageID to a PageID that can be used to print a page. 
	' The iOriginalPageId variable might be usefull to special menu systems in the templates.
	' This function is mainly used at the start of an ASP template to test the given pageId in the querystring and resolve it to a usable pageID.
  	iOriginalPageId = request("pageid")
	sPageName = Request("page")
	If (sPageName <> "") Then
		sPageIds = oPublisher.call ("GET_PageByName", sPageName)		
		asPageIds = split(sPageIds, " ")		
		
		If (UBound(asPageIds) >= 2) Then 
			iPageID = asPageIds(0)
			iProductGroupId = asPageIds(1)
			iProductId = asPageIds(2)
		End If	
		
		If ((UBound(asPageIds) < 2) or (iPageID = 0)) Then
			iPageID = oPublisher.call ("GET_PageIDByName", sPageName, "/", 1)
		End If 	
		
		iOriginalPageId = iPageID
	Else
		iPageID = oPublisher.call ("GET_PageID", iOriginalPageId)
		
		' We determine whether a specific Product Group and/or product has been selected. 
		iProductId      = Request("ProductId")
		iProductGroupId = Request("ProductGroupId")
	End If
	
	' We determine the the menu item (at the top of the screen) that is currently selected. This ID can be used to print the name of menu item.
	' LevelPageID returns the pageID of the parent of a given page at a given level in the site hierarchy.
  	iMenuItemID = oPublisher.call ("GET_LevelPageID", request("pageid"), 1)

    ' If no Product Group was given, we will check whether the current page is a catalog and determine the product group based on the page Id
    if (iProductGroupId = "") Then iProductGroupId = oPublisher.Call ("Get_ProductGroupId", iPageId) End If
	
    ' We also determine a couple of default settings used by the webshop
    sCurrency = oWebshopPublisher.call("Get_GetWebshopSetting", "Currency", "General")
    bVat = (oWebshopPublisher.call("Get_GetWebshopSetting", "IncludingVAT", "VAT") = "1")
    
	
	' The following function calls are introduced for the search functionality. If no search function is implemeneted on any paged that make use of this include
	' the code below can be omittedd, although this is not necessary and will not result in serious improvements in speed. 
	iSearchId     	 = request("SearchId")  
	sSearchValue  	 = request.Form("Search__Value")
	iSearchPage   	 = request("SearchPage")
	sSortOn          = request("SortOn")
	iSortDirection   = request("SortDirection")	
	
	If (iSortDirection="0" or iSortDirection="") then iSortDirection = "1"	' set the default sort direction

	If iSearchId="0" or iSearchId="" and sSearchValue <> "" Then ' If liSearchId eq 0 we need to initiate a search
	  ' SearchCreateID initiates a search and stores the result.
      iSearchId = oPublisher.call("get_SearchCreateId", sSearchValue)
      iSearchPage = 1 ' The initial value if a new search was initiated
    End if

    If int(iSearchId)<>0 and sSearchValue="" then
	  ' If a search has already been initiated but the searchvalue is not present anymore (for instance when the result was split over multiple pages) we retrieve the search value from the DB
	  ' SearchCurrentValue retrieves the search string of a previous search result.
      sSearchValue = oPublisher.call("get_SearchCurrentValue", iSearchId)
    end if
	
	' For guestbook get the guestbook page
	iGuestbookPage   = request("GuestbookPage")    
	If (iGuestbookPage = "0" or iGuestbookPage = "") Then iGuestbookPage = 1 End If
    
    'Backwards compatibility with misspelled variable
    iOrrigionalPageID = iOriginalPageId 
    lsSearchValue = sSearchValue 	    
%>