src_dir= `pwd`

manual_src = manual.xml
manual_out = manual.html
xsl_file = ocempgui.xsl

IMGFILES = `find img -type f -name "*.dia"`
UMLFILES = `find uml -type f -name "*.dia"`

all: html diagrams

html: $(manual) $(xsl_file) images
	@echo "Creating HTML manual..."
	@xsltproc --xinclude --output $(manual_out) $(xsl_file) $(manual_src)

images:
	@for FILE in $(IMGFILES); do \
		NAME=`echo "$$FILE" |sed -e 's/.dia//g'`.png; \
		echo "Creating $$NAME..."; \
		dia --export=$$NAME -t png $$FILE; \
	done

diagrams:
	@for FILE in $(UMLFILES); do \
		NAME=`echo "$$FILE" |sed -e 's/\.dia//g'`.png; \
		echo "Creating $$NAME..."; \
		dia --export=$$NAME -t png $$FILE; \
	done

srcclean:
	@echo "Cleaning up in doc/ ..."
	@rm -f *.html *~ *.core
	@echo "Cleaning up in doc/uml/ ..."
	@rm -f uml/*.png uml/*~ uml/*.core
	@echo "Cleaning up in doc/examples/ ..."
	@cd $(src_dir)/examples; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/examples/tictactoe ..."
	@cd $(src_dir)/examples/tictactoe; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/examples/chat ..."
	@cd $(src_dir)/examples/chat; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/img/ ..."
	@cd $(src_dir)/img; rm -f *~ *.cache *.core *.png

clean:
	@echo "Cleaning up in doc/ ..."
	@rm -f *~ *.core
	@echo "Cleaning up in doc/uml/ ..."
	@rm -f uml/*~ uml/*.core
	@echo "Cleaning up in doc/examples/ ..."
	@cd $(src_dir)/examples; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/examples/tictactoe ..."
	@cd $(src_dir)/examples/tictactoe; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/examples/chat ..."
	@cd $(src_dir)/examples/chat; rm -f  *.py~ *.pyo *.pyc *~ *.cache *.core
	@echo "Cleaning up in doc/img/ ..."
	@cd $(src_dir)/img; rm -f *~ *.cache *.core
