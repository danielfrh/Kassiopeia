#ifndef Kassiopeia_FieldCalculator_h_
#define Kassiopeia_FieldCalculator_h_

// Update: 30.09.2026

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
        FieldPointGenerator( const std::string& inLabel, const short& inDim, const uint64& inNo, const KThreeVector& inStart, const KThreeVector& inEnd )
        {
            SetName( inLabel );
            SetNPoints( inNo );
            SetStartPoint( inStart );
            SetEndPoint( inEnd );

            if( noPoints<2 ) {
                mainmsg( eWarning ) << "Please note that at least 2 points have to be computed, set noPoints=2." << eom;
                noPoints = 2;
            };
            theResultVector.clear();

            int retVal = 0;

            if( inDim==1 ) {
                SetDim( inDim );
                retVal = GenerateFieldPoints1dim();
            } else {
                SetDim( inDim );
                retVal = GenerateFieldPoints2dim();
            };
            if( retVal>0 ) mainmsg(eError) << "Field point computation failed." << eom; 
        };

        FieldPointGenerator( const std::string& inLabel, const short& inDim, const uint64& inNo, const KThreeVector& inStart, const KThreeVector& inEnd, const KThreeVector& inNormal )
        {
            SetName( inLabel );
            SetPlaneNormal (inNormal );
            SetNPoints( inNo );
            SetStartPoint( inStart );
            SetEndPoint( inEnd );

            if( noPoints<2 ) {
                mainmsg( eWarning ) << "Please note that at least 2 points have to be computed, set noPoints=2." << eom;
                noPoints = 2;
            };
            theResultVector.clear();

            int retVal = 0;

            if( inDim==2 ) {
                SetDim(inDim);
                retVal = GenerateFieldPoints2dim();
            } else {
                mainmsg(eError) << "Please note that a normal vector for a one-dimensional line has been defined, but this function generates points in a 2-dim. surface. Please reduce dimension." << eom;
            };
            if( retVal>0 ) {
                mainmsg(eError) << "Field point computation failed."<< eom;
            } 
        };


        int GenerateFieldPoints1dim( void )
        {
            // calc direction vector
            const KThreeVector directionVector = (endPoint - startPoint) / (endPoint - startPoint).Magnitude();

            // length of vector
            const double theLength = (endPoint - startPoint).Magnitude()/noPoints;

            for( unsigned int i=0; i<=noPoints; i++ )
            {
                const KThreeVector calcPoint = startPoint + ( i*theLength*directionVector );
                SetPositionToResultVector( calcPoint );
            };
            return 0;
        };

        int GenerateFieldPoints2dim( void )
        {    
            // Source: Gemini, Flash-Lite
            // 1. Normalenvektor normieren
            const double lengthNormal = GetPlaneNormal().Magnitude();
            if( lengthNormal > 1 ){ SetPlaneNormal(GetPlaneNormal()/lengthNormal); };

            // 2. Diagonalenvektor und Länge bestimmen
            const KThreeVector diagVec = GetEndPoint() - GetStartPoint();
            const double diagLength = diagVec.Magnitude();
    
            if (diagLength == 0 || GetNPoints() <= 0) mainmsg(eError) << "The length of the diag(endPoint-startPoint) is zero or the scale is smaller or equal to 0." << eom;

            // 3. Lokales Koordinatensystem in der Ebene aufspannen
            KThreeVector helper = (std::abs(GetPlaneNormal().GetX()) < 0.9) ? KThreeVector(1, 0, 0) : KThreeVector(0, 1, 0);
            KThreeVector u_axis_NonNorm=GetPlaneNormal().Cross( helper );
            KThreeVector u_axis = u_axis_NonNorm/u_axis_NonNorm.Magnitude();
            KThreeVector v_axis_NonNorm = GetPlaneNormal().Cross( u_axis );
            KThreeVector v_axis = v_axis_NonNorm/v_axis_NonNorm.Magnitude();

            // 4. Diagonale auf das lokale System projizieren (Kantenlaáengen ermitteln)
            const double Lu = diagVec.Dot(u_axis);
            const double Lv = diagVec.Dot(v_axis);
    
            const double width = std::abs(Lu);
            const double height = std::abs(Lv);
    
            // 5. Lineare Skalierung der Schrittweiten
            double step_u = GetNPoints() * ( width / diagLength);
            double step_v = GetNPoints() * ( height / diagLength);
    
            if (step_u <= 0) step_u = GetNPoints();
            if (step_v <= 0) step_v = GetNPoints();

             // 6. Äquidistante Punkte generieren
            for (double u = 0; u <= width; u += step_u) {
                for (double v = 0; v <= height; v += step_v) {
                    double sign_u = (Lu >= 0) ? 1.0 : -1.0;
                    double sign_v = (Lv >= 0) ? 1.0 : -1.0;
                    
                    const KThreeVector calcPoint = GetStartPoint() + u_axis * (u * sign_u) + v_axis * (v * sign_v);
                    SetPositionToResultVector( calcPoint );
                }
            }

            return 0;
        }

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
                mainmsg( eWarning ) << "Please note that at least two points have to be computed, set noPoints=2." << eom;
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

        // the input file defines the number of points, no scale, 1-dim case, empty container for field points, mode=3
        FieldPointGenerator( const std::string& label )
        {
            SetName( label );
            SetDim( 1 );

            theResultVector.clear();
        };

        void SetName( const std::string& input ){completeName=input;return;};
        std::string GetName() const {return completeName;};

        void SetDim( const short& input ){calcDimensions=input;return;};
        short GetDim() const {return calcDimensions;};

        void SetNPoints( const uint64& input ){noPoints=input;return;};
        uint64 GetNPoints() const {return noPoints;};

        void SetStartPoint( const KThreeVector& input ){startPoint=input;return;};
        KThreeVector GetStartPoint() const {return startPoint;};

        void SetEndPoint( const KThreeVector& input ){endPoint=input;return;};
        KThreeVector GetEndPoint() const {return endPoint;};

        void SetPlaneNormal( const KThreeVector& input ) {planeNormal = input; return;};
        KThreeVector GetPlaneNormal() const {return planeNormal;};

        void SetPositionToResultVector( const KThreeVector& input ) {
            tResult temp;
            temp.myPosition.SetComponents( input );
            temp.myField.SetComponents( 0., 0., 0. );
            theResultVector.push_back( temp );
        };

        void SetPositionAndFieldToResultVector( const KThreeVector& inputPos, const KThreeVector& inputField ) {
            tResult temp;
            temp.myPosition.SetComponents( inputPos );
            temp.myField.SetComponents( inputField );
            theResultVector.push_back( temp );
            //theResultVector.emplace_back(tResult{inputPos, inputField});
        };
        
        std::vector<tResult> theResultVector;

    private:
        string completeName;
        short calcDimensions;
        uint64 noPoints;
        KThreeVector startPoint;
        KThreeVector endPoint;
        KThreeVector planeNormal;
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
        short tDimension = 0;
        double tScale = 0;
        KThreeVector start, end, normalV;
    
        // no exponents in input file?
        for ( unsigned int i = 0; i < fNLines; i++ ) {
            input >> label >> tDimension >> tScale >> start[0] >> start[1] >> start[2] >> end[0] >> end[1] >> end[2];
            mainmsg( eDebug ) << label << tDimension << tScale << start[0] << start[1] << start[2] << end[0] << end[1] << end[2];
            if( tDimension==1 )
            {
                FieldPointGenerator myGen(label, tDimension, tScale, start, end);
                thePointSet.push_back( myGen );
            }
            else if( tDimension==2 )
            {
                input >> normalV[0] >> normalV[1] >> normalV[2];
                mainmsg( eDebug ) << normalV[0] << normalV[1] << normalV[2] << eom;

                FieldPointGenerator myGen(label, tDimension, tScale, start, end, normalV);
                thePointSet.push_back( myGen );
            }
        }
    
        input.close();
    }

};



#endif