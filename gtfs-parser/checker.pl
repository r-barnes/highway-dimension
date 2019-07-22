#!/usr/bin/env perl

use strict;
use warnings;

if (@ARGV < 1) {
  die "Provide data.\n";
}

my ($DATA) = @ARGV;
open my $fh, '<', $DATA or die "Could not open data from '$DATA'.\n";
my $header_s = <$fh>;
my $vertex_cnt = (split ' ', $header_s)[0];

my %seen;
while (my $edge = <$fh>) {
  my ($u, $v, $w) = split ' ', $edge;
  $seen{$u} = 1;
  $seen{$v} = 1;
}

my $ok = 1;
foreach my $i (0 .. ($vertex_cnt - 1)) {
  if (!exists $seen{$i}) {
    print STDERR "Vertex $i is not used.\n";
    $ok = 0;
  }
}

if ($ok) {
  print STDERR "Data is OK!\n";
} else {
  die "Errors found.\n";
}
