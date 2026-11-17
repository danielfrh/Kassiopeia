# $Id$

set( KASSIOPEIA_PYTHON_SOURCES
    KassiopeiaReader.py
    Fields/FieldCalculatorElectricHelper-1dim-CompareEmag.py
    Fields/FieldCalculatorElectricHelper-1dim-ComparePotential.py
    Fields/FieldCalculatorElectricHelper-2dim-HeatMapEmag.py
    Fields/FieldCalculatorElectricHelper-2dim-HeatMapPotential.py
    Fields/FieldCalculatorHelper-ConvertFieldLinePoints.py
    Fields/FieldCalculatorMagneticHelper-1dim-CompareBmag.py
    Fields/FieldCalculatorMagneticHelper-1dim-CompareBz.py
    Fields/FieldCalculatorMagnticHelper-2dim-HeatMapBmag.py
    Fields/FieldCalculatorMagnticHelper-2dim-HeatMapBz.py
    Fields/KassiopeiaHelper-1dim-PlotFieldFromROOTFile.py
)

install(FILES ${KASSIOPEIA_PYTHON_SOURCES} ${KASSIOPEIA_PYTHON_HEADERS} DESTINATION ${${PROJECT_NAME}_LIB_INSTALL_DIR}/python)
