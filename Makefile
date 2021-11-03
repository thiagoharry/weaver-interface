doc:
	magitex-cweb weaver-interface.tex
	dvipdf weaver-interface.dvi
doc_en:
	tex weaver-interface_en.tex
	dvipdf weaver-interface_en.dvi
test:
	ctangle weaver-interface.tex
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c tests/window.c src/interface.c  -o test_interface -lX11 -lEGL -lGLESv2 -lm
	./test_interface
test_en:
	ctangle weaver-interface_en.tex
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c tests/window.c src/interface.c  -o test_interface -lX11 -lEGL -lGLESv2 -lm
	./test_interface
clean:
	rm -f *~ *.core *.scn *.dvi *.idx *.log 
