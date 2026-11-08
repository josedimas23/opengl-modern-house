//**************************************************************************
// Práctica 5 
//**************************************************************************

#include "objetos_B3.h"
#include "file_ply_stl.hpp"
#include <time.h> 


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() 
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
 b_normales_caras=false; b_normales_vertices=false;

  ambiente=_vertex4f(0.7,0.3,0.1,1.0);    //coeficientes ambiente
  difuso=_vertex4f(0.7,0.3,0.1,1.0);      //coeficientes difuso
  especular=_vertex4f(0.5,0.5,0.5,1.0);   //coeficientes especular
  brillo=120;                             //exponente del brillo

}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();

}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores( )
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glColor3f(colores_caras[i].r,colores_caras[i].g,colores_caras[i].b);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con iluminacion plana
//*************************************************************************

void _triangulos3D::draw_iluminacion_plana()
{
  int i;
  if (b_normales_caras == false)
    calcular_normales_caras();
  glEnable(GL_LIGHTING);
  glShadeModel(GL_FLAT); 
  glEnable(GL_NORMALIZE); 
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);

  for (i = 0; i < caras.size(); i++)
  {
    glNormal3fv((GLfloat *)&normales_caras[i]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo sólido con iluminacion suave
//*************************************************************************

void _triangulos3D::draw_iluminacion_suave()
{
  if (!b_normales_vertices)
    calcular_normales_vertices();
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glBegin(GL_TRIANGLES);
  for (int i = 0; i < caras.size(); i++)
  {
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

//*************************************************************************
// dibujar en modo seleccion
//*************************************************************************

void _triangulos3D::draw_seleccion(int r, int g, int b)
{
int i;

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3ub(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r, g, b, grosor);break;
	case EDGES:draw_aristas(r, g, b, grosor);break;
	case SOLID:draw_solido(r, g, b);break;
	case SOLID_COLORS:draw_solido_colores();break;
  case DIFFUSE_FLAT:draw_iluminacion_plana();break;
  case SOLID_GOURAUD:draw_iluminacion_suave();break;
  case SELECT:draw_seleccion(r, g, b);break;
	}
}

//*************************************************************************
// normales 
//*************************************************************************

void _triangulos3D::calcular_normales_caras()
{
int i, n_c;
_vertex3f va, vb;
float modulo;

n_c=caras.size();
normales_caras.resize(n_c);
for (i=0;i<n_c;i++)
  {va=vertices[caras[i]._1]-vertices[caras[i]._0];
   vb=vertices[caras[i]._2]-vertices[caras[i]._1];
   normales_caras[i].x=va.y*vb.z-va.z*vb.y;
   normales_caras[i].y=va.z*vb.x-va.x*vb.z;   
   normales_caras[i].z=va.x*vb.y-va.y*vb.x; 
   modulo=sqrt(normales_caras[i].x*normales_caras[i].x+
              normales_caras[i].y*normales_caras[i].y+
              normales_caras[i].z*normales_caras[i].z);
   normales_caras[i].x/=modulo;
   normales_caras[i].y/=modulo;   
   normales_caras[i].z/=modulo;     
  }
}

//*************************************************************************

void _triangulos3D::calcular_normales_vertices()
{
int i, n_c, n_v;
float modulo;

n_v=vertices.size();
normales_vertices.resize(n_v);

n_c=caras.size();

for (i=0;i<n_v;i++)
 normales_vertices[i]=_vertex3f(0.0,0.0,0.0);
 
for (i=0;i<n_c;i++)
  {
  normales_vertices[caras[i]._0]+=normales_caras[i];
  normales_vertices[caras[i]._1]+=normales_caras[i];
  normales_vertices[caras[i]._2]+=normales_caras[i];
  } 
  
for (i=0;i<n_v;i++)
      {modulo=sqrt(normales_vertices[i].x*normales_vertices[i].x+
                   normales_vertices[i].y*normales_vertices[i].y+
                   normales_vertices[i].z*normales_vertices[i].z);
       normales_vertices[i].x/=modulo;
       normales_vertices[i].y/=modulo;
       normales_vertices[i].z/=modulo;            
      }
}

//*************************************************************************

void _esfera::calcular_normales_vertices()
{
  normales_vertices.resize(vertices.size());
  for (int i = 0; i < vertices.size(); i++)
  {
    normales_vertices[i] = _vertex3f(0.0, 0.0, 0.0);
    normales_vertices[i]._0 = vertices[i]._0;
    normales_vertices[i]._1 =
        vertices[i]._1;
    normales_vertices[i]._2 = vertices[i]._2;
    normales_vertices[i].normalize();
  }
  b_normales_vertices = true;
}

//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
srand (time(NULL));
for (i=0;i<n_c;i++)  
  {colores_caras[i].r=rand()%1000/1000.0;
   colores_caras[i].g=rand()%1000/1000.0;
   colores_caras[i].b=rand()%1000/1000.0;
  }
}

//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i, n_c;
n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)  
  {if (i%2==0) 
     {colores_caras[i].r=r1;
      colores_caras[i].g=g1;
      colores_caras[i].b=b1;
     }
   else 
     {colores_caras[i].r=r2;
      colores_caras[i].g=g2;
      colores_caras[i].b=b2;
     } 
  }
}

//*************************************************************************

void _triangulos3D::colors_diffuse_flat (float kr, float kg, float kb,
                             float lpx, float lpy, float lpz)
{
int i, n_c;
float modulo, escalar;
_vertex3f l;

n_c=caras.size();
colores_caras.resize(n_c);
for (i=0;i<n_c;i++)
  {
   l.x=lpx-vertices[caras[i]._0].x;
   l.y=lpy-vertices[caras[i]._0].y;
   l.z=lpz-vertices[caras[i]._0].z;
   modulo=sqrt(l.x*l.x+l.y*l.y+l.z*l.z);
   l.x/=modulo;
   l.y/=modulo;
   l.z/=modulo;
   
   escalar=l.x*normales_caras[i].x+l.y*normales_caras[i].y+l.z*normales_caras[i].z;
   if (escalar>0.0)
     {colores_caras[i].r=kr*escalar;
      colores_caras[i].g=kg*escalar;
      colores_caras[i].b=kb*escalar;
     }
   else {colores_caras[i].r=0.0;
        colores_caras[i].g=0.0;
        colores_caras[i].b=0.0;
        }
  }
}

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4; 

// normales
calcular_normales_caras();

//colores de las caras
colors_random();
}

//*************************************************************************
// clase cubo sin tres caras
//*************************************************************************

_cubo_transf::_cubo_transf(float tam)
{
	//vertices
  vertices.resize(8);
  vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
  vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
  vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
  vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
  vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
  vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
  vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
  vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

	// triangulos
  caras.resize(6);

  //caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
  //caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
  //caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
  //caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;

  caras[0]._0=5;caras[0]._1=4;caras[0]._2=6;
  caras[1]._0=4;caras[1]._1=7;caras[1]._2=6;
  caras[2]._0=0;caras[2]._1=7;caras[2]._2=4;
  caras[3]._0=0;caras[3]._1=3;caras[3]._2=7;
  caras[4]._0=3;caras[4]._1=2;caras[4]._2=7;
  caras[5]._0=2;caras[5]._1=6;caras[5]._2=7;
  //caras[6]._0=0;caras[6]._1=1;caras[6]._2=4;
  //caras[7]._0=1;caras[7]._1=5;caras[7]._2=4; 

  // normales
  calcular_normales_caras();

	// colores caras
  colors_random();

}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
int i;
//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

// normales
calcular_normales_caras();

//colores de las caras
colors_random();
}

//*************************************************************************
// clase cuña
//*************************************************************************

_cuna::_cuna(float tam, float al)
{
  //vertices
vertices.resize(6);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=0;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=0;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;

// triangulos
caras.resize(8);
caras[0]._0 = 0; caras[0]._1 = 1; caras[0]._2 = 2;
caras[1]._0 = 0; caras[1]._1 = 2; caras[1]._2 = 3;
caras[2]._0 = 0; caras[2]._1 = 3; caras[2]._2 = 4;
caras[3]._0 = 0; caras[3]._1 = 4; caras[3]._2 = 1;
caras[4]._0 = 5; caras[4]._1 = 2; caras[4]._2 = 1;
caras[5]._0 = 5; caras[5]._1 = 3; caras[5]._2 = 2;
caras[6]._0 = 5; caras[6]._1 = 4; caras[6]._2 = 3;
caras[7]._0 = 5; caras[7]._1 = 1; caras[7]._2 = 4;

// normales
calcular_normales_caras();

//colores de las caras
colors_random();
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



void _objeto_ply::parametros(char *archivo)
{
int i, n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
 
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

vertices.resize(n_ver);
caras.resize(n_car);
// vértices

for (i=0;i<n_ver;i++)
  {vertices[i].x=ver_ply[3*i];
   vertices[i].y=ver_ply[3*i+1];
   vertices[i].z=ver_ply[3*i+2];
  }

// vértices
for (i=0;i<n_car;i++)
  {caras[i].x=car_ply[3*i];
   caras[i].y=car_ply[3*i+1];
   caras[i].z=car_ply[3*i+2];
  }

// normales
calcular_normales_caras(); 

// colores
//colors_random();
colors_diffuse_flat(0.8,0.9,0.2, 20,20,20);
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, 
                           int tapa_in, int tapa_su, int tipo)
{
int i,j;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;
float radio;

// tratamiento de los vértice
radio=sqrt(perfil[0].x*perfil[0].x+perfil[0].y*perfil[0].y);

num_aux=perfil.size();
if (tipo==2) num_aux=1;
vertices.resize(num_aux*num);

for (j=0;j<num;j++)
  {for (i=0;i<num_aux;i++)
     {
      vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
      vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
                    perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
      vertice_aux.y=perfil[i].y;
      vertices[i+j*num_aux]=vertice_aux;
     }
  }

// tratamiento de las caras 
if (tipo<2)
{
for (j=0;j<num;j++)
  {for (i=0;i<num_aux-1;i++)
     {cara_aux._0=i+j*num_aux;
      cara_aux._1=i+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);

      cara_aux._0=i+((j+1)%num)*num_aux;
      cara_aux._1=i+1+((j+1)%num)*num_aux;
      cara_aux._2=i+1+j*num_aux;
      caras.push_back(cara_aux);
     }
   }
}
 
// tapa inferior
if (tapa_in==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  if (tipo==1) vertice_aux.y=-radio;
  else vertice_aux.y=perfil[0].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);
  //caras tapa inferior

  cara_aux._0=num_aux*num;
  for (j=0;j<num;j++)
    {
     cara_aux._1=((j+1)%num)*num_aux;
     cara_aux._2=j*num_aux;
     caras.push_back(cara_aux);
    }

}
 
// tapa superior

if (tapa_su==1)
{
  // punto central de la tapa
  vertice_aux.x=0.0;
  vertice_aux.y=perfil[num_aux-1].y;
  if (tipo==1) vertice_aux.y=radio;
  if (tipo==2) vertice_aux.y=perfil[1].y;
  vertice_aux.z=0.0;
  vertices.push_back(vertice_aux);

  //caras tapa superior
  if (tapa_in==1) cara_aux._0=num_aux*num+1;
  else cara_aux._0=num_aux*num;

  for (j=0;j<num;j++)
    {cara_aux._1=j*num_aux+num_aux-1;
     cara_aux._2=((j+1)%num)*num_aux+num_aux-1;
     caras.push_back(cara_aux);
    }
}

// normales
calcular_normales_caras();
calcular_normales_vertices();

//colores de las caras
colors_random();
}


//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
int i;
_vertex3f vertice_aux;
_vertex3i cara_aux;
int num_aux;

// tratamiento de los vértice

num_aux=poligono.size();
vertices.resize(num_aux*2);
for (i=0;i<num_aux;i++)
{
  vertices[2*i]=poligono[i];
  vertices[2*i+1].x=poligono[i].x+x;
  vertices[2*i+1].y=poligono[i].y+y;
  vertices[2*i+1].z=poligono[i].z+z;
}
    
// tratamiento de las caras 

caras.resize(num_aux*2);
int c=0;
for (i=0;i<num_aux;i++)         
  {
   caras[c]._0=i*2;
   caras[c]._1=(i*2+2)%(num_aux*2);
   caras[c]._2=i*2+1;    
   c=c+1;
   caras[c]._0=(i*2+2)%(num_aux*2);
   caras[c]._1=(i*2+2)%(num_aux*2)+1;
   caras[c]._2=i*2+1;    
   c=c+1;    
   }  

// normales
calcular_normales_caras();  

//colores de las caras
colors_random();
}

//************************************************************************
//************************************************************************

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

_cilindro::_cilindro(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=-altura/2.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=radio; aux.y=altura/2.0; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,0);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
vector<_vertex3f> perfil;
_vertex3f aux;

aux.x=radio; aux.y=0; aux.z=0.0;
perfil.push_back(aux);
aux.x=0.0; aux.y=altura; aux.z=0.0;
perfil.push_back(aux);
parametros(perfil,num,1,1,2);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

_esfera::_esfera(float radio, int latitud, int longitud)
{
vector<_vertex3f> perfil;
_vertex3f aux;
int i;
for (i=1;i<latitud;i++)
  {aux.x=radio*cos(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.y=radio*sin(M_PI*i/(latitud*1.0)-M_PI/2.0);
   aux.z=0.0;
   perfil.push_back(aux);
  }
parametros(perfil,longitud,1,1,1);
}


//*************************************************************************
// clase rotacion ply
//*************************************************************************


_rotacion_ply::_rotacion_ply(){
 
}

void _rotacion_ply::parametros(char *archivo, int num){

	int n_ver;
	vector<_vertex3f> perfil;

	vector<float> ver_ply ;
	vector<int>   car_ply ;
	
	_vertex3f aux;
	
 
	_file_ply::read(archivo, ver_ply, car_ply );

	n_ver=ver_ply.size()/3;

	printf("Number of vertices=%d\n", n_ver);

	vertices.resize(n_ver);

	//vertices
	for(int i=0;i<n_ver;i++){

		aux.x=ver_ply[3*i];
		aux.y=ver_ply[3*i+1];
		aux.z=ver_ply[3*i+2];

		perfil.push_back(aux);
	}

	_rotacion::parametros(perfil, num, 1, 1, 0);

}

//************************************************************************
// práctica 3-4, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// PIEZAS
//************************************************************************

//************************************************************************
// Fachada
//************************************************************************

_fachada::_fachada(){

  ancho = 1.4;
  alto = 0.9;
  fondo = 1.4;

  suelo = 3.0;
  fondo2 = 0.02;

  // Iluminación y coeficientes del material para un tono beige
  cubo.ambiente = _vertex4f(0.9, 0.8, 0.7, 1.0);   // Coeficientes ambiente beige
  cubo.difuso = _vertex4f(0.8, 0.8, 0.7, 1.0);     // Coeficientes difuso beige
  cubo.especular = _vertex4f(0.2, 0.2, 0.25, 1.0);  // Coeficientes especular (tono gris)
  cubo.brillo = 10;                                    // Exponente del brillo

  cubo_transf.ambiente = _vertex4f(0.9, 0.8, 0.7, 1.0);   // Coeficientes ambiente beige
  cubo_transf.difuso = _vertex4f(0.85, 0.75, 0.65, 1.0);     // Coeficientes difuso beige
  cubo_transf.especular = _vertex4f(0.25, 0.2, 0.2, 1.0);  // Coeficientes especular (tono gris)
  cubo_transf.brillo = 10;                                  // Exponente del brillo
};

void _fachada::draw(_modo modo, float r, float g, float b, float grosor)
{
  //SUELO

  glPushMatrix();
  glTranslatef(0.0, -alto/2.0, 0.0);
  glScalef(suelo, 0.0, suelo);
  cubo.draw(modo, 0.5,0.5,0, grosor); 
  glPopMatrix();

  //PAREDES

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo_transf.draw(modo, 0.9, 0.8, 0.7, grosor); //beige
  glPopMatrix();

  //     //PARED 1

    glPushMatrix();
    glTranslatef(-0.45, 0.0, 0.7);
    glScalef(0.5, alto, fondo2);
    cubo.draw(modo, r, g, b, grosor); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.45, 0.0, 0.7);
    glScalef(0.5, alto, fondo2);
    cubo.draw(modo,r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.2, 0.7);
    glScalef(0.4, 0.5, fondo2);
    cubo.draw(modo,r, g, b, grosor);
    glPopMatrix();

  //     //PARED 2

    glPushMatrix();
    glTranslatef(0.7, 0.0, -0.50);
    glRotatef(90,0,1,0);
    glScalef(0.4, alto, fondo2);
    cubo.draw(modo, r, g, b, grosor); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7, 0.0, 0.50);
    glRotatef(90,0,1,0);
    glScalef(0.4, alto, fondo2);
    cubo.draw(modo, r, g, b, grosor); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7, -0.25, 0.0);
    glRotatef(90,0,1,0);
    glScalef(0.6, 0.4, fondo2);
    cubo.draw(modo, r, g, b, grosor); 
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7,0.4, 0.0);
    glRotatef(90,0,1,0);
    glScalef(0.6, 0.1, fondo2);
    cubo.draw(modo, r, g, b, grosor); 
    glPopMatrix();
};

//************************************************************************
// Placa solar
//************************************************************************

_placa_solar::_placa_solar(){

  ancho = 0.9;
  alto = 0.05;
  fondo = 0.5;

  // Iluminación

  cubo.ambiente = _vertex4f(0.0, 0.0, 0.5, 1.0);   // Coeficientes ambiente azul
  cubo.difuso = _vertex4f(0.2, 0.2, 0.4 , 1.0);     // Coeficientes difuso azul
  cubo.especular = _vertex4f(0.5, 0.5, 0.5, 1.0);  // Coeficientes especular
  cubo.brillo = 120;                                 // Exponente del brillo

};
void _placa_solar::draw(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo.draw(modo, r, g, b, grosor); 
  glPopMatrix();

};

//************************************************************************
// Chimenea
//************************************************************************

_chimenea::_chimenea(){

  ancho=0.3;
  alto=0.1;
  fondo=0.3;

  ancho2=0.2;
  alto2=0.3;
  fondo2=0.2;

  esf=0.2;

  //iluminacion

  cubo.ambiente=_vertex4f(0.5, 0.0, 0.0, 1.0);    //coeficientes ambiente
  cubo.difuso=_vertex4f(0.6, 0.0, 0.0, 1.0 );      //coeficientes difuso
  cubo.especular=_vertex4f( 0.5, 0.5, 0.5, 1.0 );   //coeficientes especular
  cubo.brillo=10;                               //exponente del brillo

  humo.ambiente=_vertex4f(0.8, 0.8, 0.8, 0.4);    //coeficientes ambiente
  humo.difuso=_vertex4f( 0.7, 0.7, 0.7, 0.4);      //coeficientes difuso
  humo.especular=_vertex4f(0.2, 0.2, 0.2, 0.4);   //coeficientes especular
  humo.brillo=0;                               //exponente del brillo


};

void _chimenea::draw(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glTranslatef(0.0, alto2/2, 0.0);
  glScalef(ancho, alto, fondo);
  cubo.draw(modo, r, g, b, grosor); // granate
  glPopMatrix();

  glPushMatrix();
  glScalef(ancho2, alto2, fondo2);
  cubo.draw(modo, r, g, b, grosor);
  glPopMatrix();

  // //HUMO CHIMENEA
  
  glPushMatrix();
  glTranslatef(0.0, 0.35, 0.0);
  glScalef(esf, esf, esf);                  //humo es de tipo esfera
  humo.draw(modo, 0.8, 0.8, 0.8, grosor);   //gris clarito
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.1, 0.6, 0.0);
  glScalef(esf, esf, esf);
  humo.draw(modo, 0.8, 0.8, 0.8, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 0.85, 0.0);
  glScalef(esf, esf, esf);
  humo.draw(modo, 0.8, 0.8, 0.8, grosor);
  glPopMatrix();
};

//************************************************************************
// Puerta
//************************************************************************

_puerta::_puerta(){

  ancho=0.4;
  alto=0.4;
  fondo=0.02;

  //iluminacion

  cubo.ambiente=_vertex4f(0.6, 0.3, 0.0, 1.0);    //coeficientes ambiente
  cubo.difuso=_vertex4f( 0.55, 0.25, 0.0, 1.0);      //coeficientes difuso
  cubo.especular=_vertex4f(0.51, 0.21, 0.0, 1.0);   //coeficientes especular
  cubo.brillo=10;                               //exponente del brillo 

};

void _puerta::draw(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo.draw(modo,  r, g, b, grosor);
  glPopMatrix();

};

//************************************************************************
// Pomo
//************************************************************************

_pomo::_pomo(){

  radio=0.03;

  //iluminacion

  esfera.ambiente=_vertex4f( 1.0, 0.84, 0.0, 1.0 );    //coeficientes ambiente
  esfera.difuso=_vertex4f( 0.6, 0.6, 0.6, 1.0);      //coeficientes difuso
  esfera.especular=_vertex4f( 0.8, 0.8, 0.8, 1.0);   //coeficientes especular
  esfera.brillo=50;                               //exponente del brillo

};

void _pomo::draw(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glScalef(radio, radio, radio);
  esfera.draw(modo, r, g, b, grosor); // dorado
  glPopMatrix();

};

//************************************************************************
// Ventana
//************************************************************************

_ventana::_ventana(){

  ancho=0.6;
  alto=0.4;
  fondo=0.02;

  //iluminacion

  cubo.ambiente = _vertex4f( 0.6, 0.8, 1.0, 0.5);  // Tono azul claro para el ambiente con algo de transparencia
  cubo.difuso = _vertex4f(0.65, 0.85, 15.0, 0.5);    // Tono azul claro para la luz difusa con algo de transparencia
  cubo.especular = _vertex4f(0.69, 0.89, 19.0, 1.0); // Coeficientes de reflexión especular (pueden mantenerse grises)
  cubo.brillo = 100;                              // Mantiene el exponente del brillo


};

void _ventana::draw(_modo modo, float r, float g, float b, float grosor)
{

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo.draw(modo, r, g, b, grosor);  //azul clarito
  glPopMatrix();
};

//************************************************************************
// Casas moderna (montaje del objeto final)
//************************************************************************

_casa_moderna::_casa_moderna(){

  giro_puerta=0.0;
  giro_puerta_max= 0.0;
  giro_puerta_min=-90.0;

  giro_ventana=0.0;
  giro_ventana_max=0.0;
  giro_ventana_min=-90.0;

  giro_placa_solar=0.0;
  giro_placa_solar_max=0.0;
  giro_placa_solar_min=-45.0;

  giro_pomo= 0.0;
  giro_pomo_max= 50.0;
  giro_pomo_min= 0.0;

  int color=85;
  piezas=7;
  grosor_select=2;
  color_pick=_vertex3f(1.0,0.0,0.0); 
  color_select.resize(piezas);
  activo.resize(piezas);

  for (int i=0;i<piezas;i++)
    {activo[i]=0;
    color_select[i].r=color_select[i].g=color_select[i].b=color;
    color+=20;
    }

};

void _casa_moderna::draw(_modo modo, float r, float g, float b, float grosor){
  float r_p,g_p,b_p;
  int tam=2;

  r_p=color_pick.r;
  g_p=color_pick.g;
  b_p=color_pick.b;

  if (activo[0]==1) fachada.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else fachada.draw(modo, 0.9, 0.8, 0.7, grosor);

  glPushMatrix();
  glTranslatef(0.05, 0.475, 0.15);
  if (activo[1]==1) placa_solar.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else placa_solar.draw(modo, 0.5, 0.5, 0.5, grosor);//gris
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5, 0.525 , 0.15);  //0.05, 0.525 , 0.15  sin rotacion
  glRotatef(giro_placa_solar,0,0,1);
  glTranslatef(-0.45, 0.0, 0.0);
  if (activo[2]==1) placa_solar.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else placa_solar.draw(modo, 0.0, 0.0, 0.5, grosor);//azul oscuro
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.5, 0.55, -0.5);
  if (activo[3]==1) chimenea.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else chimenea.draw(modo, 0.5, 0.0, 0.0, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.2, -0.25, 0.7); //0.0, -0.25, 0.7  sin rotacion
  glRotatef(giro_puerta,0,1,0);
  glTranslatef(0.2, 0.0, 0.0);
  if (activo[4]==1) puerta.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else puerta.draw(modo, 0.6, 0.3, 0.0, grosor);
  glTranslatef(0.07, 0.0, pomo.radio);
  glRotatef(giro_pomo,0,0,1);     // no lo roto al centro para que gire sobre si mismo
  if (activo[5]==1) pomo.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else pomo.draw(modo, 1.0, 0.84, 0.0, grosor);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.7, 0.35, 0.0);  //0.7, 0.15, 0.0  sin rotacion
  glRotatef(90,0,1,0);
  glRotatef(giro_ventana,1,0,0);
  glTranslatef(0.0, -0.20, 0.0);
  if (activo[6]==1) ventana.draw(EDGES, r_p, g_p, b_p, grosor_select);
  else ventana.draw(modo, 0.6, 0.8, 1.0, grosor);
  glPopMatrix();


};

//************************************************************************

void _casa_moderna::seleccion()
{

  _vertex3i color;

  color=color_select[0];
  fachada.draw(SELECT, color.r, color.g, color.b, 1);

  glPushMatrix();
  glTranslatef(0.05, 0.475, 0.15);
  color=color_select[1];
  placa_solar.draw(SELECT, color.r, color.g, color.b, 1);//gris
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5, 0.525 , 0.15);  //0.05, 0.525 , 0.15  sin rotacion
  glRotatef(giro_placa_solar,0,0,1);
  glTranslatef(-0.45, 0.0, 0.0);
  color=color_select[2];
  placa_solar.draw(SELECT, color.r, color.g, color.b, 1);//azul oscuro
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.5, 0.55, -0.5);
  color=color_select[3];
  chimenea.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.2, -0.25, 0.7); //0.0, -0.25, 0.7  sin rotacion
  glRotatef(giro_puerta,0,1,0);
  glTranslatef(0.2, 0.0, 0.0);
  color=color_select[4];
  puerta.draw(SELECT, color.r, color.g, color.b, 1);
  glTranslatef(0.07, 0.0, pomo.radio);
  glRotatef(giro_pomo,0,0,1);     // no lo roto al centro para que gire sobre si mismo
  color=color_select[5];
  pomo.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();
  
  glPushMatrix();
  glTranslatef(0.7, 0.35, 0.0);  //0.7, 0.15, 0.0  sin rotacion
  glRotatef(90,0,1,0);
  glRotatef(giro_ventana,1,0,0);
  glTranslatef(0.0, -0.20, 0.0);
  color=color_select[6];
  ventana.draw(SELECT, color.r, color.g, color.b, 1);
  glPopMatrix();


};
