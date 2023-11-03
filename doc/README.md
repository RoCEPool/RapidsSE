# Documentation README

This folder contains all of the documentation for RapidsSE's documentation
as code.

This documentation is written in MarkDown format. It's convenient for
simple, single page documents.

## Overview

Documentation is written in a markup language called reStructuredText
(rST). This allows the documentation to be written with semantic
meanings included, but without worrying about the output presentation.

A tool called Sphinx is then used to turn the source document files into
the desired output formats (HTML and PDF). Some additional latex software
is required to convert into PDF format.

Finally another tool called the Railroad Diagram Generator is used to 
create railroad diagrams from a syntax file. This is included in the
repository in doc/tools/rr/rr.war. It requires Java to run it. Both
awk and vim are required in order to massage the output of this tool
into a form that can be included in the documentation.

## Software Requirements
These are the software requirements to be able to turn the documentation
markup files into HTML and PDF output documents. If these requirements
are met then the documentation will be generated when the installer is
being packaged.

1. Python 3
2. Python PIP (assumed to come with Python 3)
3. Sphinx 5.3.0+
4. TexLive (Linux) or MacTex (MacOS)
5. Java 8 or later
6. awk
7. vim
8. GNU coreutils (on MacOS)

TexLive (or MacTex) can be left off if PDF documentation is not required.

### MacOS Installation
1. Install the GNU coreutils package: `brew install coreutils`


2. Install Python3 if it is not already installed (try `which python3`): `brew install python3`


3. Test that python3 is resolvable: `which python3`
   
   If this doesn't work, first try restarting your terminal window as python3
   may have been added to the `$PATH`. 
   
   If this still doesn't work then the python
   `bin` directory will need to be added to your `$PATH`. How this is done depends
   on how Python was installed and which shell (bash/zsh) you are using.


4. Install Sphinx: `python3 -m pip install -U sphinx`
 

5. Test the Sphinx is resolvable: `which sphinx-build`

   If this doesn't work, first try restarting your terminal window as the
   sphinx bin directory may have been added to the `$PATH`.

   If this still doesn't work then the directory that sphinx was installed into
   (which itself is dependent on your Python setup) will need to be added to 
   your `$PATH`. How this is done depends on how Python was installed and which 
   shell (bash/zsh) you are using.


6. Install MacTex for building PDF documentation (approx 4 GB): `brew install mactex-no-gui` 


7. Install Java if it is not already installed (try `which java`): `brew install java`


8. Install awk if it is not already installed (try `which awk`): `brew install gawk`


9. Install vim if it is not already installed (try `which vim`): `brew install vim`


10. Restart your terminal

### Linux Installation
Note: We use the PIP version of Sphinx to ensure that it is a recent version.
1. Install Python3: `sudo yum install python3`
2. Test that `python3` is resolvable: `which python3`
3. Test that `pip` is installed: `python3 -m pip -V`
   1. If `pip` is not installed and you are on Ubuntu then use: `sudo apt install python3-pip` 
4. Install Sphinx: `sudo python3 -m pip install -U sphinx`
5. Test that `sphinx-build` is resolvable: `which sphinx-build`
6. Test that `perl` is installed: `which perl`
   1. If it isn't, use your distribution's package manager to install `perl`
   2. You may need to install certain other perl packages. e.g. `yum install perl-Digest-MD5 perl-Env`
7. Install TexLive according to the instructions [here](https://www.tug.org/texlive/quickinstall.html).
   Don't forget to set the `$PATH` variable as instructed.
   Also if you get the error "mktexfmt [ERROR]: -user mode but path setup is -sys type, bailing out." 
   then you may need to run this command:
     `/usr/local/texlive/bin/x86_64-linux/fmtutil-sys --all`
8. Install Java (NB: not the headless variant)
9. Install awk
10. Install vim
11. Restart your terminal

Note2: On AWS the TexLive installation has been saved as a portable
installation in an S3 bucket. It simply needs to be downloaded from S3,
extracted into `/usr/local/texlive` and then the following environment
variables need to be setup and exported in `/etc/profile.d/sh.local`:
```bash
TEXLIVEDIR="/usr/local/texlive"
PATH="${PATH}:${TEXLIVEDIR}/bin/x86_64-linux"
MANPATH="${MANPATH}:${TEXLIVEDIR}/texmf-dist/doc/man"
INFOPATH="${INFOPATH}:${TEXLIVEDIR}/texmf-dist/doc/info"
export PATH MANPATH INFOPATH
```

## Generating Documentation Manually
To generate all documentation (HTML and PDF):
1. `cd doc`
2. `make clean`
3. `make`

To generate HTML documentation only:
1. `cd doc`
2. `make html`

To generate PDF documentation only:
1. `cd doc`
2. `make pdf`


## Viewing Documentation
All built documentation is written to the `doc/build` directory. 

HTML documentation can be loaded from `doc/build/html/index.html`

PDF documentation can be opened at `doc/build/latex/rapidsse-<version>.pdf`


## Writing Documentation
Documentation is written in reStructuredText (rST). When writing a paragraph, a
single new line is not significant and the sentence continues. Two or more new lines 
however indicate a new paragraph.

rST has directives for linking to other pages, including images, etc. **Whitespace in
directives can be significant** and can mean the difference between doing what you want
and the directive appearing as plain text.

rST directives typically also need a blank line after them. This is because two lines
not separated by a blank are considered a continuation of a single line.

Titles and subtitles are defined by underlining the heading name completely. 
Consistency with the underlining character is important here (see below).

Here are some primers and introductions to reStructuredText:
* WriteTheDocs [introduction to reStructuredText](https://www.writethedocs.org/guide/writing/reStructuredText/)
* Docutils [quickstart](https://docutils.sourceforge.io/docs/user/rst/quickstart.html)
* Docutils [quick reference](https://docutils.sourceforge.io/docs/user/rst/quickref.html)
* [rST Cheat Sheet (PDF)](https://github.com/ralsina/rst-cheatsheet/blob/master/rst-cheatsheet.pdf)
* [rST Markup Specification](https://docutils.sourceforge.io/docs/ref/rst/restructuredtext.html) 


reStructuredText directives are defined in the docutils project, however Sphinx
extends this to add more useful ones. Here are some links to reStructuredText 
directives:
* [Sphinx directive reference](https://www.sphinx-doc.org/en/master/usage/restructuredtext/directives.html)
* [docutils directive reference](https://www.docutils.org/docs/ref/rst/directives.html)

Headings and subheadings in reStructuredText are underlined. Headings of different
levels are underlined with different characters. This documentation should follow
the docutils recommendation for characters used for underlining headings:
```= - \` : ' " ~ ^ _ * + # < >```

    Heading 1:     =========
    Heading 2:     ---------
    Heading 3:     ````````` 
    Heading 4:     :::::::::
    Heading 5:     '''''''''
    Heading 6:     """""""""
    Heading 7:     ~~~~~~~~~
    Heading 8:     ^^^^^^^^^
    Heading 9:     _________
    Heading 10:    *********
    Heading 11:    +++++++++
    Heading 12:    #########
    Heading 13:    <<<<<<<<<
    Heading 14:    >>>>>>>>>

## Other Notes

When including code samples in rST you can specify the language to get 
custom formatting. This is provided by the [Pygments](https://pygments.org)
library. You can see the supported language names [here](https://pygments.org/languages/),
and try out the formatting in a [demo here](https://pygments.org/demo/).

You can preview Sphinx themes [here](https://sphinx-themes.org/#themes)

On Craig's Mac, the alabaster theme is found in 
``~/.pyenv/versions/3.10.0/lib/python3.10/site-packages/alabaster``.

[Jinja template designer documentation](https://jinja.palletsprojects.com/en/3.1.x/templates/)

[Write The Docs Documentation Guide](https://www.writethedocs.org/guide/) is a good
introduction to documentation as a whole.

The railroad diagram generator came from [here](https://github.com/GuntherRademacher/rr)

## AWS TexLive Generation
These are the instructions for how to generate a TexLive portable installation
that can then be placed in an S3 bucket (rapidsse-build-support) for use in 
AWS EC2 instances.

Refer to https://tug.org/texlive/

1. `cd /tmp`
2. `wget https://mirror.ctan.org/systems/texlive/tlnet/install-tl-unx.tar.gz`
3. `tar xvzf install-tl-unx.tar.gz`
4. `cd install-tl-*`
5. `perl ./install-tl --no-interaction -no-doc-install -no-src-install -portable -texdir /mnt/disk1/tmp/texlive`
6. `cd /tmp`
7. `tar cvzf texlive.[version].[os].[platform].tar.gz texlive`

