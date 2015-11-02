#include "nglscene.h"
#include <iostream>
#include <ngl/NGLInit.h>
#include "lorrenzattractor.h"
#include "OneDSolver.h"

#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

const static float INCREMENT=0.01;

const static float ZOOM=1;
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))


NGLScene::NGLScene(/*QWidget *_parent*/)//: QGLWidget(_parent)
{
    setFocus ();
    m_rColor=1;
    m_gColor=1;
    m_bColor=1;

    m_spinXFace=0;
    m_spinYFace=0;
    startTimer(1);




//    this->resize(_parent->size ());
}

NGLScene::~NGLScene()
{
}


void NGLScene::lorenz( const state_type &x , state_type &dxdt , double t )
{
    dxdt[0] = sigma * ( x[1] - x[0] );
    dxdt[1] = R * x[0] - x[1] - x[0] * x[2];
    dxdt[2] = -b * x[2] + x[0] * x[1];
}

void NGLScene::write_lorenz( const state_type &x , const double t )
{
    cout << /*t <<*/ '\t' << x[0] << '\t' << x[1] << '\t' << x[2] << endl;
//    m_valuePoints.push_back(x);

}

struct wrapper
{
    Lorrenzattractor &c;
    wrapper( Lorrenzattractor &c_ ) : c( c_ )  { }
    template< typename State , typename Time >
    void operator()( State const &x , State &dxdt , Time t ) const
    {
        c.lorenz( x , dxdt/* , t*/ );
    }
};



#define EULER    0
#define MODIFIED_EULER    1
#define HEUNS    2
#define MIDPOINT 3
#define RUNGEKUTTA_4 4
#define RUNGEKUTTA_6 5

//      Dx/Dt=5x−3
double EvalFcn(double x)
{

//    return 5*x-3;
    return(-0.05 * x);
}

void NGLScene::initializeGL ()
{

    // we must call this first before any other GL commands to load and link the
    // gl commands from the lib, if this is not done program will crash
    ngl::NGLInit::instance();


    Lorrenzattractor lAttractor;


    state_type x = { 10.0 , 1.0 , 1.0 }; // initial conditions
    integrate( wrapper(lAttractor), x, 0.0 , 25.0 , 0.1 /*, write_lorenz */);


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief Added of a external case for 1D Solver integration tested visually
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///
    ///
    //      Dx/Dt=5x−3
    //


    double t;
    double dt=1;    /* Step size            */
    double T=100;     /* Simulation duration  */
    double y = 1;     /* Initial value        */
    double y2 = 1;

    OneDSolver s;
    for (t=0.1;t<T;t+=dt)
    {
       printf("%g %g\n",t,y);
       y = s.Solver1D(dt,y,EULER,(double (*)(double))EvalFcn);
       y2 = s.Solver1D(dt,y2,RUNGEKUTTA_6,(double (*)(double))EvalFcn);

       m_oneDSolverPointsEuler.push_back(y*50);
       m_oneDSolverPointsRungeKutta6.push_back(y2*50);

    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////


    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);			   // Grey Background
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    // enable multisampling for smoother drawing
    glEnable(GL_MULTISAMPLE);
    // now to load the shader and set the values
    // grab an instance of shader manager
//    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//    // we are creating a shader called Phong
//    shader->createShaderProgram("Phong");
//    // now we are going to create empty shaders for Frag and Vert
//    shader->attachShader("PhongVertex",ngl::VERTEX);
//    shader->attachShader("PhongFragment",ngl::FRAGMENT);
//    // attach the source
//    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
//    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
//    // compile the shaders
//    shader->compileShader("PhongVertex");
//    shader->compileShader("PhongFragment");
//    // add them to the program
//    shader->attachShaderToProgram("Phong","PhongVertex");
//    shader->attachShaderToProgram("Phong","PhongFragment");

//    // now we have associated this data we can link the shader
//    shader->linkProgramObject("Phong");
//    // and make it active ready to load values
//    (*shader)["Phong"]->use();
//    // the shader will use the currently active material and light0 so set them
//    ngl::Material m(ngl::GOLD);
//    // load our material values to the shader into the structure material (see Vertex shader)
//    m.loadToShader("material");



    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglDiffuseShader"]->use();

    shader->setShaderParam4f("Colour",1,1,1,1);
    shader->setShaderParam3f("lightPos",1,1,1);
    shader->setShaderParam4f("lightDiffuse",1,1,1,1);




    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(1,1,150);
    ngl::Vec3 to(0,0,0);
    ngl::Vec3 up(0,1,0);
    // now load to our new camera
    m_cam= new ngl::Camera(from,to,up);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350);

    // now create the primitives to draw
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    prim->createSphere("sphere",0.4,10);

    // as re-size is not explicitly called we need to do this.
    glViewport(0,0,width(),height());
}

void NGLScene::resizeGL(int w, int h)
{
  // set the viewport for openGL we need to take into account retina display
  // etc by using the pixel ratio as a multiplyer
  glViewport(0,0,w*devicePixelRatio(),h*devicePixelRatio());
  // now set the camera size values as the screen size has changed
  m_cam->setShape(45.0f,(float)width()/height(),0.05f,350.0f);
  update();


}

void NGLScene::loamatricestoShader(const ngl::Colour & color)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();


    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    M=m_transform.getMatrix()*m_mouseGlobalTX;
    MV=  M*m_cam->getViewMatrix();
    MVP= M*m_cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
//    shader->setShaderParamFromMat4("MV",MV);
//    shader->setShaderParamFromMat4("MVP",MVP);
//    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//    shader->setShaderParamFromMat4("M",M);

    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setRegisteredUniformFromColour("Colour",color);
}

void NGLScene::paintGL ()
{
//    std::cout<<m_rColor<<", "<<m_gColor<<", "<<m_bColor<<std::endl;
//    glClearColor (m_rColor,m_gColor,m_bColor,1);

//    // grab an instance of the shader manager
//    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
//    (*shader)["Phong"]->use();


//    // Rotation based on the mouse position for our global transform
//    ngl::Mat4 rotX;
//    ngl::Mat4 rotY;
//    // create the rotation matrices
//    rotX.rotateX(m_spinXFace);
//    rotY.rotateY(m_spinYFace);
//    // multiply the rotations
//    m_mouseGlobalTX=rotY*rotX;
//    // add the translations
//    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
//    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
//    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;




//    m_transform.reset();
//    {
//        m_transform.setPosition(0,0,0);
//        loamatricestoShader();
//    }

//    ngl::VAOPrimitives::instance()->draw("cube");








    //----------------------------------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------------------------------
    // grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglDiffuseShader"]->use();

    // Rotation based on the mouse position for our global transform
    ngl::Mat4 rotX;
    ngl::Mat4 rotY;
    // create the rotation matrices
    rotX.rotateX(m_spinXFace);
    rotY.rotateY(m_spinYFace);
    // multiply the rotations
    m_mouseGlobalTX=rotY*rotX;
    // add the translations
    m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
    m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
    m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;


     // get the VBO instance and draw the built in teapot
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    for(std::vector<state_type>::iterator it=Lorrenzattractor::valuePoints.begin();it!=Lorrenzattractor::valuePoints.end();it++)
    {
        m_transform.reset();
        m_transform.setPosition( (*it)[0],(*it)[1],(*it)[2] );
        loamatricestoShader(ngl::Colour(1,0,0,1));
        prim->draw("sphere");
    }


    for(std::vector<float>::iterator it=m_oneDSolverPointsEuler.begin();it!=m_oneDSolverPointsEuler.end();it++)
    {
        m_transform.reset();
        m_transform.setPosition( *it,-2 ,0 );
        loamatricestoShader(ngl::Colour(0,1,0,1));
        prim->draw("sphere");
    }

    for(std::vector<float>::iterator it=m_oneDSolverPointsRungeKutta6.begin();it!=m_oneDSolverPointsRungeKutta6.end();it++)
    {

        m_transform.reset();
        m_transform.setPosition( *it,-3 ,0 );
        loamatricestoShader(ngl::Colour(1,1,0,1));
        prim->draw("sphere");
    }



}



void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{
    // note the method buttons() is the button state when event was called
    // this is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated
    if(_event->buttons() == Qt::LeftButton)
    {
      int diffx=_event->x()-m_origX;
      int diffy=_event->y()-m_origY;
      m_spinXFace += (float) 0.5f * diffy;
      m_spinYFace += (float) 0.5f * diffx;
      m_origX = _event->x();
      m_origY = _event->y();
      update();

    }
          // right mouse translate code
    else if( _event->buttons() == Qt::RightButton)
    {
      int diffX = (int)(_event->x() - m_origXPos);
      int diffY = (int)(_event->y() - m_origYPos);
      m_origXPos=_event->x();
      m_origYPos=_event->y();
      m_modelPos.m_x += INCREMENT * diffX;
      m_modelPos.m_y -= INCREMENT * diffY;
      update();

     }
}
void NGLScene::mousePressEvent (QMouseEvent *_event)
{
    // this method is called when the mouse button is pressed in this case we
    // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
    if(_event->button() == Qt::LeftButton)
    {
      m_origX = _event->x();
      m_origY = _event->y();
      m_rotate =true;
    }
    // right mouse translate mode
    else if(_event->button() == Qt::RightButton)
    {
      m_origXPos = _event->x();
      m_origYPos = _event->y();
      m_translate=true;
    }
}




//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
        // right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

    // check the diff of the wheel position (0 means no change)
    if(_event->delta() > 0)
    {
        m_modelPos.m_z+=ZOOM;
    }
    else if(_event->delta() <0 )
    {
        m_modelPos.m_z-=ZOOM;
    }
    update();
}

void NGLScene::testButtonClicked(bool b)
{
    emit clicked (b);
    std::cout<<"Button Clicked - manual signal-slot connection"<<std::endl;
    m_rColor=ngl::Random::instance()->randomNumber(1);
    m_gColor=ngl::Random::instance()->randomNumber(1);
    m_bColor=ngl::Random::instance()->randomNumber(1);

    update();
}
