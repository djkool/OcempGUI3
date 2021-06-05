top_srcdir = `pwd`

SUBDIRS = \
	$(top_srcdir)/data/themes \
	$(top_srcdir)/data/themes/default \
	$(top_srcdir)/doc \
	$(top_srcdir)/ocempgui \
	$(top_srcdir)/ocempgui/access \
	$(top_srcdir)/ocempgui/access/papi \
	$(top_srcdir)/ocempgui/draw \
	$(top_srcdir)/ocempgui/events \
	$(top_srcdir)/ocempgui/object \
	$(top_srcdir)/ocempgui/widgets \
	$(top_srcdir)/ocempgui/widgets/components \
	$(top_srcdir)/ocempgui/widgets/images

DEBUGFILES = $(top_srcdir)/ocempgui/widgets/BaseWidget.py

all: build

check:
	@echo "Running checks..."
	@python -c "import setup; setup.run_checks ()"

dist:
	@echo "Creating dist..."
	@python setup.py sdist

build:
	@echo "Running build..."
	@python setup.py build
	@echo "Build finished, invoke 'make install' to install."

install:
	@echo "Installing..."
	@python setup.py install 

docs:
	@echo "Creating documentation..."
	@make -C $(top_srcdir)/doc

clean:
	@echo "Cleaning up in ./ ..."
	@rm -f *.py~ *.pyo *.pyc *~ *.cache *.core MANIFEST
	@rm -rf build dist
	@for dir in $(SUBDIRS); do \
		if test -f $$dir/Makefile; then \
			make -C $$dir clean; \
		else \
			cd $$dir; \
			echo "Cleaning up in $$dir..."; \
			rm -f *.py~ *.pyo *.pyc *~ *.cache *.core *.html; \
		fi \
	done

srcclean:
	@echo "Cleaning up in ./ ..."
	@rm -f *.py~ *.pyo *.pyc *~ *.cache *.core MANIFEST
	@rm -rf build dist
	@for dir in $(SUBDIRS); do \
		if test -f $$dir/Makefile; then \
			make -C $$dir srcclean; \
		else \
			cd $$dir; \
			echo "Cleaning up in $$dir..."; \
			rm -f *.py~ *.pyo *.pyc *~ *.cache *.core *.html; \
		fi \
	done

# The \ \ ... sequence is an evil hack to circumvent a non-indentation
# with the BSD sed. Maybe someone knows a better method here...
# -i for sed is a BSDish extension.
strip:
	@echo "Stripping debugging contents..."
	@echo "/STRIP_DEBUG_RELEASE_REPLACE/c\\" > sed_replace.tmp
	@echo "\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ sprite.update ()" >> sed_replace.tmp
	@echo "/STRIP_DEBUG_RELEASE/d" >> sed_replace.tmp
	@for file in $(DEBUGFILES); do \
		sed -i.bak -f sed_replace.tmp $$file; \
	done
	@echo "Cleaning temporary strip files..."
	@rm -f sed_replace.tmp
	@find ./ -name "*.bak" |xargs rm

release: clean strip docs build dist
