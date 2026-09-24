# $Id$

set( KASSIOPEIA_PYTHON_SOURCES
    KassiopeiaReader.py
    Fields/PlotFields-fromFieldCalculator-1dim-compare.py
    Fields/PlotFields-fromFieldCalculator-2dim.py
    Fields/PlotFields-fromKassiopeia-1dim.py
)

install(FILES ${KASSIOPEIA_PYTHON_SOURCES} ${KASSIOPEIA_PYTHON_HEADERS} DESTINATION ${${PROJECT_NAME}_LIB_INSTALL_DIR}/python)
