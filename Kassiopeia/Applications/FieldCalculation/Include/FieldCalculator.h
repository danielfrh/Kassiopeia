#ifndef Kassiopeia_FieldCalculator_h_
#define Kassiopeia_FieldCalculator_h_

#include "KThreeVector.hh"

/* Remove if already defined */
typedef long long int64;
using uint64 = unsigned long long;

// timing function
#include <ctime>
#include <sys/time.h>

using namespace Kassiopeia;
using namespace katrin;
using namespace std;

struct tResult{
    KThreeVector myPosition;
    KThreeVector myField;
};



///////////////////////////
// FIELD POINT GENERATOR //
///////////////////////////



class FieldPointGenerator{
    public:
        FieldPointGenerator( std::string label, unsigned int inDim, const uint64 no, const KThreeVector start, const KThreeVector end)
        {
            completeName=label;

            SetNPoints(no);
            SetStartPoint(start);
            SetEndPoint(end);

            if(noPoints<2) {
                mainmsg( eWarning ) << "Please note that at least 2 points have to be computed, set noPoints=2." << eom;
                noPoints = 2;
            };
            theResultVector.clear();

            int retVal = 0;

            if( inDim==1 ) {
                SetDim(inDim);
                retVal = GenerateFieldPoints1dim();
            } else {
                SetDim(inDim);
                retVal = GenerateFieldPoints2dim();
            };
            if(retVal>0) mainmsg(eError) << "Field point computation failed." << eom; 
        };

        int GenerateFieldPoints1dim( void )
        {
            // calc direction vector
            const KThreeVector directionVector = (endPoint - startPoint) / (endPoint - startPoint).Magnitude();

            // length of vector
            const double theLength = (endPoint - startPoint).Magnitude()/noPoints;

            KThreeVector calcPoint(0., 0., 0.);

            for( unsigned int i=0; i<=noPoints; i++ )
            {
                calcPoint = startPoint + ( i*theLength*directionVector );
                SetPositionToResultVector( calcPoint );
            };
            return 0;
        };

        int GenerateFieldPoints2dim( void )
        {
            // TODO
            mainmsg( eError) << "2-dimensional field maps not implemented yet." << eom;
            return 1;
        };


        // the input file defines the number of points, no scale, 1-dim case
        FieldPointGenerator( std::string label, std::string fInputFileName )
        {
            ifstream input;
            input.open(fInputFileName.c_str());

            if (!input.is_open()) {
                puts("Cannot open the  input source file!");
                puts("Program running is stopped !!! ");
                exit(1);
            }

            input >> noPoints;

            if(noPoints<2) {
                mainmsg( eWarning ) << "Please note that at least 2 points have to be computed, set noPoints=2." << eom;
                noPoints = 2;
            }
            
            SetName( label );
            SetDim( 1 );

            theResultVector.clear();

            double calcPoint[3];

            // get the start point

            input >> calcPoint[0] >> calcPoint[1] >> calcPoint[2];
            SetStartPoint( calcPoint );

            SetPositionToResultVector( calcPoint );

            for ( unsigned int i = 1; i < noPoints; i++ ) {
                input >> calcPoint[0] >> calcPoint[1] >> calcPoint[2];
                SetPositionToResultVector( calcPoint );
            }

            endPoint=calcPoint;

            input.close();
        };

        void SetName( std::string input ){completeName=input;return;};
        std::string GetName(){return completeName;};

        void SetDim( unsigned int input ){calcDimensions=input;return;};
        int GetDim(){return calcDimensions;};

        void SetNPoints( uint64 input ){noPoints=input;return;};
        uint64 GetNPoints(){return noPoints;};

        void SetStartPoint( KThreeVector input ){startPoint=input;return;};
        const KThreeVector GetStartPoint(){return startPoint;};

        void SetEndPoint( KThreeVector input ){endPoint=input;return;};
        const KThreeVector GetEndPoint(){return endPoint;};

        void SetPositionToResultVector( KThreeVector input ) {
            tResult temp;
            temp.myPosition.SetComponents( input );
            temp.myField.SetComponents( 0., 0., 0. );
            theResultVector.push_back( temp );
        };
        
        std::vector<tResult> theResultVector;

    private:
        string completeName;
        unsigned int calcDimensions;
        uint64 noPoints;
        KThreeVector startPoint;
        KThreeVector endPoint;
    };



//////////////////////
// TIME MEASUREMENT //
//////////////////////

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



////////////////////////////
// FIELD POINT SET READER //
////////////////////////////



class FieldPointSetReader{
    public:
    FieldPointSetReader( std::string theInputFile, std::vector<FieldPointGenerator>& thePointSet )
    {
        ifstream input;
        input.open(theInputFile.c_str());

        if (!input.is_open()) {
            puts("Cannot open the  input source file!");
            puts("Program running is stopped !!! ");
            exit(1);
        }

        unsigned int fNLines;
        input >> fNLines;

        if (fNLines < 1) {
            puts("fNLines is not positive !");
            puts("Program running is stopped !!! ");
            exit(1);
        }

        std::string label = ("");
        unsigned int tDimension = 0;
        uint64 tScale = 0;
        KThreeVector start, end;
    
        // no exponents in input file
        for ( unsigned int i = 0; i < fNLines; i++ ) {
            input >> label >> tDimension >> tScale >> start[0] >> start[1] >> start[2] >> end[0] >> end[1] >> end[2];
            mainmsg( eDebug ) << label << tDimension << tScale << start[0] << start[1] << start[2] << end[0] << end[1] << end[2] << eom;
            FieldPointGenerator myGen(label, tDimension, tScale, start, end);
    
            thePointSet.push_back( myGen );
        }
    
        input.close();
    }

};



#endif