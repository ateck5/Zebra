<%	'**************************************************************************
	'* ASP FormMail - Ophalen Licentie                                        *
	'*                                                                        *
	'* Aangepast voor het ophalen van licenties - ACCEPT                      *
	'*                                                                        *
    '**************************************************************************
    ' 
    ' var subject is bekend in FromMail 
    '
    ' DEMO Licentie gegevens
    ' _licentieOption  ==> _licentieOptions {vrij|start|basis|rap|extra|plus|totaal}
    ' _branchOption    ==> _branchOptions   {mkb|acc}
    '_pakketselectie   == verzameling van checkboxen
    Dim licentie, licenties, licentieOptions, branch, branches, branchOptions, strReadLineText, strReadLineTextArr, sPak

    licentieOptions = Split(Request.Form("_licentieOptions"), "|")
    if UBound(licentieOptions) > 0 then
        branches = Split(Request.Form("_branchOption"), "|")
        branchOptions = Split(Request.Form("_branchOptions"), "|")
        licenties       = Split(Request.Form("_licentieOption"), "|")

        teller = 0
	    for each name in branchOptions
		    If name = Request.Form("_branchOption") then  
			    fromOption = teller
			    exit for
		    end if
		    teller = teller + 1
	    next
	    branch = Trim(branches(fromOption))

        teller = 0
	    for each name in licentieOptions
		    If name = Request.Form("_licentieOption") then  
			    fromOption = teller
			    exit for
		    end if
		    teller = teller + 1
	    next
	    licentie = branch & left(licenties(fromOption),3)
        ' => MKBSTA, MKBBAS, MKBEXT, MKBPL, MKBTOT, MKBRAP
        ' => ACCSTA, ACCBAS, ACCEXT, ACCPL, ACCTOT, ACCRAP
   
       ' Uitlezen Licentie.txt
        Dim strFileName, objFSO, objTextFile

        strFileName = "../downloads/licentie.txt"
        Set objFSO = Server.CreateObject("Scripting.FileSystemObject")
        Set objTextFile = objFSO.OpenTextFile(Server.MapPath(strFileName))

        Do While Not objTextFile.AtEndOfStream
            strReadLineText = objTextFile.ReadLine
            If strReadLineText<>"" then
                If Instr(strReadLineText,",")>0 then
                    strReadLineTextArr=split(strReadLineText,",")
                    sPak=split(strReadLineTextArr(0),"=")
                    if (uCase(licentie) = sPak(0)) then
                        'voeg toe aan subject
                        subject = subject & VbCrLf & vbCrLf & sPak(0)
                        subject = subject & VbCrLf & "Serienummer : " & strReadLineTextArr(3)
                        subject = subject & VbCrLf & "Licentiecode: " & strReadLineTextArr(1)
                        subject = subject & VbCrLf & "Geldig t/m  : " & strReadLineTextArr(2)
                    end if
                end if 
            end if
        Loop

        ' Close and release file references
        objTextFile.Close
        Set objTextFile = Nothing
        Set objFSO = Nothing
    end if 

%>
