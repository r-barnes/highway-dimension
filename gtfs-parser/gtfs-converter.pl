#!/usr/bin/env perl

use strict;
use warnings;
use FindBin;
use lib "$FindBin::Bin";
use CsvUtils;

my $GTFS_SEP = ',';

if (@ARGV < 3) {
  die "Provide output.in stops.txt stop_times.txt\n"; # in for hd algo
}
my ($OUTPUT, $STOPS, $STOP_TIMES) = @ARGV;

open my $stops_fh, '<', $STOPS or die "Could not open stops.txt\n";
my $stops_header = <$stops_fh>;
$stops_header =~ s/\"//g; # destroy all simple quotes in header
my $stops_header_i = CsvUtils::find_header_indices $stops_header,
  ['stop_id', 'parent_station'], $GTFS_SEP;

my $vertex_cnt = 0;
my %stop_to_id;
my @roots; # stations without a parent
my %children; # contains a list of children
my %parent_to_root_resolv; # roots are their own parent
while (<$stops_fh>) {
  s/"[^"]*"//g; # remove everything between double quotes, because it's commas
  my @arr = split $GTFS_SEP;
  my $stop_id = @arr[$stops_header_i->{stop_id}];
  my $parent_station = @arr[$stops_header_i->{parent_station}];

  next if exists $stop_to_id{$stop_id}; # unlikely but safety first

  if ($parent_station) {
    unless (defined $children{$parent_station}) {
      $children{$parent_station} = [];
    }
    push @{$children{$parent_station}}, $stop_id;
  } else {
    $stop_to_id{$stop_id} = $vertex_cnt++;
    push @roots, $stop_id;
    $parent_to_root_resolv{$stop_id} = $stop_id;
  }
}
close $stops_fh;

# DFS: set children's parent to my parent
sub set_parent {
  my ($stop_id) = @_;
  my $parent = $parent_to_root_resolv{$stop_id};
  return undef unless defined $children{$stop_id};
  foreach my $child_stop_id (@{$children{$stop_id}}) {
    $parent_to_root_resolv{$child_stop_id} = $parent;
    set_parent($child_stop_id);
  }
}

foreach my $root (@roots) {
  set_parent $root;
}

open my $stop_times_fh, '<', $STOP_TIMES or
  die "Could not open stop_times.txt.\n";

my $stop_times_header = <$stop_times_fh>;
$stop_times_header =~ s/\"//g;
my $stop_times_header_i = CsvUtils::find_header_indices $stop_times_header,
  ['trip_id', 'arrival_time', 'departure_time', 'stop_id'];

sub hms_to_seconds {
  my ($hms) = @_;
  my ($h, $m, $s) = split ':', $hms;
  return $h * 3600 + $m * 60 + $s;
}

sub parse_line {
  my ($line) = @_;
  chomp $line;
  my @arr = split $GTFS_SEP, $line;
  return {
    trip_id => $arr[$stop_times_header_i->{trip_id}],
    arrival_time => hms_to_seconds(
      $arr[$stop_times_header_i->{arrival_time}]),
    departure_time => hms_to_seconds(
      $arr[$stop_times_header_i->{departure_time}]),
    stop_id => $arr[$stop_times_header_i->{stop_id}]
  };
};

my $graph = {};

my $edges = 0;
sub set_weight {
  my ($u, $v, $w) = @_;
  if (!defined $u || !defined $v) {
    die "Undefined $u, $v.\n";
  }
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

my $first_line = <$stop_times_fh>;
my $prev_stop = parse_line $first_line;

while (<$stop_times_fh>) {
  my $cur_stop = parse_line $_;

  if ($prev_stop->{trip_id} == $cur_stop->{trip_id}) {
    my $prev_vertex =
      $stop_to_id{$parent_to_root_resolv{$prev_stop->{stop_id}}};
    my $cur_vertex = 
      $stop_to_id{$parent_to_root_resolv{$cur_stop->{stop_id}}};
    my $diff = abs($cur_stop->{arrival_time} - $prev_stop->{departure_time});

    set_weight $prev_vertex, $cur_vertex, $diff;
  }

  $prev_stop = $cur_stop;
}

close $stop_times_fh;

open my $output_fh, '>', $OUTPUT;
print {$output_fh} "$vertex_cnt $edges\n";
foreach my $u (sort {$a <=> $b} (keys %$graph)) {
  foreach my $v (sort {$a <=> $b} (keys %{$graph->{$u}})) {
    my $w = $graph->{$u}->{$v};
    print {$output_fh} "$u $v $w\n";
  }
}
