## Quickly compile LaTex documents with the make command
## Make sure this file is named "Makefile" (duh!)
PROJECT=Assignment02

all: *.tex
	pdflatex $(PROJECT).tex
test: *.pdf
	xpdf $(PROJECT).pdf 2> /dev/null &

clean:
	##rm $(PROJECT).pdf
	rm *.log
	rm *.aux
