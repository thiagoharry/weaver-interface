doc:
	magitex-cweb weaver-interface.tex
	dvipdf weaver-interface.dvi
doc_en:
	tex weaver-interface_en.tex
	dvipdf weaver-interface_en.dvi
clean:
	rm -f *~ *.core *.scn *.dvi *.idx *.log 
