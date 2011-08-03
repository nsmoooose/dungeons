========
Dungeons
========

Introduction
============

A dungeon game like the dwarf fortress and goblins camp.

Development language is C.

First user interface is a rough cli based design just like
dwarf fortress.

Architecture
============

All communications between client and server shall be with a file descriptor.
This makes it possible for us to split this up into server and a client for
networking purposes.

All interactions with the game goes through messages that are serialized into
the file descriptor.

Terminal user interface
=======================

Keyboard:

* Arrow keys to move arround.
* Shift + arrow keys to move around faster.
* b == build
* d == designations
* j == jobs
* ? == help
* +/- == increase, decrease, zoom in, zoom out

Map symbols:

+--------+-----------------------------------------------+
| Symbol | Description                                   |
+========+===============================================+
| ~      | Water, blue                                   |
+--------+-----------------------------------------------+
| T      | Tree, green                                   |
+--------+-----------------------------------------------+

Layout::

 /===========================================INFO======\
 |                                         |           |
 |                                         | water     |
 |             ~~                          |           |
 |              ~~                         |           |
 |              ~X                         |           |
 |              ~~                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |                                         |           |
 |--HELP-----------------------------------|           |
 | b) build j) jobs                        |           |
 | d) designations                         |           |
 \=====================================================/


Features
========

World
-----

The world is divided into levels. Each level is 10 meters high.
w h d 1000 x 1000 x 3000

Time
----

How fast do we progress?

Shift working
-------------

All people need 8 hours of sleep every day. So for all your inhabitants. Default
will all people work from 7 -> 17 with 1 hour rest. You could divide all the people
into shifts to make sure that you wont get long periods of time with inactivity.

Employ a shift leader to take care of the management of shifts.

Stories
=======

A story is for us to visualize something happening in the game.

High mountain
-------------

Once upon a time there was a large mountain (5000 m). There was small stair up to
the peak of this mountain where a large fortress was placed. The fortress had an
excellent view of the suroundings.

Ice world
---------

Near the north pole on the glacier was a fortress built from ice only. Large
dungeons were placed there. Life was hard. Men lived of the nature by hunting
bears and whales.
