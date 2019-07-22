package CsvUtils;

use strict;
use warnings;

sub find_header_indices {
  my ($header_s, $ids, $csv_sep) = @_;
  $csv_sep = ',' if !defined $csv_sep;
  chomp $header_s;
  my @header = split $csv_sep, $header_s;

  my %result;
  foreach (@$ids) {
    $result{$_} = -1;
  }

  for (my $i = 0; $i < @header; ++$i) {
    my $cur_id = $header[$i];
    next unless defined $result{$cur_id};
    $result{$cur_id} = $i;
  }
  foreach (keys %result) {
    die "Could not find id $_" if $result{$_} < 0;
  }

  return \%result;
}

1;
