#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QMouseEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "model.h"
#include "MyGLWidget.h"

class A3GLWidget : public MyGLWidget
{
  Q_OBJECT

#define NUM_TORXES 6
#define HORA_MIN 8
#define HORA_MAX 20
#define NUM_FOCUS_FAR 2

  public:
    A3GLWidget (QWidget *parent=0);
    ~A3GLWidget ();
  public slots:
  void onFocusMoved(int hour);
  //SLOTS TORXES
  void onTorxaOne(bool OnOff);
  void onTorxaTwo(bool OnOff);
  void onTorxaThree(bool OnOff);
  void onTorxaFour(bool OnOff);
  void onTorxaFive(bool OnOff);
  void onTorxaSix(bool OnOff);
  
  void redBar(int color);
  void greenBar(int color);
  void blueBar(int color);

  signals:
  void signalFocusMovedKeyboard(int hour);
  protected:

    glm::mat4 modelTransformFar1();
    glm::mat4 modelTransformFar2();
    glm::mat4 modelTransformVaixell();
    void iniEscena ();
    void setLlums();
    void carregaShaders();

    virtual void paintGL ( );
    virtual void keyPressEvent (QKeyEvent *event);
    //--------------------------------------------------------------
    // uniform locations

    //--------------------------------------------------------------
    // Posicions
    glm::vec3 VaixellPos;

    //--------------------------------------------------------------
    // Colors

    //--------------------------------------------------------------
    // MATRIUS DE TRANSFORMACIÓ
    glm::mat4 Vaixell_TG, Far2_TG;
    GLuint llumAmbientloc, posFocusLoc, colorFocusLoc, normalMatrixLoc, llumFar1loc, llumFar2loc;
    GLuint DirF1loc, DirF2loc, posFocusTorxesLoc, llumTorxesLoc, torxesOnOffLoc, boolMarLoc, tempsLoc;
    float degreeSol, degreeFar;
    glm::vec3 posFocus;
    int horaAct;
    glm::vec3 colorTorxes;
    int torxesOnOff[NUM_TORXES];
    int boolMar;

};

