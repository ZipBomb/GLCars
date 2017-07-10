struct Punto {
	float x, y, z;
	Punto(float a, float b, float c) : x(a), y(b), z(c) {}
};

struct Coche {
	Punto posicion;
	float ang, tiempo;
	GLuint textura, id;
	char *path;
	Coche(Punto a, float b, float c, GLuint d, GLuint e, char *f) : posicion(a), ang(b), tiempo(c), textura(d), id(e), path(f) {}
};

struct Pelota {
	Punto posicion;
	float ang, tiempo;
	GLuint textura, id;
	char *path;
	Pelota(Punto a, float b, float c, GLuint d, GLuint e, char *f) : posicion(a), ang(b), tiempo(c), textura(d), id(e), path(f) {}
};

double toRad(float x);
void myTeclado(unsigned char tras, int x, int y);
void keyboardUp(unsigned char tras, int x, int y);
void cubo(float lado);
void muro(float lado);
void dibuja_cara(Punto a, Punto b, Punto c, Punto d);
void dibuja_suelo(GLuint textura);
void dibuja_carretera(GLuint textura);
void dibuja_estadio(GLuint textura);
void dibuja_porteria(float R, float G, float B);
void dibuja_skybox(GLuint skybox_textura[5]);
void dibuja_coche(Coche coche, float R, float G, float B);
void dibuja_pelota(Pelota pelota);
void dibuja_ejes();
void output(int x, int y, char *string);
int devuelve_accion_A();
int devuelve_accion_B();
int devuelve_camara();
int devuelve_luces();
int devuelve_niebla();