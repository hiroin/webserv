#!/usr/bin/perl

use CGI;
my $cgi = new CGI;
print $cgi->header;
my @params = $cgi->param();
foreach my $param (@params) {
 print $param."=".$cgi->param($param),"\n";
}

print "[ARGV[0]] => $ARGV[0]\n";
print "[ARGV[1]] => $ARGV[1]\n";
print "[QUERY_STRING] => $ENV{'QUERY_STRING'}\n";
print "[GATEWAY_INTERFACE] => $ENV{'GATEWAY_INTERFACE'}\n";
print "[PATH_INFO] => $ENV{'PATH_INFO'}\n";
print "[PATH_TRANSLATED] => $ENV{'PATH_TRANSLATED'}\n";
print "[QUERY_STRING] => $ENV{'QUERY_STRING'}\n";
print "[REMOTE_ADDR] => $ENV{'REMOTE_ADDR'}\n";
print "[REQUEST_METHOD] => $ENV{'REQUEST_METHOD'}\n";
print "[REQUEST_URI] => $ENV{'REQUEST_URI'}\n";
print "[SCRIPT_NAME] => $ENV{'SCRIPT_NAME'}\n";
print "[SCRIPT_FILENAME] => $ENV{'SCRIPT_FILENAME'}\n";
print "[SERVER_NAME] => $ENV{'SERVER_NAME'}\n";
print "[SERVER_PORT] => $ENV{'SERVER_PORT'}\n";
print "[SERVER_PROTOCOL] => $ENV{'SERVER_PROTOCOL'}\n";
print "[SERVER_SOFTWARE] => $ENV{'SERVER_SOFTWARE'}\n";
print "[AUTH_TYPE] => $ENV{'AUTH_TYPE'}\n";
print "[CONTENT_LENGTH] => $ENV{'CONTENT_LENGTH'}\n";
print "[CONTENT_TYPE] => $ENV{'CONTENT_TYPE'}\n";
print "[REMOTE_IDENT] => $ENV{'REMOTE_IDENT'}\n";
print "[REMOTE_USER] => $ENV{'REMOTE_USER'}\n";
