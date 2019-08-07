#!/usr/bin/env perl

use strict;
use warnings;
use HTTP::Tiny;
use JSON::PP;
use File::Path qw(make_path);
use Data::Dumper;
use IO::Socket::SSL;
use FindBin;

my $API_KEY = '';
my $HELP_MESSAGE = "Usage: $0 api_key";

if (@ARGV < 1) {
  print STDERR $HELP_MESSAGE, "\n";
}

$API_KEY = $ARGV[0];

sub build_url
{
  my ($path, $params) = @_;
  my $url = 'https://api.transitfeeds.com/v1';
  my @param_array;
  push @param_array, "key=$API_KEY";
  if ($params) {
    foreach my $param (keys %$params) {
      my $param_val = $params->{$param};
      push @param_array, "$param=$param_val";
    }
  }
  my $param_s = join '&', @param_array;
  return "$url/$path?$param_s";
}

my $http = HTTP::Tiny->new();

sub get_feeds
{
  my ($page) = @_;
  my $params = {
    'limit' => '1000',
    'type' => 'gtfs'
  };
  if ($page) {
    $params->{page} = $page;
  }
  my $url = build_url('getFeeds', $params);

  my $resp = $http->request('GET', $url, {
    'headers' => {
      'Accept' => 'application/json'
    }
  });

  unless ($resp->{success}) {
    die "Could not get feeds.\n";
  }

  my $json = decode_json $resp->{content};
  return $json->{results};
}

sub location_name_slug
{
  my ($location_name) = @_;
  $location_name =~ s/(\s|,)+/-/g;
  $location_name = lc $location_name;
  return $location_name;
}

sub process_raw_feed
{
  my ($feed) = @_;
  return {
    'id' => $feed->{id},
    'location' => location_name_slug($feed->{l}->{t})
  };
}

my @processed_feeds;
sub process_request_result
{
  my ($result) = @_;
  my $feeds = $result->{feeds};
  foreach my $feed (@$feeds) {
    push @processed_feeds, process_raw_feed $feed;
  }
}

my $pages_total = ~0 >> 1;
my $current_page = 1;
do {
  my $download_result = get_feeds($current_page);
  $pages_total = $download_result->{numPages};
  process_request_result $download_result;
  ++$current_page;
} while ($current_page <= $pages_total);

sub id_to_filename
{
  my ($id) = @_;
  $id =~ s|/|-|g;
  return $id;
}

sub get_feed
{
  my ($id) = @_;
  my $params = {
    'feed' => $id
  };
  my $url = build_url('getLatestFeedVersion', $params);

  my $resp = $http->request('GET', $url);

  unless ($resp->{success}) {
    die "Could not get feed $id.\n";
  }

  my $json = decode_json $resp->{content};
}

foreach my $feed (@processed_feeds) {
  print STDERR "Downloading feed $feed->{id}.\n";
  my $params = {
    'feed' => $feed->{id}
  };
  my $url = build_url 'getLatestFeedVersion', $params;
  my $resp = $http->request('GET', $url);

  unless ($resp->{success}) {
    die "Could not get feed $feed->{id}\n";
  }

  my $path = join '/', $FindBin::Bin, 'latest-feeds', $feed->{location},
                  id_to_filename($feed->{id});
  make_path($path);
  open my $fh, '>:raw', "$path/gtfs.zip" or die "Could not open destination $path";
  print $fh $resp->{content};
  close $fh;
}
