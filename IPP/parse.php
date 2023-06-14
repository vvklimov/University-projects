<?php

ini_set('display_errors', 'stderr');

###ERROR LIST
define ("SYNTAX_ERROR", 23);
define ("HEADER_ERROR", 21);
define ("PARAMETER_ERROR", 10);
define ("OPCODE_ERROR", 22);
#################################

##### XML tree settings, creates header and program container
$xml_doc = new DOMDocument('1.0', 'UTF-8');
$xml_doc->formatOutput = true;
$program_tag = $xml_doc->createElement("program");
$program_tag->setAttribute("language", "IPPcode23");
$program_tag = $xml_doc->appendChild($program_tag);
########################################

$header = false;
$headercnt = 0;
$ordercnt = 1; //we want to count orders' number

if ($argc > 1)
{
  for ($i = 0; $i < $argc; $i++)
  {
    if ($argv[$i] == "--help")
      if ($argc == 2)
      {
        echo("Usage: parse.php [filename] | [--help]\n");
        echo("Where 'filename' is the name of the file you want to send to STDIN with content written in IPPcode23 language,\n");
        echo("'--help' - writes manual of parse.php to STDOUT, extra arguments with '--help' argument are forbidden\n");
        echo("parse.php transletes from IPPcode23 language to Extensible Markup Language (XML)\n");
        exit(0);
      }
      else
        exit(PARAMETER_ERROR);

  }
    
}

  ############ REGEXES ############
  $var_regex = "^(LF|GF|TF)@[A-Za-z_\-$&%*!?][A-Za-z0-9_\-$&%*!?]*";
  $label_regex = "^[A-Za-z_\-$&%*!?][\\w\\d_\-$&%*!?]*$";
  $dt_int_regex = "^int@[+-]?\d+";
  $dt_string_regex = "^string@[\\S]*";
  $dt_bool_regex = "^bool@(true|false)";
  $dt_nil_regex = "^nil@nil$";
  $symb_regex = "^($var_regex)|($dt_int_regex)|($dt_string_regex)|($dt_nil_regex)|($dt_bool_regex)$";
  $type_regex = "^(int|bool|string|nil)";
  $forbidden_string_regex = "^\\S*\\\((\\D)|([0-9]{1}\\D)|([0-9]{2}\\D))\\S";
  $forbidden_backslash_at_the_end_regex = "\\\\$";
  #########################


#### specifing type of parsing arguments 
function arg_scanner($string)
{
  global $var_regex, $label_regex, $dt_int_regex, $dt_string_regex, $dt_bool_regex, $dt_nil_regex, $type_regex, $forbidden_string_regex, $forbidden_backslash_at_the_end_regex;
  if (preg_match("/$var_regex/", $string))
    $arg_type = "var";
  elseif (preg_match("/$dt_int_regex/", $string))
    $arg_type = "int";
  elseif (preg_match("/$dt_string_regex/", $string))
    {
      if (preg_match("/$forbidden_string_regex/", $string) || preg_match("/$forbidden_backslash_at_the_end_regex/", $string))
        exit(SYNTAX_ERROR); ###checking problematic sequence of symbols
      else
        $arg_type = "string";
    }
  elseif (preg_match("/$dt_bool_regex/", $string))
    $arg_type = "bool";
  elseif (preg_match("/$dt_nil_regex/", $string))
    $arg_type = "nil";
  elseif (preg_match("/$type_regex/", $string))
    $arg_type = "type";
  elseif (preg_match("/$label_regex/", $string))
    $arg_type = "label";
  return $arg_type; 
}
#################################

  #### building whole structure of containers inside program container
function instr_build($splitted)
{ 
  global $xml_doc, $ordercnt, $program_tag, $type_regex;
  ### building instruction container
  if (count($splitted) > 0)
  {
    $instruction_tag = $xml_doc->createELement("instruction");
    $instruction_tag->setAttribute("order", $ordercnt);
    $instruction_tag->setAttribute("opcode", strtoupper($splitted[0]));
    $program_tag->appendChild($instruction_tag);
  }
  #### building arguments container
  if (count($splitted) > 1)
  { 
    $splitted_value = $splitted[1];
    $type = arg_scanner($splitted[1]);
    if (preg_match("/$type_regex/", $type))
      $splitted_value = preg_replace("/^($type_regex)@/", '' ,$splitted[1]);
    $arg1 = $xml_doc->createElement("arg1", htmlspecialchars($splitted_value));
    $arg1->setAttribute("type", $type);
    $instruction_tag->appendChild($arg1);
  }
  if (count($splitted) > 2)
  {
    $splitted_value = $splitted[2];
    $type = arg_scanner($splitted[2]);
    if (preg_match("/$type_regex/", $type))
      $splitted_value = preg_replace("/^($type_regex)@/", '' ,$splitted[2]);
    $arg2 = $xml_doc->createElement("arg2", htmlspecialchars($splitted_value));
    $arg2->setAttribute("type", $type);
    $instruction_tag->appendChild($arg2);
  }
  if (count($splitted) > 3)
  {
    $splitted_value = $splitted[3];
    $type = arg_scanner($splitted[3]);
    if (preg_match("/$type_regex/", $type))
      $splitted_value = preg_replace("/^($type_regex)@/", '' ,$splitted[3]);
    $arg3 = $xml_doc->createElement("arg3", htmlspecialchars($splitted_value));
    $arg3->setAttribute("type", $type);
    $instruction_tag->appendChild($arg3);
  }
  $ordercnt++;
}
  

while( $line = fgets(STDIN) ) 
{
  global $ordercnt;
  $splitted = explode('#', $line); #remove comments
  $splitted = preg_replace("/\s+/", ' ', $splitted); #removing multiple spaces
  $splitted = explode(' ', trim($splitted[0])); #str.split(), trim() strips whitespaces

  if (strlen($splitted[0]) > 0) #check if it is not an empty string after formatting
  {
    if (!$header)
    {
      if ($splitted[0] == ".IPPcode23")
      {
        $header = true;
      }
      else
        exit(HEADER_ERROR); #missing or wrong
    }
  }
  
  if (strlen($splitted[0]) > 0) ###skipping empty lines
    switch(strtoupper(trim($splitted[0])))
    { 
      #header skip
      case strtoupper(trim('.IPPcode23')): 
        $headercnt++;
        if ($headercnt > 1)
          exit(OPCODE_ERROR);
        break;
      
      ###### keyword <var>
      case 'POPS':
      case 'DEFVAR':
        if (preg_match("/$var_regex/", $splitted[1]) && count($splitted) == 2)
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;
        
      ##### keyword <var> <symb>
      case 'TYPE':
      case 'INT2CHAR':
      case 'STRLEN':  
      case 'MOVE':
      case 'NOT':
        if (count($splitted) == 3 && preg_match("/$var_regex/", $splitted[1]) && preg_match("/$symb_regex/", $splitted[2]))
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;
      
      #### keyword without arguments
      case 'CREATEFRAME':
      case 'POPFRAME':
      case 'PUSHFRAME':
      case 'BREAK':
      case 'RETURN':
        if (count($splitted) == 1)
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;
      
      ##### keyword <label>
      case 'CALL':
      case 'LABEL':
      case 'JUMP':
        if (preg_match("/$label_regex/", $splitted[1]) && count($splitted) == 2)
        instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;

      ### keyword <symb>
      case 'PUSHS':
      case 'WRITE':
      case 'EXIT':
      case 'DPRINT':
        if (preg_match("/$symb_regex/", $splitted[1]) && count($splitted) == 2)
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;

      ### keyword var <symb1> <symb2>
      case 'ADD':
      case 'SUB':
      case 'MUL':
      case 'IDIV':
      case 'LT':
      case 'GT':
      case 'EQ':
      case 'AND':
      case 'OR':
      case 'STRI2INT':
      case 'CONCAT':
      case 'GETCHAR':
      case 'SETCHAR':
      if (count($splitted) == 4 &&
      preg_match("/$var_regex/", $splitted[1]) &&
      preg_match("/$symb_regex/", $splitted[2]) &&
      preg_match("/$symb_regex/", $splitted[3]))
        instr_build($splitted);
      else
        exit(SYNTAX_ERROR);
      break;
      ### keyword <var> <type>
      case 'READ':
        if (count($splitted) == 3 && preg_match("/$var_regex/", $splitted[1]) && preg_match("/$type_regex$/", $splitted[2]))
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;

      ### keyword <label> <symb1> <symb2>
      case 'JUMPIFEQ':
      case 'JUMPIFNEQ':
        if (count($splitted) == 4 &&
        preg_match("/$label_regex/", $splitted[1]) &&
        preg_match("/$symb_regex/", $splitted[2]) &&
        preg_match("/$symb_regex/", $splitted[3]))
          instr_build($splitted);
        else
          exit(SYNTAX_ERROR);
        break;

      default:
      exit(OPCODE_ERROR);
      
    }
  }
  echo $xml_doc->saveXML();

?>