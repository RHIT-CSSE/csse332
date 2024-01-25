# "Improved" Makefile for LaTeX documents
# INTPUFILE This defines the name of the file you want to latex
# the make file will automatically look for INPUT.tex as the latex file

# check if main file name is in .latexmain, if not, ask for it and save it.
MAINFILE := $(shell if [ -f .latexmain ]; then cat .latexmain; else read -p "Enter main tex file name: " name; echo $$name > .latexmain; cat .latexmain; fi)
INPUTFILE := $(shell basename -s .tex $(MAINFILE))
OTHERFILES := $(shell find . -name '*.tex' -not -name $(MAINFILE))

LMK_FLAGS := -output-directory=./build/ -pdf -pdflatex="pdflatex -synctex=1"

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

all: build/${INPUTFILE}.pdf

build/${INPUTFILE}.pdf: ${INPUTFILE}.tex $(OTHERFILES)
	latexmk ${LMK_FLAGS} ${INPUTFILE}
	@echo '****************************************************************'
	@echo '******** Did you spell-check the paper? ********'
	@echo '****************************************************************'

clean:
	latexmk -output-directory=./build/ -c ${INPUTFILE}
	rm -rf ${INPUTFILE}.bbl ./build/ *.bak

cleaner:
	latexmk -output-directory=./build/ -C ${INPUTFILE}
	rm -f ${INPUTFILE}.bbl ./build/ *.bak

view: build/${INPUTFILE}.pdf
	${OPEN_APP} build/${INPUTFILE}.pdf

count:
	texcount -brief *.tex

# add an option to watch for changes in the file and recompiled directly
watch: LMK_FLAGS += -pvc -quiet
watch: build/${INPUTFILE}.pdf

debug: ${INPUTFILE}.tex
	@sed -i '.bak' 's/^\\releasetrue/\\releasefalse/' ${INPUTFILE}.tex
	latexmk -output-directory=./build/ -pdf ${INPUTFILE}

release: ${INPUTFILE}.tex
	@sed -i '.bak' 's/^\\releasefalse/\\releasetrue/' ${INPUTFILE}.tex
	latexmk -output-directory=./build/ -pdf ${INPUTFILE}

.PHONY: count clean cleaner monitor all view

