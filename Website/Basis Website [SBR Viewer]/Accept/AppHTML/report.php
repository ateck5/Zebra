<?php
// ----------------------------------------------------------------------------
// Zoom Search Engine Statistics Script 1.4 (15/May/2009)
// Log File Parser
// A PHP Script for generateing statistics from a Zoom Search log file.
// Copyright (C) Wrensoft 2000 - 2009
//
// This script is designed for PHP 4.3+ only.
//
// email: zoom@wrensoft.com
// www: http://www.wrensoft.com
// ----------------------------------------------------------------------------
if(strcmp('4.3.0', phpversion()) > 0)
    die("This version of the script requires PHP 4.3.0 or higher.<br />");
    
if (version_compare(phpversion(), '5.0.0', '<')) 
{
	eval('
    function clone($object) {
      return $object;
    }
    ');
}

// For versions of PHP before 4.1.0
// we will emulate the superglobals by creating references
// NOTE: references created are NOT superglobals
if (!isset($_SERVER) && isset($HTTP_SERVER_VARS))
	$_SERVER = &$HTTP_SERVER_VARS;   

$SelfURL = $_SERVER['PHP_SELF'];

//------------------------------------------------------------
//--------------------Customizable Settings------------------- 

//Name of the logfile to use.
$LOGFILENAME	= "searchwords.log";

//Name of temporary log file. This file will be created and written to during a trim operation.
//If the file already exists it will be overwritten.
$TEMPLOGNAME	= "_tempsearchwords.log";

//The length in in pixels of the longest bar in each bar chart .
$BAR_LENGTH = 300;

//Uncomment this line if you want a password to be required to generate a report. 
$PASSWORD_REPORT = "Rel24686"; //change "password" to be whatever you want the password to be.

//Uncomment this line if you want a password to be required to trim the log file. 
$PASSWORD_TRIM = "Rel24686"; //change "password" to be whatever you want the password to be.

//Set to 1 to enable trim functionality, 0 to disable
$ENABLE_TRIM = 1;

//Debug data on or off, you probably don't want to change this setting.
$_DEBUG			= 0;
?>

<HTML>
<head>
<meta http-equiv="content-type" content="text/html; charset=windows-1252" />
<title>Zoom Search Statistics</title>
<style type="text/css">
<!--
	/*Config Page Styling*/
	.err_text 		{color:#FF0000}  /*text displayed when there is an error with the input*/
	.input_err 		{background-color:#FFBB99} /*incorrect config field*/
	.config_table 	{} /*Table containing the report setup fields*/
	.trim_table		{} /*Table containing the trim setup fields*/
	
	/*Report page styling*/
	.report_table		{padding: 0px; border-color:#BBBBBB; border-style:solid; border-width:10px;} /*the tables containing the reports*/
	.report_table_header_row {background-color:#DCF0F0;}/*the header row in the tables containing the reports*/
	.report_table_header_col {padding-right: 15px;} /*the header columns in the tables containing the reports*/
	.report_table_row_even	{ background-color:#F0F0FF} /*the even rows in the tables containing the reports*/
	.report_table_odd_even	{} /*the odd rows in the tables containing the reports*/
	.report_table_col1  {padding-right: 15px} /*the first column in the tables containing the reports*/
	.report_table_col2	{padding-right: 15px} /*the second column in the tables containing the reports*/
	.report_table_col3	{padding-right: 15px} /*the third column in the tables containing the reports*/
	.report_table_barcol{ background-color:#FFFFFF } /*the column containing the bar graph in the tables containing the reports*/
	.graph_bar			{background-color:#AABB99} /*Color of the bars in the bar graphs*/
	.info_header		{} /*Info text at the top of the report (report generation date etc. */
	.report_error		{} /*Text on report generation error*/
	.reporttime			{} /*Time taken to generate report*/
	.zoomlink			{} /*Link to zoom homepage*/
	
-->
</style>
</head>
<body>
<?php

//------------------------------------------------------------
//-------------------------Definitions------------------------
#log file columns
$LOG_DATE 		= 0;
$LOG_TIME 		= 1;
$LOG_IP 		= 2;
$LOG_PHRASE 	= 3;
$LOG_MATCHES 	= 4;
$LOG_AND_OR 	= 5;
$LOG_PERPAGE 	= 6;
$LOG_PAGENUM 	= 7;
$LOG_CAT 		= 8;
$LOG_SEARCHTIME	= 9;
$LOG_REC 		= 10;

$MAX_RAW_OUTPUT = 5000;






//------------------------------------------------------------
//---------Read GET input and decide what to do---------------



//Set Default Values (for filling the form)

//Reports to output
$CONF_OUT_TOP10 = 1;
$CONF_OUT_TOP0 = 0;
$CONF_OUT_DAYS = 0;
$CONF_OUT_WEEKS = 0;
$CONF_OUT_MONTHS = 0;
$CONF_OUT_TOPX = 0;
$CONF_OUT_RAW = 0;

//Report Settings
$CONF_STARTDATE_STR = date ( "Y-m-01" ); //First day of current month
$CONF_FINDATE_STR = "now";
$CONF_FINNOW = 1;
$CONF_NUMTOPPHRASES = 100;
//Config errors
$ERR_STARTDATE = 0;
$ERR_FINDATE = 0;
$ERR_FINNOW = 0;
$ERR_NUMDAYS = 0;
$ERR_NUMWEEKS = 0;
$ERR_NUMMONTHS = 0;
$ERR_NUMTOPPHRASES = 0;



if (isset($_GET["report"])) //If they submited with the Create Report button.
	$OUTPUT_REPORT = 1; 
else
	$OUTPUT_REPORT = 0;

//Read/Verify GET values
//----------------------
if (isset($_GET["report"]) || isset($_GET["config"])) //Read configuration options
{
	//Start Date
	$CONF_STARTDATE_STR = $_GET["start"];
	$CONF_STARTDATE = strtotime($CONF_STARTDATE_STR);
	if ($CONF_STARTDATE == FALSE)
	{
		$OUTPUT_REPORT = 0;
		$ERR_STARTDATE = 1;
		echo "<P class=err_text>Bad Start Date: \"".$CONF_STARTDATE_STR."\"</P>\n";
	}
	
	//Finish Date
	$CONF_FINDATE_STR = $_GET["finish"];
	$CONF_FINDATE = strtotime($CONF_FINDATE_STR);
	if ($CONF_FINDATE == FALSE)
	{
		$OUTPUT_REPORT = 0;
		$ERR_FINDATE = 1;
		echo "<P class=err_text>Bad End Date: \"".$CONF_FINDATE_STR."\"</P>\n";
	}
	
	//is finish date "now"
	if (strcasecmp($_GET["finish"], "now") == 0)
		$CONF_FINNOW = 1;
	else
		$CONF_FINNOW = 0;
		
	
	
	//Check Start isn't after Finish (if both dates are valid)
	if ($CONF_STARTDATE > $CONF_FINDATE && !$ERR_STARTDATE && !$ERR_FINDATE)
	{
		$OUTPUT_REPORT = 0;
		$ERR_STARTDATE = 1;
		$ERR_FINDATE = 1;
		echo "<P class=err_text>Start date (".date_format($CONF_STARTDATE, "Y-m-d").") is after finish date (".date_format($CONF_FINDATE, "Y-m-d").").</P>\n";
	}
	
	//Get the toggle options for the reports. The form should return a value of 1 if they are on or nothing otherwise.
	$CONF_OUT_TOP10 = 0;
	$CONF_OUT_TOP0 = 0;
	$CONF_OUT_DAYS = 0;
	$CONF_OUT_WEEKS = 0;
	$CONF_OUT_MONTHS = 0;
	$CONF_OUT_TOPX = 0;
	$CONF_OUT_RAW = 0;
	
	if (isset($_GET["TOP10"]))
		$CONF_OUT_TOP10 = 1;
	if (isset($_GET["TOP0"]))
		$CONF_OUT_TOP0 = 1;
	if (isset($_GET["DAYS"]))
		$CONF_OUT_DAYS = 1;
	if (isset($_GET["WEEKS"]))
		$CONF_OUT_WEEKS = 1;
	if (isset($_GET["MONTHS"]))
		$CONF_OUT_MONTHS = 1;
	if (isset($_GET["TOPX"]))
		$CONF_OUT_TOPX = 1;
	if (isset($_GET["RAW"]))
		$CONF_OUT_RAW = 1;
	
	//If none of the reports are selected don't try and output a report and display a warning.
	if (!$CONF_OUT_TOP10 && !$CONF_OUT_TOP0 && !$CONF_OUT_DAYS && !$CONF_OUT_WEEKS && !$CONF_OUT_MONTHS && !$CONF_OUT_TOPX && !$CONF_OUT_RAW) 
	{
		$OUTPUT_REPORT = 0;
		echo "<P class=err_text>No reports selected for output. Please select at least one report.</P>\n";
	}
	
	
	if ($CONF_OUT_TOPX)
	{
		$CONF_NUMTOPPHRASES = intval($_GET["num_phrases"]); //Number of phrases to show in top phrases list
		if ($CONF_NUMTOPPHRASES < 1)
		{
			$OUTPUT_REPORT = 0;
			$ERR_NUMTOPPHRASES = 1;
			echo "<P class=err_text>Incorrect Number of Phrases, please select a number greater than 0</P>\n";
		}
	}
	
	
}

if (isset($PASSWORD_REPORT) && isset($_GET["report"])) //If report password required check if it is there and correct and we are trying to output a report.
{
	if ($_GET["password"] !== $PASSWORD_REPORT)
	{
		$OUTPUT_REPORT = 0;
		$ERR_REPPASS = 1;
		echo "<P class=err_text>Bad Password</P>\n";
	}
}
	
//If the trim button was pressed parse different options
if (isset($_GET["trim"]) && $ENABLE_TRIM)
{
	$CONFIRM_TRIM_PAGE = 1;
	//Trim date
	$TRIM_DATE_STR = $_GET["trim_date"];
	if (strtotime($TRIM_DATE_STR) == FALSE)
	{
		$CONFIRM_TRIM_PAGE = 0;
		$ERR_TRIMDATE = 1;
		echo "<P class=err_text>Bad Trim Date: \"".$TRIM_DATE_STR."\"</P>\n";
	}
	
	if (isset($PASSWORD_TRIM)) //If trim password required check if it is there and correct.
	{
		if ($_GET["password"] !== $PASSWORD_TRIM)
		{
			$CONFIRM_TRIM_PAGE = 0;
			$ERR_TRIMPASS = 1;
			echo "<P class=err_text>Bad Password</P>\n";
		}
	}
}


//If the user confirmed the trim parse different options
if (isset($_GET["trim_confirm"]) && $ENABLE_TRIM && !isset($ERR_TRIMDATE))
{
	//Trim date
	$TRIM_DATE_STR = $_GET["trim_date"];
	$TRIM_COMPLETE = 1;
	
	if (strtotime($TRIM_DATE_STR) == FALSE) //This should never happen as we have already checked it on the trim confirm page, check anyway.
	{
		$ERR_TRIMDATE = 1;
		$TRIM_COMPLETE = 0;
		echo "<P class=err_text>Bad Trim Date: \"".$TRIM_DATE_STR."\"</FONT></P>\n";
	}
	if (isset($PASSWORD_TRIM)) //Check at this stage too so people who know the format can't just skip knowing the password
	{
		if ($_GET["password"] !== $PASSWORD_TRIM)
		{
			$ERR_TRIMPASS = 1;
			$TRIM_COMPLETE = 0;
			echo "<P class=err_text>Bad Password</FONT></P>\n";
		}
	}
	
	if ($TRIM_COMPLETE != 0) //Trim the log file
	{
		$TRIM_DATE = strtotime($TRIM_DATE_STR);
		$inHandle = fopen($LOGFILENAME, 'r');
		$outHandle = fopen($TEMPLOGNAME, 'w');
		if ($inHandle !== FALSE || $outHandle !== FALSE )
		{
			while ( !feof($inHandle) )
			{
				$tempLine = fgets($inHandle);
				$tempExp = explode(',',$tempLine);
				$tempDate = strtotime($tempExp[$LOG_DATE]);
				if ($tempDate >= $TRIM_DATE) //Only copy records after this date.
				{
					fwrite($outHandle, $tempLine);	
				}
			}
			fclose($inHandle);
			fclose($outHandle);
			
			
			unlink ($LOGFILENAME);
			rename ($TEMPLOGNAME, $LOGFILENAME);
		}
		$TRIM_COMPLETE = 1;
	
	}
}


//------------------------------------------------------------
//--------------------Trim Confirm Page-----------------------
//------------------------------------------------------------
if (isset($CONFIRM_TRIM_PAGE) && $ENABLE_TRIM && !isset($ERR_TRIMDATE))
{
	echo "<h1>Confirm Trim</h1>\n";
	$tempDate = strtotime($TRIM_DATE_STR);
	echo "<P>Confirm delete all records dated before: ".date("Y-m-d", $tempDate)."<br>\n";
	echo "This action will <u>permanently delete</u> these records from the logfile and cannot be undone, are you sure you want to continue.</P>\n";
	echo "<form action=\"\" method=\"get\" name=\"config\">\n";
	echo "<input name=\"trim_date\" type=\"hidden\" value=\"$TRIM_DATE_STR\" />\n";
	if (!isset($PASSWORD_TRIM))
		$PASSWORD_TRIM = '';
	echo "<input name=\"password\" type=\"hidden\" value=\"$PASSWORD_TRIM\" />\n";
	echo "<input name=\"trim_confirm\" type=\"submit\" value=\"Confirm Trim Log\"/> (this may take several minutes)<br>\n";
	echo "</form>\n";
	echo "<P><A HREF=\"".$SelfURL."\">Return to report setup</A></P>";
}

//------------------------------------------------------------
//---------------------Trim Finish Page-----------------------
//------------------------------------------------------------
if (isset($TRIM_COMPLETE) && $ENABLE_TRIM)
{
	echo "<h1>Trim Completed</h1>\n";
	$tempDate = strtotime($TRIM_DATE_STR);
	echo "<P>All records dated before: ".date("Y-m-d", $tempDate)." were deleted<br>\n";
	echo "<A HREF=\"".$SelfURL."\">Return to report setup</A></P>";
	echo "</form>\n";
}

//------------------------------------------------------------
//--------------------Configuration Form----------------------
//------------------------------------------------------------
	
//If the input wasn't present or it was incorrect show the config screen
if (!$OUTPUT_REPORT && ((!isset($CONFIRM_TRIM_PAGE) && !isset($TRIM_COMPLETE)) || isset($ERR_TRIMDATE))) //not outputting a report, or outputing trim confirmation, so therefore outputing config screen
{
	$checked = " checked=\"checked\"";
	$err_class = " class=\"input_err\"";
	
	
	//Report configuration form
	echo "<h1>Zoom Search Statistics</h1>\n";
	echo "<p>Select reports and date range.</P>";
	echo "<form action=\"\" method=\"get\" name=\"config\">\n";
	
	echo "<table class=\"config_table\">";
	
	echo "<TR><TD>Start Date:</TD><TD><input name=\"start\" type=\"text\" value=\"$CONF_STARTDATE_STR\" size=\"20\"";		
	if ($ERR_STARTDATE) {echo $err_class;} 		
	echo " /> (yyyy-mm-dd)</TD></TR>\n";
	
	echo "<TR><TD>Finish Date:</TD><TD><input name=\"finish\" type=\"text\" value=\"$CONF_FINDATE_STR\" size=\"20\"";
	if ($ERR_FINDATE) {echo $err_class;} 
	echo " /> (yyyy-mm-dd)</TD></TR>\n";
	
	echo "<TR><TD>Top 10 Search Phrases</TD><TD><input name=\"TOP10\" type=\"checkbox\" value=1";	
	if ($CONF_OUT_TOP10) 	{echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>Top 10 Search Phrases that returned no matches</TD><TD><input name=\"TOP0\" type=\"checkbox\" value=1";	
	if ($CONF_OUT_TOP0) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>Searches per Day over</TD><TD><input name=\"DAYS\" type=\"checkbox\" value=1";	
	if ($CONF_OUT_DAYS) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>Searches per Week</TD><TD><input name=\"WEEKS\" type=\"checkbox\" value=1";	
	if ($CONF_OUT_WEEKS) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>Searches per Month</TD><TD><input name=\"MONTHS\" type=\"checkbox\" value=1";
	if ($CONF_OUT_MONTHS) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>List the top <input name=\"num_phrases\" type=\"text\" value=\"$CONF_NUMTOPPHRASES\" size=\"3\"";
	if ($ERR_NUMTOPPHRASES) {echo $err_class;} 
	echo " /> search words</TD><TD>	<input name=\"TOPX\" type=\"checkbox\" value=1";	
	if ($CONF_OUT_TOPX) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	echo "<TR><TD>Output Raw Data</TD><TD><input name=\"RAW\" type=\"checkbox\" value=1";
	if ($CONF_OUT_RAW) {echo $checked;} 
	echo " /></TD></TR>\n";
	
	if (isset($PASSWORD_REPORT))
	{
		echo "<TR><TD>Password:</TD><TD><input name=\"password\" type=\"password\" size=\"20\"";
		if ($ERR_REPPASS) 	{echo $err_class;} 	
		echo " /></TD></TR>\n";
	}
	echo "</TABLE>\n";
	echo "<input name=\"report\" type=\"submit\" value=\"Create Report\"/> (this may take several minutes)<br>\n";
	echo "</form>\n";
	echo "<HR>";
	
	
	//Trim Log File Form
	if ($ENABLE_TRIM)
	{
		echo "<h1>Trim Log</h1>\n";
		echo "<P>Permanently delete records before a set date.</P>";
		echo "<form action=\"\" method=\"get\" name=\"trim\">\n";
		
		echo "<table class=\"trim_table\">";
		if (isset($PASSWORD_TRIM))
		{
			echo "<TR><TD>Password:</TD><TD><input name=\"password\" type=\"password\" size=\"20\"";
			if ($ERR_TRIMPASS) 	{echo $err_class;} 	
			echo " /></TD></TR>\n";
		}
		else
		{
			echo "<TR><TD colspan=2>Password checking disabled, please consider enabling a password for the trim function.</TD></TR>";
		}
		
		if (!isset($TRIM_DATE_STR))
			$TRIM_DATE_STR = '';
		echo "<TR><TD>Trim Entries Before:</TD><TD><input name=\"trim_date\" type=\"text\" value=\"$TRIM_DATE_STR\" size=\"20\"";
		if (isset($ERR_TRIMDATE)) 	{echo $err_class;} 	
		echo " /> (yyyy-mm-dd)</TD></TR>\n";
		
		echo "</TABLE>\n";
		echo "<input name=\"trim\" type=\"submit\" value=\"Trim Log\"/><br>\n";
		echo "</form>\n";
		echo "<HR>";
	}
	
	
	echo "<div class=\"zoomlink\"><br /><A HREF=\"http://wrensoft.com/zoom/index.html\">Zoom Search Engine</A></div>\n";
}//if (!$OUTPUT_REPORT)






//------------------------------------------------------------
//-----------------------Report Section-----------------------
//------------------------------------------------------------

//If the input was present and correct output the report
if ($OUTPUT_REPORT)
{
	
	$Report_Fatal_Error = 0; //If we encounter an error set this to something other than zero (one of the below codes)
	$RFA_FailedToOpen 	= 1;
	$RFA_OutOfMemory 	= 2;
	$RFA_NoRecords	 	= 3;
	$RFA_NotLogFile		= 4;
	//------------------------------------------------------------
	//------------------------Start Timing------------------------
	$mtime = explode(" ", microtime());
	$starttime = doubleval($mtime[1]) + doubleval($mtime[0]);
	
	//------------------------------------------------------------
	//------------------------Output Header-----------------------
	echo "<h1>Zoom Search Statistics</h1>\n";
	echo "<P class=\"info_header\">Generated at ".date("H:i:s j M Y")."<BR />\n";
	echo "Date Range: ".date("Y-m-d", $CONF_STARTDATE)." to ".date("Y-m-d", $CONF_FINDATE)."</P>\n";
	
	//Form to return to the config page, send current config back with hidden vars
	echo "<form action=\"\" method=\"get\" name=\"config\">\n";
	
	echo "<input name=\"start\" type=\"hidden\" value=\"$CONF_STARTDATE_STR\" />\n";
	echo "<input name=\"finish\" type=\"hidden\" value=\"$CONF_FINDATE_STR\" />\n";
	
	if ($CONF_OUT_TOP10)
		echo "<input name=\"TOP10\" type=\"hidden\" value=1 />\n";
	if ($CONF_OUT_TOP0)
		echo "<input name=\"TOP0\" type=\"hidden\" value=1 />\n";
	if ($CONF_OUT_DAYS)
		echo "<input name=\"DAYS\" type=\"hidden\" value=1 />\n";
	if ($CONF_OUT_WEEKS)
		echo "<input name=\"WEEKS\" type=\"hidden\" value=1 />\n";
	if ($CONF_OUT_MONTHS)
		echo "<input name=\"MONTHS\" type=\"hidden\" value=1 />\n";
	if ($CONF_OUT_RAW)
		echo "<input name=\"RAW\" type=\"hidden\" value=1 />\n";

	
	if ($CONF_OUT_TOPX)
	{
		echo "<input name=\"TOPX\" type=\"hidden\" value=1 />\n";
		echo "<input name=\"num_phrases\" type=\"hidden\" value=\"$CONF_NUMTOPPHRASES\">\n";
	}
	
	echo "<input name=\"config\" type=\"submit\" value=\"Change Settings\"/><br>\n";
	echo "</form>\n";
	echo "<hr>\n";



	//------------------------------------------------------------
	//----------------------Read in Log File----------------------
	$handle = fopen($LOGFILENAME, 'r');
	
	if ($handle === FALSE)
		$Report_Fatal_Error = $RFA_FailedToOpen;
		
	if (!$Report_Fatal_Error)
	{
		//Figure out 95% of the total max memory configured by php is
		$memory_limit_95per = floatval(ini_get("memory_limit"));
		$memory_limit_95per *= 1048576*0.95;
		
		$row = 0;
		$bad_lines = 0;
		$tempDate = 0;
		while (($data[$row] = fgetcsv($handle, 500, ",")) !== FALSE)
		{
			
			//If the first 10 lines are all bad then assume this isn't a proper log file.
			if ($row == 0 && $bad_lines > 10)
			{
				$Report_Fatal_Error = $RFA_NotLogFile;
				break; //exit file parsing loop
			}
			//If the first value can't be parsed into a date skip the line
			$tempDate = strtotime($data[$row][$LOG_DATE]);
			if ($tempDate == FALSE) 
			{
				$bad_lines++;
				continue;
			}

			//Exclude anything outside our date range
			if ($tempDate > $CONF_FINDATE || $tempDate < $CONF_STARTDATE) 
			{
				//Do nothing (row will be overwritten)
				continue;
			}			
			//Do not include requests for subsequent pages
			else if (strcmp (trim($data[$row][$LOG_PAGENUM]), "PageNum = 0") != 0) 
			{
				//Do nothing (row will be overwritten)
				continue;
			}
			//Do not include lines with too many values (work around logs with old multi-ip address bug)
			else if (sizeof($data[$row]) > 11) 
			{
				//Do nothing (row will be overwritten)
				$bad_lines++;
				continue;
			}
			else
			{
				$data[$row][$LOG_PHRASE] = 	strtolower($data[$row][$LOG_PHRASE]); //Make all phrases lower case, do not differenciate.
				
				//Clean up vcolumn with redundant information
				$data[$row][$LOG_MATCHES] = str_replace("Matches = ", "", $data[$row][$LOG_MATCHES]);
				$data[$row][$LOG_PERPAGE] = str_replace("PerPage = ", "", $data[$row][$LOG_PERPAGE]);
				$data[$row][$LOG_PAGENUM] = str_replace("PageNum = ", "", $data[$row][$LOG_PAGENUM]);
				$data[$row][$LOG_CAT] = str_replace("Cat = ", "", $data[$row][$LOG_CAT]);
				$data[$row][$LOG_SEARCHTIME] = str_replace("Time = ", "", $data[$row][$LOG_SEARCHTIME]);
				$data[$row][$LOG_REC] = str_replace("Rec = ", "", $data[$row][$LOG_REC]);
				$row++;	
				
				
				//Do not exceed using 95% of the max memory usable by PHP. Otherwise assume 5% is enough for the rest of the script.
				if( function_exists('memory_get_usage') )
				{
					if (memory_get_usage() > $memory_limit_95per) // _FIXME_
					{
						$Report_Fatal_Error = $RFA_OutOfMemory;
						break; //exit file parsing loop
					}
				}
			}	
		}
		unset($data[$row]); //Last call to fgetcsv inserted a blank row, remove it.
		fclose($handle);
		
		
		if ($row == 0 && $Report_Fatal_Error == 0) //If we retrieved 0 rows and didn't encounter any other errors.
		{
			
			$Report_Fatal_Error = $RFA_NoRecords;
		}
		
		if ($_DEBUG)
		{
			echo "<P>Number of Records read in: $row<br>";
			echo "Memory limit: ".intval(ini_get("memory_limit"))."MB<br>";
			echo "95% of memory limit: ".round($memory_limit_95per/1048576)."MB<br>";
			echo "Total memory use: ".round(memory_get_usage()/1048576)."MB<br>";
			echo "Memory per record read: ".round(round(memory_get_usage()/1024)/$row, 3)."KB per row<br>";
		}
	}
	
	
	
	//------------------------------------------------------------
	//-----------------Most Popular Search Phrases----------------
	if ($CONF_OUT_TOP10 && !$Report_Fatal_Error)
	{
		//Get a count of each phrase.
		//works by using an associative array with each phrase acting as an index
		$total = 0;
		foreach ($data as $row)
		{
			if (!isset($assoc_arr[$row[$LOG_PHRASE]]))
				$assoc_arr[$row[$LOG_PHRASE]] = 0;
			$assoc_arr[$row[$LOG_PHRASE]]++;
			$total++;
		}
		
		arsort($assoc_arr); //Sort the phrases in descending order
		
		//Format data into array
		$i=0;
		$other_count = 0;
		foreach ( $assoc_arr as $phrase=>$val )
		{
			if ($i < 10) //If in the first ten output as is
			{
				$output_array[$i][0] = $phrase;
				$output_array[$i][1] = $val;
				$output_array[$i][2] = round(($val/$total)*100 , 1)."%"; //Percentage of total
				$i++;
			}
			else //Add the rest together
			{
				$other_count+=$val;
			}
		}
		//Add the last rows
		$output_array[$i][0] = "All other phrases";
		$output_array[$i][1] = $other_count;
		$output_array[$i][2] = round(($other_count/$total)*100 , 1)."%"; //Percentage of total
		$output_array[$i+1][0] = "Total";
		$output_array[$i+1][1] = $total;
		$output_array[$i+1][2] = round(($total/$total)*100 , 1)."%"; //Percentage of total
		
		//Header Array
		$header_array[0] = "Phrase";
		$header_array[1] = "Count";
		$header_array[2] = "Percent";
		
		echo "<h1>Most popular search phrases</h1>\n";
		arrayToTable($output_array, $header_array, 1); //Output data to table
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	//------------------------------------------------------------
	//-------------------Search Phrases Return 0------------------
	if ($CONF_OUT_TOP0 && !$Report_Fatal_Error)
	{
		//Get a count of each phrase.
		//works by using an associative array with each phrase acting as an index
		$total = 0;
		foreach ($data as $row)
		{
			if ($row[$LOG_MATCHES] == "0") //Only of rows whith 0 matches
			{
				if (!isset($assoc_arr[$row[$LOG_PHRASE]]))
					$assoc_arr[$row[$LOG_PHRASE]] = 0;
				$assoc_arr[$row[$LOG_PHRASE]]++;
				$total++;
			}
		}
		
		arsort($assoc_arr); //Sort the phrases in descending order
		
		//Format data into array
		$i=0;
		$other_count = 0;
		foreach ( $assoc_arr as $phrase=>$val )
		{
			if ($i < 10) //If in the first ten output as is
			{
				$output_array[$i][0] = $phrase;
				$output_array[$i][1] = $val;
				$output_array[$i][2] = round(($val/$total)*100 , 1)."%"; //Percentage of total
				$i++;
			}
			else //Add the rest together
			{
				$other_count+=$val;
			}
		}
		//Add the last rows
		$output_array[$i][0] = "All other phrases";
		$output_array[$i][1] = $other_count;
		$output_array[$i][2] = round(($other_count/$total)*100 , 1)."%"; //Percentage of total
		$output_array[$i+1][0] = "Total";
		$output_array[$i+1][1] = $total;
		$output_array[$i+1][2] = round(($total/$total)*100 , 1)."%"; //Percentage of total
		
		//Header Array
		$header_array[0] = "Phrase";
		$header_array[1] = "Count";
		$header_array[2] = "Percent";
		
		echo "<h1>Search phrases returning 0 results</h1>\n";
		arrayToTable($output_array, $header_array, 1); //Output data to table
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	
	
	//------------------------------------------------------------
	//----------------------Searches per Day----------------------
	if ($CONF_OUT_DAYS && !$Report_Fatal_Error)
	{
		//Intialize the array with 0's for every day we are generating for
		$tempDate = $CONF_FINDATE;
		while ($CONF_STARTDATE < $tempDate)
		{
			$tempStr = date("Y-m-d", $tempDate); //YYYY-MM-DD
			$assoc_arr[$tempStr] = 0;
			$tempDate = strtotime("-1 day", $tempDate);
		}
		
		
		//Parse Data
		$tempDate = 0;
		$total_searches = 0;
		foreach ($data as $row)
		{
			$tempDate = strtotime($row[$LOG_DATE]);
			if ($tempDate > $CONF_STARTDATE)
			{
				$assoc_arr[$row[$LOG_DATE]]++;
				$total_searches++;
			}
		}
		

		
		$i=0;
		$largest_val=-1;
		$largest_name="N/A";
		foreach ( $assoc_arr as $date=>$val )
		{
			$output_array[$i][0] = $date;
			$output_array[$i][1] = $val;
			//find the largest in the array
			if ($val > $largest_val)
			{
				$largest_val=$val;
				$largest_name=$date;
			}
			$i++;
		}
		
		//Header Array
		$header_array[0] = "Day";
		$header_array[1] = "Count";
		
		echo "<h1>Searches per day</h1>\n";
		$output_array = array_reverse($output_array); //Decided to present data in opposite order, this is a quick fix, recode for effeciency if needed.
		arrayToTable($output_array, $header_array, 1); //Output data to table
		echo "<p><b>Total number of searches: </b>".$total_searches."<br />\n";
		echo "<b>Most searches in a day: </b>".$largest_val." (".$largest_name.")<br />\n";
		echo "<b>Average searches per day: </b>".round($total_searches/count($output_array), 1)."</p>\n";
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	//------------------------------------------------------------
	//----------------------Searches per Week----------------------
	if ($CONF_OUT_WEEKS && !$Report_Fatal_Error)
	{
		//Find the monday of the last week
		$lastMonday = strtotime("last Monday", $CONF_FINDATE);
		
		//Find the sunday of the last week
		$lastSunday = strtotime("next Sunday", $CONF_FINDATE);
		
		//If the finish date was not set to now don't include the last week if it only partial
		if ($CONF_FINNOW !== 1 && $lastSunday > $CONF_FINDATE)
		{
			$lastMonday = strtotime("-1 week", $lastMonday);
			$lastSunday = strtotime("-1 week", $lastSunday);
		}
		
		//Intialize the array with 0's for every week we are generating for
		$tempMon  = $lastMonday;
		while ($CONF_STARTDATE <= $tempMon)
		{
			$tempStr = date("Y, \W\e\ek W", $tempMon); //YYYY Week W
			$assoc_arr[$tempStr] = 0;
			$tempMon = strtotime("-1 week", $tempMon);
		}
		
		
		//Parse Date
		$tempMon = strtotime("+1 week", $tempMon); //Earliest monday to include
		$tempDate = 0;
		$total_searches = 0;
		foreach ($data as $row)
		{
			$tempDate = strtotime($row[$LOG_DATE]);
			if ($tempDate >= $tempMon && $tempDate <= $lastSunday)
			{
				$tempStr = date("Y, \W\e\ek W", $tempDate); //YYYY Week W
				$assoc_arr[$tempStr]++;
				$total_searches++;
			}
		}
		
		
		//generate a more useful date name, week starts on mondays.
		//Set the title of each week to be the date of the monday of that week.
		$tempMon  = $lastMonday;
		$i=0;
		while ($CONF_STARTDATE <= $tempMon)
		{
			$tempStr = date("Y-m-d", $tempMon); //YYYY-MM-DD
			$output_array[$i][0] = $tempStr;
			$tempMon = strtotime("-1 week", $tempMon);
			$i++;
		}
		
		
		
		
		$i=0;
		$largest_val=-1;
		$largest_name="N/A";
		foreach ( $assoc_arr as $date=>$val )
		{
			$output_array[$i][1] = $val;
			//find the largest in the array
			if ($val > $largest_val)
			{
				$largest_val=$val;
				$largest_name=$output_array[$i][0];
			}
			$i++;
		}
		
		//Header Array
		$header_array[0] = "Week";
		$header_array[1] = "Count";
		
		echo "<h1>Searches per Week</h1>\n";
		if ($CONF_FINNOW == 0)
			echo "<p>Weeks begin on Monday's. Weeks that are only partialy in the date range have not been included.</p>\n";
		if ($CONF_FINNOW == 1)
			echo "<p>Weeks begin on Monday's. Weeks that are only partialy in the date range have not been included, however this weeks statistics to date have been.</p>\n";
		$output_array = array_reverse($output_array); //Decided to present data in opposite order, this is a quick fix, recode for effeciency if needed.
		arrayToTable($output_array, $header_array, 1); //Output data to table
		echo "<p><b>Total number of searches: </b>".$total_searches."<br />\n";
		echo "<b>Most searches in a week: </b>".$largest_val." (".$largest_name.")<br />\n";
		echo "<b>Average searches per week: </b>".round($total_searches/count($output_array), 1)."</p>\n";
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	
	
	//------------------------------------------------------------
	//---------------------Searches per Month---------------------
	if ($CONF_OUT_MONTHS && !$Report_Fatal_Error)
	{
	
		//Find the last day of the last month to include
		$lastDay = $CONF_FINDATE;
		//First find the first day of the month after the last month.
		if ($CONF_FINNOW == 0) //If not including the current month when partial
		{
			$lastDay = strtotime("+1 Day", $lastDay);
			$lastDay = strtotime(date("Y/m/01", $lastDay));
		}
		else
		{
			$lastDay = strtotime(date("Y/m/01", strtotime("+1 Month",$lastDay)));
		}
		$lastDay = strtotime("-1 Day", $lastDay); //Move to the last day of the previous month and that is our last day.
	
		//Find the first day of the last month
		$firstDay = $lastDay;
		$firstDay = strtotime(date("Y/m/01", $firstDay));
	
		//Intialize the array with 0's for every month we are generating for
		$tempDate = $lastDay;
		while ($firstDay >= $CONF_STARTDATE)
		{
			$tempStr = date("Y F", $firstDay); //eg. 2007 January
			$assoc_arr[$tempStr] = 0;
			$firstDay = strtotime("-1 month", $firstDay);
		}
		
		
		
		//Parse Data
		$firstDay = strtotime("+1 month", $firstDay);
		$total_searches = 0;
		$tempDate = 0;
		foreach ($data as $row)
		{
			$tempDate = strtotime($row[$LOG_DATE]);
			if ($tempDate >= $firstDay && $tempDate <= $lastDay)
			{
				$tempStr = date("Y F", $tempDate);
				$assoc_arr[$tempStr]++;
				$total_searches++;
			}
		}
		
		
		$i=0;
		$largest_val=-1;
		$largest_name="N/A";
		foreach ( $assoc_arr as $date=>$val )
		{
			$output_array[$i][0] = $date;
			$output_array[$i][1] = $val;
			//find the largest in the array
			if ($val > $largest_val)
			{
				$largest_val=$val;
				$largest_name=$date;
			}
			$i++;
		}
		
		//Header Array
		$header_array[0] = "Month";
		$header_array[1] = "Count";
		
		
		echo "<h1>Searches per Month</h1>\n";
		if ($CONF_FINNOW == 0)
			echo "<p>Months that are only partialy in the date range have not been included.</p>\n";
		if ($CONF_FINNOW == 1)
			echo "<p>Months that are only partialy in the date range have not been included, however this months statistics to date have been.</p>\n";
		$output_array = array_reverse($output_array); //Decided to present data in opposite order, this is a quick fix, recode for effeciency if needed.
		arrayToTable($output_array, $header_array, 1); //Output data to table
		echo "<p><b>Total number of searches: </b>".$total_searches."<br />\n";
		echo "<b>Most searches in a month: </b>".$largest_val." (".$largest_name.")<br />\n";
		echo "<b>Average searches per month: </b>".round($total_searches/count($output_array), 1)."</p>\n";
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	
	//------------------------------------------------------------
	//--------------------Top X Search Phrases--------------------
	if ($CONF_OUT_TOPX && !$Report_Fatal_Error)
	{
		//Get a count of each phrase.
		//works by using an associative array with each phrase acting as an index
		$total = 0;
		foreach ($data as $row)
		{
			if (!isset($assoc_arr[$row[$LOG_PHRASE]]))
				$assoc_arr[$row[$LOG_PHRASE]] = 0;
			$assoc_arr[$row[$LOG_PHRASE]]++;
			$total++;
		}
		
		arsort($assoc_arr); //Sort the phrases in descending order
		
		//Format data into array
		$i=0;
		$other_count = 0;
		foreach ( $assoc_arr as $phrase=>$val )
		{
			if ($i < $CONF_NUMTOPPHRASES) //If in the first ten output as is
			{
				$output_array[$i][0] = $phrase;
				$output_array[$i][1] = $val;
				$i++;
			}
			else //Add the rest together
			{
				$other_count+=$val;
			}
		}
	
		//Header Array
		$header_array[0] = "Phrase";
		$header_array[1] = "Count";
		
		echo "<h1>Top $CONF_NUMTOPPHRASES search phrases</h1>\n";
		arrayToTable($output_array, $header_array); //Output data to table
		echo "<hr>\n";
		
		//Free memory and clear arrays (we will reuse these vars so important that they are empty)
		unset($output_array);
		unset($assoc_arr);
		unset($header_array);
	}
	
	
	//!!!!!!!Should be done last, may make modifications to the initial data set. (too memory intensive to make a copy)
	//------------------------------------------------------------
	//------------------------Raw output--------------------------
	if ($CONF_OUT_RAW && !$Report_Fatal_Error)
	{
		
		//Header Array
		$header_array[0] = "Date";
		$header_array[1] = "Time";
		$header_array[2] = "IP Address";
		$header_array[3] = "Phrase";
		$header_array[4] = "Matches";
		$header_array[5] = "AND/OR";
		$header_array[6] = "Results per Page";
		$header_array[7] = "Page Number";
		$header_array[8] = "Categories";
		$header_array[9] = "Search Time";
		$header_array[10] = "Recommended Links";
		
		//Shrink the data set if it is too large for output
		$i=0;
		foreach ($data as $row)
		{
			$i++;
			if ($i>$MAX_RAW_OUTPUT)
				unset($row);
		}
		
		echo "<h1>Raw Data</h1>\n";
		echo "<P>Log entries for pages other than the first page have been filtered out.</P>\n";
		arrayToTable($data, $header_array); //Output data to table
		if ($i>$MAX_RAW_OUTPUT)
			echo "<P>Raw output truncated to $MAX_RAW_OUTPUT records.</P>\n";
		echo "<hr>\n";
		
		unset($header_array);
	}
	
	//------------------------------------------------------------
	//--------------Output info on any fatal errors---------------
	if ($Report_Fatal_Error)
	{
	
		$tempExp = explode('/', $_SERVER['PHP_SELF']); //Explode out the path to the script
		array_pop($tempExp); //remove the script itself from the array
		$tempPath = implode('/', $tempExp); //Put the path back together minus the scipt name itself
		
		if ($Report_Fatal_Error == $RFA_FailedToOpen )
		{
			echo "<P class=\"report_error\"><b>Error:<br /> Failed to open $tempPath/$LOGFILENAME, please check this file exists and that PHP has permission to access it.</b></P>";
		}
		
		if ($Report_Fatal_Error == $RFA_OutOfMemory )
		{
			echo "<P class=\"report_error\"><b>Error:<br /> The script has terminated because it has expended the allowable memory as configured by PHP.<br />" ;
			echo "Consider creating a report for a smaller date range, using the log file trim feature or increasing the value of memory_limit in php.ini.</b></P>";
		}
		
		if ($Report_Fatal_Error == $RFA_NoRecords )
		{
			echo "<P class=\"report_error\"><b>Error:<br /> There are no records within the specified date range or the log file ($tempPath/$LOGFILENAME) is invalid.</b></P>";
		}
		
		
		if ($Report_Fatal_Error == $RFA_NotLogFile )
		{
			echo "<P class=\"report_error\"><b>Error:<br /> Failed to parse $tempPath/$LOGFILENAME, file does not appear to be a valid Zoom Search log file.</b></P>";
		}
	}
	
	//------------------------------------------------------------
	//------------------------Finish Timing------------------------
	$mtime = explode(" ", microtime());
	$endtime   = doubleval($mtime[1]) + doubleval($mtime[0]);
	$difference = abs($starttime - $endtime);
	$timetaken = number_format($difference, 3, '.', '');
	echo "<div class=\"reporttime\"><br /><br /> Time to generate report: " . $timetaken . " Seconds</div>\n";
	
	//Output zoom link
	echo "<div class=\"zoomlink\"><br /><A HREF=\"http://wrensoft.com/zoom/index.html\">Zoom Search Engine</A></div>\n";
	
} //if ($OUTPUT_REPORT)













//------------------------------------------------------------
//--------------------------Functions-------------------------
//------------------------------------------------------------

//------------------------------------------------------------
//----------------------2D Array to Table---------------------

//Outputs a 2d array of any dimensions. If bgindex is set it also displays a bargraph using the values in
//the column specified by the index
//&$array2d - any 2d array with printable data in it (&$array2d[row][column])
//&$headerArr - array of column headers, should have as many columns as array2d
//$bgindex - bar graph index, refers to a column in array2d, uses this column to generate a bar graph in a final extra column (defaults to -1 = off)
function arrayToTable(&$array2d, &$headerArr, $bgindex=-1)
{
	global $BAR_LENGTH; //Length of the longest bar in the bar graph
	//If we are outputting a bargraph find the largest value so we can normalize
  	if ($bgindex >= 0)
	{
		$normalize=1;
		$largest=-1;
		foreach ($array2d as $row)
  		{
			if ($row[$bgindex] > $largest)
			{
				$largest = $row[$bgindex];
			}
		}
		if ($largest > 0)
		{
			$normalize = $BAR_LENGTH/$largest; //largest bar should be 300 pixels long
		}
	}
	
	//Make the table
  	echo "<p><table class=\"report_table\">\n";
	
	//Output header row
	echo "\t<TR class=\"report_table_header_row\">\n";
	foreach ($headerArr as $column)
	{
		echo "\t\t<TD class=\"report_table_header_col\">";
		echo $column;
		echo "</TD>\n";
	}
	
	$x=1;
  	foreach ($array2d as $row)
  	{
		if ($x == 0)
		{
  			echo "\t<TR class=\"report_table_row_even\">\n";
			$x = 1;
		}
		else
		{
			echo "\t<TR class=\"report_table_row_odd\">\n";
			$x = 0;
		}
		
		$i=1;
  		foreach ($row as $column)
		{
			echo "\t\t<TD class=\"report_table_col".$i."\">";
			echo htmlentities ($column);
			echo "</TD>\n";	
			$i++;
		}
		if ($bgindex >= 0) //Add another column with a bargraph representation
		{
			echo "\t\t<TD class=\"report_table_barcol\"><DIV class=\"graph_bar\" style=\"width:".round($row[$bgindex] * $normalize)."px\">&nbsp;</DIV></TD>\n";	
		}
		echo "\t</TR>\n";
  	}
  	echo "</table></p>\n";
}

//------------------------------------------------------------
//------------------------myDateCreate------------------------

//Takes a date in the format YYYY-MM-DD and puts into into a DateTime object
//Much more effecient than date_create which handles lots of possible date formats
//in: &$dateString - reference to a string formatted in YYYY-MM-DD format
//out: &$dateTimeObj - reference to a DateTime object to set, creating a DateTime object
//						has serious performance penalties so have one passed in so the caller
//						can reuse it (for looping)
/*function myDateCreate(&$dateTimeObj, &$dateString)
{
	$explodeDate = explode("-",$dateString);
	date_date_set($dateTimeObj, intval($explodeDate[0]), intval($explodeDate[1]), intval($explodeDate[2]));
}*/
?> 

</body>
</HTML>