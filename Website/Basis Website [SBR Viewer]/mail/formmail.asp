
<%@ LANGUAGE="VBScript" %>
<%      '**************************************************************************
        '* ASP FormMail                                                           *
        '*                                                                        *
        '* Do not remove this notice.                                             *
        '*                                                                        *
        '* Copyright 1999-2008 by Mike Hall.                                      *
        '* Please see http://www.brainjar.com for documentation and terms of use. *
        '**************************************************************************

        '- Customization of these values is required, see documentation. ----------

    Option Explicit
        Dim smtpServer, allowedHosts, allowedRecipients, allowedEnvars, allowCcToFlag, botCheckFlag, botCheckId, botCheckMinTime
        Dim errorMsgs, startTime, host, fromAddresses, fromAddrStr, fromAddr, fromOptions, fromOption, teller, bccToAddr, toAddr
        Dim subject, str, fieldOrder, name, body, html_body, html_repeater, text, envars, msg, fromSelectie, sLicentieExtra, fromHulp
        Dim strFileName, objFSO, objTextFile, strReadLineText

        smtpServer = "mail.rootsrvr.eu"

        allowedHosts      = Array("www.boekvoud.nl", "boekvoud.nl", "www.rootsrvr.eu", "rootsrvr.eu", "www.sbrviewer.eu", "sbrviewer.eu")
        allowedRecipients = Array()
        allowedEnvars     = Array("HTTP_USER_AGENT", "REMOTE_ADDR", "REMOTE_USER")

        allowCcToFlag = true

        botCheckFlag    = false
        botCheckID      = "MyBotCheckID"
        botCheckMinTime = 5

        '- End required customization section. ------------------------------------

        'Initialize.
        Response.Buffer = true
        errorMsgs = Array()

        'Check for form data.
        if Request.ServerVariables("Content_Length") = 0 then
                call AddErrorMsg("No form data submitted.")
        end if

        'If bot checking is enabled, check the elapsed time.
        if botCheckFlag then
                startTime = Session(botCheckID)
                if not IsDate(startTime) then
                        call AddErrorMsg("Invalid submission.")
                elseif DateDiff("s", startTime, Now()) < botCheckMinTime then
                        call AddErrorMsg("Invalid submission.")
                end if
        end if

        'Check if the referering host is allowed.
        if UBound(allowedHosts) >= 0 then
                host = GetHost(Request.ServerVariables("HTTP_REFERER"))
                if host = "" then
                        call AddErrorMsg("No referer.")
                elseif not InList(host, allowedHosts) then
                        call AddErrorMsg("Unauthorized host: '" & host & "'.")
                end if
        end if

        'Get the Email From
        fromAddresses = Split(Request.Form("_replyToField"), "|")
        fromSelectie  = Trim(Request.Form("_replyToFieldOption"))      
        If fromSelectie = "" then       
                fromOption   = 0
                fromSelectie = "SBR Viewer"
        else
                fromOption   = 0
                fromOptions  = Split(Request.Form("_replyToFieldOption$"), "|")
                teller       = 0
                for each name in fromOptions
                        If name = fromSelectie then  
                                fromOption = teller
                                exit for
                        end if
                        teller = teller + 1
                next  
        end if      
        fromAddr = (fromSelectie & " <" & Trim(fromAddresses(fromOption)) & ">")
        fromAddrStr= (fromSelectie & " &lt;" & Trim(fromAddresses(fromOption)) & "&gt;")
        'Get the Email BCC. 
        bccToAddr =  fromAddr 'stuur Bcc naar je zelf!

        'Get the Email To
        toAddr = Trim(Request.Form("_recipients"))

        'Get the subject text.
        subject = Request.Form("_subject") 


        'If required fields are specified, check them.
        if Request.Form("_requiredFields") <> "" then
                required = Split(Request.Form("_requiredFields"), ",")
                for each name in required
                        name = Trim(name)
                        if Left(name, 1) <> "_" and Request.Form(name) = "" then
                                call AddErrorMsg("Missing value for " & name)
                        end if
                next
        end if
        'If a field order was given, use it. Otherwise use the order the fields
        'were received in.
        str = ""
        if Request.Form("_fieldOrder") <> "" then
                fieldOrder = Split(Request.Form("_fieldOrder"), ",")
                for each name in fieldOrder
                        if str <> "" then
                                str = str & ","
                        end if
                        str = str & Trim(name)
                next
                fieldOrder = Split(str, ",")
        else
                fieldOrder = FormFieldList()
        end if

        'If there were no errors, build the email note and send it.
        if UBound(errorMsgs) < 0 then

            ' Uitlezen mail-body
            ' variabelen --> @title@, @onderwerp@, @betreft@, @REPEATER@

            strFileName = Server.MapPath(Request.Form("_template"))

            Set objFSO      = Server.CreateObject("Scripting.FileSystemObject")
            Set objTextFile = objFSO.OpenTextFile(strFileName,1,true)

            Do While Not objTextFile.AtEndOfStream      
                strReadLineText = objTextFile.ReadLine
                html_body = html_body & strReadLineText
            Loop

            ' Close and release file references
            objTextFile.Close
            Set objTextFile = Nothing
            Set objFSO = Nothing

            ' Uitlezen mail-repeater
           ' variabelen --> >@title@, @text@
            strFileName     = Server.MapPath("email-template-repeater.html")
            Set objFSO      = Server.CreateObject("Scripting.FileSystemObject")
            Set objTextFile = objFSO.OpenTextFile(strFileName,1,true)

            Do While Not objTextFile.AtEndOfStream      
                strReadLineText = objTextFile.ReadLine
                html_repeater = html_repeater & strReadLineText
            Loop

            ' Close and release file references
            objTextFile.Close
            Set objTextFile = Nothing
            Set objFSO = Nothing

            '------------------------------------------------------------------------------
            ' HEADER
            '------------------------------------------------------------------------------
            html_body = replace(html_body,"@title@","Accept")
            html_body = replace(html_body,"@betreft@",subject)
            html_body = replace(html_body,"@onderwerp@",(Request.Form("_pretopic") & fromAddrStr & Request.Form("_posttopic")))

            '------------------------------------------------------------------------------
            ' INVOERGEGEVENS
            '------------------------------------------------------------------------------
            body = html_repeater
            body = replace(body,"@title@",Request.Form("_fieldOpeningText"))
                'Build table of form fields and values.
                text = "<table width=""550"" style=""font-family: Arial,Lucida Sans, Tahoma; font-size:13px;"">"
                for each name in fieldOrder
                    if Request.Form(name) <> "" then
                        text = text & "<tr>" & "<td><strong>" & FormFieldListName(name) & ":</strong></td>" & "<td>" & Request.Form(name) & "</td>" & "</tr>"
                    end if
                next
                text = text & "</table>" 
            body = replace(body,"@text@",text)
            html_body = replace(html_body,"@REPEATER@",body & "@REPEATER@") 

            '------------------------------------------------------------------------------
            ' ENVIRONMENT VARIABELEN
            '------------------------------------------------------------------------------
            'Add a table for any requested environment variables.
            if Request.Form("_envars") <> "" then
                body = html_repeater
                body = replace(body,"@title@","")
                text = "<table width=""550"" style=""font-family: Arial,Lucida Sans, Tahoma; font-size:13px;"">"
                envars = Split(Request.Form("_envars"), ",")
                for each name in envars
                        name = Trim(name)
                        'Only show environment variables in the permitted list.
                        showEnvar = true
                        if UBound(allowedEnvars) >= 0 then
                                showEnvar = InList(name, allowedEnvars)
                        end if
                        if showEnvar then
                                text = text & "<tr>" & "<td><strong>" & name & ":</strong></td>" & "<td>" & Request.ServerVariables(name) & "</td>" & "</tr>" 
                        end if
                next
                text = text & "</table>" 
                body = replace(body,"@text@",text)
                if body <> "" then html_body = replace(html_body,"@REPEATER@",body & "@REPEATER@")
            end if

            '------------------------------------------------------------------------------
            ' SLUITTEKST
            '------------------------------------------------------------------------------
            body = html_repeater
            body = replace(body,"@title@","")
            body = replace(body,"@text@",Request.Form("_fieldClosingText"))
            if body <> "" then html_body = replace(html_body,"@REPEATER@",body & "@REPEATER@")

            '------------------------------------------------------------------------------
            '------------------------------------------------------------------------------
            body = replace(replace(html_body,"@REPEATER@",""),"?","")

            dim debug_
            'debug_ = true
            if (debug_) then
                ' Wegschrijven debug file
                strFileName     = Server.MapPath("debug.txt")
                Set objFSO      = Server.CreateObject("Scripting.FileSystemObject")
                Set objTextFile = objFSO.CreateTextFile(strFileName,true,true)

                objTextFile.WriteLine(body)

                ' Close and release file references
                objTextFile.Close
                Set objTextFile = Nothing
                Set objFSO = Nothing

            end if

            'Send it.
            str = SendMail()
            if str <> "" then
                AddErrorMsg(str)
            else
                'Clear the bot check timestamp.
                Session.Contents.Remove(botCheckID)

                'Redirect if a URL was given.
                if Request.Form("_redirectUrl") <> "" then
                    Response.Redirect(Request.Form("_redirectUrl"))
                end if
            end if

        end if %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
        <meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
        <title>Form Mail</title>
        <style type="text/css">
            body {
                background-color: #ffffff;
                color: #000000;
                font-family: "Arial","sans-serif";
                font-size: 10pt;
            }

            table {
                border: solid 1px #000000;
                border-collapse: collapse;
            }

            td, th {
                border: solid 1px #000000;
                font-family: "Arial","sans-serif";
                font-size: 10pt;
                padding: 2px 8px;
            }

            th {
                background-color: #c0c0c0;
            }

            .error {
                color: #c00000;
            }
        </style>
</head>
<body>
<%      if UBound(errorMsgs) >= 0 then %>
        <p class="error">
        Form could not be processed due to the following errors:</p>
        <ul>
<%              for each msg in errorMsgs %>
                <li class="error"><% = msg %></li>
<%              next %>
        </ul>
        <p><a href="#" onclick="history.go(-1); return false;">Back</a></p>
<%      else %>
        <table cellpadding="0" cellspacing="0">
                <tr>
                        <th colspan="2" valign="bottom">Thank you, the following information has been sent:</th>
                </tr>
<%              for each name in fieldOrder %>
                <tr valign="top">
                        <td style="font-size:10pt; font-family:"Arial","sans-serif";"><strong><% = name %></strong></td>
                        <td style="font-size:10pt; font-family:"Arial","sans-serif";"><% = Request.Form(name) %></td>
                </tr>
<%              next %>
        </table>
<%              if Request.Form("_continueUrl") <> "" then %>
        <p><a href="<% = Request.Form("_continueUrl") %>">Continue</a></p>
<%              end if
        end if %>
</body>
</html>
<%      '==========================================================================
        ' Functions and subroutines.
        '==========================================================================

        '--------------------------------------------------------------------------
        ' Adds an error message to the list.
        '--------------------------------------------------------------------------
        sub AddErrorMsg(msg)

                dim n

                n = UBound(errorMsgs)
                Redim Preserve errorMsgs(n + 1)
                errorMsgs(n + 1) = msg

        end sub

        '--------------------------------------------------------------------------
        ' Extracts the host name from a URL.
        '--------------------------------------------------------------------------
        function GetHost(url)

                dim i, str

                GetHost = ""

                'Strip down to host or IP address and port number, if any.
                if Left(url, 7) = "http://" then
                        str = Mid(url, 8)
                elseif Left(url, 8) = "https://" then
                        str = Mid(url, 9)
                end if
                i = InStr(str, "/")
                if i > 1 then
                        str = Mid(str, 1, i - 1)
                end if
                GetHost = str

        end function

        '--------------------------------------------------------------------------
        ' Returns true if the given string is in the given array.
        '--------------------------------------------------------------------------
        function InList(str, list)

                dim item

                InList = false

                'Scan the list.
                for each item in list
                        if str = item then
                                InList = true
                                exit function
                        end if
                next

        end function

        '--------------------------------------------------------------------------
        ' Returns true if the given email address is in valid format.
        '--------------------------------------------------------------------------
        function IsValidEmailAddress(addr)

                dim list, item
                dim i, c

                IsValidEmailAddress = true

                'Exclude any address with '..'.
                if InStr(addr, "..") > 0 then
                        IsValidEmailAddress = false
                        exit function
                end if

                'Split email address into the user and domain names.
                list = Split(addr, "@")
                if UBound(list) <> 1 then
                        IsValidEmailAddress = false
                        exit function
                end if

                'Check both names.
                for each item in list

                        'Make sure the name is not zero length.
                        if Len(item) <=  0 then
                                IsValidEmailAddress = false
                                exit function
                        end if

                        'Make sure only valid characters appear in the name.
                        for i = 1 to Len(item)
                                c = Lcase(Mid(item, i, 1))
                                if InStr("abcdefghijklmnopqrstuvwxyz&_-.", c) <= 0 and not IsNumeric(c) then
                                        IsValidEmailAddress = false
                                        exit function
                                end if
                        next

                        'Make sure the name does not start or end with invalid characters.
                        if Left(item, 1) = "." or Right(item, 1) = "." then
                                IsValidEmailAddress = false
                                exit function
                        end if

                next

                'Check for a '.' character in the domain name.
                if InStr(list(1), ".") <= 0 then
                        IsValidEmailAddress = false
                        exit function
                end if

        end function

        '--------------------------------------------------------------------------
        ' Builds an array of form field names ordered as they were received.
        ' Note that fields whose name starts with an underscore are ignored.
        '--------------------------------------------------------------------------
        function FormFieldList()
                dim str, i, name
                str = ""
                for i = 1 to Request.Form.Count
                        for each name in Request.Form
                                If Right(name, 1) <> "$" then
                                        if ((Left(name, 1) <> "_") or (name = "_recipients")) and Request.Form(name) is Request.Form(i) then
                                                if str <> "" then
                                                        str = str & ","
                                                end if
                                                str = str & name
                                                exit for
                                        end if
                                end if
                        next
                next
                FormFieldList = Split(str, ",")
        end function
    
        function FormFieldListName(name)

            if (name="_recipients") then 
                FormFieldListName = "E-mail"
            else 
                FormFieldListName = name
            end if
        end function

        '--------------------------------------------------------------------------
        ' Sends email based on mail component. Uses global variables for parameters
        ' because there are so many.
        '--------------------------------------------------------------------------

        function SendMail()

                dim mailObj, cdoMessage, cdoConfig

                SendMail = ""

                set cdoMessage = Server.CreateObject("CDO.Message")
                set cdoConfig = Server.CreateObject("CDO.Configuration")
                cdoConfig.Fields("http://schemas.microsoft.com/cdo/configuration/sendusing") = 2
                cdoConfig.Fields("http://schemas.microsoft.com/cdo/configuration/smtpserver") = smtpServer
                cdoConfig.Fields("http://schemas.microsoft.com/cdo/configuration/smtpauthenticate") = 1
                cdoConfig.Fields("http://schemas.microsoft.com/cdo/configuration/sendusername") = "frank@rootsrvr.eu"
                cdoConfig.Fields("http://schemas.microsoft.com/cdo/configuration/sendpassword") = "Rel24686"
                cdoConfig.Fields.Update
                set cdoMessage.Configuration = cdoConfig
                cdoMessage.From     = fromAddr
                cdoMessage.To       = toAddr
                cdoMessage.BCC      = bccToAddr
                cdoMessage.Subject  = subject
                cdoMessage.HtmlBody = body
                on error resume next
                cdoMessage.Send
                if Err.Number <> 0 then
                        SendMail = "Email send failed: " & Err.Description & "."
                end if
                set cdoMessage = Nothing
                set cdoConfig = Nothing

        end function %>