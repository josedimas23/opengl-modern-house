//**************************************************************************
// Práctica 5 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"

#include "CImg.h"
using namespace cimg_library;
using namespace std;

// tipos
typedef enum{CUBO, CUBO_TRANSF, PIRAMIDE, OBJETO_PLY, ROTACION, CILINDRO, 
ROTACION_PLY, CONO, ESFERA, EXTRUSION, CASA_MODERNA, CUNA, TEXTURA} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=650,Window_high=650;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_cuna cuna(0.85,1.3);
_cubo_transf cubo_transf;
_objeto_ply  ply;
_rotacion_ply ply1; 
_rotacion rotacion;
_cilindro cilindro(1,2,6); 
_cono cono(1,2,6);
_esfera esfera(1,6,6);
_extrusion *extrusion;
_casa_moderna casa_moderna;

float giro1=0, giro2=0, giro3=0, giro4=0, alfa=0;;
int pulsar=0;
int paso=0;

int estadoRaton, xc, yc;
int cambio=0;
float izq=-5.0, derch=5.0, abaj=-5.0, arriba=5.0, factor=1.0;
int ancho, alto;

//**************************************************************************
// TEXTURAS
//**************************************************************************

// FUNCIÓN QUE PREPARA LA TEXTURA
void OnDraw();
void prepara_textura(char *file, GLuint *tex_id);

// FUNCIÓN QUE DIBUJA
void dibuja(void);

// FUNCIÓN QUE LIBERA LA TEXTURA DE GPU
void libera_textura(GLuint *tex_id);


// IDENTIFICADOR DE TEXTURA
GLuint textura_id;

// CUBO

GLfloat cubo_vertices[][3] = { -0.9, -0.9, 0.9,      0.9, -0.9, 0.9,
                                0.9, 0.9, 0.9,      -0.9, 0.9, 0.9,
                               -0.9, -0.9, -0.9,     0.9, -0.9, -0.9,
                                0.9, 0.9, -0.9,     -0.9, 0.9, -0.9                               
                            };


//**************************************************************************
//
//***************************************************************************

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();
 glViewport(0, 0,ancho, alto );

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
    case CUNA: cuna.draw(modo,1.0,0.0,0.0,5);break;
    case CUBO_TRANSF: cubo_transf.draw(modo,1.0,0.0,0.0,5);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,5);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,5);break;
	    case ROTACION_PLY: ply1.draw(modo,1.0,0.6,0.0,5);break;
        case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
        case CONO: cono.draw(modo,1.0,0.0,0.0,5);break;
        case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
        case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
        case CASA_MODERNA: casa_moderna.draw(modo,1.0,0.0,0.0,5);break;
        case TEXTURA: dibuja();break;
	}

}


//**************************************************************************
//
//***************************************************************************

void cambio_camara(){   //ortogonal o vistas multiples

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0.5*alto,0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0,0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(90,1,0,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0.5*ancho, 0.5*alto, 0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(-90,0,1,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0.5*ancho, 0, 0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(60,1,0,0);
  glRotatef(30,0,1,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  draw_objects();


}

void cambio_camara_seleccion(){

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0.5*alto,0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  casa_moderna.seleccion();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0,0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(90,1,0,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  casa_moderna.seleccion();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0.5*ancho, 0.5*alto, 0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(-90,0,1,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  casa_moderna.seleccion();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0.5*ancho, 0, 0.5*ancho, 0.5*alto );
  glOrtho(izq*factor, derch*factor, abaj*factor, arriba*factor, -100, 100);
  glRotatef(60,1,0,0);
  glRotatef(30,0,1,0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  draw_axis();
  casa_moderna.seleccion();


}


//**************************************************************************
//
//***************************************************************************

void luces()
{
    GLfloat luz_ambiental[]={0.05, 0.05, 0.05, 1.0},
            luz_difusa[]={1.0, 1.0, 1.0, 1.0},
            luz_especular[]={1.0, 1.0, 1.0, 1.0},
            luz_posicion[]={0.0, 0.0, 20.0, 1.0};

    GLfloat luz_ambiental2[]={0.05, 0.05, 0.05, 1.0},
            luz_difusa2[]={1.0, 0.9, 0.0, 1.0}, // Amarillo claro
            luz_especular2[]={1.0, 0.9, 0.0, 1.0}, // Amarillo claro
            luz_posicion2[]={0.0, 10.0, 20.0, 1.0};

    glLightfv (GL_LIGHT1, GL_AMBIENT, luz_ambiental);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, luz_difusa);
    glLightfv (GL_LIGHT1, GL_SPECULAR, luz_especular);
    glLightfv (GL_LIGHT1, GL_POSITION, luz_posicion);

    glLightfv (GL_LIGHT2, GL_AMBIENT, luz_ambiental2);
    glLightfv (GL_LIGHT2, GL_DIFFUSE, luz_difusa2);
    glLightfv (GL_LIGHT2, GL_SPECULAR, luz_especular2);
    glLightfv (GL_LIGHT2, GL_POSITION, luz_posicion2);

    glPushMatrix();
    glRotatef(alfa,0,0,1);
    glLightfv (GL_LIGHT2, GL_POSITION, luz_posicion2);
    glPopMatrix();

    glDisable (GL_LIGHT0);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHT2);
}

//**************************************************************************
//
//***************************************************************************

void draw(void){
    
glDrawBuffer(GL_FRONT);
clean_window();
change_projection();
if(cambio==0){
  change_observer();
  draw_axis();
  draw_objects();
}else cambio_camara();
luces();

if (t_objeto==CASA_MODERNA)
  {glDrawBuffer(GL_BACK);
   clean_window();
   change_projection();
   if(cambio==0){

    change_observer();
    casa_moderna.seleccion();

   } else cambio_camara_seleccion();
   
  }
//glutSwapBuffers();
glFlush();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
ancho=Ancho1;
alto=Alto1;
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_COLORS;break;
    case '5':modo=DIFFUSE_FLAT;break;
    case '6':modo=SOLID_GOURAUD;break;
        case 'B':t_objeto=TEXTURA;break;
        case 'P':t_objeto=PIRAMIDE;break;
        case 'M':t_objeto=CUNA;break;
        case 'C':t_objeto=CUBO;break;
        case 'V':t_objeto=CUBO_TRANSF;break;
        case 'A':t_objeto=CASA_MODERNA;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'Z':t_objeto=ROTACION_PLY;break;
        case 'L':t_objeto=CILINDRO;break;
        case 'N':t_objeto=CONO;break;
        case 'E':t_objeto=ESFERA;break;
        case 'X':t_objeto=EXTRUSION;break;
         case ',':cambio=0;break;
        case '.':cambio=1;break;
        case 'S': if(pulsar==0){
                        giro1=0.75;
                        giro2=1.0;
                        giro3=1.0;
                        giro4=1.0;
                        pulsar=1;
                    }else{
                        giro1=0.0;
                        giro2=0.0;
                        giro3=0.0;
                        giro4=0.0;
                        pulsar=0;
                    }
                    break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

switch (Tecla1){
   case GLUT_KEY_LEFT:Observer_angle_y--;break;
   case GLUT_KEY_RIGHT:Observer_angle_y++;break;
   case GLUT_KEY_UP:Observer_angle_x--;break;
   case GLUT_KEY_DOWN:Observer_angle_x++;break;
   case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
   case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;

    //PUERTA
    case GLUT_KEY_F1:casa_moderna.giro_puerta-=1; //abrir puerta
        if (casa_moderna.giro_puerta < casa_moderna.giro_puerta_min)
            casa_moderna.giro_puerta = casa_moderna.giro_puerta_min;break;
   case GLUT_KEY_F2:casa_moderna.giro_puerta+=1;  //cerrar puerta
        if (casa_moderna.giro_puerta > casa_moderna.giro_puerta_max)
            casa_moderna.giro_puerta = casa_moderna.giro_puerta_max;break;
    //VENTANA
    case GLUT_KEY_F3:casa_moderna.giro_ventana-=1; //Abrir ventana
        if (casa_moderna.giro_ventana < casa_moderna.giro_ventana_min)
            casa_moderna.giro_ventana = casa_moderna.giro_ventana_min;break;
   case GLUT_KEY_F4:casa_moderna.giro_ventana+=1; //Cerrar ventana
        if (casa_moderna.giro_ventana > casa_moderna.giro_ventana_max)
            casa_moderna.giro_ventana = casa_moderna.giro_ventana_max;break;
    //PLACA SOLAR
    case GLUT_KEY_F5:casa_moderna.giro_placa_solar-=1; //Subir placa 
        if (casa_moderna.giro_placa_solar < casa_moderna.giro_placa_solar_min)
            casa_moderna.giro_placa_solar = casa_moderna.giro_placa_solar_min;break;
   case GLUT_KEY_F6:casa_moderna.giro_placa_solar+=1; //Bajar Placa
        if (casa_moderna.giro_placa_solar > casa_moderna.giro_placa_solar_max)
            casa_moderna.giro_placa_solar = casa_moderna.giro_placa_solar_max;break;
    //POMO
    case GLUT_KEY_F7:casa_moderna.giro_pomo+=1; //Girar pomo izquierda
        if (casa_moderna.giro_pomo > casa_moderna.giro_pomo_max)
            casa_moderna.giro_pomo = casa_moderna.giro_pomo_max;break;  
    case GLUT_KEY_F8:casa_moderna.giro_pomo-=1; //Girar pomo derecha
        if (casa_moderna.giro_pomo < casa_moderna.giro_pomo_min)
            casa_moderna.giro_pomo = casa_moderna.giro_pomo_min;break; 

    case GLUT_KEY_F9:alfa+=5;break;                             
	}
glutPostRedisplay();
}

void animacion(){

    switch(paso){

        case 0:casa_moderna.giro_pomo+=giro1;
            if(casa_moderna.giro_pomo>45.0) paso=1;
            break;

        case 1:casa_moderna.giro_puerta-=giro2;
            if(casa_moderna.giro_puerta<-80.0) paso=2;
            break;

        case 2:casa_moderna.giro_ventana-=giro3;
            if(casa_moderna.giro_ventana<-80.0) paso=3;
            break; 

        case 3:casa_moderna.giro_placa_solar-=giro4;
            if(casa_moderna.giro_placa_solar < casa_moderna.giro_placa_solar_min){

                casa_moderna.giro_placa_solar= casa_moderna.giro_placa_solar_min;
                paso=0;
                casa_moderna.giro_pomo=0.0;
                casa_moderna.giro_puerta=0.0;
                casa_moderna.giro_ventana=0.0;
                casa_moderna.giro_placa_solar=0.0;
            }
            break;        
    }
    glutPostRedisplay();
}

//***************************************************************************
// Funciones para la selección por color
//***************************************************************************

void procesar_color(unsigned char color[3])
{
int i;

for (i=0;i<casa_moderna.piezas;i++)
   {if (color[0]==casa_moderna.color_select[i].r &&
        color[1]==casa_moderna.color_select[i].g &&
        color[2]==casa_moderna.color_select[i].r)
       {if (casa_moderna.activo[i]==0) 
                      {casa_moderna.activo[i]=1;
                      }
                  else 
                      {casa_moderna.activo[i]=0;
                      }
         glutPostRedisplay();
        }
    }                
}

//***************************************************************************

void pick_color(int x, int y)
{
GLint viewport[4];
unsigned char pixel[3];

glGetIntegerv(GL_VIEWPORT, viewport);
glReadBuffer(GL_BACK);
glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(GLubyte *) &pixel[0]);
printf(" valor x %d, valor y %d, color %d, %d, %d \n",x,y,pixel[0],pixel[1],pixel[2]);

procesar_color(pixel);
}

//***************************************************************************
// Funciones para manejo de eventos del ratón
//***************************************************************************

void clickRaton( int boton, int estado, int x, int y )
{
if(boton==GLUT_RIGHT_BUTTON) 
  {
   if(estado==GLUT_DOWN) 
     {
      estadoRaton=1;
      xc=x;
      yc=y;
     } 
   else estadoRaton=0;
   }
if(boton==GLUT_LEFT_BUTTON) 
  {
   if(estado==GLUT_DOWN) 
     {
      estadoRaton=2;
      xc=x;
      yc=y;
      pick_color(xc, yc);
     } 
  }
 if(boton==3) 
  {
    factor*=1.1;
    glutPostRedisplay();
  }
 if(boton==4) 
  {
    factor/=1.1;
    glutPostRedisplay();
  }
}

/*************************************************************************/

void RatonMovido( int x, int y )
{ 
if(estadoRaton==1) 
    {Observer_angle_y=Observer_angle_y-(x-xc);
     Observer_angle_x=Observer_angle_x+(y-yc);
     xc=x;
     yc=y;
     glutPostRedisplay();
    }
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,Window_width,Window_high);

//glEnable(GL_COLOR_MATERIAL);
glMatrixMode(GL_PROJECTION);
prepara_textura("./skybox.jpg", &textura_id);

}



//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{
 
// perfil 

vector<_vertex3f> perfil, poligono;
_vertex3f aux;
int num =6;
float radio = 0.5, radio1 = 1.0;

for (int i=1;i<=num/2;i++)
  {aux.x=radio*cos(M_PI*i/(num*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(num*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
aux.x=0.3; aux.y=0.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=0.3; aux.y=0.5; aux.z=0.0;
perfil.push_back(aux);

rotacion.parametros(perfil,6,1,0,0);

//extrusion

aux.x=1.0; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);
aux.x=1.0; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-0.8; aux.y=0.0; aux.z=0.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);

extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

// glEnable(GL_DEPTH_TEST);
// glMatrixMode(GL_PROJECTION);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 5");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
//glutDisplayFunc(OnDraw);

// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);

glutIdleFunc(animacion);


// funcion de inicialización
initialize();

glutMouseFunc(clickRaton);
glutMotionFunc(RatonMovido);

glutSwapBuffers();

// creación del objeto ply
ply.parametros(argv[1]);
ply1.parametros(argv[2],12);
//ply = new _objeto_ply(argv[1]);



// inicio del bucle de eventos
glutMainLoop();

libera_textura(&textura_id);
return 0;
}

// ----------------------------------------------------------------------
void prepara_textura (char *file, GLuint *tex_id )
{
   vector<unsigned char> data; 
   CImg<unsigned char> image;

   image.load(file);

   // empaquetamos bien los datos
   for (long y = 0; y < image.height(); y ++)
      for (long x = 0; x < image.width(); x ++)
      {
	 unsigned char *r = image.data(x, y, 0, 0);
	 unsigned char *g = image.data(x, y, 0, 1);
	 unsigned char *b = image.data(x, y, 0, 2);
	 data.push_back(*r);
	 data.push_back(*g);
	 data.push_back(*b);
      }

   glGenTextures(1, tex_id);
   glBindTexture(GL_TEXTURE_2D, *tex_id);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // TRASFIERE LOS DATOS A GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(),
		0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
}

// coordenadas cubo

GLfloat caraDeAbajo[][2] = {0.25, 0.75,      0.5, 0.75,      0.5, 0.5,   0.25, 0.5}, 
    caraDeArriba[][2] = {0.25, 0.25,     0.5, 0.25,      0.5, 0.0,   0.25, 0.0};
        
GLfloat caraDeDelante[][2] = {0.25, 0.5,     0.5, 0.5,       0.5, 0.25,     0.25, 0.25},
    caraDeAtras[][2] = {1.0,0.5,         0.75, 0.5,      0.75, 0.25,    1.0, 0.25};
             
GLfloat caraIzquierda[][2] = {0.25, 0.5,     0.0, 0.5,       0.0, 0.25,      0.25, 0.25},  
    caraDerecha[][2] = {0.5, 0.5,        0.75, 0.5,      0.75, 0.25,     0.5, 0.25};
        
void dibuja (void)
{
   glColor3f(1.0,1.0,1.0);
   glEnable(GL_TEXTURE_2D);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

   glBindTexture(GL_TEXTURE_2D, textura_id);

//  CUBO

    // cara de abajo
    glBegin(GL_QUADS);
    glTexCoord2fv(caraDeAbajo[0]);
    glVertex3fv(cubo_vertices[0]);
    glTexCoord2fv(caraDeAbajo[1]);
    glVertex3fv(cubo_vertices[1]);
    glTexCoord2fv(caraDeAbajo[2]);
    glVertex3fv(cubo_vertices[5]);
    glTexCoord2fv(caraDeAbajo[3]);
    glVertex3fv(cubo_vertices[4]);
    glEnd();

    // cara de arriba
    glBegin(GL_QUADS);
    glTexCoord2fv(caraDeArriba[0]);
    glVertex3fv(cubo_vertices[3]);
    glTexCoord2fv(caraDeArriba[1]);
    glVertex3fv(cubo_vertices[2]);
    glTexCoord2fv(caraDeArriba[2]);
    glVertex3fv(cubo_vertices[6]);
    glTexCoord2fv(caraDeArriba[3]);
    glVertex3fv(cubo_vertices[7]);
    glEnd();

    // cara frontal
    glBegin(GL_QUADS);
    glTexCoord2fv(caraDeDelante[0]);
    glVertex3fv(cubo_vertices[0]);
    glTexCoord2fv(caraDeDelante[1]);
    glVertex3fv(cubo_vertices[1]);
    glTexCoord2fv(caraDeDelante[2]);
    glVertex3fv(cubo_vertices[2]);
    glTexCoord2fv(caraDeDelante[3]);
    glVertex3fv(cubo_vertices[3]);
    glEnd();

    // cara de atras
    glBegin(GL_QUADS);
    glTexCoord2fv(caraDeAtras[0]);
    glVertex3fv(cubo_vertices[4]);
    glTexCoord2fv(caraDeAtras[1]);
    glVertex3fv(cubo_vertices[5]);
    glTexCoord2fv(caraDeAtras[2]);
    glVertex3fv(cubo_vertices[6]);
    glTexCoord2fv(caraDeAtras[3]);
    glVertex3fv(cubo_vertices[7]);
    glEnd();

    // cara izquierda
    glBegin(GL_QUADS);
    glTexCoord2fv(caraIzquierda[0]);
    glVertex3fv(cubo_vertices[0]);
    glTexCoord2fv(caraIzquierda[1]);
    glVertex3fv(cubo_vertices[4]);
    glTexCoord2fv(caraIzquierda[2]);
    glVertex3fv(cubo_vertices[7]);
    glTexCoord2fv(caraIzquierda[3]);
    glVertex3fv(cubo_vertices[3]);
    glEnd();

    // cara derecha
    glBegin(GL_QUADS);
    glTexCoord2fv(caraDerecha[0]);
    glVertex3fv(cubo_vertices[1]);
    glTexCoord2fv(caraDerecha[1]);
    glVertex3fv(cubo_vertices[5]);
    glTexCoord2fv(caraDerecha[2]);
    glVertex3fv(cubo_vertices[6]);
    glTexCoord2fv(caraDerecha[3]);
    glVertex3fv(cubo_vertices[2]);
    glEnd();
   
   glDisableClientState(GL_VERTEX_ARRAY);
   glBindTexture(GL_TEXTURE_2D, 0);
   glDisable(GL_TEXTURE_2D);
}

void libera_textura (GLuint *tex_id)
{
   glDeleteTextures(1, tex_id);
}