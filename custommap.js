(function ($) {
  Drupal.behaviors.custommap = {
    attach: function(context,settings) {
        // Map options.
		var lmarkers = [];
		var customIcons = {
		         school: {icon: 'http://labs.google.com/ridefinder/images/mm_20_white.png'},
			     office: {icon: 'http://labs.google.com/ridefinder/images/mm_20_orange.png'},
                 museum: {icon: 'http://labs.google.com/ridefinder/images/mm_20_blue.png'},
                 library: {icon: 'http://labs.google.com/ridefinder/images/mm_20_green.png'},	
                 university: {icon: 'http://labs.google.com/ridefinder/images/mm_20_yellow.png' },	
				 shopping: {icon: 'http://labs.google.com/ridefinder/images/mm_20_red.png' },
				 business: {icon: 'http://labs.google.com/ridefinder/images/mm_20_purple.png' },
              };
		function init(){
		     var centerLat=42.395640;
			 var centerLng=-71.177611;
			 var center= new google.maps.LatLng(centerLat,centerLng);
			 var zoom = 13;
			 var mapTypeId = 'roadmap';
			 var animation = google.maps.Animation.DROP;
			 var infoWindow = new google.maps.InfoWindow({ content: content});
     	     var mapProp = {
					center:center,
					zoom:zoom,
					center: center,
					mapTypeId:mapTypeId
			};
			 var map = new google.maps.Map(document.getElementById("googleMap"),mapProp);
			 var mcOptions = {gridSize: 50, maxZoon: 15};
			 var positions = [];
			 var settingArray=Drupal.settings['custommap'];
			 var settingLen = settingArray.length;
			 for (var i = 0; i < settingLen; i++) {
                 var address = settingArray[i]['address'];
               
			     var icon = customIcons[settingArray[i]['type']] || {};
                 var point = new google.maps.LatLng(parseFloat(settingArray[i]['lat']),parseFloat(settingArray[i]['lng']));
				 positions.push([parseFloat(settingArray[i]['lat']), parseFloat(settingArray[i]['lng'])]);
                 var html = "<b>" + name + "</b> <br/>" + address; 
            	 var div = document.getElementById('legend');
				 var marker = new google.maps.Marker({
											map: map,
											position: point,
											icon: icon.icon,
											animation: animation,
                     						
                        });
					   bindInfoWindow(marker, map, infoWindow,address,html); 
					   lmarkers.push(marker);
					 				  
				 }
				  voronoiDiagram(positions,map);
				  var mc = new MarkerClusterer(map,lmarkers,mcOptions);
				 }
			 
			 function voronoiDiagram(positions,map) {
			        voronoiPolygons = [];
					var voronoi = d3.geom.voronoi().clipExtent([[0, -100], [60, -40]]);
					//var voronoi = d3.geom.voronoi();
					var polygons = voronoi(positions);
					//console.log(polygons);
			 
					// iterate through polygons, draw on map
					for(var i = 0; i < polygons.length; i++) {
					  if(!polygons[i]) continue;
					  if(polygons[i].length <= 0) continue;
			 
					  var voronoiCoords = [];
					  // store coordinates for each polygon
					  for(var j = 0; j < polygons[i].length; j++) {
						voronoiCoords.push(new google.maps.LatLng(polygons[i][j][0], polygons[i][j][1]));
					  }
					  // complete polygon
					  
					  voronoiCoords.push(new google.maps.LatLng(polygons[i][0][0], polygons[i][0][1]));
					  //console.log(voronoiCoords);
			 
					  voronoiPolygons.push(new google.maps.Polygon({
						paths: voronoiCoords,
						strokeColor: '#FF0000',
						strokeOpacity: 0.2,
						strokeWeight: 2,
						fillColor: '#FF0000',
						fillOpacity: 0.0
					  }));
			          
					  voronoiPolygons[voronoiPolygons.length - 1].setMap(map);
					  
					}
					 }
				 			
			 function bindInfoWindow(marker, map, infoWindow, address, html) {
					/*google.maps.event.addListener(marker, 'click', function() {
							infoWindow.setContent(html);
							infoWindow.open(map, marker);
					});*/
					google.maps.event.addListener(marker, 'mouseover', function() {
						infoWindow.setContent(address);
						infoWindow.open(map, marker);
					});
					google.maps.event.addListener(marker,'dblclick', function() {
						marker.setMap(null);
					});
					
				}
          
		  google.maps.event.addDomListener(window, 'load', init);
    }
  };
})(jQuery);
