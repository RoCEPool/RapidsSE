# Documentation README

This folder contains all of the documentation for RapidsSE's documentation
as code.

## Overview

Documentation is written in a markup language called reStructuredText
(reST, or rST). This allows the documentation to be written with semantic
meanings included, but without worrying about the output presentation.

A tool called Sphinx is then used to turn the source document files into
the desired output formats (HTML and PDF). Some additional latex software
is required to convert into PDF format.

## Software Requirements
These are the software requirements to be able to turn the documentation
markup files into HTML and PDF output documents. If these requirements
are met then the documentation will be generated when the installer is
being packaged.

1. Python 3
2. Python PIP (assumed to come with Python 3)
3. Sphinx 5.3.0+
4. TexLive (Linux) or MacTex (MacOS)

TexLive (or MacTex) can be left off if PDF documentation is not required.

### MacOS Installation
1. `sudo python3 -m pip install -U sphinx`
2. `brew install mactex-no-gui` (~4GB, required for PDF documentation)

### Linux Installation
Note: We use the PIP version of Sphinx to ensure that it is a recent version.
1. `sudo yum install python3`
2. `sudo python3 -m pip install -U sphinx`
3. Install TexLive according to the instructions [here](https://www.tug.org/texlive/quickinstall.html).

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
2. `make pdflatex`

## Writing Documentation
Documentation is written in reStructuredText. 

Here are some primers and introductions to reStructuredText:
* WriteTheDocs [introduction to reStructuredText](https://www.writethedocs.org/guide/writing/reStructuredText/)
* Docutils [quickstart](https://docutils.sourceforge.io/docs/user/rst/quickstart.html#sections)
* Docutils [quick reference](https://docutils.sourceforge.io/docs/user/rst/quickref.html)

reStructuredText directives are defined in the docutils project, however Sphinx
extends this to add more useful ones. Here are some links to reStructuredText 
directives:
* [Sphinx directive reference](https://www.sphinx-doc.org/en/master/usage/restructuredtext/directives.html)
* [docutils directive reference](https://www.docutils.org/docs/ref/rst/directives.html)

Headings and subheadings in reStructuredText are underlined. Headings of different
levels are underlined with different characters. This documentation should follow
the docutils recommendation for characters used for underlining headings:
```= - ` : ' " ~ ^ _ * + # < >```