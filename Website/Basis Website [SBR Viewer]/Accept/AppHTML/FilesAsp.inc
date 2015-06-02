<%
        ' Mask: @name@, @name_excl@, @ext@, @size@, @date@, @type@, @link@. @download@
        '
        Sub ListFolder(path, bIsAutoUpdate,bDoList, sMask, sTarget) 
                Dim fs, rootPath
                path = sBronMap & path

                Set fs = CreateObject("Scripting.FileSystemObject")
                If fs.FolderExists(Server.MapPath(path)) Then
                        rootPath = Replace(path, Server.MapPath("/"), "") & "\"
                        ListFolderContents fs.GetFolder(Server.MapPath(path)), PathEncode(rootPath), bIsAutoUpdate, 0, 0, bDoList, sMask, sTarget
                else
                        Say "FOUT! - Kan de locatie " & path & " niet vinden!"
                end if
                Set fs = Nothing
        End Sub
                
        Sub SetUpdateVersie(sVersie)
                sAFWVersie  = Mid(sVersie,1,3) 
                sAFWBuild   = Mid(sVersie,5,3) 
                sRAPVersie  = Mid(sVersie,9,7) 
                sRAPRevisie = Mid(sVersie,17,1) 
        End Sub

		Sub ListFolderContents(folder, relativePath, bIsAutoUpdate, bToonMap, bUpdateMap, bDoList, sMask, sTarget)
                Dim child, lastchild, bOddEven, sOddEven, sOutput, sArrayName, sName, i, sLi, sEndLi, nSize, sSize, sExt

                if (bDoList = 1) then 
                        sLi="<li>"
                        sEndLi="</li>"
                end if
                if ((bIsAutoUpdate = 0) and (bDoList = 1)) then Say "<ul>"
                if (bToonMap) then Say sLi & "<div class=""folder"">" & h(folder.Name) & "</div>"

                For Each child In folder.SubFolders
                        If Not IsHidden(child) Then
                                if (bIsAutoUpdate) then
                                        Set lastchild = child
                                else
                                        ListFolderContents child, relativePath & PathEncode(child.Name) & "/", 0, 1, 0, bDoList, sMask, sTarget
                                end if
                        End If  
                Next

                if (bUpdateMap) then SetUpdateVersie folder.Name
                if (bIsAutoUpdate = 1) then 
                        ListFolderContents lastchild, relativePath & PathEncode(lastchild.Name) & "/", 0, 0, 1, bDoList, sMask, sTarget
                elseif (bIsAutoUpdate = -1) then 
                        ListFolderContents lastchild, relativePath & PathEncode(lastchild.Name) & "/", -2, 0, 1, bDoList, sMask, sTarget
                end if
                                                        
                if (bIsAutoUpdate <> -2) then
                        relativePath = h(relativePath)
                        if (sTarget <> "") then sTarget = "target=""" & sTarget & """"
                        bOddEven=false
                        For Each child In folder.Files
                                If Not IsHidden(child) Then
                                        sOutput = sMask
                                        sName=""
                                        sArrayName=Split(h(child.Name),".")
                                        for i=0 to ubound(sArrayName)-1
                                                sName=sName & sArrayName(i)
                                        next 
										sExt = sArrayName(ubound(sArrayName))
                                        if (InStr(sOutput,"@name_excl@") > 0) then sOutput=replace(sOutput,"@name_excl@",sName)
                                        if (instr(sOutput,"@ext@") > 0) then sOutput=replace(sOutput,"@ext@",sExt)
                                        sOutput=replace(sOutput,"@name@",h(child.Name))
										nSize = (h(child.Size) * 0.000000977)
										if Int(nSize) = 0 then
											sSize = (Int(nSize * 1000.0)) & " KB"
										else
											sSize = (Int(nSize)) & " MB"
										end if
                                        sOutput=replace(sOutput,"@size@",sSize)
                                        sOutput=replace(sOutput,"@date@",(Pd(day(child.DateLastModified),2) & "-" & Pd(month(child.DateLastModified),2) & "-" & year(child.DateLastModified)))
                                        sOutput=replace(sOutput,"@type@",h(child.Type))

										if (uCase(sExt) = "PDF") then
											sOutput=replace(sOutput,"@download@","Downloadfile.php?file=@link@")
										else
											sOutput=replace(sOutput,"@download@","@link@")
										end if

										sOutput=replace(sOutput,"@link@",(relativePath & h(PathEncode(child.Name))))

                                        if bOddEven then sOddEven=" fileOdd" else sOddEven=" fileEven"
                                        'Say sLi & "<div class=""file" & sOddEven & """><a href=""" & relativePath & h(PathEncode(child.Name)) & """" & sTarget & ">" & sOutput & "</a></div>" & sEndLi
                                        Say sLi & "<div class=""file" & sOddEven & """>" & sOutput & "</div>" & sEndLi
                                        bOddEven=not(bOddEven)
                                End If
                        Next

                        if ((bIsAutoUpdate = 0) and (bDoList = 1)) then Say "</ul>"            
                end if
    end sub

        Sub Say(s)
                Response.Write s & vbNewLine
        End Sub

    Function pd(n, totalDigits) 
        if totalDigits > len(n) then 
            pd = String(totalDigits-len(n),"0") & n 
        else 
            pd = n 
        end if 
    End Function 
        
        Function h(s)
                h = Server.HTMLEncode(s)
        End Function

        Function PathEncode(s)
                ' this creates a more correct variant of what Server.URLEncode would do
                PathEncode = Replace(s, "\", "/")
                PathEncode = Server.URLEncode(PathEncode)
                PathEncode = Replace(PathEncode, "+", "%20")
                PathEncode = Replace(PathEncode, "%2F", "/")
                PathEncode = Replace(PathEncode, "%2E", ".")
                PathEncode = Replace(PathEncode, "%5F", "_")
        End Function

        Function IsHidden(File)
                IsHidden = File.Attributes And 2 = 2
        End Function
%>