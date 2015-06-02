<% 
	' http://api.maps.ovi.com/devguide/getting_started.html#BasicMap
%>
  <script src="http://api.maps.ovi.com/jsl.js" type="text/javascript" charset="utf-8"></script> 
  <div id="map" style="width:600px; height:400px;"></div>
    <script type="text/javascript">
	var map = new ovi.mapsapi.map.Display(document.getElementById("map"), {
			components: [
					new ovi.mapsapi.map.component.zoom.DoubleClick(),
					//needed for marker drag
					new ovi.mapsapi.map.component.objects.DragMarker()],
					zoomLevel: 14,
					center: [51.5603, 4.6131]
			});

	//create a marker and add it to the map
	var marker = new ovi.mapsapi.map.StandardMarker([51.5603, 4.6131], {
		text: "Acc", //small label
		draggable: true  //the marker is marked to be draggable



	});
	map.objects.add(marker);

//    var map = new ovi.mapsapi.map.Display(
//        document.getElementById("map"),
//        {
//            'zoomLevel': 14, //zoom level for the map
//            'center': [51.5603, 4.6131] //center coordinates

//	http://vancouver-webpages.com/META/DMS.html
//            'center': [51.33'36'95, 4.36'47'00] //center coordinates
//        }
//    );    

	map.addComponent( new ovi.mapsapi.map.component.zoom.DoubleClick() );
	map.addComponent( new ovi.mapsapi.map.component.panning.Drag() );
	map.addComponent( new ovi.mapsapi.map.component.panning.Kinetic() );
	</script>
