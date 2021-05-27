#!/usr/bin/perl

use CGI;
my $cgi = new CGI;
print $cgi->header;
my @params = $cgi->param();
foreach my $param (@params) {
 print $param."=".$cgi->param($param),"<br>\n";
}

print "$ARGV[0]<br>\n";
print "$ARGV[1]<br>\n";
print "$ENV{'QUERY_STRING'}<br>\n";
print "$ENV{'GATEWAY_INTERFACE'}<br>\n";
print "$ENV{'PATH_INFO'}<br>\n";
print "$ENV{'PATH_TRANSLATED'}<br>\n";
print "$ENV{'QUERY_STRING'}<br>\n";
print "$ENV{'REMOTE_ADDR'}<br>\n";
print "$ENV{'REQUEST_METHOD'}<br>\n";
print "$ENV{'REQUEST_URI'}<br>\n";
print "$ENV{'SCRIPT_NAME'}<br>\n";
print "$ENV{'SCRIPT_FILENAME'}<br>\n";
print "$ENV{'SERVER_NAME'}<br>\n";
print "$ENV{'SERVER_PORT'}<br>\n";
print "$ENV{'SERVER_PROTOCOL'}<br>\n";
print "$ENV{'SERVER_SOFTWARE'}<br>\n";
print "$ENV{'AUTH_TYPE'}<br>\n";
print "$ENV{'CONTENT_LENGTH'}<br>\n";
print "$ENV{'CONTENT_TYPE'}<br>\n";
print "$ENV{'REMOTE_IDENT'}<br>\n";
print "$ENV{'REMOTE_USER'}<br>\n";
