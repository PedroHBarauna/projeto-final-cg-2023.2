#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

float thetaX = 0.0;
float thetaY = 0.0;
int girando = 0;
float moveZ = 0.0, moveX = 0.0;
float angulo, X = 0.0, Z = 0.0;
float angX = 0.0;
float angBraco = 0.0;
float inc = 1.0;

void chao()
{
    glPushMatrix();
    glTranslatef(0.0, -0.5, 0.0);
    glScalef(3.5, 0.5, 3.5);

    glColor3f(0.5, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(-0.25, -0.25, 0.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, 0.25, -0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(-0.25, 0.25, 0.25);
    glVertex3f(-0.25, 0.25, -0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, 0.25);
    glVertex3f(0.25, 0.25, -0.25);
    glVertex3f(-0.25, 0.25, -0.25);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glEnd();
    glPopMatrix();
}

void lightning()
{
    GLfloat light0_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

    // Atenuação radial
    // glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);   //define a0
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.15f);    //define a1
    // glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1f);  //define a2

    // Fonte de Luz Direcional - Efeito de Holofote
    GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    GLfloat direction[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction); // vetor direção
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);         // espalhamento angular
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.1f);        // atenuação angular

    // Parâmetros definidos globalmente
    // GLfloat global_ambient[] = {0.9f, 0.9f, 0.9f, 1.0f};
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    ////Reflexão especular definida como a posição de visão corrente
    // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    ////Habilitar cálculos de iluminação para ambas as faces dos polígonos
    // glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void init(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // define a cor de fundo
    glEnable(GL_DEPTH_TEST);              // habilita o teste de profundidade

    // glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_MODELVIEW); // define que a matrix é a model view
    glLoadIdentity();           // carrega a matrix de identidade
    gluLookAt(0.0, 1.0, 3.0,    // posição da câmera
              0.0, 0.0, 0.0,    // para onde a câmera aponta
              0.0, 1.0, 0.0);   // vetor view-up
                                // vetor view-up

    glMatrixMode(GL_PROJECTION); // define que a matrix é a de projeção
    glLoadIdentity();            // carrega a matrix de identidade
    // glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
    gluPerspective(45.0, 1.0, 2.0, 8.0); // define uma projeção perspectiva
    // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva simétrica
    // glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva obliqua
    glViewport(0, 0, 500, 500);

    lightning();
}

void corpo()
{
    glPushMatrix();
    glScalef(0.5, 1.0, 0.35);

    // Esfera esticada para simular o corpo de Fall Guys
    glColor3f(1.0, 1.0, 0.0); // Cor amarela
    glutSolidSphere(0.3, 20, 20);

    glPopMatrix();
}

void cabeca()
{
    glPushMatrix();
    glTranslatef(0.0, 0.10, 0.03);
    glScalef(0.4, 0.5, 0.4);

    // Desenhar uma esfera para a cabeça
    glColor3f(1.0, 1.0, 1.0); // Cor branca
    glutSolidSphere(0.25, 20, 20);

    glPopMatrix();
}

void OlhoD()
{
    // Desenhar olho direito
    glPushMatrix();
    glTranslatef(0.03, 0.1, 0.1);  // Posição do olho direito
    glScalef(0.8, 1.5, 1.0);       // Proporção de escala vertical (aumentei para 1.5)
    glColor3f(0.0, 0.0, 0.0);      // Cor preta
    glutSolidSphere(0.03, 10, 10); // Reduzi o raio para 0.03
    glPopMatrix();
}

void OlhoE()
{
    // Desenhar olho esquerdo
    glPushMatrix();
    glTranslatef(-0.03, 0.1, 0.1); // Posição do olho esquerdo
    glScalef(0.8, 1.5, 1.0);       // Proporção de escala vertical (aumentei para 1.5)
    glColor3f(0.0, 0.0, 0.0);      // Cor preta
    glutSolidSphere(0.03, 10, 10); // Reduzi o raio para 0.03
    glPopMatrix();
}

void bracoDireito()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(0.15, 0.02, 0.0);  // Ajuste do deslocamento lateral
    glRotatef(45.0, 0.0, 0.0, 1.0); // Rotação para inclinar o braço
    glRotatef(thetaX, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.26, 0.06);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void bracoEsquerdo()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glTranslatef(-0.15, 0.02, 0.0);  // Ajuste do deslocamento lateral
    glRotatef(-45.0, 0.0, 0.0, 1.0); // Rotação para inclinar o braço
    glRotatef(-thetaX, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.26, 0.06);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void pernaDireita()
{
    glPushMatrix();
    glTranslatef(-0.05, -0.25, 0.0); // Ajuste da posição da perna
    glRotatef(-thetaX, 1.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);     // Cor
    glScalef(0.04, 0.13, 0.06);   // Ajuste da escala para uma esfera mais alongada
    glutSolidSphere(1.0, 20, 20); // Substituição do cubo por uma esfera
    glPopMatrix();
}

void pernaEsquerda()
{
    glPushMatrix();
    glTranslatef(0.05, -0.25, 0.0); // Ajuste da posição da perna
    glRotatef(thetaX, 1.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);     // Cor
    glScalef(0.04, 0.13, 0.06);   // Ajuste da escala para uma esfera mais alongada
    glutSolidSphere(1.0, 20, 20); // Substituição do cubo por uma esfera
    glPopMatrix();
}

void peDireito()
{
    glPushMatrix();
    glTranslatef(-0.05, -0.35, 0.02);
    glRotatef(thetaX, 1.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);     // Cor
    glScalef(0.02, 0.03, 0.07);   // Ajuste da escala para uma esfera mais pequena
    glutSolidSphere(1.0, 20, 20); // Desenhar pé direito
    glPopMatrix();
}

void peEsquerdo()
{
    glPushMatrix();
    glTranslatef(0.05, -0.35, 0.02); // Ajuste da posição do pé esquerdo
    glRotatef(thetaX, 1.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 0.0);     // Cor
    glScalef(0.02, 0.03, 0.07);   // Ajuste da escala para uma esfera mais pequena
    glutSolidSphere(1.0, 20, 20); // Desenhar pé esquerdo
    glPopMatrix();
}

void robo()
{
    cabeca();
    corpo();
    OlhoE();
    OlhoD();
    bracoDireito();
    bracoEsquerdo();
    pernaDireita();
    pernaEsquerda();
    peDireito();  // Adiciona o pé direito
    peEsquerdo(); // Adiciona o pé esquerdo
}

void desenha()
{
    glClearColor(0, 0, 0, 0); // Preto
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    chao();
    glPopMatrix();

    glPushMatrix();
    printf("ang = %f\n", thetaX);
    if (girando)
    {
        glTranslatef(X, 0.0, Z);
        glRotatef(angX, 0.0, 1.0, 0.0);
        angulo = angX;
    }
    else
    {
        glTranslatef(X, 0.0, Z);
        glRotatef(angulo, 0.0, 1.0, 0.0);
        X += moveZ * sin(angX * PI / 180);
        Z += moveZ * cos(angX * PI / 180);
    }

    // Modificação aqui: ajustar a movimentação no eixo Z
    glTranslatef(0.0, 0.0, moveZ);

    robo();
    glPopMatrix();

    glFlush();
}

void teclado(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        girando = 0;
        moveZ = 0.01;
        break;
    case 'a':
    case 'A':
        girando = 1;
        moveZ = -0.01;
        angX -= 1;
        break;
    case 's':
    case 'S':
        girando = 0;
        moveZ = -0.01;
        break;
    case 'd':
    case 'D':
        girando = 1;
        moveZ = 0.01;
        angX += 1;
        break;
    default:
        break;
    }
    if (thetaX >= 12)
        inc = -1.0;
    else if (thetaX <= -12)
        inc = 1.0;
    thetaX += inc;
    glutPostRedisplay();
}

void tecladoEspecial(int tecla, int x, int y)
{
    switch (tecla)
    {
    case GLUT_KEY_RIGHT:
        thetaY += 2;
        break;
    case GLUT_KEY_LEFT:
        thetaY -= 2;
        break;
    case GLUT_KEY_UP:
        thetaX += 2;
        break;
    case GLUT_KEY_DOWN:
        thetaX -= 2;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Guy");
    glutDisplayFunc(desenha);
    glutSpecialFunc(tecladoEspecial);
    glutKeyboardFunc(teclado);
    init();
    glutMainLoop();
    return 0;
}