doc:
	magitex-cweb weaver-interface.tex
	dvipdf weaver-interface.dvi
doc_en:
	tex weaver-interface_en.tex
	dvipdf weaver-interface_en.dvi
test:
	ctangle weaver-interface.tex
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c tests/window.c src/interface.c  -o test_interface -lX11 -lEGL -lGLESv2 -lm
	./test_interface
test_en:
	ctangle weaver-interface_en.tex
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -DW_DEBUG_INTERFACE -Wall -O2 tests/test.c tests/window.c src/interface.c  -o test_interface -lX11 -lEGL -lGLESv2 -lm
	./test_interface
test_web:
	ctangle weaver-interface.tex
	emcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -s ASYNCIFY=1 -Wall -O2 tests/test.c tests/window.c src/interface.c  -o docs/test_interface.html
test_web_en:
	ctangle weaver-window_en.tex
	emcc $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -DW_FORCE_LANDSCAPE -s ASYNCIFY=1 -Wall -O2 tests/test.c tests/window.c src/interface.c  -o docs/test_interface.html
clean:
	rm -f *~ *.core *.scn *.dvi *.idx *.log 
