# Makefile for Sphinx documentation

SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = source
BUILDDIR      = build

SPHINX_INSTALLED := $(shell command -v $(SPHINXBUILD) 2> /dev/null)

# Put it first so that "make" without argument is like "make all".
all:
ifdef SPHINX_INSTALLED
	@echo "Checking links"
	@$(MAKE) linkcheck
	@echo "Building HTML documentation"
	@$(MAKE) html
else
	$(warning Not building the documentation because $(SPHINXBUILD) is not installed)
endif

clean:
	@rm -rf "$(BUILDDIR)"

help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: all clean help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
