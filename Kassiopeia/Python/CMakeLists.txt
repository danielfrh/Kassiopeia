# $Id$

set( KASSIOPEIA_PYTHON_SOURCES
    KassiopeiaReader.py
    Fields/FieldCalculatorHelper-1dim-Compare2Fields.py
    Fields/FieldCalculatorHelper-1dim-Compare2FieldsRelError.py
    Fields/FieldCalculatorHelper-2dim-HeatMap.py
    Fields/KassiopeiaHelper-1dim-PlotFieldFromROOTFile.py
)

install(FILES ${KASSIOPEIA_PYTHON_SOURCES} ${KASSIOPEIA_PYTHON_HEADERS} DESTINATION ${${PROJECT_NAME}_LIB_INSTALL_DIR}/python)
