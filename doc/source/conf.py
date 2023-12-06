# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html
from datetime import date

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'RapidsSE'
copyright = '2023, Boray Data Technology Limited'
author = 'Boray Data Technology Limited'
version = '1.2'
release = '1.2.0-dev1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration
# https://www.mankier.com/1/sphinx-all

root_doc = 'index'
extensions = ['sphinx.ext.todo']

templates_path = ['_templates']
exclude_patterns = ['compilation/**']
if tags.has('internal'):
  exclude_patterns = []

suppress_warnings = ['toc.excluded']

nitpicky = True

today_fmt = '%d %B, %Y'
numfig = True   # https://stackoverflow.com/questions/2686310/referencing-figures-with-numbers-in-sphinx-and-restructuredtext

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
# https://alabaster.readthedocs.io/en/latest/customization.html

html_theme = 'alabaster'
html_static_path = ['_static',
                    '_static/custom.css']
html_extra_path = []
html_copy_source = False
html_favicon = '_static/favicon.ico'

html_theme_options = {
  'show_powered_by' : 'False',
  'show_relbars'    : 'True',
  'logo'            : 'logo200px.png',
}


# -- Options for Latex (& PDF) output ----------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-latex-output
# https://www.sphinx-doc.org/en/master/latex.html
# https://www.sphinx-doc.org/en/master/usage/configuration.html#confval-latex_engine

# We are setting the output filename here.
# Tuple format: (root document, output latex filename, title, author, latex theme, exclude root document)
latex_documents = [ (root_doc, f"rapidsse-{release}.tex", "", author, "manual", False) ]

latex_engine = 'xelatex' # to support unicode characters

# Various latex customisations
# Sources:
#  https://stackoverflow.com/questions/5422997/sphinx-docs-remove-blank-pages-from-generated-pdfs?rq=1
#  https://stackoverflow.com/questions/26120632/sphinx-remove-chapter-title-in-my-pdf-with-latexpdf?rq=1
latex_elements = {
  'papersize': 'a4paper',
#  'extraclassoptions': 'openany,oneside',       # remove blank pages after title, TOC and end of chapers
  'fncychap': '\\usepackage[Rejne]{fncychap}',  # Nicer chapter headings

  # Number headings below 3
  'preamble': r'''
  \setcounter{secnumdepth}{5}
  \usepackage{lscape}
  ''',
}

latex_logo = '_static/logo.pdf'

latex_show_pagerefs = True
latex_use_xindy = False   # xindy (indexes) doesn't run on CentOS 7 - requires newer glibc

# -- Options for extensions --------------------------------------------------

# Display todos by setting to True
# https://www.sphinx-doc.org/en/master/usage/extensions/todo.html
todo_include_todos = True


# -- Making variables accessible within rst files ----------------------------
# Credit here: https://stackoverflow.com/a/69211912

release_date = date.today().strftime("%d %B, %Y")

variables_to_export = [
  "project",
  "copyright",
  "author",
  "version",
  "release",
  "release_date",
]
frozen_locals = dict(locals())
rst_epilog = '\n'.join(map(lambda x: f".. |{x}| replace:: {frozen_locals[x]}", variables_to_export))
del frozen_locals