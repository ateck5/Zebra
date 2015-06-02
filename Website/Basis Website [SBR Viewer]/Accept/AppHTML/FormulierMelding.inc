<% 
	Dim  iSiteMelding, sArrayTitel(), sArrayMelding(), iMsg
	'------------------------------------------------------------------------
	' URL-Argumenten
	'------------------------------------------------------------------------
	iSiteMelding = request("message")
	'------------------------------------------------------------------------
	if (iSiteMelding > 0 )then
		sub AddMessage (sTitle, sMessage)
			iMsg=iMsg+1
			ReDim Preserve sArrayTitel(iMsg) 
			ReDim Preserve sArrayMelding(iMsg) 
			sArrayTitel(iMsg)   = sTitle
			sArrayMelding(iMsg) = sMessage
		end sub
		iMsg=0
		AddMessage "- Verzonden - ", "Het bericht is verzonden.<br/><br/> Wij nemen zo spoedig mogelijk contact op."
		AddMessage "- Fout - ", "U heeft een onjuist e-mail adres ingegeven. Het bericht is niet verzonden."
		AddMessage "- Fout - ", "U heeft niet alle verplichte velden ingevuld. Het bericht is niet verzonden."
		AddMessage "- Fout - ", "Er is een fout opgetreden. Het bericht is niet verzonden."
		AddMessage "- Verzonden - ", "Bedankt voor het aanvragen van de demo licentie.<br/><br/> U ontvangt zo dadelijk een e-mail bericht met de licentie."

		if (cInt(iSiteMelding) <= iMsg) then
			%>
			<div id="MesssageContrainer">
				<div id="Overlay">
				</div>
				<div id="Message">
					<h2><%=sArrayTitel(cInt(iSiteMelding)) %></h2>
					<p><%=sArrayMelding(cInt(iSiteMelding)) %></p>

					<input id="MsgOk" class="blue" name="messageOk" type="BUTTON" value="OK" onclick="javascript:reloadPage('message')" />
				</div>
			</div>
			<%
		end if
	end if
%>
