#!/usr/bin/env perl

use strict;
use warnings;

# TODO: might not be consistent across all GTFS feeds
my ($TRIP_ID_I, $ARRIVAL_TIME_I, $DEPARTURE_TIME_I, $STOP_ID_I) = (0, 1, 2, 3);

if (@ARGV < 1) {
  die "Provide stop_times.txt.\n";
}
my ($STOP_TIMES) = @ARGV;

open my $fh, '<', $STOP_TIMES or die "Could not open stop_times.txt.\n";
my $header_s = <$fh>;
# print STDERR "Header: '$header_s'.\n";

sub hms_to_seconds {
  my ($hms) = @_;
  my ($h, $m, $s) = split ':', $hms;
  return $h * 3600 + $m * 60 + $s;
}

sub parse_line {
  my ($line) = @_;
  # print STDERR "Line: '$line'.\n";
  my @arr = split ',', $line;
  return {
    trip_id => $arr[$TRIP_ID_I],
    arrival_time => hms_to_seconds($arr[$ARRIVAL_TIME_I]),
    departure_time => hms_to_seconds($arr[$DEPARTURE_TIME_I]),
    stop_id => $arr[$STOP_ID_I]
  };
};

my $vertex_i = 0;
my %vertex_map;

sub get_vertex_i {
  my ($vertex) = @_;
  if (!exists $vertex_map{$vertex}) {
    $vertex_map{$vertex} = $vertex_i++;
  }

  return $vertex_map{$vertex};
}

my $graph = {};

my $edges = 0;
sub set_weight {
  my ($u, $v, $w) = @_;
  if ($u > $v) {
    ($u, $v) = ($v, $u)
  }

  if (!exists $graph->{$u}) {
    $graph->{$u} = {};
  }

  if (!exists $graph->{$u}->{$v}) {
    $graph->{$u}->{$v} = $w;
    ++$edges;
  } else {
    if ($graph->{$u}->{$v} > $w) {
      $graph->{$u}->{$v} = $w;
    }
  }
}

my $first_line = <$fh>;
my $prev_stop = parse_line $first_line;

while (<$fh>) {
  # print STDERR "Line number: $.\n";
  my $cur_stop = parse_line $_;

  if ($prev_stop->{trip_id} == $cur_stop->{trip_id}) {
    my $prev_vertex = get_vertex_i $prev_stop->{stop_id};
    my $cur_vertex = get_vertex_i $cur_stop->{stop_id};
    my $diff = abs($cur_stop->{arrival_time} - $prev_stop->{departure_time});

    set_weight $prev_vertex, $cur_vertex, $diff;
  }

  $prev_stop = $cur_stop;
}

print "$vertex_i $edges\n";
foreach my $u (keys %$graph) {
  foreach my $v (keys %{$graph->{$u}}) {
    my $w = $graph->{$u}->{$v};
    print "$u $v $w\n";
  }
}
