# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'RapidsSE'
copyright = '2022, Boray Data Technology Limited'
author = 'Boray Data Technology Limited'
version = '1.1'
release = '1.1.0-dev0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

root_doc = 'index'
extensions = ['sphinx.ext.todo']

templates_path = ['_templates']
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']


# -- Options for Latex (& PDF) output ----------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-latex-output
# https://www.sphinx-doc.org/en/master/latex.html

# We are setting the output filename here.
# Tuple format: (root document, output latex filename, title, author, latex theme, exclude root document)
latex_documents = [ (root_doc, f"rapidsse-{release}.tex", "", author, "manual", False) ]


# -- Options for extensions --------------------------------------------------

# Display todos by setting to True
# https://www.sphinx-doc.org/en/master/usage/extensions/todo.html
todo_include_todos = True
