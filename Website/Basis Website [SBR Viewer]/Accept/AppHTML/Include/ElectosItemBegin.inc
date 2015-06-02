<%
' To print the news items, first we must the determine how many news items should be presented on this page
' CountChildren is used to get the number of activated children for a given category or multi-item page.
iNumberOfItems = oPublisher.call ("GET_CountChildren", iPageID)

' Second we build a loop in which we will print all news items. In the "ElectosItemEnd" include this loop will be closes so be sure to include it.
For iCounter = 1 To iNumberOfItems
  ' For each item in this page the iChildPageID will be determined. This can be used to print the Item.
  ' ChildPageID returns the pageID of the n-th activated child of a category or multi-item page, where "n" is determined by the "iCounter" parameter. 
  iChildPageID = oPublisher.call ("GET_ChildPageID", iPageID, iCounter)
%>