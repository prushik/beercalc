# beercalc

A beer recipe calculator, for serving through the unweave webserver

beercalc currently has 4 components:
 - The "library files" - a collection of C source code files designed to be easily integrated into other projects. The code in these files is responsible for all the essential homebrew calculations.
 - The web backend api - a simple backend api that connects to a sqlite database and provides access to the server-side calculations and data storage.
 - The web frontend ui - a simple frontend written in C, HTML and javascript. 
 - The database        - a collection sql scripts needed to build a reasonably usable database of brewing ingredients and beer styles, as well as a few example recipes.


For an example of all parts of beercalc working together, see:
http://beer.betteros.org
