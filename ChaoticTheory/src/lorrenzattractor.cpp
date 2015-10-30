#include "lorrenzattractor.h"


std::vector<Lorrenzattractor::state_type> Lorrenzattractor::valuePoints;


Lorrenzattractor::Lorrenzattractor()
{
//    state_type x = { {10.0 , 1.0 , 1.0} }; // initial conditions
//   // integrate( lorenz, x , 0.0 , 25.0 , 0.1 , write_lorenz );

//    void (Lorrenzattractor::*takes_a_function) (void*) = &Lorrenzattractor::prints_an_int;
//    Lorrenzattractor obj;
//      (obj.*takes_a_function)((void*)1);//call: Foo::f() through an object

}
Lorrenzattractor::~Lorrenzattractor(){}

void Lorrenzattractor::lorenz( const state_type &x , state_type &dxdt /*, double t*/ )
{
    dxdt[0] = sigma * ( x[1] - x[0] );
    dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
    dxdt[2] = -b * x[2] + x[0] * x[1];

    valuePoints.push_back(x);
}

void Lorrenzattractor::write_lorenz( const state_type &x/* , const double t*/ )
{
    cout << /*t <<*/ '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << endl;


}


//void Lorrenzattractor::prints_an_int(void *data) {
//  // The idiom for converting a void pointer to another kind
//  // of pointer.  NO NEED TO CAST.  Note this behavior is only
//  // defined if the pointer data really does point to an int.
//  int *i = static_cast<int*> (data);
//  printf("%d", *i);
//}

//void Lorrenzattractor::takes_a_function(void (*f)(void *), void *data) {
//  f(data);
//}


