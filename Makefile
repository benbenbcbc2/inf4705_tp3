.PHONY: all clean cleanalgos dist rapport

all :
	make -C common
	make -C runAlgo
	make -C evalSolution

cleanalgos :
	make clean -C common
	make clean -C runAlgo
	make clean -C evalSolution


# Commands
MAKE=make
RM=rm
PDFLATEX=pdflatex
ZIPNAME=matricule1_matricule2_tp3

# File and directory locations
M4SOURCE=rapport.m4
SOURCE=rapport.tex
BIBFILE=$(SOURCE:%.tex=%.bib)

rapport: $(SOURCE:%.tex=%.pdf)

${SOURCE} : ${M4SOURCE}
	m4 ${@:%.tex=%.m4} > $@

$(SOURCE:%.tex=%.pdf) : ${SOURCE}
	pdflatex --shell-escape $(@:%.pdf=%.tex)
	pdflatex --shell-escape $(@:%.pdf=%.tex)
	pdflatex --shell-escape $(@:%.pdf=%.tex)

clean: cleanalgos
	-rm $(SOURCE:%.tex=%.pdf) $(SOURCE:%.tex=%.aux) \
            $(SOURCE:%.tex=%.out) $(SOURCE:%.tex=%.log) \
            $(SOURCE:%.tex=%.bbl) $(SOURCE:%.tex=%.blg) \
            $(SOURCE:%.tex=%.toc)
	-rm *.eps *converted-to.pdf *.gnuplot *gnuplottex*

dist: all
	zip ${ZIPNAME} {common,runAlgo,evalSolution}/*.{cpp,h}
	zip ${ZIPNAME} Makefile README.md rapport.{m4,pdf} tp.sh
