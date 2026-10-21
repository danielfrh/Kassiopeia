// Transform MagfieldCoils input to XML for Kassiopeia

#include <cstdlib>
#include <iostream>
#include <cassert>
#include <cmath>

#include <string>
#include <istream>
#include <ostream>
#include <ostream>

#include <array>
#include <vector>

#include <cstdlib>
#include <fstream>
#include <sstream>

#include "KThreeVector.hh"
#include "Transform_Input.hh"


using namespace std;
using namespace katrin;

// Transformation of MagfieldCoils Input to XML geometry- and model-file
// argv[1]: Input coil file name in MagfieldCoils format
// argv[2]: Name of the simulation output

// Updates:
// 18.10.2026: Rotation and trafo -x/-y to interface class KMagfieldCoilsFieldSolver
// 15.10.2026: Trafo of x- and y-coord. and rotation added
// 13.10.2026: Initial version


int main(int argc, char** argv)
{
    std::vector<Coil> ferencCoilsNew;
    fstream inputCoils;

    if( argc < 3 ) {
        std::cout << "Not all parameters given, exit program." << std::endl;
        exit(1);
    }

    string simName = string(argv[2]);

    string theInputFile = string(argv[1]);

    inputCoils.open(theInputFile.c_str());
    if (!inputCoils.is_open()) {
        puts("Cannot open the  input source file!");
        puts("Program running is stopped !!! ");
        exit(1);
    }

    unsigned int fNLines;
    inputCoils >> fNLines;
    cout << "Read " << fNLines << " = fNLines coils by " << theInputFile << endl;
    if (fNLines < 1) {
        puts("fNLines is not positive !");
        puts("Program running is stopped !!! ");
        exit(1);
    }

    // variables for reading file in MagfieldCoils-format:
    std::stringstream coilName;
    double readCurrent;
    KThreeVector readCenterPoint;
    KThreeVector readEulerAngles; // please note that only the first two components are significant and will be used
    double readTurns;
    double readLength;
    double readRmin;
    double readRmax;


    // reading MagfieldCoils input file
    for( unsigned int i = 0; i < fNLines; i++ )
    {
        std::stringstream coilName;
        coilName << "Coil" << i;

        inputCoils >> readCurrent >> readCenterPoint[0] >> readCenterPoint[1] >> readCenterPoint[2] >> readEulerAngles[0] >> readEulerAngles[1];
        inputCoils >> readTurns >> readLength >> readRmin >> readRmax;

        // coord trafo needed for Kassiopeia XML
        //readCenterPoint[0] = -1. * readCenterPoint[0];
        //readCenterPoint[1] = -1. * readCenterPoint[1];

        // rotation
        //readEulerAngles[0] = readEulerAngles[0] + 180.;
        //readEulerAngles[1] = readEulerAngles[1] + 180.;

        Coil myCoil( coilName.str(), readCurrent, readTurns, readCenterPoint, readEulerAngles, readLength, readRmin, readRmax );

        //myCoil.PrintParameters();

        ferencCoilsNew.push_back( myCoil );
    }

    inputCoils.close();

    XMLFileWriter outputFiles;


    //
    // geometry
    //


    // open output geometry file and write XML

    std::ofstream geoFile(simName+"_Geometry.xml");
    if( geoFile.is_open() ) outputFiles.WriteGeoOpening( geoFile );

    for( unsigned int i = 0; i < ferencCoilsNew.size(); i++ ) {
        if( geoFile.is_open() ) outputFiles.WriteTubeSpaceElement( geoFile, ferencCoilsNew[i] );
    }

    if( geoFile.is_open() ) outputFiles.WriteSpaceOpening( geoFile );

    for( unsigned int i = 0; i < ferencCoilsNew.size(); i++ ) {
        if( geoFile.is_open() ) outputFiles.WriteSpaceElement( geoFile, ferencCoilsNew[i] );
    }

    if( geoFile.is_open() ) outputFiles.WriteSpaceClosing( geoFile );

    if( geoFile.is_open() ) outputFiles.WriteGeoClosing( geoFile );

    // close geometry output file
    geoFile.close();

    //
    // magnet model
    //

    // open output model file and write XML

    std::ofstream modelFile(simName+"_Model.xml");

    if( modelFile.is_open() ) outputFiles.WriteIncludeForGeoFile( modelFile, simName );

    if( modelFile.is_open() ) outputFiles.WriteGeoOpening( modelFile );
    
    for( unsigned int i = 0; i < ferencCoilsNew.size(); i++ ) {
        if( modelFile.is_open()) outputFiles.WriteElectromagnet( modelFile, ferencCoilsNew[i] );
    }

    if( modelFile.is_open()) outputFiles.WriteGeoClosing( modelFile );

    // close model output file
    modelFile.close();

    std::cout << "=> XML-geometry-file " << simName+"_Geometry.xml and XML-model-file " <<  simName+"_Model.xml successfully been written from MagfieldCoils input file " << theInputFile << std::endl;

    return 0;
};
