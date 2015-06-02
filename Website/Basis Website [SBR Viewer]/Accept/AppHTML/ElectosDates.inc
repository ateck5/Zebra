<%
    Dim aMonths, aDays, aMonthsShort
    
    aMonths = Array("", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December")
    aMonthsShort= Array("", "Jan", "Feb", "Mrt", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec")
    aDays = Array("", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday")

    Function ParseElectosDate(sDate)
        Dim aParts
        
        aParts = Split(Replace(sDate, "/", "-"), "-")
        
        If (UBound(aParts) >= 2) Then
            If (IsNumeric(aParts(0)) and IsNumeric(aParts(1)) and IsNumeric(aParts(2))) Then
                ParseElectosDate = DateSerial(cInt(aParts(2)), cInt(aParts(1)), CInt(aParts(0)))
            Else
                ParseElectosDate = DateSerial(1, 1, 1)
            End If
        Else
            ParseElectosDate = DateSerial(1, 1, 1)
        End If
    End Function
    
    Function FormatDateLong(dDate)
        FormatDateLong = LCase(aDays(Weekday(dDate)) & "&nbsp;" & Day(dDate)& "&nbsp;" & aMonths(Month(dDate)))
    End Function

    Function FormatDate(dDate, sFormatString)
        Dim sResult
    
        sResult = Replace(sFormatString, "{monthshort}", aMonthsShort(Month(dDate)))
        sResult = Replace(sResult, "{monthlong}", aMonths(Month(dDate)))
        sResult = Replace(sResult, "{month}", Month(dDate))
        sResult = Replace(sResult, "{day}", Day(dDate))
        sResult = Replace(sResult, "{weekday}", Weekday(dDate))
        sResult = Replace(sResult, "{weekdaylong}", aDays(Weekday(dDate)))
        sResult = Replace(sResult, "{year}", Year(dDate))
        
        FormatDate = sResult
    End Function
%>