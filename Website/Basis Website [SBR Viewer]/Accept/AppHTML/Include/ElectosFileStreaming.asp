<%
Function BepaalModified(strFilePath)
   Dim Datum, Tijd, Week, sWeek, Maand, sMaand, fs

   set fs = Server.CreateObject("Scripting.FileSystemObject")
   set f = fs.GetFile(strFilePath)
   Datum = Trim(Left(f.DateLastModified,10))
   Tijd = Trim(Right(f.DateLastModified,8))
   set f = nothing
   set fs = nothing

   Week = weekday(Datum,vbSunday)
   Select Case Week   
   Case 1 
	sWeek = "Sun"
   Case 2
        sWeek = "Mon"
   Case 3
        sWeek = "Tue"
   Case 4
        sWeek = "Wed"
   Case 5
        sWeek = "Thu"
   Case 6
        sWeek = "Fri"
   Case Else
        sWeek = "Sat"
   End Select	

   Maand = Month(Datum)
   Select Case Maand
   Case 1
        sMaand = "Jan" 
   Case 2
        sMaand = "Feb" 
   Case 3
        sMaand = "Mar" 
   Case 4
        sMaand = "Apr" 
   Case 5
        sMaand = "May" 
   Case 6
        sMaand = "Jun" 
   Case 7 
        sMaand = "Jul" 
   Case 8
        sMaand = "Aug" 
   Case 9
        sMaand = "Sep" 
   Case 10
        sMaand = "Oct" 
   Case 11
        sMaand = "Nov" 
   Case Else
        sMaand = "Dec" 
   End Select
   
   Tijd = Right(("0" & Tijd),8)
   BepaalModified = sWeek & ", " & DAY(Datum) & " " & sMaand & " " & Right(Datum,4) & " " & Tijd & " GMT"
End Function

    Dim sFileName, sFilePath, iFileId, iRestricted
    Dim sWebsiteName, sSessionKey, iMemberId 
    Dim iSize, iChunk
    Dim sDate, sModified
    
    'Get the parameters from tthe querystring
    iFileId         = Request("FileId")		
    
    'Check whether there is a sessionkey and memberId assigned to this visitor
  	sWebsiteName    = oPublisher.call("get_websitename")
	sSessionKey     = request.cookies(sWebsiteName)("skey")
	iMemberId       = request.cookies(sWebsiteName)("mid")
    
    
    'Determine the Physical path to the file and the filename. We make a distinction between normal files and thumbnail files. If an empty string is returned, the file could not be
    'found or is nor accessible for the current logged in member
    if (Request("Thumbnail")) Then
        sFilePath = oPublisher.call("GET_FileThumbnailPath", iFileId, sSessionKey, iMemberId, Request("MaxWidth"), Request("MaxHeight"), Request("Style"))
    Else
        sFilePath = oPublisher.call("GET_FilePath", iFileId, sSessionKey, iMemberId)
    End If
    sFileName = mid(sFilePath,(InstrRev(sFilePath, "\",-1, 1))+1)   
	
	USER_AGENT = Request.ServerVariables("HTTP_USER_AGENT")
	IS_IE = InStr(USER_AGENT,"MSIE")

	If IS_IE then
		sFileName = Replace(sFileName," ","%20") 
	End If
    'Error handling. When sFilePath is empty the file could not be found or is restricted and the current memeber doesn't have access. To present the user with a custom error 
    'message, augment the lines below.
    if (sFilePath="") Then
        Response.write "File unavailable, you may not be authorized to access this file."
        Response.end
    End If
    
If (Instr(sFilePath,".jpg")) then
   sModified = BepaalModified(sFilePath)
   sDate = Request.ServerVariables("HTTP_IF_MODIFIED_SINCE") 
Else
   sModified = "unknown"
   sDate     = ""
end if

If (sDate = sModified) then
    Response.Status = 304
else     
    'Setup the Response stream  
    Set objStream = Server.CreateObject("ADODB.Stream")
    objStream.Open
    objStream.Type = 1 ' Binary
    objStream.LoadFromFile sFilePath
    iSize = objStream.Size  
    
    'Set the response characteristics
    Response.Buffer = true    
    Response.ContentType = oPublisher.call("Get_ContentType", iFileId)
    Response.AddHeader "Content-Disposition", "inline;filename=""" & sFileName & """"
    Response.AddHeader "Content-Length", iSize
    Response.AddHeader "Last-modified", sModified


    'Upload the file in chunck and check whether the browser is still contected
    iChunk = 1048576 '1MB
    For i = 1 To (iSize \ iChunk)
        If Not Response.IsClientConnected Then Exit For
        Response.BinaryWrite objStream.Read(iChunk)
        Response.Flush
    Next

    'Upload the last bit op de stream
    If (iSize Mod iChunk) > 0 Then
        If Response.IsClientConnected Then
            Response.BinaryWrite objStream.Read(iSize Mod iChunk)
            Response.Flush
        End If
    End If

    'The upload the file all in once, we would use the following line.
    'Response.BinaryWrite objStream.Read
    'Response.Flush
    
    'Close the stream and clean up
    objStream.Close
    Set objStream = Nothing
end if
%>
