<div id="files">
    <% 
        sGlyMap = oPublisher.call ("Get_GetWebsiteParameterValue", "FileMap", iChildPageID, 0, 0)
        if (sGlyMap <> "") then
            if (bNoSitePanel=1)  then
                sGlyClass = " large"
            else
                sGlyClass = ""
            end if

            sGlyTarget = oPublisher.call ("Get_GetWebsiteParameterValue", "FileTarget", iChildPageID, 0, 0)
            ListFolder sGlyMap, (oPublisher.call ("Get_GetWebsiteParameterValue", "IsAutoUpdate", iChildPageID, 0, 0)),0, _
            "<ul>" & _
            "<li class=""ext @ext@""><a href=""@download@"">" & "Download" & "</a></li>" & _
            "<li class=""name" & sGlyClass & """><a href=""@link@"" target=""_blank"">" &  "@name_excl@ " & "</a></li>" & _
            "<li class=""size"">" &  "@size@ " & "</li>" & _
            "<li class=""date"">" &  "@date@ " & "</li></ul>", sGlyTarget
        end if
    %>
</div>
<script>
    //http://webdesign.about.com/od/php/ht/force_download.htm
</script>
