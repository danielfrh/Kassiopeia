#include "KMessage.h"
#include "KRandom.h"
#include "KSFieldFinder.h"
#include "KSMainMessage.h"
#include "KSRootMagneticField.h"
#include "KTextFile.h"
#include "KThreeMatrix.hh"
#include "KThreeVector.hh"
#include "KXMLInitializer.hh"
#include "KXMLTokenizer.hh"

#include <cstdlib>
#include <iostream>
#include <sstream>
// time measurement, field point generator and field point set reader
#include "FieldCalculator.h"

#define ELECTRIC 0
#define MAGNETIC 1


int main(int argc, char** argv)
{
    cout << "FieldCalculator: Field calculation field line / map definition and output to text file (both optional in text files), field configs within XML file." << endl;
    cout << "output (optional): either field lines or field maps in files which can be processed by Python script PlotFields-fromFieldCalculator.py" << endl;
    cout << "usage: ./FieldCalculator <config_file.xml> <Computation Mode> <Input File> <Write output file (0,1)> <field_name1> [<field_name2> <...>] " << endl << endl;

    if (argc < 4) {
        cout << "Missing arguments, program stopped!" << endl;
        exit(-1);
    };

    // ---------------------------------------
    // initialization and gathering parameters
    // ---------------------------------------

    auto& tXML = KXMLInitializer::GetInstance();
    tXML.AddDefaultIncludePath(CONFIG_DEFAULT_DIR);
    tXML.Configure(argc, argv, true);

    deque<string> tParameters = tXML.GetArguments().ParameterList();
    tParameters.pop_front();  // strip off config file name

    // scale: field points between start- and end point
    const unsigned int myScale( 1e7 );

    // precision for output of values
    const unsigned int myPrecision( 16 );
    KMessageTable::GetInstance().SetPrecision( myPrecision );
    cout.precision( myPrecision );

    // dimensions: fieldlines (dim 1), field maps (dim 2)
    unsigned int myDimension( 1 );

    // saving different point sets to vector
    std::vector<FieldPointGenerator> pointSets;

    const string inMode(tParameters[0]);
    int tMode = stoi(inMode);
    cout<<"Mode: (0 = manual, 1 field point file, 2 field point set file): "<<tMode<<endl;

    // file name
    //istringstream Converter(tParameters[1]);
    //const string tInputFileName = Converter.str();
    const string tInputFileName = tParameters[1];
    cout<<"tInputFileName: "<<tInputFileName<<endl;

    if( tMode==1 ) { // mode = 1 : input file: 1-dim field points (= 1 field point set)
        myDimension = 1;
        cout << "read one field point set from file - 1 Dimension (field lines) - only 1 dim!" << endl;
        FieldPointGenerator generatorFromFile( "configfile1", tInputFileName );
        pointSets.push_back( generatorFromFile );
    } else if( tMode==2 ) { // mode = 2 : read field point sets from file
        cout << "read field point set(s) from file - 1 Dimension (field lines) or 2 Dimensions (field maps)" << endl;
        FieldPointSetReader readFileSet( tInputFileName, pointSets);
    }
    // ------------------------------------------------
    // define point sets for all fields - tFieldObjects
    // ------------------------------------------------

    if( tMode==0 ) // mode = 0 : define field points manually
    {
        cout << "define fieldpoint sets manually within source file - 1 Dimension (field lines) or 2 Dimensions (field maps)" << endl;
        mainmsg(eNormal) << "START: Computation of manual defined point sets (containing field point vectors) for all definied fields." << eom;

        // field points on-axis
        const unsigned int scaleOnAxis( myScale/100 );
        const unsigned int dimensionOnAxis( myDimension );
        const KThreeVector startOnAxis(0., 0., -0.75);
        const KThreeVector endOnAxis(0., 0., 0.75);
        FieldPointGenerator fieldOnAxis(
            "Fields-1d1e7-OnAxis",
            dimensionOnAxis,
            scaleOnAxis,
            startOnAxis,
            endOnAxis
        );
        pointSets.push_back( fieldOnAxis );

        // Comparison 1: r=5cm, near z=0, myScale = 1e7 points:
        const unsigned int scaleComp1OffAxis( myScale/100 );
        const unsigned int dimensionComp1OffAxis( myDimension );
        const KThreeVector startComp1OffAxis(0., 0.05, -0.025);
        const KThreeVector endComp1OffAxis(0., 0.05, 0.025);
        FieldPointGenerator fieldComp1OffAxis(
            "Fields-5cm-OffAxis",
            dimensionComp1OffAxis,
            scaleComp1OffAxis,
            startComp1OffAxis,
            endComp1OffAxis
        );
        pointSets.push_back( fieldComp1OffAxis );

        // Comparison 1: r=9.5cm, near coils, myScale = 1e7 points:
        const unsigned int scaleComp1Remote( myScale );
        const unsigned int dimensionComp1Remote( myDimension );
        const KThreeVector startComp1Remote(0., 0.095, -0.025);
        const KThreeVector endComp1Remote(0., 0.095, 0.025);
        FieldPointGenerator fieldComp1Remote(
            "Fields-10cm-Remote",
            dimensionComp1Remote,
            scaleComp1Remote,
            startComp1Remote,
            endComp1Remote
        );
        //pointSets.push_back( fieldComp1Remote );

        // Comparison 2: r=5cm, near z=0, myScale = 1e7 points:
        const unsigned int scaleComp2OffAxis( myScale );
        const unsigned int dimensionComp2OffAxis( myDimension );
        const KThreeVector startComp2OffAxis(0., 0.05, -0.75);
        const KThreeVector endComp2OffAxis(0., 0.05, 0.75) ;
        FieldPointGenerator fieldComp2OffAxis(
            "Fields-5cm-OffAxis",
            dimensionComp2OffAxis,
            scaleComp2OffAxis,
            startComp2OffAxis,
            endComp2OffAxis
        );
        //pointSets.push_back( fieldComp2OffAxis );

        // Comparison 2: r=9.5cm, near coils, myScale = 1e7 points:
        const unsigned int scaleComp2Remote( myScale );
        const unsigned int dimensionComp2Remote( myDimension );
        const KThreeVector startComp2Remote(0., 0.85, -0.75);
        const KThreeVector endComp2Remote(0., 0.85, 0.75);
        FieldPointGenerator fieldComp2Remote(
            "Fields-10cm-Remote",
            dimensionComp2Remote,
            scaleComp2Remote,
            startComp2Remote,
            endComp2Remote
        );
        //pointSets.push_back( fieldComp2Remote );

        mainmsg(eNormal) << eom << "DONE: Computation of all calculation point vectors for " << pointSets.size()*myScale << " field points" << eom;

    }

    std::cout << "point sets size " << pointSets.size() << std::endl;

    // option to write output file: 0, 1 valid, true, false invalid
    // By default, std::cin only accepts numeric input for Boolean variables: 0 is false, and 1 is true.
    // Any other numeric value will be interpreted as true, and will cause
    // std::cin to enter failure mode. Any non-numeric value will be interpreted as false and will cause std::cin to enter failure mode.    
    // To convert an integer to a boolean in C++, any non-zero integer will be converted to true,
    // while zero will be converted to false. This is done implicitly when assigning an integer to a boolean variable.
    istringstream Converter(tParameters[2]);
    bool writeToFiles;
    Converter >> writeToFiles;
    cout<<"writeToFiles: "<<writeToFiles<<endl;

    // initialitzing variables for measurement of computation times
    uint64 tStartTime( 0 );
    uint64 tStopTime( 0 );
    uint64 tTimeSum( 0 );

#ifdef MAGNETIC
#if MAGNETIC == 1

    // -----------------------
    // init of magnetic fields
    // -----------------------

    // initialize magnetic field
    std::vector<KSMagneticField*> tMagneticFields;

    for (size_t tIndex = 3; tIndex < tParameters.size(); tIndex++) {
        KSMagneticField* tMagneticFieldObject = getMagneticField(tParameters[tIndex]);
        std::cout << tMagneticFieldObject->GetName() << std::endl;
        tMagneticFieldObject->Initialize();
        mainmsg(eNormal) << "Initialization of " << tMagneticFieldObject->GetName() << " finished." << eom << eom;
        tMagneticFields.push_back(tMagneticFieldObject);
    }

    KThreeVector tMagneticField;
    // TODO: KThreeMatrix tMagneticFieldGradient;

#endif
#endif

// TODO: KThreeVector tElectricField

std::cout << "here" << std::endl;


    // ----------------------------------
    // for-loop over tFieldObjects
    // ----------------------------------
    for (auto& tFieldObject : tMagneticFields)
    {
        // ------------------------------
        // for-loop over field point sets
        // ------------------------------

        for( auto it1PointSets:pointSets )
        {
            mainmsg(eNormal) << "START: Computation of fields with config " << it1PointSets.GetName() << " by " << tFieldObject->GetName() << eom;

            // setting composed name with current tFieldObject
            it1PointSets.SetName( tFieldObject->GetName() + it1PointSets.GetName() );
            
            if(!tFieldObject->IsInitialized()) tFieldObject->Initialize();

            tTimeSum = 0;
            
            mainmsg( eDebug ) << it1PointSets.theResultVector.size() << "  " << it1PointSets.GetName() << eom;
            
            // for-loop over pointSets.at(i)
            for ( unsigned int j = 0; j < it1PointSets.theResultVector.size(); j++)
            {
                try
                {
                    //pmagfield->MagfieldElliptic(P, B);
                    //bell=sqrt(B[1]*B[1]+B[2]*B[2]+B[0]*B[0]);
                    //bool zonal = pmagfield->Magfield(P, B);
                    
                    //BField = magfieldCoils.MagneticField(pos);
                    //BField = integratingFieldSolver.MagneticField(pos);
                    
                    mainmsg( eDebug ) << "position: " << it1PointSets.theResultVector[j].myPosition << eom;
                    
                    // setting start time for field config with point set pointSets.at(i)
                    tStartTime = GetTimeMs64();

                    tFieldObject->CalculateField(it1PointSets.theResultVector[j].myPosition, 0.0, tMagneticField);
                    //tFieldObject->CalculateFieldAndGradient(P,0.0,tMagneticField,tMagneticFieldGradient);
                    
                    tStopTime = GetTimeMs64();

                    tTimeSum += (tStopTime - tStartTime);
                    // store field value to vector
                    it1PointSets.theResultVector[j].myField = tMagneticField;

                    // for (int j = 0; j <= 2; j++) {
                    //     n[j] = -BField[j] / b;
                    //     pos[j] += ds * n[j];
                    // }

                } // try
                catch (...)
                {
                    int tIndex = 0;
                    mainmsg(eWarning) << "> error processing index <" << tIndex << "> - cannot calculate field at position <"
                        << it1PointSets.theResultVector[j].myPosition << ">" << eom;
                    continue;
                } // catch

            } // for - field single point set
            
            mainmsg(eNormal) << "> Elapsed time for field <" << tFieldObject->GetName() << "> and config " << it1PointSets.GetName() << " with " << it1PointSets.theResultVector.size() << " values is <" << tTimeSum << "> ms " << eom;
            mainmsg(eNormal) << eom;

            // n.b. for magfield coils the count of computed field points field and used
            // calculation method does not work, instead the values will be summes up and
            // have to be computed manually
            tFieldObject->Deinitialize();

            // --------------------------------------
            // writing positions and results to files
            // --------------------------------------

            //const char* env_p = std::getenv("HOME");

            if( writeToFiles ) {

                katrin::KTextFile* outputFile = katrin::KTextFile::CreateOutputTextFile("./" , string(it1PointSets.GetName()+".csv"));
                mainmsg(eNormal) << "WRITING results to file " << outputFile->GetDefaultPath() << "/" << outputFile->GetDefaultBase() << eom;
                mainmsg( eNormal) << "label: " << it1PointSets.GetName() << eom;
                outputFile->Open(KFile::eWrite);
                fstream& file1=*(outputFile->File());
                file1 << "Id" << "\t" << "x" << "\t" << "y" << "\t" << "z" << "\t" << "Bx" << "\t" << "By" << "\t" << "Bz" << "\t" << "absB" << endl;

                for (unsigned int m = 0; m < it1PointSets.theResultVector.size(); m++) {
                    file1 << m << "\t" << std::scientific << std::setprecision( myPrecision )
                    << it1PointSets.theResultVector[m].myPosition[0] << "\t"
                    << it1PointSets.theResultVector[m].myPosition[1] << "\t"
                    << it1PointSets.theResultVector[m].myPosition[2] << "\t"
                    << it1PointSets.theResultVector[m].myField[0] << "\t"
                    << it1PointSets.theResultVector[m].myField[1] << "\t"
                    << it1PointSets.theResultVector[m].myField[2] << "\t"
                    << it1PointSets.theResultVector[m].myField.Magnitude() << "\n";
                }

                outputFile->Close();

            } // if-write to files
        } // point set
    } // for - loop over tFieldObjects

    for (size_t tIndex = 3; tIndex < tParameters.size(); tIndex++) {
        KSMagneticField* tMagneticFieldObject = getMagneticField(tParameters[tIndex]);
        tMagneticFieldObject->Deinitialize();
    }

    return 0;

};