# "Improved" Makefile for LaTeX documents
# INTPUFILE This defines the name of the file you want to latex
# the make file will automatically look for INPUT.tex as the latex file
INPUTFILE=quiz

ifeq ($(OS),Windows_NT)
	detected_OS := Windows
else
	detected_OS := $(shell uname)
endif

ifeq ($(detected_OS),Windows)
	OPEN_APP := start
else
	ifeq ($(detected_OS),Linux)
		OPEN_APP := xdg-open
	else
		OPEN_APP := open
	endif
endif


default:
	latexmk -output-directory=./build/ -pdf ${INPUTFILE}
	@echo '****************************************************************'
	@echo '******** Did you spell-check the paper? ********'
	@echo '****************************************************************'

pdf: default

clean:
	latexmk -output-directory=./build/ -c ${INPUTFILE}
	rm -f ${INPUTFILE}.bbl

allclean:
	latexmk -output-directory=./build/ -C ${INPUTFILE}
	rm -f ${INPUTFILE}.bbl

monitor:
	latexmk -pdf -pvc ${INPUTFILE}

cleaner: allclean

view: build/${INPUTFILE}.pdf
	${OPEN_APP} build/${INPUTFILE}.pdf

${INPUTFILE}.pdf: default

count:
	texcount -brief *.tex
