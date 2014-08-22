<?php
/**
 * Returns the latitude and longitude of a location
 */
function get_latitude_longitude($add){
   try {
        $prepAddr = str_replace(' ','+',$add.'Belmont MA');
        $geocode=file_get_contents('http://maps.google.com/maps/api/geocode/json?address='.$prepAddr.'&sensor=false');
        $output= json_decode($geocode);
  		$lat_lng = array (
		 'lat' => $output->results[0]->geometry->location->lat,
		 'lng' => $output->results[0]->geometry->location->lng,
			);
	 }
	catch (Exception $e) {
	   drupal_set_message(t('Issue in retrieving the location data. Message = %message, query=%query', 
	           array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');	
	}
      return $lat_lng;
}
 /**
 * Creates table header for location display. 
 */
 function create_table_header(){
  return $header = array(
					array('data' => 'Address',  'field' => 'address'),
					array('data' => 'Actions', 'colspan' => 2)
		//			array('data' => 'Edit'),				
				);
}
/**
* Returns type list for colored markers on the map. 
*/
function type_list(){
  return array("office" => 'office', 
			   "library" => 'library',
			   "school"  => 'school',
			   "museum" => 'museum',
			   "shopping"=>'shopping');
}
/**
* Returns delete form after user clicks on delete link in the table. 
*/
function create_delete_form($form, $form_state,$form_type){
 $locationDetails = map_select_data_id($form_state['id']);
 
 if ($form_type == 'delete'){
 $form['delete']['confirm'] = array(
  '#markup' => '<p>Are you sure you want to <font color="red">delete</font> location '.$locationDetails['address']. ' This action cannot be undone?</p>'
  );
  }
 else {
  $form['delete_all']['confirm'] = array(
  '#markup' => '<p>Are you sure you want to <font color="red">delete</font> all locations. This action cannot be undone?</p>'
  );
 
 }
  
 $form['submit'] =array (
       '#type' => 'submit',
	   '#value' => 'DELETE'
	   );
	  
 $form['cancel'] = array(
      '#type' => 'button',
      '#value' => 'CANCEL',
      '#validate' => array(),
         '#attributes' => array(
            'onClick' => 'history.go(-1); return true;'),
		'#post_render' => array('change_button_type'),	
		  
    ); 
  
return $form;
 
}

function create_update_form($form, $form_state){
$values = map_select_data_id($form_state['id']);
  
 $typeOptions = type_list();
   $form['update'] = array(
    '#type' => 'fieldset', 
	'#title' => 'Update existing location',
  );
  $form['update']['address'] = array(
    '#type' => 'textfield',
	'#title' => 'Address',
	'#required' => TRUE,
	'#attributes' =>array('placeholder' => t('125 Mount Auburn Street')),
	'#size' => 50,
  ); 
  $form['id'] = array (
    '#type' => 'value',
	'#value' => $id  
  );
 
  $form['existing_values'] = array(
      '#type' => 'fieldset', //container for multiple fields
	  '#title' => t('Existing Values'),
    );
   $form['existing_values']['address'] = array (
	   '#type' => 'item',
	   '#title' => 'Address',
	   '#markup' =>  $values['address']
	 );
	 
	$form['submit'] =array (
           '#type' => 'submit',
	       '#value' => 'UPDATE'
		   );
	    
   $form['cancel'] = array(
      '#type' => 'button',
      '#value' => 'CANCEL',
      '#validate' => array(),
	     '#attributes' => array(
            'onClick' => 'history.go(-1); return true;'),
		'#post_render' => array('change_button_type'),	
		  
    );  
return $form;
}
/**
* creates a form for adding and updating locations 
*/
function create_form($form, $form_state, $form_type){
 
 $typeOptions = type_list();
  if ($form_type == 'add'){
    $form['add'] = array(
      '#type' => 'fieldset', 
	  '#title' => 'Add new Location',
    );
	}
	else if ($form_type == 'add_mult'){
    $form['add_mult'] = array(
      '#type' => 'fieldset', 
	  '#title' => 'Add multiple Locations',
    );
	}
  
 if ($form_type != 'add_mult'){
  $form[$form_type]['address'] = array(
    '#type' => 'textfield',
	'#title' => 'Address',
	'#required' => TRUE,
	'#attributes' =>array('placeholder' => t('125 Mount Auburn Street')),
	'#size' => 50,
  ); 
 }
  else {
    $form['add_mult']['data'] = array(
    '#type' => 'textarea' ,
	'#title' =>'Enter details for multiple locations',
	'#required' => TRUE,
	'#attributes' =>array('placeholder' => t('125 Mount Auburn Street')),
 );
   $form['add_mult']['format'] = array(
    '#type' => 'item' ,
	'#title' => 'Expected format',
	'#markup' =>  '<b><font color="red"> 125 Mount Auburn Street</font></b>'
 );
  }
  $form['submit'] =array (
       '#type' => 'submit',
	   '#value' => 'ADD'
	   );
	 
 	  
	 $form['cancel'] = array(
      '#type' => 'button',
      '#value' => 'CANCEL',
      '#validate' => array(),
	     '#attributes' => array(
            'onClick' => 'history.go(-1); return true;'),
		'#post_render' => array('change_button_type'),	
		  
    ); 
  
return $form;
}

/**
* changes button type 
*/
function change_button_type($markup, $element) {
    $markup = str_replace('type="submit', 'type="button', $markup);
    return $markup;
}
/*
* return_modal_form()
* parameters: form type: add, delete, delete_all or update, name of the location 
* in case of delete and update, id in the case of delete and update
* Returns: a modal form of add/delete/delete_all/update
*/
function return_modal_form($variable, $name=NULL, $id=NULL){
 if ($variable === 'add'){
  $form_state = array(
      'ajax' => TRUE,
      'title' => t('Add Location'),
    );
     $output = ctools_modal_form_wrapper('add_locations_form', $form_state);    
  }
if ($variable === 'delete'){
   $form_state = array(
    'ajax' => TRUE,
	'title' => 'Delete Location:  '.$name,
	'id' => $id
   );
   $output = ctools_modal_form_wrapper('delete_location_form', $form_state);
   }  
if ($variable === 'update')
 {
   $form_state = array(
      'ajax' => TRUE,
      'title' => t('Update Location'),
    );
 
    $output = ctools_modal_form_wrapper('update_locations_form', $form_state);
   }
 if ($variable === 'delete_all')
 {
   $form_state = array(
      'ajax' => TRUE,
      'title' => t('Delete All Locations'),
    );
 
    $output = ctools_modal_form_wrapper('delete_all_locations_form', $form_state);
   }
  if ($variable === 'add_mult')
 {
   $form_state = array(
      'ajax' => TRUE,
      'title' => t('Add Multiple Locations'),
    );
 
    $output = ctools_modal_form_wrapper('add_mult_locations_form', $form_state);
   }
   if ($variable === 'upload_file')
 {
   $form_state = array(
      'ajax' => TRUE,
      'title' => t('Upload Files'),
    );
 
    $output = ctools_modal_form_wrapper('upload_file_form', $form_state);
   }
   
  if (!empty($form_state['ajax_commands'])) {
      $output = $form_state['ajax_commands'];
    }
  if (!empty($form_state['executed'])) {

    ctools_add_js('ajax-responder');

    $output = array();
    $output[] = ctools_modal_command_dismiss();
    $output[] = ctools_ajax_command_reload();
     }
  return $output;
}

/*
* creates and returns array of map values
*/
function return_map_values($address,$lat,$lng){
  return $mapValue = array (
		'@address' => $address,
		'@lat' => $lat,
		'@lng' => $lng
		);
}
/*
* creates and returns map values in a table
*/
 function create_map_table(){
  // drupal_add_js(drupal_get_path('module', 'testmap') . '/testmap_locations.js', 'file');
   ctools_include('ajax');
   ctools_include('modal');
   ctools_modal_add_js();
   
   $delete_image = drupal_get_path('module', 'testmap') . '/images/th.jpg' ;
   $update_image = drupal_get_path('module', 'testmap') . '/images/update.jpg' ;
   $header = create_table_header(); 
   $rows =array();
   $map_data = paging_map_data();
   $output ='';
   $links = '';
   $add_button = make_button('Add');
   $links = drupal_render($add_button);
   $delete_all_button = make_button('Delete All');
   $links .= drupal_render($delete_all_button);
   $add_multiple_button = make_button('Add Multiple');
   $links .= drupal_render($add_multiple_button);   
   $upload_file_button = make_button('Upload File');
   $links .= drupal_render($upload_file_button); 
     
  try {
       
		
		foreach($map_data as $result) {
			$rows[] = array(
		//	'class' => array('location-row' => 'location-row-'. $result->id),
			'data' => array(
			$result->address,
			ctools_modal_image_button($delete_image, 'testmap/delete/nojs/'.$result->id, '', 'ctools-use-modal'),
			ctools_modal_image_button($update_image, 'testmap/update/nojs/'.$result->id, '', 'ctools-use-modal'),
				),
			); 
		} 
		$output .= $links;
		$output .=  theme('table', array('header' => $header, 'rows' => $rows, 'class' => 'location-row'));
	 	$output .= theme('pager');
		
			
	}
	catch (Exception $e){
	  drupal_set_message(t('Failed to create map table. Message = %message, query=%query', 
	       array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
	 }
	 //drupal_add_js('jQuery(document).ready(function () { alert(document.URL+"hello"); });', 'inline');
	return $output;
 
 }
  
  function make_button($value=NULL) {
  $form = array();
  if($value == 'Add'){
  $form['url'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => array('testmap-add-button-url')),
    '#value' => url('testmap/add/nojs'),
  );
   $form['ajax_button'] = array(
    '#type' => 'button',
    '#value' => 'Add new location',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#id' => 'testmap-add-button',
	);
   }
   else if ($value == 'Delete All') {
   $form['url'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => array('testmap-delete-all-button-url')),
    '#value' => url('testmap/delete_all/nojs'),
  );
   $form['ajax_button'] = array(
    '#type' => 'button',
    '#value' => 'Delete all locations',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#id' => 'testmap-delete-all-button',
	);
   
   
   }
   else if ($value == 'Add Multiple'){
   $form['url'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => array('testmap-add-mult-button-url')),
    '#value' => url('testmap/add_mult/nojs'),
  );
   $form['ajax_button'] = array(
    '#type' => 'button',
    '#value' => 'Add Multiple Locations',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#id' => 'testmap-add-mult-button',
	);
   }
   else if ($value == 'Upload File'){
   $form['url'] = array(
    '#type' => 'hidden',
    '#attributes' => array('class' => array('testmap-upload-file-button-url')),
    '#value' => url('testmap/upload/nojs'),
  );
   $form['ajax_button'] = array(
    '#type' => 'button',
    '#value' => 'Upload File',
    '#attributes' => array('class' => array('ctools-use-modal')),
    '#id' => 'testmap-upload-file-button',
	);
   
   
   }
   return $form;
}

function create_upload_file($form, $form_state){
 $form['upload'] = array(
      '#type' => 'fieldset', 
	  '#title' => t('Upload Files'),
    );
	
	$form['upload']['data']  = array(
	  '#type' => 'item',
	  '#title' => 'Expected data format',
	  '#markup' =>  '<b>125 Mount Auburn Street</b>'
	
	);
	$form['upload']['file_ext']  = array(
	  '#type' => 'item',
	  '#title' => 'Expected file extension',
	  '#markup' =>  '<b>Only files with extension "csv" accepted this time.</b>'
	
	);	 
    $form['upload']['file'] = array (
	   '#type' => 'file',
	   '#title' => 'File: ',
	   '#id' => 'file',
	 //  '#description' => 'Upload a file, allowed extension: <font color="red"><b>csv</b></font>',
	   '#attributes' => array(
	    'onChange' => 'var ext=document.getElementById(\'file\').value.split(".").pop();
                  		if (ext != "csv") {
							alert("Pls upload a text or csv file");
							this.form.reset();
							return false;
						} 
						else {
						     return true;
						     }'),

	);
	  
	 
	 $form['upload']['reset'] = array(
	  '#type' => 'button',
      '#value' => 'Remove',
	  '#validate' => array(),
	  '#attributes' => array(
	    'onClick' => 'if (document.getElementById(\'file\').value == "")
		               {alert (\'No file selected to be removed\');
					    return false;}
					   else {
					     this.form.reset();
					     return false;
					   }')
	);
	
	 $form['upload']['submit'] =array (
       '#type' => 'submit',
	   '#value' => 'Upload',
	   '#id' => 'upload',
	   '#attributes' => array (
	    'onClick' => 'if (document.getElementById(\'file\').value == "")
		               {alert (\'Please select a file to upload\');
					    this.form.reset();
  					    return false;}
					   else {
					     return true;
					   }')
	   );

 return $form;
}

function parse_data($data){
      $location_values = array();
	 try{
      foreach(explode("\n", $data) as $line) {
		   $row = explode(',', $line);
	    	if ($row[0] !=''){
				$location_values = array(
				'address' => $row[0],
				'lat' => array_shift(get_latitude_longitude($row[0])),
				'lng' => array_pop(get_latitude_longitude($row[0])),
				);
				$success = map_insert_data($location_values);
				}
				
			}
		
		}
		catch (Exception $e) {
	      drupal_set_message(t('Issue in retrieving the location data. Message = %message, query=%query', 
	           array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');	
	     }
		
		return $success; 
	}
