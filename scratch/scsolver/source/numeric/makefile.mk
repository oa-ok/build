PRJ=..$/..
PRJNAME=scsolver
TARGET=numeric
ENABLE_EXCEPTIONS=TRUE

# --- Settings -----------------------------------------------------

.INCLUDE :  settings.mk
.INCLUDE :  $(PRJ)$/util$/makefile.pmk

# --- Files --------------------------------------------------------

#
# FIXME - should go in workben ?
#
TESTFILES= \
        $(SLO)$/nlp_test.obj \
        $(SLO)$/matrix_test.obj \
        $(SLO)$/simplex_test.obj

SLOFILES =  \
        $(SLO)$/diff.obj \
        $(SLO)$/diff_test.obj \
        $(SLO)$/funcobj.obj \
        $(SLO)$/lpbase.obj \
        $(SLO)$/lpmodel.obj \
        $(SLO)$/lpsimplex.obj \
        $(SLO)$/matrix.obj \
        $(SLO)$/nlpbase.obj \
        $(SLO)$/nlpmodel.obj \
        $(SLO)$/nlpqnewton.obj \

# --- Tagets -------------------------------------------------------

.INCLUDE :  target.mk

