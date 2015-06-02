<!-- #Include file='Include/ElectosMenuEngine.inc' -->
<%
' ElectosMenu is an include file that can be used to print special types of Menu's. This Class uses the ElectosMenuEngine Class to traverse the site hierarchy.

Class ElectosMenu
' SetPublisher needs to be called before MenuCreateTree to give the WebApp Publisher object to the instance
'   Parameters:
'   - oObject: The oPublisher object
'
'
' MenuCreateTree needs to be called from the template to Start the menu generation
'   Parameters:
'   - sObjectname: A name of the object wich can be used with the output (usable for creating more menu's @ 1 page)
'   - iStartID: The ID in the tree from wich the tree starts (1 = Whole tree)
'   - iSelectedID: The ID of a selected node (this node gets bSelected = true when calling MenuPrintITem)
'   - iMaxLevel: The Maximum of levels to be printed (iStartID level)
'   - sTypeConstrain: "CAT" shows only categories, "DOC" shows only documents and everything else shows everything
'
'
' The procedures MenuHeader, MenuPrintItem, MenuPrintFooter should be customized for the menu
'
' MenuPrintHeader is called @ the start of the menu, it can contain code that prints commands that initialize the menu.
'   Parameters:
'    - sObjectname: The name that is given with the MenuCreateTree function, could be used for creating more menu's at one page.
'
'
' MenuPrintItem is called for every menuitem of the menu. It needs to have the code that display's or generates one
' menu item.
'   Parameters:
'   - sObjectname: The name that is given with the MenuCreateTree function, could be used for creating more menu's at one page
'   - iID: The unique (nummeric) ID of the item
'   - iParentID: The ID of its Parent item
'   - sname: The name of the object
'   - sURL: The url that needs to be called to open the page (example: "normalmenu.asp?pageid=709")
'   - iLevel: The level of the item called from the start of the menu
'   - bSelected: Is true when the ID of the item is the same as the iSelectedID given with the MenuCreateTree
'   - bHasChilds: Is true when the item contains sub items
'   - sDocType: The type of the document (usually: "DOC" or "CAT")
'
'
' MenuPrintFooter is called after the items are displayed, it can contain code to initilize the menu engine or start it.
'   Parameters:
'   - sObjectname: The name that is given with the MenuCreateTree function, could be used for creating more menu's at one page.

    Dim iLastLevel, bStart, iMainLevelSelectedID

    Public Sub AddToMenu(sHTMLtoAdd)
        oElectosMenuEngine.AddToMenu sHTMLtoAdd
    End Sub

    '
    '   Called before the MenuPrintItem is called, initializes the bStart variable so MenuPrintItem
    '   will know it is called for the first time.
    '
    Public Sub MenuPrintHeader(sObjectname)
        bStart = true
    End Sub

    '
    '   Called for each menu item. Generates an <li elements for each menu item and opens and closes
    '   <ul elements for submenu's (if available).
    '
    Public Sub MenuPrintItem(sObjectname, iID, iParentID, sname, sURL, iLevel, bSelected, bHasChildren, sDocType)
        Dim iLoop, sClass

        '   Encode stuff in the url that can cause issues
        sURL = Replace(sURL," ","%20")
        sURL = Replace(sURL,"&amp;","&")
        sURL = Replace(sURL,"&","&amp;")
        sURL = Replace(sURL, Chr(34),Chr(39))
        
		' Accept - verberg menunaam
		if instr(ucase(sname), "[HIDDEN]") > 0 then 
			sname = ""
			sURL = "#"
		end if

        '   Close previous item
        If (iLevel <= iLastLevel and iLastLevel <> "") Then
            AddToMenu "</li>" + vbCrLf
        End If


        '   Close previous submenu's
        If (iLevel < iLastLevel) Then
            iLoop = iLastLevel

            Do While iLevel < iLoop

                AddToMenu "</ul>" + vbCrLf
                AddToMenu "</li>" + vbCrLf

                iLoop = iLoop - 1
            Loop
        End If


        '   Start a new level (if needed)
        If (iLevel > iLastLevel or iLastLevel = "") Then
            If (bStart) Then
                AddToMenu "<ul id=""nav"" class=""sf-menu"">"
            Else
                AddToMenu "<ul>"
            End If

        End If


        '   Print the item
        sClass = ""
        If (bSelected) Then sClass = "current"
        If (iMainLevelSelectedID = iID) Then sClass = sClass & " rootcurrent"

        AddToMenu "<li class=""" + sClass + """>"
        AddToMenu "<a href=""" + sURL + """>"
        AddToMenu sname
        AddToMenu "</a>"
        AddToMenu vbCrLf

        '   Update the global properties
        bStart = false
        iLastLevel = iLevel
    End Sub


    '
    '   Called after the MenuPrintItem method is called for each items. Properly closes all HTML
    '   elements.
    '
    Public Sub MenuPrintFooter(sObjectname)
        '   Properly close all tags untill we are back at level 1
        Do While 0 < iLastLevel

            AddToMenu "</li>" + vbCrLf
            AddToMenu "</ul>" + vbCrLf

            iLastLevel = iLastLevel - 1
        Loop
    End Sub





' - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
'         P L E A S E   D O   N O T   C H A N G E   T H E   F O L L O W I N G    C O D E
' - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Public oElectosMenuEngine

    Private Sub Class_Initialize()
        Set oElectosMenuEngine = new ElectosMenuEngine
        oElectosMenuEngine.SetElectosMenu(me)

    End Sub

    Public Sub SetPublisher(oObject)
        oElectosMenuEngine.SetPublisher(oObject)
    End Sub

    Public Sub MenuCreateTree (sObjectname, iStartID, iSelectedID, iMaxLevel, sTypeConstrain)
        '   Custom code for determining the main level selected id
        iMainLevelSelectedID = oPublisher.call("get_LevelPageID", iSelectedID, oPublisher.call("get_PageLevel", iStartID) + 1)
        If (iMainLevelSelectedID = 0) Then iMainLevelSelectedID = iSelectedID

        oElectosMenuEngine.MenuCreateTree sObjectName, iStartID, iSelectedID, iMaxLevel, sTypeConstrain
    End Sub

End Class
%>