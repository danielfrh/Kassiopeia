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

// timing function
#include <ctime>
#include <sys/time.h>

/* Remove if already defined */
typedef long long int64;
using uint64 = unsigned long long;

/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */

uint64 GetTimeMs64()
{
    /* Linux */
    struct timeval tv;

    gettimeofday(&tv, nullptr);

    uint64 ret = tv.tv_usec;
    /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
    ret /= 1000;

    /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
    ret += (tv.tv_sec * 1000);

    return ret;
}

using namespace Kassiopeia;
using namespace katrin;
using namespace std;


struct tResult{
    KThreeVector myPosition;
    KThreeVector myField;
};

class fieldPoints{
    public:
        fieldPoints( string label, unsigned int dim, unsigned int no, KThreeVector start, KThreeVector end) {
            completeName=label;calcDimensions=dim;noPoints=no;startPoint=start;endPoint=end;
            if(noPoints<2) {
                mainmsg( eWarning ) << "Please note that at least 2 points have to be computed, set noPoints=2." << eom;
                noPoints = 2;
            }
            theResultVector.clear();
        };

        void ComputeFieldPoints1dim( void );
        void ComputeFieldPoints2dim( void );

        std::vector<tResult> theResultVector;

        std::string GetName(){return completeName;};
        void SetName( std::string input ){completeName=input;return;};
    private:
        string completeName;
        unsigned int calcDimensions;
        unsigned int noPoints;
        KThreeVector startPoint;
        KThreeVector endPoint;
        KThreeVector normalVector;
};

void fieldPoints::ComputeFieldPoints1dim( void )
{
    completeName = completeName + "-1dim";

    // calc normal vector
    normalVector = (endPoint - startPoint) / (endPoint - startPoint).Magnitude();

    //length of vector
    double theLength = (endPoint - startPoint).Magnitude()/noPoints;

    tResult temp;
    KThreeVector empty(0., 0., 0.);
    KThreeVector calcPoint(0., 0., 0.);

    for(unsigned int i=0; i<=noPoints; i++){
        calcPoint = startPoint + ( i*theLength*normalVector );
        
        temp.myPosition = calcPoint;
        temp.myField = empty;

        theResultVector.push_back( temp );
    };

    
};

void fieldPoints::ComputeFieldPoints2dim( /*unsigned int noPoints, KThreeVector startPoint, KThreeVector endPoint*/ )
{
        completeName = completeName + "-2dim";
};

int main(int argc, char** argv)
{

    cout << "(16.09.2026) FieldCalculator: Linear field calculation with N dimensional output to text file, config field configs within XML file." << endl;
    cout << "usage: ./FieldCalculator <config_file.xml> <N dimensions: 1 or 2> <magnetic_field_name1> [<magnetic_field_name2> <...>] " << endl << endl;

    if (argc < 4) {
        cout << "Missing arguments, program stopped!" << endl;
        exit(-1); };

    // --------------
    // initialization
    // --------------

    auto& tXML = KXMLInitializer::GetInstance();
    tXML.AddDefaultIncludePath(CONFIG_DEFAULT_DIR);
    tXML.Configure(argc, argv, true);

    deque<string> tParameters = tXML.GetArguments().ParameterList();
    tParameters.pop_front();  // strip off config file name

    istringstream Converter(tParameters[0]);
    unsigned int myDimension( 1 );
    Converter >> myDimension;

    // -----------------------
    // computation of magnetic field vectors with time measurement
    // -----------------------

    KMessageTable::GetInstance().SetPrecision( 16 );
    cout.precision( 16 );

    bool writeToFiles = true;

    // initialize magnetic field
    vector<KSMagneticField*> tMagneticFields;

    for (size_t tIndex = 1; tIndex < tParameters.size(); tIndex++) {
        KSMagneticField* tMagneticFieldObject = getMagneticField(tParameters[tIndex]);
        tMagneticFieldObject->Initialize();
        mainmsg(eNormal) << "Initialization of " << tMagneticFieldObject->GetName() << " finished." << eom << eom;
        tMagneticFields.push_back(tMagneticFieldObject);
    }

    // ----
    KThreeVector tMagneticField;
    // KThreeMatrix tMagneticFieldGradient;
    // ----

    uint64 tStartTime( 0 );
    uint64 tStopTime( 0 );
    uint64 tTimeSum( 0 );

    // ----------------------------------
    // for-loop over tFieldObjects
    // ----------------------------------
    for (auto& tFieldObject : tMagneticFields)
    {
        // ------------------------------------------------
        // define point sets for current field tFieldObject
        // ------------------------------------------------

        mainmsg(eNormal) << "START: Computation of defined point sets (containing point vectors) for field " << tFieldObject->GetName() << eom;

        // field points on-axis
        KThreeVector startpointOnAxis(0., 0., -0.75);
        KThreeVector endpointOnAxis(0., 0., 0.75);
        unsigned int scaleOnAxis( 1e6 );

        // field points off-axis
//        KThreeVector startpointOffAxis(0., 0.05, -0.75);
//        KThreeVector endpointOffAxis(0., 0.05, 0.75);
//        unsigned int scaleOffAxis( 1e6 );
        KThreeVector startpointOffAxis(0., 0.05, -0.025);
        KThreeVector endpointOffAxis(0., 0.05, 0.025);
        unsigned int scaleOffAxis( 1e7 );

        // field points remote
//        KThreeVector startpointRemote(0., 0.85, -0.75);
//        KThreeVector endpointRemote(0., 0.85, 0.75);
//        unsigned int scaleRemote( 1e6 );
        KThreeVector startpointRemote(0., 0.095, -0.025);
        KThreeVector endpointRemote(0., 0.095, 0.025);
        unsigned int scaleRemote( 1e7 );


        fieldPoints fieldConfigOnAxis( "Fields-OnAxis", myDimension, scaleOnAxis, startpointOnAxis, endpointOnAxis );
        fieldPoints fieldConfigOffAxis( "Fields-OffAxis", myDimension, scaleOffAxis, startpointOffAxis, endpointOffAxis );
        fieldPoints fieldConfigRemote( "Fields-Remote", myDimension, scaleRemote, startpointRemote, endpointRemote );

        // saving different point sets to vector
        std::vector<fieldPoints> pointSets;
        pointSets.push_back( fieldConfigOnAxis );
        pointSets.push_back( fieldConfigOffAxis );
        pointSets.push_back( fieldConfigRemote );

        // ------------------------------
        // for-loop over field point sets
        // ------------------------------

        for( auto it1PointSets:pointSets )
        {
            if(myDimension==1) it1PointSets.ComputeFieldPoints1dim(); // case N=1
            //if(myDimension==2) wgtsCenter.ComputeFieldPoints2dim(); // N=2
            mainmsg(eNormal) << eom << "DONE: Computation of " << it1PointSets.theResultVector.size() << " calculation point vectors for " << it1PointSets.GetName() << " and " << eom;
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
                    file1 << m << "\t" << std::scientific << std::setprecision(16) << it1PointSets.theResultVector[m].myPosition[0] << "\t"
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

    for (size_t tIndex = 1; tIndex < tParameters.size(); tIndex++) {
        KSMagneticField* tMagneticFieldObject = getMagneticField(tParameters[tIndex]);
        tMagneticFieldObject->Deinitialize();
    }

    return 0;
   
};