# Documentation README

This folder contains all of the documentation for RapidsSE's documentation
as code.

## Overview

Documentation is written in a markup language called reStructuredText
(reST, or rST). This allows the documentation to be written with semantic
meanings included, but without worrying about the output presentation.

A tool called Sphinx is then used to turn the source document files into
the desired output formats.

## Software Requirements
These are the software requirements to be able to turn the documentation
markup files into HTML and PDF output documents. If these requirements
are met then the documentation will be generated when the installer is
being packaged.

1. Python 3
2. Python PIP (assumed to come with Python 3)
3. Sphinx 5.3.0+

### MacOS Installation
1. `sudo python3 -m pip install -U sphinx`

### Linux Installation
#### CentOS
Note: CentOS 7 YUM package manager has a really old version of sphinx. So
we use pip to install it instead.
1. `sudo yum install python3`
2. `sudo python3 -m pip install -U sphinx`

#### Ubuntu
1. `sudo apt-get install python3-sphinx`

## Generating Documentation Manually
TODO:
