Kaboom! - an april's fools practical joke.


Legal Stuff
-----------

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

This is free software, it has been placed in the public domain.  Feel
free to use it or modify it as you see fit, but don't hold me responsible
for bugs it may contain.

This program does not read or write any disk file directly, nor does it
directly address any memory outside it's own memory space (as if OS/2
would let you do that), therefore it is highly unlikely that it will
cause any damage to a machine it is run on, however, I do not make
any guarantees that it will not convert your precious PC to a pile of
smoking silicon mush.  Use this program at your own risk.

So much for the legal stuff.  If you are not too scared, read on.


How to Use
----------

Simply run kaboom.exe from an OS/2 command line, or create a program
icon that references kaboom.exe in whatever directory you decide to
put it into.

For maximum practical joke effect, put a program icon for kaboom in 
the startup folder of some unsuspecting soul, or change an icon that
your target uses often (solitare seems to be a good choice around
here) to point to kaboom.exe instead of the normal program.  For the
latter approach, be careful to keep the same icon, the kaboom mushroom
cloud icon tends to give it away.

What it does
------------

In a nutshell, kaboom blast holes in the screen, complete with nuclear
explosions and mushroom clouds, to reveal a circuit board beneath the 
screen.

When the program starts, the current screen is displayed without any
changes.  When the user clicks the left mouse button, a 50 pixel square
area on the screen is engulfed by a nuclear explosion, leaving a jagged
edged hole showing a circuit board underneath.  After 11 left
button clicks, or a double click of the right button, the explosion/jagged
hole sequence repeats for the entire screen.  After the entire screen is
blasted, the program displays an "about" dialog box with the program name
and my name, and the program ends.

An escape sequence is also built into the program.  If you press the Ctrl-C
key combination while the program is active, the program will display the 
"about" dialog and end.


Why I wrote it
--------------

I am a RPG/400 programmer that works primarily with IBM AS/400 computers.
In the course of my work, I use an IBM product called Code/400 which
requires OS/2.  So here I am with a 486DX2-66 with 16 mb of RAM and the
worlds best PC operating system (OS/2) sitting on my desk.  As a 
programmer, I detest the idea of using any programmable machine which 
I have never programmed, so I set out with the company's trusty Borland 
C++ for OS/2 compiler with the plan to learn Presentation Manager 
programming.  Given that April Fools day was coming soon, and I could 
not think of any useful program that would not require more work than
I was prepared to invest in a learning experience, I decided on a 
practical joke program.  The rest, as they say, was history.



Why you (presumably) found it in the public domain
--------------------------------------------------

In the course of learning OS/2 PM programming, that vast majority of
the useful information that I found was obtained from public sources.
The Borland C++ for Win-OS/2 on Compuserve provided the basic shell
from which the program was formed (see credits below).  Also, the Hobbes
OS/2 archives on Internet (mainly obtained via Walnut Creek CD-ROM's
OS/2 CD - $25 and worth every cent) provided additional sample programs
and FAQ files.  Even the bitmaps used for the explosions and the
circuit board background were obtained from public sources.  With all
this, I figured that maybe someone else would be able to profit from
working source code that does weird stuff with the screen, and that
will be my way of repaying those who helped me, not to mention allow
others to freak out their bosses and co-workers with exploding screens
on April 1.


TANSTAAFL - There Ain't No Such Thing As A Free Lunch
-----------------------------------------------------

No, I don't expect you to send me any money, but if you use this program
as the basis for another program, be it joke or otherwise, I do expect
you to place it in the public domain if you think it is worthwhile.  If
you do make any modifications to this program, please change the "about"
box displayed at the end of the program to read "original program by 
Vincent Greene - Modified by XXX XXX" or similar wording to make it 
really clear that I didn't write that version.

If you really want to send me money, that is also okay with me.

Oh yeah.  A curse will be inflicted on any jerk that uses this program 
or any of the concepts learned from it to write a virus.  I figure ground
zero of a real nuclear explosion is too good for virus authors.

P.S. if you see my name on a resume at your company, put in a good
     word for me too, but don't remind your boss that it was my 
     program that melted his screen unless he has a good sense of
     humor.



Credits
-------

Original shell program by Mike O'Shea - obtained in the Borland C++ 
for Win-OS/2 forum on Compuserve (library 8 - HELLO.CPP).  Without this 
shell, I never would have got past the initial step of displaying a window.

Bitmaps for nuclear explosion sequence swiped from the ABM Command game
written by Kent Rollins.  This game is based on the Atari Missle Command
game.  It is available on the CICA internet archives in the WIN3\GAMES 
directory as ABM_CMD.ZIP.  (it's a fun game, one of my favorites)

the circuit board was found on one of the forums on compuserve (I'm not
sure which one) as a GIF file named CHIP1.GIF.  The original image 
was created by Chris Ryan and Kurt McKeever in 256 colors.  The original
image is 256 colors, and really looks a lot better than what I converted
to a bitmap with 16 colors.  By the way, I apologize for slicing the 
names of the image creators from the image in the program, but I didn't
really want any irate users calling the phone number that appeared 
magically on their screen after it dissolved in a nuclear explosion.


Future Plans for this program
-----------------------------

Either:

a) toss it aside and totally ignore it until just before april fools
   day next year,

or,

b) add a dialog box to allow customization of the program settings -
   kill zone size, location of ground zero, number of clicks to 
   total meltdown, and name of jokster to show in the "about" box
   at the end of the program.  If I do this, I'll post the new version
   the same places I posted this one.


How to contact the author
-------------------------

Vincent Greene

Compuserve - 73316,570
Internet   - 73316.570@compuserve.com (or something like that)

