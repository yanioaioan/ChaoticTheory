#ifndef NGLSCENE_H
#define NGLSCENE_H

//#include <QGLWidget>//old way of doing it (with legacy QGLWidget)

#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Camera.h>
#include <ngl/Material.h>
#include <ngl/VAOPrimitives.h>
#include <QMouseEvent>
#include <vector>

#include <iostream>
#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>

using namespace std;
using namespace boost::numeric::odeint;




#include <QOpenGLWidget>

class NGLScene: public QOpenGLWidget
{
    Q_OBJECT
public:
    NGLScene(/*QWidget *_parent*/);
    ~NGLScene();

    float m_rColor,m_gColor,m_bColor;
    ngl::Transformation m_transform;


    typedef boost::array< double , 3 > state_type;

    void lorenz( const state_type &x , state_type &dxdt , double t );

    void write_lorenz( const state_type &x , const double t );

protected:
    void mouseMoveEvent (QMouseEvent * _event);
    void mousePressEvent (QMouseEvent * _event);

    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent * _event );
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent(QWheelEvent *_event);


    void loamatricestoShader();
    void initializeGL ();
    void resizeGL (int _w, int _h);
    void paintGL ();
private:


    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;

    std::vector<state_type> valuePoints;


    ngl::Camera *m_cam;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the x rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief used to store the y rotation mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_spinYFace;

    ngl::Mat4 m_mouseGlobalTX;

    ngl::Vec3 m_modelPos;

    /// @brief the previous x mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;

    /// @brief the previous x mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the previous y mouse value for Position changes
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;

    /// @brief flag to indicate if the mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief flag to indicate if the Right mouse button is pressed when dragging
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;

    QOpenGLContext *m_context;



signals:
    void clicked(bool);

public slots:
  void testButtonClicked(bool);

};

#endif // NGLSCENE_H
