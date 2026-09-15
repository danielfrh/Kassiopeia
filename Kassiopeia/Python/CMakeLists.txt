# $Id$

set( KASSIOPEIA_PYTHON_SOURCES
    KassiopeiaReader.py
    Fields/PlotFields-fromFieldCalculator.py
    Fields/PlotFields-fromKassiopeia.py
)

install(FILES ${KASSIOPEIA_PYTHON_SOURCES} ${KASSIOPEIA_PYTHON_HEADERS} DESTINATION ${${PROJECT_NAME}_LIB_INSTALL_DIR}/python)
