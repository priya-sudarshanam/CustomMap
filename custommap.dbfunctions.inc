<?php
/**
* This file is for database functions like retrieving, 
* adding, deleting functions
*/

/**
* map_select_data
* Retrieve all existing data from the table('saimap')
* parameters: none
* returns all existing data from the table('saimap')
 */
 function map_select_data(){
    $table = 'saimap';
	$osmap_true = 1;
	$query = ('SELECT * FROM '. $table. ' WHERE ' . $osmap_true);
	
	try {
         $result = db_query($query);
	    }
	catch(Exception $e)	{
	           drupal_set_message(t('Issue in retrieving the location data. Message = %message, query=%query', 
	           array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
	    }
	 return $result->fetchAll();
}

/**
 * paging_map_data
 * parameters: none
 * Creating paging and sorting in the data table by extending 'pagerdefault' and 'sort'
 * returns paged and sorted data from the table('saimap')
 */
function paging_map_data(){
   $header = create_table_header();
   $table = 'saimap';
   try {
         $select = db_select($table)
                 ->fields($table, array('id','address'))
                 ->extend('PagerDefault')
				 ->limit(5)
                 ->extend('TableSort')
				 ->orderByHeader($header)
				 ->execute();
      }
	catch (Exception $e) {
	   drupal_set_message(t('Issue in retrieving the location data. Message = %message, query=%query', 
	           array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');	
	}
   
   return $select;
}

/**
 * Retrieve data from the table('saimap') for a given id
 * parameters: id
 * returns data from the table('saimap') for a given id
 */
 function map_select_data_id($id){
    $table = 'saimap';
	$alias = 's';
	try {
         $selected = db_select($table,$alias)
	               ->fields($alias)
			       ->condition('id',$id)
			       ->execute()
			       ->fetchAssoc();
	}
	catch(Exception $e)	{
	     drupal_set_message(t('Issue in retrieving the location data for location id= '.$id.'. Message = %message, query=%query', 
	           array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
	}
	return $selected;
}
	

/**
 * map_insert_data
 * Insert new location values into the table
 * parameters: array of values
 * returns success after successful insertion of data into table('saimap') 
 */
function map_insert_data($mapValues){
  $dataInsert = NULL;
  $table = 'saimap';
  try {
        $dataInsert = db_insert($table)
	                ->fields(array(
					  'address' => $mapValues['address'],
					  'lat' => $mapValues['lat'],
					  'lng' => $mapValues['lng'],
					  ))
					->execute();
     }
  catch(Exception $e) {
        drupal_set_message(t('Adding the new location failed. Message = %message, query=%query', 
	    array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
  }
  return $dataInsert; 
}

/**
 * map_update_data
 * Update location values in the table for the given location id
 * parameters: array of values,id
 * returns success after successful updation of data in table('saimap')
 */
function map_update_data($mapValues,$id){
  $table = 'saimap';
 try {
       $updated = db_update($table)
	            ->fields(array(
					  'address' => $mapValues['address'],
					  'lat' => $mapValues['lat'],
					  'lng' => $mapValues['lng'],
					  ))
	  		    ->condition('id',$id)
			    ->execute();  
  }
  catch (Exception $e){
    
     drupal_set_message(t('Issue in updating the location information  with location id:'.$id.' . Message = %message, query=%query', 
	 array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
  }
  return $updated; 
}

/**
 * map_delete_data
 * Delete location in the table for the given location id
 * parameters: id
 * returns success after successful deletion of data in table('saimap')
 */
function map_delete_data($delId){
     $table = 'saimap';
     try {
         $success = db_delete($table)
		       	  ->condition('id', $delId['id'])
		       	  ->execute();
		}
	catch(Exception $e){
	     drupal_set_message(t('Issue in deleting location: '.$row.'. Message = %message, query=%query', 
	     array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
	   }
	return $success;
 }
 
 /**
 * map_delete_all_data
 * Delete all locations in the table 
 * parameters: none
 * returns success after successful deletion of all data in table('saimap')
 */
 function map_delete_all_data(){
    $table = 'saimap';
    try {
         $success = db_delete($table)
		       	  ->execute();
		}
	catch(Exception $e){
	     drupal_set_message(t('Issue in deleting locations: '.$id.'. Message = %message, query=%query', 
	     array('%message' =>$e->getMessage(),'%query' => $e->query_string)), 'error');
	   }
	return $success;
 }
