    <% 
        if (sGlyMap <> "") then
            sGlyTarget = oPublisher.call ("Get_GetWebsiteParameterValue", "FileTarget", iChildPageID, 0, 0)
            ListFolder sGlyMap, 0,1, _
            "<ul>" & _
                "<li class=""name""><a href=""@link@"" target=""_blank"">" &  "@name_excl@ " & "</a></li>" & _
            </ul>", sGlyTarget
            sGlyMap=""
        end if
    %>
