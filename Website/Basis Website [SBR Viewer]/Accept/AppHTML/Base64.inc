<%

' ** Set up the Base64 arrays

  const BASE_64_MAP_INIT ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
  dim nl
     dim Base64EncMap(63)
     dim Base64DecMap(127)
  initcodecs



' ** The initCodecs() routine initialises the Base64 arrays

   PUBLIC SUB initCodecs()
          nl = "<P>" & chr(13) & chr(10)
          dim max, idx
          max = len(BASE_64_MAP_INIT)
          For idx = 0 to max - 1
               Base64EncMap(idx) = mid(BASE_64_MAP_INIT, idx + 1, 1)
          next
          For idx = 0 to max - 1
               Base64DecMap(ASC(Base64EncMap(idx))) = idx
          next
   End SUB

' ** Base 64 Encoding function **

   PUBLIC Function base64Encode(plain)

      call initCodecs

          If len(plain) = 0 then
               base64Encode = ""
               exit Function
          End If

          dim ret, ndx, by3, first, second, third
          by3 = (len(plain) \ 3) * 3
          ndx = 1
          do While ndx <= by3
               first  = asc(mid(plain, ndx+0, 1))
               second = asc(mid(plain, ndx+1, 1))
               third  = asc(mid(plain, ndx+2, 1))
               ret = ret & Base64EncMap(  (first \ 4) and 63 )
               ret = ret & Base64EncMap( ((first * 16) and 48) + ((second \ 16) and 15 ) )
               ret = ret & Base64EncMap( ((second * 4) and 60) + ((third \ 64) and 3 ) )
               ret = ret & Base64EncMap( third and 63)
               ndx = ndx + 3
          Loop
          ' check for stragglers
          If by3 < len(plain) then
               first  = asc(mid(plain, ndx+0, 1))
               ret = ret & Base64EncMap(  (first \ 4) and 63 )
               If (len(plain) MOD 3 ) = 2 then
                    second = asc(mid(plain, ndx+1, 1))
                    ret = ret & Base64EncMap( ((first * 16) and 48) +((second \16) and 15 ) )
                    ret = ret & Base64EncMap( ((second * 4) and 60) )
               Else
                    ret = ret & Base64EncMap( (first * 16) and 48)
                    ret = ret & "="
               End If
               ret = ret & "="
          End If

          base64Encode = ret
     End Function

' ** Base 64 decoding function **

     PUBLIC Function base64Decode(scrambled)

          If len(scrambled) = 0 then
               base64Decode = ""
               exit Function
          End If

          ' ignore padding
          dim realLen
          realLen = len(scrambled)
          do While mid(scrambled, realLen, 1) = "="
               realLen = realLen - 1
          Loop
          do While instr(scrambled," ")<>0
              scrambled=left(scrambled,instr(scrambled," ")-1) & "+" & mid(scrambled,instr(scrambled," ")+1)
          Loop
          dim ret, ndx, by4, first, second, third, fourth
          ret = ""
          by4 = (realLen \ 4) * 4
          ndx = 1
          do While ndx <= by4
               first  = Base64DecMap(asc(mid(scrambled, ndx+0, 1)))
               second = Base64DecMap(asc(mid(scrambled, ndx+1, 1)))
               third  = Base64DecMap(asc(mid(scrambled, ndx+2, 1)))
               fourth = Base64DecMap(asc(mid(scrambled, ndx+3, 1)))
               ret = ret & chr( ((first * 4) and 255) +   ((second \ 16) and 3))
               ret = ret & chr( ((second * 16) and 255) + ((third \ 4) and 15) )
               ret = ret & chr( ((third * 64) and 255) +  (fourth and 63) )
               ndx = ndx + 4
          Loop
          ' check for stragglers, will be 2 or 3 characters
          If ndx < realLen then
               first  = Base64DecMap(asc(mid(scrambled, ndx+0, 1)))
               second = Base64DecMap(asc(mid(scrambled, ndx+1, 1)))
               ret = ret & chr( ((first * 4) and 255) +   ((second \ 16) and 3))
               If realLen MOD 4 = 3 then
                    third = Base64DecMap(asc(mid(scrambled,ndx+2,1)))
                    ret = ret & chr( ((second * 16) and 255) + ((third \ 4) and 15) )
               End If
          End If

          base64Decode = ret
     End Function


' ** The SimpleXor encryption algorithm. **
' ** NOTE:    This is a placeholder really.  Future releases of VSP Form will use AES or TwoFish.  Proper encryption **
' **       This simple function and the Base64 will deter script kiddies and prevent the "View Source" type tampering **
' **      It won't stop a half decent hacker though, but the most they could do is change the amount Field to something **
' **      else, so provided the vendor checks the reports and compares amounts, there is no harm done.  It's still **
' **      more secure than the other PSPs who don't both encrypting their forms at all **

Public Function SimpleXor(InString,Key)
    Dim myIN, myKEY, myC, myPub, i, j
    Dim Keylist()
    
    myIN = InString
    myKEY = Key
    
    redim KeyList(Len(myKEY))
    
    i = 1
    do While i<=Len(myKEY)
        KeyList(i) = Asc(Mid(myKEY, i, 1))
        i = i + 1
    Loop       
    
    j = 1
    i = 1
    do While i<=Len(myIn)
        myC = myC & Chr(Asc(Mid(myIN, i, 1)) Xor KeyList(j))
        i = i + 1
        If j = Len(myKEY) Then j = 0
        j = j + 1
    Loop
 
    SimpleXor = myC
End Function


' ** The getToken function. **
' ** NOTE:    A function of convenience that extracts the value from the "name=value&name2=value2..." VSP reply string **
' **      Works even if one of the values is a URL containing the & or = signs.  **

public Function getToken(thisString,thisToken)

  ' Can't just rely on & characters because these may be provided in the URLs.
  Dim Tokens
  Dim subString
  Tokens = Array( "Status", "StatusDetail", "VendorTxCode", "VPSTxId", "TxAuthNo", "AVSCV2", "Amount", "AddressResult", "PostCodeResult", "CV2Result", "GiftAid", "3DSecureStatus", "CAVV" )
  
  If instr(thisString,thisToken+"=")=0 then
  
    '  If the token isn't present, empty the Output.  We can Error later
    getToken=""
    
  Else
    
    ' Right get the rest of the string
    subString=mid(thisString,instr(thisString,thisToken)+len(thisToken)+1)
    
    ' Now strip off all remaining tokens if they are present.
    
    i = LBound( Tokens )
    do While i <= UBound( Tokens )
    
      'Find the next token and lop it off
      If Tokens(i)<>thisToken then
      
        If instr(subString,"&"+Tokens(i))<>0 then 
          substring=left(substring,instr(subString,"&"+Tokens(i))-1)
        End If
            
      End If
      
      i = i + 1
    
    Loop  
    
    getToken=subString
  
  End If

  
End Function



%>