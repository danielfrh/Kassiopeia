#ifndef Transform_Input_h_
#define Transform_Input_h_

#include "KThreeVector.hh"
#include <iomanip>
#include <limits>

using namespace std;
using namespace katrin;

class Coil{
    public:
        Coil( const string& inCoilName,
        const double& inCoilCurrent,
        const double& inCoilTurns,
        const KThreeVector& inCoilCenterPoint,
        const KThreeVector& inCoilEulerAngles,
        const double& inCoilLength,
        const double& inCoilRmin,
        const double& inCoilRmax ) :
        longitudinal_mesh_count( 10 ),
        longitudinal_mesh_power( 1 ),
        radial_mesh_count( 30 ),
        radial_mesh_power( 1 ),
        axial_mesh_count( 128 )
        {
            coilName = inCoilName;
            coilCurrent = inCoilCurrent;
            coilTurns = inCoilTurns;
            coilCenterPoint = inCoilCenterPoint;
            coilEulerAngles = inCoilEulerAngles;
            coilLength = inCoilLength;

            SetLocalZCoordinate();

            coilRmin = inCoilRmin;
            coilRmax = inCoilRmax;
        };
        void PrintParameters( void )
        {
            std::cout << "coilName = " << coilName << std::endl;
            std::cout << "coilCurrent = " << coilCurrent << std::endl;
            std::cout << "coilTurns = " << coilTurns << std::endl;
            std::cout << "coilCenterPoint = " << coilCenterPoint << std::endl;
            std::cout << "coilEulerAngles = " << coilEulerAngles << std::endl;
            std::cout << "coilLength = " << coilLength << std::endl;
            std::cout << "coilLocalZ1 = " << coilLocalZ1 << std::endl;
            std::cout << "coilLocalZ2 = " << coilLocalZ2 << std::endl;
            std::cout << "coilRmin = " << coilRmin << std::endl;
            std::cout << "coilRmax = " << coilRmax << std::endl;
            std::cout << "---" << std::endl;
            std::cout << "longitudinal_mesh_count = " << longitudinal_mesh_count << std::endl;
            std::cout << "longitudinal_mesh_power = " << longitudinal_mesh_power<< std::endl;
            std::cout << "radial_mesh_count = " << radial_mesh_count << std::endl;
            std::cout << "radial_mesh_power = " << radial_mesh_power << std::endl;
            std::cout << "axial_mesh_count = " << axial_mesh_count << std::endl;
        };

        void SetMeshCouts( const unsigned int& inLongitudinal_mesh_count,
            const double& inLongitudinal_mesh_power,
            const unsigned int& inRadial_mesh_count,
            const double& inRadial_mesh_power,
            const unsigned int& inAxial_mesh_count ) {
                longitudinal_mesh_count = inLongitudinal_mesh_count;
                longitudinal_mesh_power = inLongitudinal_mesh_power;
                radial_mesh_count = inRadial_mesh_count;
                radial_mesh_power = inRadial_mesh_power;
                axial_mesh_count = inAxial_mesh_count;
                return;
        };

        void SetLocalZCoordinate( void ) {
            coilLocalZ1 = -coilLength/2.;
            coilLocalZ2 = coilLength/2.;
            return;
        };


        string coilName;

        double coilCurrent;
        double coilTurns;
        KThreeVector coilCenterPoint;
        KThreeVector coilEulerAngles;
        double coilLength;
        double coilLocalZ1;
        double coilLocalZ2;
        double coilRmin;
        double coilRmax;

        unsigned int longitudinal_mesh_count;
        double longitudinal_mesh_power;
        unsigned int radial_mesh_count;
        double radial_mesh_power;
        unsigned int axial_mesh_count;
    };


class XMLFileWriter{
    public:
    void WriteGeoOpening( std::ostream& out )
    {
        out << "<geometry>" << std::endl;
        out << std::endl;
    }

    void WriteTubeSpaceElement( std::ostream& out, const Coil &myCoil )
    {
        out << std::setprecision(16);
        out << "    <tag name=\"magnet_tag\">" << std::endl;
        out << "        <tag name=\"" << myCoil.coilName << "_tag\">" << std::endl;
        out << "            <cylinder_tube_space" << std::endl;
        out << "                name=\"" << myCoil.coilName << "_cylinder_tube_space\"" << std::endl;
        out << "                z1=\"" << myCoil.coilLocalZ1 << "\""  << std::endl;
        out << "                r1=\"" << myCoil.coilRmin << "\""  << std::endl;
        out << "                z2=\"" << myCoil.coilLocalZ2 << "\"" << std::endl;
        out << "                r2=\"" << myCoil.coilRmax << "\""  << std::endl;
        out << "                longitudinal_mesh_count=\"" << myCoil.longitudinal_mesh_count << "\"" << std::endl;
        out << "                longitudinal_mesh_power=\"" << myCoil.longitudinal_mesh_power << "\"" << std::endl;
        out << "                radial_mesh_count=\"" << myCoil.radial_mesh_count << "\"" << std::endl;
        out << "                radial_mesh_power=\"" << myCoil.radial_mesh_power << "\"" << std::endl;
        out << "                axial_mesh_count=\""   << myCoil.axial_mesh_count <<  "\""  << std::endl;
        out << "            />" << std::endl;
        out << "        </tag>" << std::endl;
        out << "    </tag>" << std::endl;
        out << std::endl;
    }

    void WriteSpaceOpening( std::ostream& out )
    {
        out << "<space name=\"coils_assembly\">" << std::endl;
        out << std::endl;
    }

    void WriteSpaceElement( std::ostream& out, const Coil &myCoil )
    {
        out << std::setprecision(16);
        out << "    <space name=\"" << myCoil.coilName << "_space\" tree=\"" << myCoil.coilName << "_cylinder_tube_space\">" << std::endl;
        out << "        <transformation displacement=\"{" << myCoil.coilCenterPoint[0] << "} {" << myCoil.coilCenterPoint[1] << "} {" << myCoil.coilCenterPoint[2] << "}\" rotation_euler=\"{" << myCoil.coilEulerAngles[0] << "} {" << myCoil.coilEulerAngles[1] << "} {" << myCoil.coilEulerAngles[2] << "}\"/>" << std::endl;
        out << "    </space>" << std::endl;
        out << std::endl;
    }

    void WriteSpaceClosing( std::ostream& out )
    {
        out << "</space>" << std::endl;
        out << std::endl;
    }

    void WriteIncludeForGeoFile( std::ostream& out, string theName )
    {
        out << "<include name=\"" << theName << "_Geometry.xml\"/>" << std::endl;
        out << std::endl << std::endl;
    }

/*

Info von Ferenc:

Alle LFCS Spulen sollten (mit positiven Stromwerten) dieselbe Magnetfeldrichtung haben,
als die SC-Magneten. Die "Uhr" ist so definiert, wie man nach Nord,
also +z Richtung schaut (von PS2 zu PCH Richtung),
und deshalb counter-clockwise mit positiven Strom bedeutet Magnetfeld nach Sued
(also in -z Richtung: Bz<0).

*/

    // MagfieldCoils: 
    //       cu  Cx Cy Cz  alpha  beta  tu  L Rmin Rmax
    //
    //    alpha, beta:  coil direction Euler angles in degrees.
    //    Coil direction unit vector (ux, uy, uz) is defined by
    //           ux=sin(beta)*sin(alpha),   uy=-sin(beta)*cos(alpha),   uz=cos(beta),
    //  (e.g. beta=0: u in+z direction;  beta=180:  u in -z direction)
    //
    //  Positive current: current flow has right-handed screw with the (ux,uy,uz) coil direction vector
    //   (e.g. beta=0, cu positive: magnetic field in +z direction).

    void WriteElectromagnet( std::ostream& out, const Coil &myCoil )
    {
        out << std::setprecision(16);
        out << "<electromagnet";
        out << "        spaces=\"coils_assembly/" << myCoil.coilName << "_space\"" << std::endl;
        out << "        current=\"" << abs(myCoil.coilCurrent) << "\"" << std::endl;
        out << "        allow_non_integer_turns=\"true\"" << std::endl;
        out << "        scaling_factor=\"" << myCoil.coilTurns << "\"" << std::endl;

        if( myCoil.coilCurrent <  0 )  out << "        direction=\"counter_clockwise\"";
        if( myCoil.coilCurrent >= 0 )  out << "        direction=\"clockwise\"";

        out << "/>" << std::endl;
        out << std::endl;
    }

    void WriteGeoClosing( std::ostream& out )
    {
        out << std::endl;
        out << "</geometry>" << std::endl;
    }
};
#endif