# Makefile for Sphinx documentation

SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

RR_GRAMMAR    = source/cmdRef/grammar.bnf
RR_OUTPUT_DIR = source/_static/rr
RR_INDEX_HTML = source/_static/rr/index.html
RR_ZIPFILE    = $(RR_OUTPUT_DIR)/rr.zip

SPHINX_INSTALLED := $(shell command -v $(SPHINXBUILD) 2> /dev/null)
LATEX_INSTALLED  := $(shell command -v latex 2> /dev/null)
LATEXMK_INSTALLED:= $(shell command -v latexmk 2> /dev/null)

# Let's work out what OS we are on
UNAME_S := $(shell uname -s)

# Put it first so that "make" without argument is like "make all".
all: pdf
#	@echo "Validating links in rst files..."
#	@echo
#	@$(MAKE) linkcheck
#	@echo

checkSphinx:
ifndef SPHINX_INSTALLED
	$(error ERROR: Not building the documentation because $(SPHINXBUILD) is not installed ***)
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
	@echo "This is about a 4GB download."
else ifeq ($(UNAME_S),Darwin)
	@echo "To install latex and latexmk on MacOS, the easiest way is to run the command:"
	@echo "  brew install mactex-no-gui"
	@echo "This is about a 4.7GB download."
endif # UNAME_S
endif # SPHINX_INSTALLED

checkLatex:
ifndef LATEXMK_INSTALLED
	$(error ERROR: Not building the documentation because latex and/or latexmk is not installed ***)
	@echo "If you only wish to build the HTML documentation, you can simply run: make html"
	@echo
ifeq ($(UNAME_S),Linux)
	@echo "To install latex and latexmk on Linux, the easiest way is to install the full"
	@echo "TeX Live installation via the instructions at:"
	@echo "  https://www.tug.org/texlive/quickinstall.html"
	@echo "This is about a 4GB download."
else ifeq ($(UNAME_S),Darwin)
	@echo "To install latex and latexmk on MacOS, the easiest way is to run the command:"
	@echo "  brew install mactex-no-gui"
	@echo "This is about a 4.7GB download."
endif # UNAME_S
endif # LATEXMK_INSTALLED

# synonym target
rr: railroad ;

railroad:
	@echo "Building Command Reference railroad diagrams"
	@mkdir -p "$(RR_OUTPUT_DIR)"
	@java -jar tools/rr/rr.war -suppressebnf -nofactoring -noinline -color:#FFE270 -offset:166 -png -out:$(RR_ZIPFILE) "$(RR_GRAMMAR)"
	@unzip -o -d "$(RR_OUTPUT_DIR)" "$(RR_ZIPFILE)" > /dev/null
	@rm -f "$(RR_ZIPFILE)"
	@echo "Command Reference railroad diagrams created at: $(RR_OUTPUT_DIR)"


#$(RR_GRAMMAR): ;

$(RR_INDEX_HTML): $(RR_GRAMMAR)
	@$(MAKE) railroad

linkcheck: checkSphinx
	@echo "Validating links in rst files..."
	@echo
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) linkcheck
	@echo

version:
	@echo "Updating version information..."
	@./updateVersion.sh
	@echo "done"
	@echo

html: checkSphinx version $(RR_INDEX_HTML)
	@echo "Building HTML documentation..."
	@echo
	@$(SPHINXBUILD) -M html "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)
	@echo

pdf: checkLatex html
	@echo "Building PDF documentation..."
	@echo
	@$(SPHINXBUILD) -M latexpdf "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) LATEXMKOPTS="-interaction=nonstopmode"
	@echo

clean:
	@rm -rf "$(BUILDDIR)"
	@rm -rf "$(RR_OUTPUT_DIR)"

help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: all checkSphinx checkLatex clean help html linkcheck pdf railroad rr version

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
#%: Makefile
#	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
