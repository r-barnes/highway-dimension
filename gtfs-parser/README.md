# Data parser

This folder contains a Julia module which converts a GTFS feed into a weighted graph.

## Methodology

Each stop is converted into a vertex.
There are stations which are a substation of another station; they are all joined into a single station.
Edge weight between a pair of stops of a trip is calculated as the difference between arrival time of destination and departure time of the departure station.
There may be multiple weights depending on the time of the day, therefore we take the minimum.

## Output

Let *n* be the number of vertices and *m* the number of edges.
The first line of the output is be *n* and *m*.
Then *m* lines follow for each edge, where on each line there are three numbers *u v w*, *(u, v)* is an edge and *w* is its weight.
Vertices are numbered from *0* to *n - 1*.

The output may contain edges with weight *0*.
