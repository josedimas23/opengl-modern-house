.PHONY: x clean

EJECUTABLE= practica_objetos_B3

MODULOS:= practica_objetos_B3.o\
         objetos_B3.o\
         file_ply_stl.o

CPPFLAGS:= -Wall -g
CC= g++

ifeq ($(shell uname -s),Darwin)
  # macOS: OpenGL/GLUT del sistema + libjpeg (brew install jpeg-turbo)
  JPEG_PREFIX:= $(shell brew --prefix jpeg-turbo 2>/dev/null)
  CPPFLAGS+= -DGL_SILENCE_DEPRECATION -I$(JPEG_PREFIX)/include
  LIBS:= -framework OpenGL -framework GLUT -L$(JPEG_PREFIX)/lib -ljpeg -lm -lpthread
else
  # Linux: freeglut + Mesa/X11 + libjpeg
  CPPFLAGS+= -fmax-errors=2
  LIBS:= -lglut -lGLU -lGL -lX11 -ljpeg -lm -lpthread
endif

x: $(EJECUTABLE)
	./$(EJECUTABLE) beethoven peonza

$(EJECUTABLE): $(MODULOS)
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS)

clean:
	rm -rf *.o $(EJECUTABLE)
