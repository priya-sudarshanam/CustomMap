(function ($) {
  Drupal.behaviors.map_sample = {
    attach: function(context,settings) {
        // Map options.
		var customIcons = {
		         school: {icon: 'http://labs.google.com/ridefinder/images/mm_20_white.png'},
			     office: {icon: 'http://labs.google.com/ridefinder/images/mm_20_orange.png'},
                 museum: {icon: 'http://labs.google.com/ridefinder/images/mm_20_blue.png'},
                 library: {icon: 'http://labs.google.com/ridefinder/images/mm_20_green.png'},	
                 university: {icon: 'http://labs.google.com/ridefinder/images/mm_20_yellow.png' },	
				 shopping: {icon: 'http://labs.google.com/ridefinder/images/mm_20_red.png' },
				 business: {icon: 'http://labs.google.com/ridefinder/images/mm_20_purple.png' },
              };
		var lmarkers = [];
		var cTypes = {};
		function init(){
		     var centerLat=42.395640;
			 var centerLng=-71.177611;
			 var center= new google.maps.LatLng(centerLat,centerLng);
			 var content = 'HWP';
			 var zoom = 12;
			 var mapTypeId = 'roadmap';
			 var animation = google.maps.Animation.DROP;
			 var number = 1;
			 
			 var infoWindow = new google.maps.InfoWindow({
			        content: content});
     	     var mapProp = {
					center:center,
					zoom:zoom,
					center: center,
					mapTypeId:mapTypeId
			};
			
			 var map = new google.maps.Map(document.getElementById("googleMap"),mapProp);
			 var mcOptions = {gridSize: 50, maxZoon: 15};
			
			 var settingArray=Drupal.settings['testmap'];
			 var settingLen = settingArray.length;
			 for (var i = 0; i < settingLen; i++) {
                 var address = settingArray[i]['address'];
               
			     var icon = customIcons[settingArray[i]['type']] || {};
                 var point = new google.maps.LatLng(parseFloat(settingArray[i]['lat']),parseFloat(settingArray[i]['lng']));
                 var html = "<b>" + name + "</b> <br/>" + address; 
                 var title = settingArray[i]['title'];
				 var div = document.getElementById('legend');
				 var marker = new google.maps.Marker({
											map: map,
											position: point,
											icon: icon.icon,
											animation: animation,
                     						
                        });
					  var sidebarEntry = createSidebarEntry(marker);
					  lmarkers.push(marker);
					  
					//  map.controls[google.maps.ControlPosition.LEFT_BOTTOM].push(
					//	document.getElementById('legend'));
					var sidebar = document.getElementById('sidebar');
  
					   bindInfoWindow(marker, map, infoWindow,name,html); 
					   
					 				  
				 }
				  var mc = new MarkerClusterer(map,lmarkers,mcOptions);
				   sidebar.innerHTML = '';
				   if (settingLen == 0) {
					 sidebar.innerHTML = 'No results found.';
					 map.setCenter(center);
					 return;
				   }
				   
				   for(var j = 0; j < lmarkers.length; j++){
					 var sidebarEntry = createSidebarEntry(lmarkers[j],lmarkers[j]['name']);
					 sidebar.appendChild(sidebarEntry);
			   }

			 }
				 			
				
			 function bindInfoWindow(marker, map, infoWindow, name, html) {
					google.maps.event.addListener(marker, 'click', function() {
							infoWindow.setContent(html);
							infoWindow.open(map, marker);
					});
					google.maps.event.addListener(marker, 'mouseover', function() {
						infoWindow.setContent(name);
						infoWindow.open(map, marker);
					});
					
				}
          
		  function createSidebarEntry(marker,name){
		      var div = document.createElement('div');
			  var html = '<tr><td><b>'+ name +'</b></td><td></td></tr>';
			  div.innerHTML = html;
			  div.style.cursor = 'pointer';
			  div.style.marginBottom = '5px';
			  google.maps.event.addDomListener(div,'click',function(){
			    google.maps.event.trigger(marker, 'click');
			  });
		      google.maps.event.addDomListener(div, 'mouseover', function() {
				div.style.backgroundColor = '#eee';
			  });
			  google.maps.event.addDomListener(div, 'mouseout', function() {
				div.style.backgroundColor = '#fff';
			  });
		     return div;
		  }
		   
					   
		   $("#location_table #library").click( function()
           {
       //      alert(this.id);
			 for (var i=0; i<lmarkers.length; i++) {
				 if (lmarkers[i].type == this.id) {
				    if(lmarkers[i].getVisible() == true) {
				      lmarkers[i].setVisible(false);
					}
					else {
					  lmarkers[i].setVisible(true);
					}
				   }
				 
				 }
				 }
				
			  );
			 $("#location_table #museum").click( function()
           {
       //      alert(this.id);
			 for (var i=0; i<lmarkers.length; i++) {
				 if (lmarkers[i].type == this.id) {
				    if(lmarkers[i].getVisible() == true) {
				      lmarkers[i].setVisible(false);
					}
					else {
					  lmarkers[i].setVisible(true);
					}
				   }
				 
				 }
				 }
				
			  );
			$("#location_table #office").click( function()
			   {
				 for (var i=0; i<lmarkers.length; i++) {
				 if (lmarkers[i].type == this.id) {
				    if(lmarkers[i].getVisible() == true) {
				      lmarkers[i].setVisible(false);
					}
					else {
					  lmarkers[i].setVisible(true);
					}
				   }
				 
				 }
			   }
			);
				
			google.maps.event.addDomListener(window, 'load', init);
    }
  };
})(jQuery);
