//**************************************************************************
// Práctica 5 
//**************************************************************************

#include <vector>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID, SELECT,SOLID_COLORS,DIFFUSE_FLAT, SOLID_GOURAUD} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D 
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
vector<_vertex3f> colores_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

       _triangulos3D();
       void draw_aristas(float r, float g, float b, int grosor);
       void draw_solido(float r, float g, float b);
       void draw_solido_colores();
       void draw_iluminacion_plana();
       void draw_iluminacion_suave();
       void    draw_seleccion(int r, int g, int b);
       void draw(_modo modo, float r, float g, float b, float grosor);

// Asignación de colores
       void colors_random();
       void colors_chess(float r1, float g1, float b1, float r2,
                                   float g2, float b2);
       void colors_diffuse_flat (float kr, float kg, float kb,
                                   float lpx, float lpy, float lpz);                            
       void colors_diffuse_gouraud  (float kr, float kg, float kb,
                                   float lpx, float lpy, float lpz); 

//Calcular normales  

       void calcular_normales_caras();
       void calcular_normales_vertices();

//Variable
       bool b_normales_caras, b_normales_vertices;
       int calculadas_normales_caras, brillo;
       _vertex4f ambiente;
       _vertex4f difuso;
       _vertex4f especular;

       vector<_vertex3i> caras;
       vector<_vertex3f> colores_caras;
       vector<_vertex3f> colores_vertices;
       vector<_vertex3f> normales_caras;
       vector<_vertex3f> normales_vertices;
};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};

//*************************************************************************
// clase cubo sin tres caras
//*************************************************************************

class _cubo_transf: public _triangulos3D
{
public:

	_cubo_transf(float tam=0.5);

};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};

//*************************************************************************
// clase cuña
//*************************************************************************

class _cuna: public _triangulos3D
{
public:

	_cuna(float tam=0.5, float al=1.0);

};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************


class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num,  
                 int tapa_in, int tapa_su, int tipo);
};

// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa
// tipo=0 revolucion normal
// tipo=1 esfera
// tipo=2 cono

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};


//************************************************************************
// objeto por revolución especiales
//************************************************************************

 
//************************************************************************
// cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
       _cilindro(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// cono
//************************************************************************

class _cono: public _rotacion
{
public:
       _cono(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
       _esfera(float radio=1.0, int latitud=6, int longitud=6);
       void calcular_normales_vertices();
};


//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_ply: public _rotacion{
	public:
		_rotacion_ply();

	void parametros(char *archivo, int num);

};


//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************


//************************************************************************
// fachada
//************************************************************************

class _fachada: public _triangulos3D{
       
       public:
              _fachada();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       float suelo;

       float fondo2;

       protected:
       _cubo cubo;
       _cubo_transf cubo_transf;
};

//************************************************************************
// Placa solar
//************************************************************************

class _placa_solar: public _triangulos3D{
       
       public:
              _placa_solar();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       protected:
       _cubo cubo;
};

//************************************************************************
// Chimenea
//************************************************************************

class _chimenea: public _triangulos3D{
       
       public:
              _chimenea();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       float ancho2;
       float alto2;
       float fondo2;

       float esf;

       protected:
       _cubo cubo;
       _esfera humo;
};

//************************************************************************
// Puerta
//************************************************************************

class _puerta: public _triangulos3D{
       
       public:
              _puerta();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       protected:
       _cubo cubo;
};

//************************************************************************
// Pomo
//************************************************************************

class _pomo: public _triangulos3D{
       
       public:
              _pomo();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float radio;

       protected:
       _esfera esfera;
};

//************************************************************************
// Ventana
//************************************************************************

class _ventana: public _triangulos3D{
       
       public:
              _ventana();
       void  draw(_modo modo, float r, float g, float b, float grosor);

       float ancho;
       float alto;
       float fondo;

       protected:
       _cubo cubo;
       
};

//************************************************************************
// Casa moderna (montaje del objeto final)
//************************************************************************

class _casa_moderna: public _triangulos3D
{
public:
       _casa_moderna();
       
void  draw(_modo modo, float r, float g, float b, float grosor);
void  seleccion(); // método para selección

  float giro_puerta;
  float giro_puerta_max;
  float giro_puerta_min;

  float giro_ventana;
  float giro_ventana_max;
  float giro_ventana_min;

  float giro_placa_solar;
  float giro_placa_solar_max;
  float giro_placa_solar_min;

  float giro_pomo;
  float giro_pomo_max;
  float giro_pomo_min;

       _vertex3f color_pick;
       vector<_vertex3i> color_select;
       vector<int> activo;
       int piezas;
       int grosor_select;  

protected:

_fachada fachada;
_placa_solar placa_solar;
_chimenea chimenea;
_ventana ventana;
_puerta puerta;
_pomo pomo;

};
