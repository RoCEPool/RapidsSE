" This is a file of vim Ex commands
"
"
" (0) Go to the top of the file
1;
"
"
" (1) Go to the line that contains: </style><svg
"     and add 'style="height:0px;"' to it.
"     Note, we need to escape the forward slash in the search command.
/<\/style><svg
s/<svg /<svg style="height:0px;" /
"
"
" (2) Find the <style> tag within <head> and remove all lines before it
1;
/<style
1,.-1 d
"
"
" (3) Find the closing </head> tag and remove it
/<\/head>
s/<\/head>//
"
"
" (4) Rename <body> tag to <div class="rrbody">.
"     Also change the closing </body> tag to </div>
/<body>
s/<body>/<div class="rrbody">/
/<\/body>
s/<\/body>/<\/div>/
"
"
" (5) Remove closing </html> tag
/<\/html>
d
"
"
" (6) Change css body selector to "div.rrbody"
/body$
s/body/div.rrbody/
"
"
" (7) Global search/replace "<xhtml:" with "<"
%s/<xhtml:/</g
"
"
" (8) Global search/replace "</xhtml:" with "</"
%s/<\/xhtml:/<\//g
"
"
" (9) Global search/replace 'xmlns:\w+="[^"]*"' with ''
%s/xmlns:\w\+="[^"]*"//g
"
"
" (10) Global search/replace 'xmlns="[^"]*"' with ''
%s/xmlns="[^"]*"//g
"
"
" (11) Global search/replace 'xlink:' with ''
%s/xlink://g
"
"
" (12) Indent content of the file 2 spaces
%s/^/  /
"
"
" (13) Add to the top of the file '.. raw:: html' and another new line
1;
i
.. raw:: html

.
"
"
" (14) Split long lines at word boundaries, keeping each line 2 space indented.
"      (rST has a line length limit that we exceed with the SVG images).
%s/\([a-zA-Z0-9-]\+ \)\{100\}/\0\r  /g