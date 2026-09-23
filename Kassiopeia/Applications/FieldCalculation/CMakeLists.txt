option (Kassiopeia_ENABLE_APP "Build Kassiopeia applications" ON)
if (Kassiopeia_ENABLE_APP)

    set( APPLICATIONS_FIELDS_HEADER_BASENAMES
        FieldCalculator.h
    )

    set( APPLICATIONS_FIELDS_SOURCE_BASENAMES
        FieldCalculator
    )

    set( APPLICATIONS_FIELDS_BASENAMES

        SimpleElectricFieldCalculator
        SimpleElectricFieldCalculatorAlongZaxis
        SimpleElectricFieldCalculatorAlongFieldline
        SimpleElectricFieldCalculatorOverXYplane
        SimpleElectricFieldCalculatorFromFile
        SimpleElectricFieldCalculatorSpeedTest

        SimpleMagneticFieldCalculator
        SimpleMagneticFieldCalculatorAlongZaxis
        SimpleMagneticFieldCalculatorAlongFieldline
        SimpleMagneticFieldCalculatorOverXYplane
        SimpleMagneticFieldCalculatorFromFile
        SimpleMagneticFieldCalculatorSpeedTest

        SimpleMagneticGradientCalculator
    )

    set( APPLICATIONS_FIELDS_HEADER_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
    )

    set( APPLICATIONS_FIELDS_SOURCE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    )

    set( APPLICATIONS_FIELDS_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    )

    foreach( BASENAME ${APPLICATIONS_FIELDS_SOURCE_BASENAMES} )
        list( APPEND APPLICATIONS_FIELDS_SOURCE_FILENAMES ${APPLICATIONS_SOURCE_HEADER_PATH}/${BASENAME} )
    endforeach( BASENAME )

    foreach( BASENAME ${APPLICATIONS_FIELDS_HEADER_BASENAMES} )
        list( APPEND APPLICATIONS_FIELDS_HEADER_FILENAMES ${APPLICATIONS_FIELDS_HEADER_PATH}/${BASENAME} )
    endforeach( BASENAME )


    # data files
    set( APPLICATIONS_FIELDS_DATA_BASENAMES
        FieldCalculator_ExampleFieldPoints.txt
    )
    set( APPLICATIONS_FIELDS_DATA_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/Data
    )

    foreach( BASENAME ${APPLICATIONS_FIELDS_DATA_BASENAMES} )
        list( APPEND APPLICATIONS_FIELDS_DATA_FILENAMES ${APPLICATIONS_FIELDS_DATA_PATH}/${BASENAME} )
    endforeach( BASENAME )

    foreach( BASENAME ${APPLICATIONS_FIELDS_SOURCE_BASENAMES} )
        # if cxx needs of header, header and source have the same name
        add_executable( ${BASENAME} ${APPLICATIONS_FIELDS_SOURCE_PATH}/${BASENAME}.cxx )
        target_include_directories(${BASENAME}
        PUBLIC $<BUILD_INTERFACE:${APPLICATIONS_FIELDS_HEADER_PATH}> $<INSTALL_INTERFACE:include> )
            target_link_libraries( ${BASENAME} KassiopeiaBindings KGeoBagBindings KEMFieldBindings )
    endforeach( BASENAME )

    foreach( BASENAME ${APPLICATIONS_FIELDS_BASENAMES} )
        add_executable( ${BASENAME} ${APPLICATIONS_FIELDS_PATH}/${BASENAME}.cxx )
        target_link_libraries( ${BASENAME} KassiopeiaBindings KGeoBagBindings KEMFieldBindings )
    endforeach( BASENAME )


    kasper_install_executables( ${APPLICATIONS_FIELDS_BASENAMES} )
    kasper_install_executables( ${APPLICATIONS_FIELDS_SOURCE_BASENAMES} )
    kasper_install_headers( ${APPLICATIONS_FIELDS_HEADER_FILENAMES} )
    kasper_install_data( ${APPLICATIONS_FIELDS_DATA_FILENAMES} )

endif(Kassiopeia_ENABLE_APP)
