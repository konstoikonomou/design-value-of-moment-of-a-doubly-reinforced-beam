
/*

Program that calculates the design value of moment of a doubly reinforced rectangular beam, according to Eurocode 2. 

It takes as input the geometry of the rectangular beam and the quantity and size of steel bars 
(in tension and compression).

At first, it checks if the failure is occuring with the desired form (first the steel bars in tension reach the
yield point and then concrete fails in compression) and calculates the design value of moment, 
as well as the final depth of neutral axis in mm.

Input: 
beam geometry characteristics in mm
quantity of steel bars (in tension and in compression)
diameter of steel bars (in tension and in compression)

Output:
design value of moment in kNm
final depth of neutral axis in mm

*/

#include <iostream> 
using namespace std;

const double pi = 3.141592653589793238463;

// Geometry 
const int b = 300; // in mm
const int h = 550; // in mm
const int d_1 = 50; // in mm
const int d_2 = 50; // in mm
const int d = 500; // in mm

// Material attributes
const int f_ck = 25; // characteristic strength of concrete in MPa
const double g_c = 1.5; // safety factor for concrete
const double e_cu = 0.0035; // distortion
const int f_yk = 500; // characteristic strength of reinforcing steel in MPa
const double g_s = 1.15; // safety factor for steel
const int E_s = 200; // in GPa
const double a_cc = 0.85; // factor for bending
const double f_cd = a_cc * f_ck / g_c ; // design strength of concrete in MPa
const double f_yd = f_yk / g_s; // desing strength of steel in MPa
const double e_yd = f_yd / (1000 * E_s); // steel design distortion
const double j_lim = (f_ck <= 50) ? 0.45 : 0.35; // limit from EC2 for decent plasticity

int quantity; // quantity of steel bars
int bar_diameter;
double surface;
double tension_surface_till_now = 0; // surface of steel bars (tension)
double compression_surface_till_now = 0; // surface of steel bars (compression)

int main(){

    cout << f_cd << " " << f_yd << " " << e_yd << " " << endl;

    while (1){
        cout << "Give quantity of tension steel bars (or \"0\" to proceed): ";
        cin >> quantity;

        if(quantity == 0){
            if(tension_surface_till_now == 0){
                cout << "Surface of tension steel is required!" << endl;
                exit(1);
            }
            else{
                break;
            }
        }

        cout << "Give F (diameter) of tension steel bars in mm: ";
        cin >> bar_diameter;

        tension_surface_till_now = tension_surface_till_now + quantity * (pi * bar_diameter * bar_diameter) / 4; 
    }

    cout << "Surface of tension steel As1: " << tension_surface_till_now << " mm^2" <<endl;

    while (1){
        cout << "Give quantity of compression steel bars (or \"0\" to proceed): ";
        cin >> quantity;

        if(quantity == 0){
                break;
        }

        cout << "Give F (diameter) of compression steel bars in mm: ";
        cin >> bar_diameter;

        compression_surface_till_now = compression_surface_till_now + quantity * (pi * bar_diameter * bar_diameter) / 4; 
    }

    cout << "Surface of compression steel As2: " << compression_surface_till_now << " mm^2"<< endl;

    double x_distance = 0; // initial
    double total_forces = 1000;
    double tolerance = 0.1;
    double j;
    double F_c, F_s1, F_s2;
    double e_s1, e_s2;
    double tension_s1, tension_s2;
    double righting_lever_zc, corrected_zc;
    double righting_lever_zs, corrected_zs;
    double moment_F_c, moment_F_s2;


    while (total_forces > tolerance){

        x_distance = x_distance + 0.01;
        j = x_distance / d;
        if (j <= j_lim){
            //cout << "J <= Jlim OK" << endl;

            e_s1 = e_cu * (d - x_distance) / x_distance;
            e_s2 = e_cu * (d_2 - x_distance) / x_distance;

            F_c = -1 * 0.8 * x_distance * b * f_cd / 1000; // result in KN
            //cout << F_c <<endl;
            if(e_s1 < 0){
                if(((-1) * e_s1) < e_yd){
                    tension_s1 = e_s1 * E_s * 1000;
                }
                else{
                    tension_s1 = (-1) * f_yd;
                }
            }
            else if (e_s1 > 0){
                if(e_s1 < e_yd){
                    tension_s1 = e_s1 * E_s * 1000;
                }
                else {
                    tension_s1 = f_yd;
                }

            }

            F_s1 = tension_surface_till_now * tension_s1 / 1000;

            if(e_s2 < 0){
                if(((-1) * e_s2) < e_yd){
                    tension_s2 = e_s2 * E_s * 1000;
                }
                else{
                    tension_s2 = (-1) * f_yd;
                }
            }
            else if (e_s2 > 0){
                if(e_s2 < e_yd){
                    tension_s2 = e_s2 * E_s * 1000;
                }
                else {
                    tension_s2 = f_yd;
                }

            }

            F_s2 = compression_surface_till_now * tension_s2 / 1000;       

            total_forces = F_s1 + F_c + F_s2;
            //cout << "Total forces: "<< total_forces << endl;

        }
        else{
            cout << "j > j_lim, so not desired failure type" << endl;
        }
    }

    cout << "_________________________________" << endl;

    if(e_s1 > 0){
        if(e_s1 < e_yd){
            cout << "Not yield of tension steel bars!" << endl;
        }
        else {
            cout << "Yield of tension steel bars!" << endl;
        }
    }
    else if(e_s1 < 0){
        if((-1) * e_s1 < e_yd){
            cout << "Not yield of tension steel bars!" << endl;
        }
        else {
            cout << "Yield of tension steel bars!" << endl;
        }
    }

    if(e_s2 > 0){
        if(e_s2 < e_yd){
            cout << "Not yield of compression steel bars!" << endl;
        }
        else {
            cout << "Yield of compression steel bars!" << endl;
        }
    }
    else if(e_s2 < 0){
        if((-1) * e_s2 < e_yd){
            cout << "Not yield of compression steel bars!" << endl;
        }
        else {
            cout << "Yield of compression steel bars!" << endl;
        }
    } 

    righting_lever_zc = d - 0.4 * x_distance;
    righting_lever_zs = d - d_2;

    corrected_zc = righting_lever_zc / d;
    corrected_zs = righting_lever_zs / d;

    if(F_c < 0){
        moment_F_c = (-1) * F_c * righting_lever_zc / 1000; // in KNm
    }
    else{
        moment_F_c = F_c * righting_lever_zc / 1000; // in KNm
    }

    if(F_s2 < 0){
        moment_F_s2 = (-1) * F_s2 * righting_lever_zs / 1000; // in KNm
    }
    else{
        moment_F_s2 = F_s2 * righting_lever_zs / 1000; // in KNm
    }

    double design_moment_Mrd = moment_F_c + moment_F_s2;
    cout << "Design moment Mrd: " << design_moment_Mrd << " kNm"<< endl;


    cout << "Final depth of neutral axis x: " << x_distance << " mm";

    return (0);
}