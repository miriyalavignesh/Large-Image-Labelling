#!/usr/bin/perl
#order of arguments seedfile labelfile featurefile
my $result_file = 'varun_rsult.txt';
open my $result, '>', $result_file
  or die "Can't open your_filename: $!\n";

open my $file_handle, '<', $ARGV[0]
  or die "Can't open your_filename: $!\n";

while (<$file_handle>) {
chomp;
@file = split(/,/);
$file[0]=int($file[0]+0.5);
$file[1]=int($file[1]+0.5);
#print " $file[0] $file[1] $file[2]\n";
open my $file_handle2, '<', $ARGV[1]
  or die "Can't open your_filename: $!\n";
while (<$file_handle2>)
{
if($.==$file[1]){
chomp;
@array=split(/,/);
$index=$array[$file[0]-1];
open my $file_handle3, '<', $ARGV[2]
  or die "Can't open your_filename: $!\n";
#print "   $index\n";
while(<$file_handle3>)
{

if($.==$index)
{
chomp;
$line=$_;
$line.="   $file[2]\n";
print $result "$line";


}

}
last;

}
}}

close $result;
