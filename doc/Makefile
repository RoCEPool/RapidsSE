# Makefile for Sphinx documentation

SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

SPHINX_INSTALLED := $(shell command -v $(SPHINXBUILD) 2> /dev/null)
LATEX_INSTALLED  := $(shell command -v latex 2> /dev/null)
LATEXMK_INSTALLED:= $(shell command -v latexmk 2> /dev/null)

# Let's work out what OS we are on
UNAME_S := $(shell uname -s)

# Put it first so that "make" without argument is like "make all".
all:
ifdef SPHINX_INSTALLED
	@echo "Checking links"
	@echo
	@$(MAKE) linkcheck
	@echo
	@echo "Updating version information"
	@./updateVersion.sh
	@echo "done"
	@echo
	@echo "Building HTML documentation"
	@echo
	@$(MAKE) html
	@echo
	
	@echo "Building PDF documentation"
	@echo
ifdef LATEXMK_INSTALLED
	@$(MAKE) latexpdf
else
	$(warning *** Not generating PDF documentation because latex and/or latexmk is not installed ***)
ifeq ($(UNAME_S),Linux)
	@echo "To install latex and latexmk on Linux, the easiest way is to run the command:"
	@echo "  sudo yum install latexmk"
	@echo "or"
	@echo "  sudo apt install latexmk"
else ifeq ($(UNAME_S),Darwin)
	@echo "To install latex and latexmk on MacOS, the easiest way is to run the command:"
	@echo "  brew install mactex-no-gui"
	@echo "(requires ~4.7GB download)"
endif # UNAME_S
endif # LATEXMK_INSTALLED

else
	$(warning *** Not building the documentation because $(SPHINXBUILD) is not installed ***)
	@echo "Building the RapidsSE documentation requires the following pre-requisites:"
	@echo "  1. Python 3"
	@echo "  2. Sphinx"
	@echo "  3. latex & latexmk (optional, for PDF documentation)"
	@echo ""
	@echo "After installing Python 3, you can install Sphinx with the following command:"
	@echo "  python3 -m pip install --user -U sphinx"
	@echo ""
ifeq ($(UNAME_S),Linux)
	@echo "To install latex and latexmk on Linux, the easiest way is to install the full"
	@echo "TeX Live installation via the instructions at:"
	@echo "  https://www.tug.org/texlive/quickinstall.html"
else ifeq ($(UNAME_S),Darwin)
	@echo "To install latex and latexmk on MacOS, the easiest way is to run the command:"
	@echo "  brew install mactex-no-gui"
	@echo "(requires ~4.7GB download)"
endif # UNAME_S
endif # SPHINX_INSTALLED

clean:
	@rm -rf "$(BUILDDIR)"

help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: all clean help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
