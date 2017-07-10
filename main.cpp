#include <windows.h>
#include <GLUT.h>
#include <SOIL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "auxiliar.h"

#define W_WIDTH 1024
#define W_HEIGHT 768
#define aceleracion 0.001
#define frenado 0.001

int w_pressed_A = 0;
int s_pressed_A = 0;
int a_pressed_A = 0;
int d_pressed_A = 0;

int w_pressed_B = 0;
int s_pressed_B = 0;
int a_pressed_B = 0;
int d_pressed_B = 0;

int estado_A, estado_B, estado_pelota;
int camara;

Punto pos_inicial_A = { 0.0f, 0.0f, -2460.0f };
float ang_inicial_A = 0.0f;

Punto pos_inicial_B = { 0.0f, 0.0f, 700.0f };
float ang_inicial_B = 180.0f;

Punto pos_inicial_pelota = { 0.0f, 0.0f, -900.0f };
float ang_inicial_pelota = 0.0f;

float R = 1.0f;
float G = 1.0f;
float B = 1.0f;

unsigned int marcador_A = 0;
unsigned int marcador_B = 0;
char s[32];

static struct Coche coche_A = { pos_inicial_A, ang_inicial_A, 0.0f, (GLuint)-1, (GLuint)-1, "texturas/car.bmp" }; // Posición, Ángulo, Textura, ID, path
static struct Coche coche_B = { pos_inicial_B, ang_inicial_B, 0.0f, (GLuint)-1, (GLuint)-1, "texturas/car.bmp" }; // Posición, Ángulo, Textura, ID, path
static struct Pelota pelota = { pos_inicial_pelota, ang_inicial_pelota, 0.0f, (GLuint)-1, (GLuint)-1, "texturas/ball.jpg" }; // Posición, Ángulo, Textura, ID, path

GLuint cesped_textura = (GLuint)-1;
GLuint cesped_id = (GLuint)-1;
GLuint carretera_textura = (GLuint)-1;
GLuint carretera_id = (GLuint)-1;
GLuint estadio_textura = (GLuint)-1;
GLuint estadio_id = (GLuint)-1;
GLuint skybox_textura[6];
GLuint skybox_id = (GLuint)-1;

char *skybox_path[5] = { "texturas/skybox/right.jpg", "texturas/skybox/left.jpg", "texturas/skybox/top.jpg", "texturas/skybox/back.jpg", "texturas/skybox/front.jpg" };
char *cesped_path = "texturas/grass.bmp";
char *carretera_path = "texturas/road.jpg";
char *estadio_path = "texturas/concrete.jpg";

void init() {
	estado_A = -1;
	estado_B = -1;
	camara = 0;

	int i;

	coche_A.textura = SOIL_load_OGL_texture(coche_A.path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	coche_A.id = glGenLists(1);
	glNewList(coche_A.id, GL_COMPILE);
		dibuja_coche(coche_A, 1.0f, 0.0f, 0.0f);
	glEndList();

	coche_B.textura = SOIL_load_OGL_texture(coche_B.path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	coche_B.id = glGenLists(1);
	glNewList(coche_B.id, GL_COMPILE);
		dibuja_coche(coche_B, 0.0f, 0.0f, 1.0f);
	glEndList();

	pelota.textura = SOIL_load_OGL_texture(pelota.path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	pelota.id = glGenLists(1);
	glNewList(pelota.id, GL_COMPILE);
		dibuja_pelota(pelota);
	glEndList();

	cesped_textura = SOIL_load_OGL_texture(cesped_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	cesped_id = glGenLists(1);
	glNewList(cesped_id, GL_COMPILE);
		dibuja_suelo(cesped_textura);
	glEndList();

	carretera_textura = SOIL_load_OGL_texture(carretera_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	carretera_id = glGenLists(1);
	glNewList(carretera_id, GL_COMPILE);
		dibuja_carretera(carretera_textura);
	glEndList();

	estadio_textura = SOIL_load_OGL_texture(estadio_path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	estadio_id = glGenLists(1);
	glNewList(estadio_id, GL_COMPILE);
		dibuja_estadio(estadio_textura);
	glEndList();

	for (i = 0; i < 5; i++) skybox_textura[i] = SOIL_load_OGL_texture(skybox_path[i], SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	skybox_id = glGenLists(1);
	glNewList(skybox_id, GL_COMPILE);
		dibuja_skybox(skybox_textura);
	glEndList();

	snprintf(s, sizeof(s), "ROJO %d - AZUL %d", marcador_A, marcador_B);
}

void acelerar_A() {
	float desplazamiento = 0.5 * aceleracion * pow(coche_A.tiempo, 2);
	coche_A.posicion.x += (float)(sin(toRad(coche_A.ang)) * desplazamiento);
	coche_A.posicion.z += (float)(cos(toRad(coche_A.ang)) * desplazamiento);
}

void frenar_A() {
	float desplazamiento = 0.5 * frenado * pow(coche_A.tiempo, 2);
	coche_A.posicion.x += (float)(sin(toRad(coche_A.ang)) * desplazamiento);
	coche_A.posicion.z += (float)(cos(toRad(coche_A.ang)) * desplazamiento);
}

void girarDerecha_A() {
	coche_A.ang -= 0.4;
	if (coche_A.ang < 0.0f)
		coche_A.ang += 360.0f;
}

void girarIzquierda_A() {
	coche_A.ang += 0.4;
	if (coche_A.ang > 360.0f)
		coche_A.ang -= 360.0f;
}

void acelerar_B() {
	float desplazamiento = 0.5 * aceleracion * pow(coche_B.tiempo, 2);
	coche_B.posicion.x += (float)(sin(toRad(coche_B.ang)) * desplazamiento);
	coche_B.posicion.z += (float)(cos(toRad(coche_B.ang)) * desplazamiento);
}

void frenar_B() {
	float desplazamiento = 0.5 * frenado * pow(coche_B.tiempo, 2);
	coche_B.posicion.x += (float)(sin(toRad(coche_B.ang)) * desplazamiento);
	coche_B.posicion.z += (float)(cos(toRad(coche_B.ang)) * desplazamiento);
}

void girarDerecha_B() {
	coche_B.ang -= 0.4;
	if (coche_B.ang < 0.0f)
		coche_B.ang += 360.0f;
}

void girarIzquierda_B() {
	coche_B.ang += 0.4;
	if (coche_B.ang > 360.0f)
		coche_B.ang -= 360.0f;
}

void acelerar_pelota() {
	float desplazamiento = 0.5 * aceleracion * pow(pelota.tiempo, 2);
	pelota.posicion.x += (float)(sin(toRad(pelota.ang)) * desplazamiento);
	pelota.posicion.z += (float)(cos(toRad(pelota.ang)) * desplazamiento);
}

void actualizacion() {
	if (w_pressed_A) {
		coche_A.tiempo += 0.05f;
		acelerar_A();
		if (coche_A.tiempo >= 60.0f) {
			coche_A.tiempo = 60.0f;
		}
	}
	else if (s_pressed_A) {
		coche_A.tiempo -= 0.01f;
		frenar_A();
		if (coche_A.tiempo <= 0.0f) {
			coche_A.tiempo = 0.0f;
		}
	}
	if (d_pressed_A)
		girarDerecha_A();
	if (a_pressed_A)
		girarIzquierda_A();

	if (w_pressed_B) {
		coche_B.tiempo += 0.05f;
		acelerar_B();
		if (coche_B.tiempo >= 60.0f) {
			coche_B.tiempo = 60.0f;
		}
	}
	else if (s_pressed_B) {
		coche_B.tiempo -= 0.01f;
		frenar_B();
		if (coche_B.tiempo <= 0.0f) {
			coche_B.tiempo = 0.0f;
		}
	}
	if (d_pressed_B)
		girarDerecha_B();
	if (a_pressed_B)
		girarIzquierda_B();

	// ROZAMIENTO
	acelerar_pelota();
	if (pelota.tiempo >= 80.0f)
		pelota.tiempo = 80.0f;
	pelota.tiempo -= 0.04f;
	if (pelota.tiempo <= 0.0f)
		pelota.tiempo = 0.0f;

	glutPostRedisplay();
}

boolean compruebaColisionCuerpos(float x1, float z1, float x2, float z2, float radio) {
	boolean control;
	float dist = 50.0f;
	if ((fabs(x1 - x2) < dist + radio) && (fabs(z1 - z2) < dist + radio))
		control = true;
	else
		control = false;
	return control;
}

boolean compruebaGol() {
	float dist_pelota = 100.0f;
	return((pelota.posicion.z + dist_pelota >= 920.0f && pelota.posicion.x >= -280.0f && pelota.posicion.x <= 280.0f) ||
		   (pelota.posicion.z - dist_pelota <= -2640.0f && pelota.posicion.x >= -280.0f && pelota.posicion.x <= 280.0f));
}

boolean controlColisionFondo[3];

void compruebaColisionFondo() {
	float dist_coche = 70.0f;
	float dist_pelota = 100.0f;
	(coche_A.posicion.x - dist_coche <= -840.0f || coche_A.posicion.x + dist_coche >= 920.0f || coche_A.posicion.z + dist_coche >= 920.0f || coche_A.posicion.z - dist_coche <= -2640.0f) ? controlColisionFondo[0] = true : controlColisionFondo[0] = false;
	(coche_B.posicion.x - dist_coche <= -840.0f || coche_B.posicion.x + dist_coche >= 920.0f || coche_B.posicion.z + dist_coche >= 920.0f || coche_B.posicion.z - dist_coche <= -2640.0f) ? controlColisionFondo[1] = true : controlColisionFondo[1] = false;
	(pelota.posicion.x - dist_pelota <= -840.0f || pelota.posicion.x + dist_pelota >= 920.0f || pelota.posicion.z + dist_pelota >= 920.0f || pelota.posicion.z - dist_pelota <= -2640.0f) ? controlColisionFondo[2] = true : controlColisionFondo[2] = false;
}

// Función de display
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camara = devuelve_camara();
	
	switch (camara) {
		// Cámara exterior
		case 0: { gluLookAt(-3000.0f, 2200.0f, -880.0f - 200.0f,
				  0.0f, 0.0f, -880.0f - 200.0f,
				  0.0f, 1.0f, 0.0f); break; }
		// Primera persona
		case 1: { gluLookAt(coche_A.posicion.x + (float)(sin(toRad(coche_A.ang)) * 45), 60.0f, coche_A.posicion.z + (float)(cos(toRad(coche_A.ang)) * 45) - 200.0f,
				  coche_A.posicion.x + (float)(sin(toRad(coche_A.ang)) * 45) + sin(toRad(coche_A.ang)), 60.0f, coche_A.posicion.z + (float)(cos(toRad(coche_A.ang)) * 45) - 200.0f + (float)cos(toRad(coche_A.ang)),
				  0.0f, 1.0f, 0.0f); break; }
		// Tercera persona
		case 2: { gluLookAt(coche_A.posicion.x - (float)(sin(toRad(coche_A.ang)) * 150.0f), 120.0f, coche_A.posicion.z + (-1)*(400.0f + (float)((cos(toRad(coche_A.ang)) * 150.0f) - 200.0f)) - 20.0f,
				  coche_A.posicion.x + (float)(sin(toRad(coche_A.ang)) * 150.0f), 5.0, coche_A.posicion.z + (float)((cos(toRad(coche_A.ang)) * 150.0f) - 200.0f),
				  0, 1, 0); break; }
	}

	estado_A = devuelve_accion_A();
	estado_B = devuelve_accion_B();

	switch (estado_A) {
		case 0: { w_pressed_A = 1; s_pressed_A = 0; break; }
		case 1: { a_pressed_A = 1; d_pressed_A = 0; break; }
		case 2: { d_pressed_A = 1; a_pressed_A = 0; break; }
		case 3: { s_pressed_A = 1; w_pressed_A = 0; break; }
		case 4: { a_pressed_A = 0; break; }
		case 5: { d_pressed_A = 0; break; }
		default: break;
	}

	switch (estado_B) {
		case 0: { w_pressed_B = 1; s_pressed_B = 0; break; }
		case 1: { a_pressed_B = 1; d_pressed_B = 0; break; }
		case 2: { d_pressed_B = 1; a_pressed_B = 0; break; }
		case 3: { s_pressed_B = 1; w_pressed_B = 0; break; }
		case 4: { a_pressed_B = 0; break; }
		case 5: { d_pressed_B = 0; break; }
		default: break;
	}

	// RESOLUCIÓN COLISIONES
	if (compruebaGol()) {
		if (pelota.posicion.z + 100.0f >= 920.0f) {
			R = 1.0f; G = 0.0f; B = 0.0f;
			marcador_A++;
			if (marcador_A == 4) {
				snprintf(s, sizeof(s), "GANA EL JUGADOR ROJO");
				marcador_A = 0; marcador_B = 0;
			}
			else 
				snprintf(s, sizeof(s), "ROJO %d - AZUL %d", marcador_A, marcador_B);
		}
		else {
			R = 0.0f; G = 0.0f; B = 1.0f;
			marcador_B++;
			if (marcador_B == 4) {
				snprintf(s, sizeof(s), "GANA EL JUGADOR AZUL");
				marcador_B = 0; marcador_A = 0;
			}
			else
				snprintf(s, sizeof(s), "ROJO %d - AZUL %d", marcador_A, marcador_B);
		}
	}

	compruebaColisionFondo();

	if (controlColisionFondo[0]) {
		float dist = 70.0f;
		if (coche_A.posicion.z + dist >= 920.0f) {
			coche_A.ang = 180.0f - coche_A.ang;
			coche_A.posicion.z = 850.0f;
		}
		else if (coche_A.posicion.z - dist <= -2640.0f) {
			coche_A.ang = 180.0f - coche_A.ang;
			coche_A.posicion.z = -2570.0f;
		}
		else if (coche_A.posicion.x + dist >= 920.0f) {
			coche_A.ang = 360.0f - coche_A.ang;
			coche_A.posicion.x = 850.0f;
		}
		else if (coche_A.posicion.x - dist <= -840.0f) {
			coche_A.ang = 360.0f - coche_A.ang;
			coche_A.posicion.x = -770.0f;
		}
	}

	if (controlColisionFondo[1]) {
		float dist = 70.0f;
		if (coche_B.posicion.z + dist >= 920.0f) {
			coche_B.ang = 180.0f - coche_B.ang;
			coche_B.posicion.z = 850.0f;
		}
		else if (coche_B.posicion.z - dist <= -2640.0f) {
			coche_B.ang = 180.0f - coche_B.ang;
			coche_B.posicion.z = -2570.0f;
		}
		else if (coche_B.posicion.x + dist >= 920.0f) {
			coche_B.ang = 360.0f - coche_B.ang;
			coche_B.posicion.x = 850.0f;
		}
		else if (coche_B.posicion.x - dist <= -840.0f) {
			coche_B.ang = 360.0f - coche_B.ang;
			coche_B.posicion.x = -770.0f;
		}
	}

	if (controlColisionFondo[2]) {
		float dist = 100.0f;
		if (pelota.posicion.z + dist >= 920.0f) {
			pelota.ang = 180.0f - pelota.ang;
			pelota.posicion.z = 820.0f;
			pelota.tiempo -= 10;
		}
		else if (pelota.posicion.z - dist <= -2640.0f) {
			pelota.ang = 180.0f - pelota.ang;
			pelota.posicion.z = -2540.0f;
			pelota.tiempo -= 10;
		}
		else if (pelota.posicion.x + dist >= 920.0f) {
			pelota.ang = 360.0f - pelota.ang;
			pelota.posicion.x = 820.0f;
			pelota.tiempo -= 10;
		}
		else if (pelota.posicion.x - dist <= -840.0f) {
			pelota.ang = 360.0f - pelota.ang;
			pelota.posicion.x = -740.0f;
			pelota.tiempo -= 10;
		}
	}

	if (compruebaColisionCuerpos(coche_A.posicion.x, coche_A.posicion.z, coche_B.posicion.x, coche_B.posicion.z, 15.0f)) {
		int i;
		if (coche_A.tiempo >= coche_B.tiempo) {
			coche_B.ang = coche_A.ang;
			coche_B.tiempo += coche_A.tiempo / 2;
			coche_A.tiempo = coche_A.tiempo / 1.2;
			for (i = 0; i < 10; i++)
				acelerar_B();
		}
		else {
			coche_A.ang = coche_B.ang;
			coche_A.tiempo += coche_B.tiempo / 2;
			coche_B.tiempo = coche_B.tiempo / 1.2;
			for (i = 0; i < 10; i++)
				acelerar_A();
		}
	}

	if (compruebaColisionCuerpos(coche_A.posicion.x, coche_A.posicion.z, pelota.posicion.x, pelota.posicion.z, 40.0f)) {
		pelota.ang = coche_A.ang;
		pelota.tiempo += coche_A.tiempo * 1.5;
		acelerar_pelota();
	}

	if (compruebaColisionCuerpos(coche_B.posicion.x, coche_B.posicion.z, pelota.posicion.x, pelota.posicion.z, 40.0f)) {
		pelota.ang = coche_B.ang;
		pelota.tiempo += coche_B.tiempo * 1.5;
		acelerar_pelota();
	}

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, -200.0f);

		glPushMatrix();
			glTranslatef(0.0f, 850.0f, -1080.0f);
			output(0.0f, 0.0f, s);
		glPopMatrix();

		// COCHE_A
		glPushMatrix();
			glTranslatef(coche_A.posicion.x, coche_A.posicion.y + 28.0f, coche_A.posicion.z);
			glRotatef(coche_A.ang, 0, 1, 0);
			glPushMatrix();
				glCallList(coche_A.id);
			glPopMatrix();
		glPopMatrix();

		// COCHE_B
		glPushMatrix();
			glTranslatef(coche_B.posicion.x, coche_B.posicion.y + 28.0f, coche_B.posicion.z);
			glRotatef(coche_B.ang, 0, 1, 0);
			glPushMatrix();
				glCallList(coche_B.id);
			glPopMatrix();
		glPopMatrix();

		// PELOTA
		glPushMatrix();
			glTranslatef(pelota.posicion.x, pelota.posicion.y + 60.0f, pelota.posicion.z);
			glRotatef(pelota.ang, 0, 1, 0);
			glPushMatrix();
				glCallList(pelota.id);
			glPopMatrix();
		glPopMatrix();

		// SKYBOX
		glPushMatrix();
			glTranslatef(-600.0f, 50.0f, -900.0f);
			glCallList(skybox_id);
		glPopMatrix();

		// CESPED
		glPushMatrix();
			glTranslatef(0.0f, 2.0f, 0.0f);
			glCallList(cesped_id);
		glPopMatrix();

		// CARRETERA
		glPushMatrix();
			glTranslatef(0.0f, 5.0f, 0.0f);
			glCallList(carretera_id);
		glPopMatrix();

		// ESTADIO
		glPushMatrix();
			glCallList(estadio_id);
			dibuja_porteria(R, G, B);
		glPopMatrix();

	glPopMatrix();

	glutSwapBuffers();
	glFlush();

	if (compruebaGol()) {
		Sleep(1000);
		coche_A.posicion = pos_inicial_A;
		coche_A.ang = ang_inicial_A;
		coche_A.tiempo = 0.0f;
		coche_B.posicion = pos_inicial_B;
		coche_B.ang = ang_inicial_B;
		coche_B.tiempo = 0.0f;
		w_pressed_A = 0; a_pressed_A = 0; s_pressed_A = 0; d_pressed_A = 0;
		w_pressed_B = 0; a_pressed_B = 0; s_pressed_B = 0; d_pressed_B = 0;
		pelota.posicion = pos_inicial_pelota;
		pelota.ang = ang_inicial_pelota;
		pelota.tiempo = 0.0f;
		R = 1.0f; G = 1.0f; B = 1.0f;
		snprintf(s, sizeof(s), "ROJO %d - AZUL %d", marcador_A, marcador_B);
	}
}

void reshape(int w, int h) {
	GLfloat formato = (GLfloat)w / (GLfloat)h;

	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, formato, 1.0f, 10000.0f);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Coches-Fútbol en OpenGL");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(myTeclado);
	glutKeyboardUpFunc(keyboardUp);
	glutReshapeFunc(reshape);
	glutIdleFunc(actualizacion);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glutMainLoop();

	return 0;
}