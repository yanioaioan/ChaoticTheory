#ifndef LORRENZATTRACTOR
#define LORRENZATTRACTOR

#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
#include "lorrenzattractor.h"
#include <vector>

using namespace std;
using namespace boost::numeric::odeint;


const double R = 28.0;
const double sigma = 10.0;
const double b = 8.0 / 3.0;

class Lorrenzattractor
{
public:
    Lorrenzattractor();
    ~Lorrenzattractor();

//    void takes_a_function(void (*f)(void *), void *data);
//    void prints_an_int(void *data) ;

    typedef boost::array< double , 3 > state_type;

    void lorenz(const state_type &x , state_type &dxdt );
    void write_lorenz(const state_type &x );


    static std::vector<state_type> valuePoints;

//    Lorrenzattractor *ptr;
};


//int main(int argc, char **argv)
//{
//    state_type x = { 10.0 , 1.0 , 1.0 }; // initial conditions
//    integrate( lorenz , x , 0.0 , 25.0 , 0.1 , write_lorenz );
//}

#endif // LORRENZATTRACTOR

