# Makefile for Sphinx documentation

SPHINXOPTS  ?=
SPHINXBUILD ?= sphinx-build
SOURCEDIR   = source
BUILDDIR    = build

RR_GRAMMAR       = source/cmdRef/grammar.bnf
RR_OUTPUT_DIR    = source/rr
RR_INDEX_HTML    = $(RR_OUTPUT_DIR)/index.html
RR_INDEX_XHTML   = $(RR_OUTPUT_DIR)/rr.xhtml
RR_ZIPFILE       = $(RR_OUTPUT_DIR)/rr.zip

# Syntax tree generation and file fragments
RR_HTML_FRAG     = source/cmdRef/rr.html.fragment
RR_HTML_FRAG_GEN = source/cmdRef/rrHtmlFragGen.sh
RR_PDF_FRAG      = source/cmdRef/rr.rst.fragment
RR_PDF_FRAG_GEN  = source/cmdRef/rrPdfFragGen.sh
RR_COMMON_ARGS   = -suppressebnf -nofactoring -noinline -color:\#FFE270 -offset:166 -width:750


JDBC_URL_GRAMMAR = source/interfaces/jdbcDriver/url.bnf
JDBC_URL_OUTPUT  = source/interfaces/jdbcDriver/connectionURL
JDBC_URL_TMP     = source/interfaces/jdbcDriver/tmp
JDBC_URL_ZIP     = $(JDBC_URL_TMP)/url.zip
JDBC_URL_PNG 	 = $(JDBC_URL_OUTPUT)/connectionUrl.png

# Let's work out what OS we are on
UNAME_S := $(shell uname -s)

SPHINX_INSTALLED    := $(shell command -v $(SPHINXBUILD) 2> /dev/null)
LATEX_INSTALLED     := $(shell command -v latex 2> /dev/null)
LATEXMK_INSTALLED   := $(shell command -v latexmk 2> /dev/null)

PDFTEX_PATH := $(shell command -v pdftex)
PDFTEX_PATH := $(shell realpath $(PDFTEX_PATH) 2>/dev/null)
PDFTEX_PATH := $(shell dirname $(PDFTEX_PATH) 2>/dev/null)

export PATH := $(PATH):$(PDFTEX_PATH)

RST_FILES := $(shell find $(SOURCEDIR) -name "*.rst")

# Wireline protocol message definitions
WLP_MSG_DEF_DIR := $(shell realpath ../src/wlp/thrift 2>/dev/null)
WLP_MSG_DEF_INC := messageDefinitions
WLP_MSG_DEF_DEPS := $(shell find $(WLP_MSG_DEF_DIR)/$(WLP_MSG_DEF_INC) -name "*.thrift")
WLP_MSG_DEF_SCRIPT := $(shell realpath source/interfaces/wireline/v5/messages/createMessagesFragment.sh 2>/dev/null)

WLP_MSG_DEF_ZIP_NAME := RapidsSEWirelineMessages.zip
WLP_MSG_DEF_ZIP_PATH := $(shell realpath source/_static 2>/dev/null)/$(WLP_MSG_DEF_ZIP_NAME)
WLP_MSG_DEF_RST := $(shell realpath source/interfaces/wireline/v5/messages 2>/dev/null)/messages.rst.fragment


###############################################################################
### 								RULES									###
###############################################################################

# Put it first so that "make" without argument is like "make all".
all: html pdf

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


# Purposely empty rules
$(RR_GRAMMAR): ;
$(RR_HTML_FRAG_GEN): ;
$(RR_PDF_FRAG_GEN): ;
%.rst: ;

# synonym target
rr: railroad ;

# Convenience target to build all railroad diagrams
railroad: $(RR_INDEX_HTML) $(RR_HTML_FRAG) $(RR_PDF_FRAG)

# railroad HTML fragments to be included in the command reference
$(RR_HTML_FRAG) : $(RR_INDEX_XHTML) $(RR_HTML_FRAG_GEN)
	@echo "Extracting railroad SVG diagrams into a HTML fragment"
	@$(RR_HTML_FRAG_GEN) $(RR_INDEX_XHTML) $(RR_HTML_FRAG)

$(RR_PDF_FRAG) : $(RR_INDEX_HTML) $(RR_PDF_FRAG_GEN)
	@echo "Extracting railroad PNG diagrams into a rST fragment"
	@$(RR_PDF_FRAG_GEN) $(RR_GRAMMAR) $(RR_PDF_FRAG)

# Rule to generate the SVG railroad diagrams
$(RR_INDEX_XHTML): $(RR_GRAMMAR)
	@# Now build the version with svg images so we can just include the xhtml file
	@echo
	@echo "Building Command Reference railroad diagrams (SVG)"
	@java -jar tools/rr/rr.war $(RR_COMMON_ARGS) -out:$(RR_INDEX_XHTML) "$(RR_GRAMMAR)"
	@echo "Command Reference railroad diagrams (SVG) created at: $(RR_OUTPUT_DIR)"

# Rule to generate the PDF railroad diagrams
$(RR_INDEX_HTML): $(RR_GRAMMAR)
	@echo
	@echo "Building Command Reference railroad diagrams (PNG)"
	@mkdir -p "$(RR_OUTPUT_DIR)"
	@# First build the one with png files so the documentation can include them
	@java -jar tools/rr/rr.war $(RR_COMMON_ARGS) -png -out:$(RR_ZIPFILE) "$(RR_GRAMMAR)"
	@unzip -o -d "$(RR_OUTPUT_DIR)" "$(RR_ZIPFILE)" > /dev/null
	@rm -f "$(RR_ZIPFILE)"
	@echo "Command Reference railroad diagrams (PNG) created at: $(RR_OUTPUT_DIR)"


# Rule to generate the PNG JDBC URL railroad diagram
jdbcConnectionUrl: $(JDBC_URL_PNG)
$(JDBC_URL_PNG): $(JDBC_URL_GRAMMAR)
	@echo
	@echo "Building JDBC connection URL railroad diagrams (PNG)"
	@mkdir -p "$(JDBC_URL_OUTPUT)"
	@mkdir -p "$(JDBC_URL_TMP)"
	@java -jar tools/rr/rr.war $(RR_COMMON_ARGS) -png -out:$(JDBC_URL_ZIP) "$(JDBC_URL_GRAMMAR)"
	@unzip -o -d "$(JDBC_URL_TMP)" "$(JDBC_URL_ZIP)" > /dev/null
	@# Copy *.png files to output dir. Ignore the rr-1.67.png file
	@rm -f "$(JDBC_URL_TMP)"/diagram/rr-*.png
	@cp "$(JDBC_URL_TMP)"/diagram/*.png "$(JDBC_URL_OUTPUT)" > /dev/null
	@rm -rf "$(JDBC_URL_TMP)"
	@echo "JDBC connection URL railroad diagrams (PNG) created at: $(JDBC_URL_OUTPUT)"


# Rule to build the zip file containing the wireline protocol message definitions.
# This is provided with the HTML documentation.
wirelineMessageDefsZip: $(WLP_MSG_DEF_ZIP_PATH)
$(WLP_MSG_DEF_ZIP_PATH):
	@echo
	@echo "Creating the wireline protocol message definition bundle..."
	@cd $(WLP_MSG_DEF_DIR) && zip -q -r $(WLP_MSG_DEF_ZIP_NAME) $(WLP_MSG_DEF_INC)/*
	@mv $(WLP_MSG_DEF_DIR)/$(WLP_MSG_DEF_ZIP_NAME) $(WLP_MSG_DEF_ZIP_PATH)
	@echo "Wireline protocol message definition bundle created at: $(WLP_MSG_DEF_ZIP_PATH)"

# Rule to build the rst fragment file containing the wireline protocol message definitions.
# This is used by the PDF documentation.
wirelineMessageDefsRst: $(WLP_MSG_DEF_RST)
$(WLP_MSG_DEF_RST): $(WLP_MSG_DEF_DEPS) $(WLP_MSG_DEF_SCRIPT)
	@echo "Creating the wireline protocol message definition rst fragment..."
	@$(WLP_MSG_DEF_SCRIPT) "$(WLP_MSG_DEF_DIR)/$(WLP_MSG_DEF_INC)" "$(WLP_MSG_DEF_RST)"
	@echo "Wireline protocol message definition rst fragment created at: $(WLP_MSG_DEF_RST)"


linkcheck: checkSphinx
	@echo "Validating links in rst files..."
	@echo
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) linkcheck
	@echo

version:
	@echo "Updating version information..."
	@./updateVersion.sh
	@echo "done"

html: checkSphinx version railroad wirelineMessageDefsZip wirelineMessageDefsRst jdbcConnectionUrl
	@echo
	@echo "Building HTML documentation..."
	@echo
	@$(SPHINXBUILD) -M html "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)
	@echo

pdf: checkSphinx checkLatex railroad wirelineMessageDefsRst jdbcConnectionUrl
	@echo
	@echo "Building PDF documentation..."
	@echo
	@LATEXMKOPTS="-interaction=nonstopmode" $(SPHINXBUILD) -M latexpdf "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)
	@echo

# This target builds the html documentation with some internal information such as the compilation guide.
internal_html: export SPHINXOPTS = -t internal
internal_html: html

docclean:
	@rm -rf "$(BUILDDIR)"

clean:
	@rm -rf "$(BUILDDIR)"
	@rm -rf "$(RR_OUTPUT_DIR)"
	@rm -rf "$(RR_OUTPUT_DIR)"
	@rm -rf "$(RR_HTML_FRAG)"
	@rm -rf "$(RR_PDF_FRAG)"
	@rm -f  "$(WLP_MSG_DEF_ZIP_PATH)"
	@rm -f  "$(WLP_MSG_DEF_RST)"
	@rm -rf "$(JDBC_URL_OUTPUT)"

help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: all checkSphinx checkLatex clean help html internal_html jdbcConnectionUrl linkcheck pdf railroad rr version

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
#%: Makefile
#	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
