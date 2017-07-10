#include <windows.h>
#include <GLUT.h>
#include <stdio.h>
#include <math.h>
#include "esfera.h"

#define PI 3.1415926
#define DISTANCIA 10

int accion_A = -1;
int accion_B = -1;
int luces = 0;
int niebla = 0;
int cam = 0;

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

double toRad(float x) {
	return x * PI / 180;
}

void myTeclado(unsigned char tras, int x, int y)
{
	switch (tras) {
	case 'w':
		accion_A = 0;
		break;
	case 'a':
		accion_A = 1;
		break;
	case 'd':
		accion_A = 2;
		break;
	case 's':
		accion_A = 3;
		break;
	case 'i':
		accion_B = 0;
		break;
	case 'j':
		accion_B = 1;
		break;
	case 'l':
		accion_B = 2;
		break;
	case 'k':
		accion_B = 3;
		break;
	case 'v':
		cam = (cam + 1) % 3;
		break;
	case 'b':
		luces = (luces + 1) % 2;
		break;
	case 'n':
		niebla = (niebla + 1) % 2;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void keyboardUp(unsigned char tras, int x, int y) {
	switch (tras) {
	case 'a':
		accion_A = 4;
		break;
	case 'd':
		accion_A = 5;
		break;
	case 'j':
		accion_B = 4;
		break;
	case 'l':
		accion_B = 5;
		break;
	}
}

int devuelve_accion_A() {
	return accion_A;
}

int devuelve_accion_B() {
	return accion_B;
}

int devuelve_camara() {
	return cam;
}

int devuelve_luces() {
	return luces;
}

int devuelve_niebla() {
	return niebla;
}

void cubo(float lado) {
	glScalef(lado, lado, lado);
	glBegin(GL_QUADS);
		// Cara frontal
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		// Cara trasera
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		// Cara superior
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		// Cara inferior
		glNormal3f(0.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		// Cara derecha
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		// Cara izquierda
		glNormal3f(-1.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

void dibuja_coche(Coche coche, float R, float G, float B) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, coche.textura);
		cubo(30.0f);
		glBindTexture(GL_TEXTURE_2D, NULL);
	glPopMatrix();
	glTranslatef(0.0f, 40.0f, 0.0f);
	glColor3f(R, G, B);
	glutSolidCube(20.0f);
}

void dibuja_pelota(Pelota pelota) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, pelota.textura);
	esfera(60.0f);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void muro(float lado) {
	glTranslatef(0.0f, 20.0f, 0.0f);
	glScalef(lado, 40.0f, lado);
	glBegin(GL_QUADS);
		// Cara frontal
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		// Cara trasera
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		// Cara superior
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		// Cara inferior
		glNormal3f(0.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		// Cara derecha
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
		// Cara izquierda
		glNormal3f(-1.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

void dibuja_cara(Punto a, Punto b, Punto c, Punto d) {
	glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(a.x, a.y, a.z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(d.x, d.y, d.z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(c.x, c.y, c.z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(b.x, b.y, b.z);
	glEnd();
}

void dibuja_suelo(GLuint textura) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textura);
	int i, j;
	for (i = -2400; i <= 3200; i += 160)
		for (j = -4200; j <= 2800; j += 160) {
			float aux_i = float(i);
			float aux_j = float(j);
			Punto a = { aux_i, 0.0f, aux_j };
			Punto b = { aux_i + 160.0f, 0.0f, aux_j };
			Punto c = { aux_i + 160.0f, 0.0f, aux_j + 160.0f };
			Punto d = { aux_i, 0.0f, aux_j + 160.0f };
			dibuja_cara(a, b, c, d);
		}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void dibuja_carretera(GLuint textura) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textura);
	int i, j;
	for (i = -800; i <= 800; i += 120)
		for (j = -2600; j <= 800; j += 120) {
			float aux_i = float(i);
			float aux_j = float(j);
			Punto a = { aux_i, 0.0f, aux_j };
			Punto b = { aux_i + 120.0f, 0.0f, aux_j };
			Punto c = { aux_i + 120.0f, 0.0f, aux_j + 120.0f };
			Punto d = { aux_i, 0.0f, aux_j + 120.0f };
			dibuja_cara(a, b, c, d);
		}
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void dibuja_estadio(GLuint textura) {
	int i;
	// FONDO Z
	for (i = -840; i <= -360; i += 80) {
		glPushMatrix();
			glTranslatef(i, 20.0f, 920);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();
	}

	for (i = 360; i <= 960; i += 80) {
		glPushMatrix();
			glTranslatef(i, 20.0f, 920);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();
	}

	// FONDO -Z
	for (i = -840; i <= -360; i += 80) {
		glPushMatrix();
			glTranslatef(i, 20.0f, -2640);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();
	}

	for (i = 360; i <= 960; i += 80) {
		glPushMatrix();
			glTranslatef(i, 20.0f, -2640);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();
	}

	// LATERALES
	for (i = 840; i >= -2600; i -= 80) {
		glPushMatrix();
			glTranslatef(-840.0f, 20.0f, i);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(920.0f, 20.0f, i);
			glBindTexture(GL_TEXTURE_2D, textura);
			muro(40.0f);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glPopMatrix();
	}

}

void dibuja_porteria(float R, float G, float B) {
	int i;
	// PORTERIA FONDO Z
	for (i = -280; i <= 280; i += 40) {
		glPushMatrix();
			glTranslatef(i, 40.0f, 920);
			glColor3f(R, G, B);
			glutSolidCube(80.0f);
		glPopMatrix();
	}

	// PORTERIA FONDO -Z
	for (i = -280; i <= 280; i += 40) {
		glPushMatrix();
			glTranslatef(i, 40.0f, -2640);
			glColor3f(R, G, B);
			glutSolidCube(80.0f);
		glPopMatrix();
	}
}

void dibuja_skybox(GLuint textura[5]) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
		glScalef(3200.0f, 1600.0f, 3200.0f);
		glBindTexture(GL_TEXTURE_2D, textura[0]);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textura[1]);
		glBegin(GL_QUADS);
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textura[2]);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textura[3]);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, textura[4]);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
		glEnd();
	glPopMatrix();
}

void dibuja_ejes() {
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 20.0f, 0.0f);
		glVertex3f(100.0f, 20.0f, 0.0f);

		glVertex3f(0.0f, 20.0f, 0.0f);
		glVertex3f(0.0f, 120.0f, 0.0f);

		glVertex3f(0.0f, 20.0f, 0.0f);
		glVertex3f(0.0f, 20.0f, 100.0f);
	glEnd();
}

void output(int x, int y, char *string)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glRasterPos2f(x, y);
	int len, i;
	len = (int)strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
}