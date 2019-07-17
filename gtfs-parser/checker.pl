#!/usr/bin/env perl

use strict;
use warnings;

if (@ARGV < 2) {
  die "Provide `stop_times.txt` `stops.txt`.\n";
}
my ($STOP_TIMES, $STOPS) = @ARGV;

open my $stops_fh, '<', $STOPS or die "Could not open stops.txt.\n";
my $stops_header = <$stops_fh>;
my %stops;
while (<$stops_fh>) {
  chomp;
  my @arr = split ',', $_;
  $stops{$arr[0]} = 1;
}
close $stops_fh;

open my $stop_times_fh, '<', $STOP_TIMES or
  die "Could not open stop_times.txt.\n";
my $stop_times_header = <$stop_times_fh>;
while (<$stop_times_fh>) {
  chomp;
  my @arr = split ',', $_;
  --$stops{$arr[3]};
}
close $stop_times_fh;

my @unseen;
foreach (keys %stops) {
  if ($stops{$_} == 1) {
    push @unseen, $_;
  }
}

print scalar(@unseen), "\n";
foreach (@unseen) {
  print $_, "\n";
}
