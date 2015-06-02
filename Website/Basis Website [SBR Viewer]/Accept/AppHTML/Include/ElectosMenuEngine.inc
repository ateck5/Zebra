<%
' The ElectosMenuEngine class contains the logic to loop through the site structure and call the functions in ElectosMenuActuator. This code should not be changed.
Class ElectosMenuEngine
    Private oElectosMenu , oPublisher, oCache, sPreFix
    Public bCache, bByName

    Private Sub Class_Initialize
        bCache = False
    End Sub

    Private Sub Class_Terminate
        Set oCache = Nothing
        Set oPublisher = Nothing
        Set oElectosMenu = Nothing
    End Sub

    Public Sub SetElectosMenu (oObject)
        Set oElectosMenu  = oObject
    End Sub

    Public Sub SetPublisher(oObject)
        Set oPublisher = oObject
        If (oPublisher.call ("Get_GetWebsiteParameterValue", "CacheMenu") = 1) Then
            bCache = true
        End If
    End Sub	

    Private Sub MenuTreeBrowser(sObjectName, iID, iParentID, iSelectedID, iMaxLevel, sTypeConstrain, iLevel)
        dim sUrl, sName, sDocType, sType, sChildType, sTarget
        dim iLocalLevel, iCount, iChilds, iChildID, iProdGrp
        dim bVisible, bSelected, bChilds

        iLocalLevel = iLevel

        bVisible = (oPublisher.call ("GET_Visible", iID)=1)
        sType = oPublisher.call("GET_ObjectType", iID)

        iChildID = oPublisher.call ("GET_NextChildPageID", iId, 0)

        If (bVisible) Then
            If(iLocalLevel > 0) then
                sName           = Replace(oPublisher.call ("GET_PageName", iID), "'", "'")
                sDocType        = oPublisher.call ("GET_ObjectType", iID)
                If(int(iID) = int(iSelectedID)) then bSelected = True Else bSelected = False

                If (sDocType = "LNK") Then
                    sTarget = oPublisher.Call ("Get_ObjectTarget", iID, "")
                    If (sTarget = "0") then
                        sUrl = "javascript:OpenWin("""& oPublisher.call ("GET_ObjectUrl", iID) & """);"	
                    Else
                        sUrl = "javascript:window.top.location.href="""& oPublisher.call ("GET_ObjectUrl", iID) & """;"	
                    End If
                Else
                    sUrl = oPublisher.Call ("Get_PageURL", iID)
                End If

                oElectosMenu.MenuPrintItem sObjectName, iID, iParentID, sName, sURL, iLocalLevel, bSelected, (iChildID <> "-1"), sDocType
            End If

            iLocalLevel = iLocalLevel + 1
            If(iMaxLevel => iLocalLevel) or (iMaxLevel = 0) then

                Do While (iChildID <> "-1")
                    sChildType = oPublisher.call("GET_ObjectType", iChildID)

                    If(sChildType = sTypeConstrain) or (sTypeConstrain = "") then
                        'added for webshop
                        If (sChildType = "WSC") then
                            iProdGrp = oPublisher.call ("GET_ProductGroupID", iChildID)
                            WebshopTreeBrowser sObjectName, iChildID, iID, iSelectedID, iMaxLevel, sTypeConstrain, iLocalLevel, iProdGrp, iChildID
                        Else
                            'original function
                            MenuTreeBrowser sObjectName, iChildID, iID, iSelectedID, iMaxLevel, sTypeConstrain, iLocalLevel
                        End If
                    End If

                    'get next child if available
                    iChildID = oPublisher.call ("GET_NextChildPageID", iID, iChildID)
                Loop 		
            End If
        End If
    End Sub

    Public Sub WebshopTreeBrowser(sObjectName, iID, iParentID, iSelectedID, iMaxLevel, sTypeConstrain, iLevel, iProductGroup, iCatalog)
        Dim iLocalLevel, iChilds, iCount, iChildID, iMenuChildID, iMenuParentID
        Dim bChilds, bSelected, bVisible, bVisibleInWebshop
        Dim sName, sUrl, sDocType

        sDocType = "WSC"		
        iLocalLevel = iLevel

        bVisible = (oPublisher.call ("GET_Visible", iCatalog)=1)

        If (bVisible) Then
            bVisibleInWebshop = (oPublisher.call ("GET_ProductGroupVisible", iProductGroup)=1)

            If (bVisibleInWebshop) Then
                iChildID = oPublisher.call ("GET_NextChildProductGroupID", iProductGroup, 0)				

                If (oPublisher.call ("GET_ProductGroupID", iID) = iProductGroup) then   'If the ProductgroupiD is the catalog ID, get the catalogname instead of the productgroupname.
                    sName = Replace(oPublisher.call ("GET_PageName", iID), "'", "'")
                Else
                    sName = Replace(oPublisher.call ("GET_ProductGroupName", iProductGroup), "'", "'")			
                End If

                If(int(iID) = int(iSelectedID)) then bSelected = True Else bSelected = False

                sUrl = oPublisher.Call ("Get_PageURL", iCatalog, iProductGroup)	

                oElectosMenu.MenuPrintItem sObjectName, iID, iParentID, sName, sURL, iLocalLevel, bSelected, (iChildID <> "-1"), sDocType    				

                iLocalLevel = iLocalLevel + 1
                If(iMaxLevel => iLocalLevel) or (iMaxLevel = 0) then

                    Do While (iChildID <> "-1")
                        iMenuChildID = iCatalog & "9" & iChildID & "999"					
                        iMenuParentID = iCatalog & "9" & iProductGroup & "999"
                        WebshopTreeBrowser sObjectName, iMenuChildID, iID, iSelectedID, iMaxLevel, sTypeConstrain, iLocalLevel, iChildID, iCatalog		

                        'get next child if available
                        iChildID = oPublisher.call ("GET_NextChildProductGroupID", iProductGroup, iChildID)
                    Loop
                End If
            End if		
        End If 	
    End Sub

    Public Sub MenuCreateTree (sObjectName, iStartID, iSelectedID, iMaxLevel, sTypeConstrain)
        Dim sLastUpdated

        If ((sTypeConstrain <> "DOC") and (sTypeConstrain <> "CAT") and (sTypeConstrain <> "LNK")) Then sTypeConstrain = ""

        '   The prefix should contain all details that make the menu unique
        sPreFix = sObjectname & "_menu_" & iStartID & "_" & iSelectedID & "_" & iMaxLevel & "_" & sTypeConstrain & "_" & Request.ServerVariables ("HTTP_HOST") & "_" & Request.ServerVariables ("HTTPS") & "_"

        '   If the user is logged in we cache on the session object because the menu is specific for this user
        If (iMemberId <> "" ) and (iMemberId <> "0") and (sSessionKey <> "") Then
            Set oCache = Session
        Else
            Set oCache = Application
        End If

        '   Check the cache setting and make sure that we do not cache when previewing
        If (bCache and Not(Request("PreviewKey") <> "")) Then
            
            ' The code in this procedure is executed before the items are printed..
            sLastUpdated = oPublisher.call ("Get_LastUpdated")

            '   Check if we need to regenerate
            If (sLastUpdated <> oCache(sPreFix + "LastUpdated") or oCache(sPreFix + "LastUpdated") = "" or sLastUpdated = "" or sLastUpdated = "0") Then
                oCache(sPreFix + "LastUpdated") = sLastUpdated
                oCache(sPreFix + "MenuHTML") = ""

                oElectosMenu.MenuPrintHeader sObjectName
                MenuTreeBrowser sObjectName, iStartID, 0, iSelectedID, iMaxLevel, sTypeConstrain, 0
                oElectosMenu.MenuPrintFooter sObjectName
            End If

            '   Write out the cache
            Response.write(oCache(sPreFix + "MenuHTML"))
        Else
            oCache(sPreFix + "MenuHTML") = ""

            oElectosMenu.MenuPrintHeader sObjectName
            MenuTreeBrowser sObjectName, iStartID, 0, iSelectedID, iMaxLevel, sTypeConstrain, 0
            oElectosMenu.MenuPrintFooter sObjectName

            Response.write(oCache(sPreFix + "MenuHTML"))

            '   Make sure that we clear the cache (since we decided not to cache)
            oCache(sPreFix + "LastUpdated") = ""
            oCache(sPreFix + "MenuHTML") = ""
        End If
    End Sub

    Public Sub AddToMenu (sCode)
        oCache(sPreFix + "MenuHTML") = oCache(sPreFix + "MenuHTML") & sCode
    End Sub
End Class
%>