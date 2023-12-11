#include "utils.h"

float angMembros = 0.0;
int flagRotating = 0;
int flagFreeCamera = 0;
float moveZ = 0.0, moveX = 0.0;
float angulo;
float characterPosX = 0.0, characterPosY = 0.0, characterPosZ = 0.0;
float cameraPosX = 0.0, cameraPosY = 1.0, cameraPosZ = 0.0;
float lookingAngle = 0.0;
float freeAngle = 0.0;
float inc = 1.0;
float eyeX, eyeY, eyeZ;
float X = 0.0, Z = 0.0;

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

    glMatrixMode(GL_PROJECTION); // define que a matrix é a de projeção
    glLoadIdentity();            // carrega a matrix de identidade
    // glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
    gluPerspective(45.0, 1.0, 2.0, 8.0); // define uma projeção perspectiva
    // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva simétrica
    // glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva obliqua
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    lightning();
}

void corpo()
{
    glPushMatrix();

    // Esfera superior (corpo)
    glPushMatrix();
    glScalef(0.4, 0.8, 0.41);
    glTranslatef(0.0, 0.1, 0.031);
    glColor3f(0.96, 0.84, 0.69); // Bege
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    // Esfera inferior (calça)
    glPushMatrix();
    glScalef(0.4, 0.4, 0.41);        // Reduz a escala na direção y para metade
    glTranslatef(0.0, -0.09, 0.031); // Move para baixo para posicionar a metade inferior
    glColor3f(0.0, 0.0, 1.0);        // Cor azul (pode ser ajustada)
    glutSolidSphere(0.3, 20, 20);    // Use o mesmo raio da esfera superior (ajuste conforme necessário)
    glPopMatrix();

    glPopMatrix();
}

void cabeca()
{
    glPushMatrix();
    glTranslatef(0.0, 0.4, 0.031);
    glScalef(0.4, 0.5, 0.4);

    // Desenhar uma esfera para a cabeça
    glColor3f(0.96, 0.84, 0.69); // Bege
    glutSolidSphere(0.25, 20, 20);

    glPopMatrix();
}

void chapeu()
{
    glPushMatrix();
    glTranslatef(0.0, 0.5, 0.03); // Ajuste da posição do chapéu em relação à cabeça

    // Adiciona uma rotação de -90 graus em torno do eixo x para deitar a coroa
    glRotatef(-90.0, 1.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);          // Cor verde para a coroa
    glutSolidTorus(0.04, 0.11, 7, 11); // Desenhar uma coroa usando um toro sólido
    glPopMatrix();

    // Adiciona a esfera acima do chapeu
    glPushMatrix();
    glTranslatef(0.0, 0.5, 0.03);  // Ajuste da posição da esfera acima do chapeu
    glColor3f(0.0, 1.0, 0.0);      // Cor verde para a esfera
    glutSolidSphere(0.09, 20, 20); // Desenhar uma esfera sólida
    glPopMatrix();
}

void OlhoD()
{
    // Desenhar olho direito
    glPushMatrix();
    glTranslatef(0.03, 0.4, 0.1);  // Posição do olho direito
    glScalef(0.8, 1.5, 1.0);       // Proporção de escala vertical (aumentei para 1.5)
    glColor3f(0.0, 0.0, 0.0);      // Cor preta
    glutSolidSphere(0.03, 10, 10); // Reduzi o raio para 0.03
    glPopMatrix();
}
void Boca()
{
    glPushMatrix();
    glTranslatef(0.0, 0.32, 0.1);  // Posição da boca
    glScalef(0.5, 0.2, 1.0);       // Ajuste de escala para a boca
    glColor3f(1.0, 1.0, 1.0);      // Cor vermelha (pode ser ajustada)
    glutSolidSphere(0.02, 10, 10); // Use um tamanho e resolução adequados
    glPopMatrix();
}

void Nariz()
{
    glPushMatrix();
    glTranslatef(0.0, 0.38, 0.1);      // Posição do nariz
    glScalef(0.5, 0.5, 1.0);           // Ajuste de escala para o nariz
    glColor3f(0.96, 0.84, 0.69);       // Bege
    glutSolidCone(0.02, 0.05, 10, 10); // Use um tamanho e resolução adequados
    glPopMatrix();
}

void OlhoE()
{
    // Desenhar olho esquerdo
    glPushMatrix();
    glTranslatef(-0.03, 0.4, 0.1); // Posição do olho esquerdo
    glScalef(0.8, 1.5, 1.0);       // Proporção de escala vertical (aumentei para 1.5)
    glColor3f(0.0, 0.0, 0.0);      // Cor preta
    glutSolidSphere(0.03, 10, 10); // Reduzi o raio para 0.03
    glPopMatrix();
}

void bracoDireito()
{
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege

    // Braço superior
    glTranslatef(0.15, 0.15, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.13, 0.06); // Reduzir pela metade em relação à altura original
    glutSolidSphere(0.8, 20, 20);

    glPopMatrix();

    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege

    // Braço superior
    glTranslatef(0.2, 0.1, 0.0);
    glRotatef(45.0, 0.0, 0.0, 1.0);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.1, 0.06); // Reduzir pela metade em relação à altura original
    glutSolidSphere(0.8, 20, 20);

    glPopMatrix();
}

void bracoEsquerdo()
{
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege

    // Braço superior
    glTranslatef(-0.15, 0.15, 0.0);
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    glRotatef(-angMembros, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.13, 0.06); // Reduzir pela metade em relação à altura original
    glutSolidSphere(0.8, 20, 20);

    glPopMatrix();

    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege

    // Braço inferior
    glTranslatef(-0.2, 0.1, 0.0); // Ajuste na posição para ser proporcional ao braço direito
    glRotatef(-45.0, 0.0, 0.0, 1.0);
    glRotatef(-angMembros, 1.0, 0.0, 0.0);
    glScalef(0.04, 0.1, 0.06); // Reduzir pela metade em relação à altura original
    glutSolidSphere(0.8, 20, 20);

    glPopMatrix();
}

void pernaDireita()
{
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege
    glTranslatef(-0.05, -0.3, 0.0);
    glRotatef(-angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.065, 0.03);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege
    glTranslatef(-0.05, -0.19, 0.0);
    glRotatef(-angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.065, 0.03);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // Adicionar a esfera para simular o joelho
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69);     // Bege
    glTranslatef(-0.05, -0.25, 0.0); // Posição da esfera na união dos objetos
    glutSolidSphere(0.023, 20, 20);  // Ajuste o raio conforme necessário
    glPopMatrix();
}

void pernaEsquerda()
{
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege
    glTranslatef(0.05, -0.3, 0.0);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.065, 0.03);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69); // Bege
    glTranslatef(0.05, -0.19, 0.0);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.065, 0.03);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // Adicionar a esfera para simular o joelho
    glPushMatrix();
    glColor3f(0.96, 0.84, 0.69);
    glTranslatef(0.05, -0.25, 0.0); // Posição da esfera na união dos objetos
    glutSolidSphere(0.023, 20, 20); // Ajuste o raio conforme necessário
    glPopMatrix();
}

void peDireito()
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(-0.05, -0.35, 0.02);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.03, 0.07);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void peEsquerdo()
{
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.05, -0.35, 0.02);
    glRotatef(angMembros, 1.0, 0.0, 0.0);
    glScalef(0.02, 0.03, 0.07);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

void human()
{
    cabeca();
    chapeu();
    corpo();
    Boca();
    Nariz();
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
    if(!flagRotating){
        characterPosX += moveZ * sin(lookingAngle * PI / 180);
        characterPosZ += moveZ * cos(lookingAngle * PI / 180);
    }
  
    if(flagFreeCamera){
        cameraPosX = characterPosX - 4 * sin(freeAngle * PI / 180);
        cameraPosY = 1.5;
        cameraPosZ = characterPosZ - 4 * cos(freeAngle * PI / 180);
    }
    else{
        cameraPosX = characterPosX - 4 * sin(lookingAngle * PI / 180);
        cameraPosY = 1.5;
        cameraPosZ = characterPosZ - 4 * cos(lookingAngle * PI / 180);
    }
  
  printf("\nCoordenadas do boneco: %f; %f\n", characterPosX, characterPosZ);
  printf("Coordenadas da camera: %f; %f\n", cameraPosX, cameraPosZ);
  printf("Angulo: %f\n", lookingAngle);
  

  glLoadIdentity();
  gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 
            characterPosX, characterPosY * 0.5, characterPosZ, 
            0.0, 1.0, 0.0);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glPushMatrix();

  if (flagRotating)
  {
    glTranslatef(characterPosX, characterPosY, characterPosZ);
    glRotatef(lookingAngle, 0.0, 1.0, 0.0);
    angulo = lookingAngle;
  }
  else
  {
    glTranslatef(characterPosX, characterPosY, characterPosZ);
    glRotatef(angulo, 0.0, 1.0, 0.0);
  }
  human();
  glPopMatrix();

  glPushMatrix();
  chao();
  glPopMatrix();

    glFlush();
}

void rotacoes(int key, int x, int y)
{
  if(flagFreeCamera == 0){
    freeAngle = lookingAngle;
  }

  switch (key)
  {
  case GLUT_KEY_RIGHT:
    flagFreeCamera = 1;
    freeAngle -= 1;
    break;
  case GLUT_KEY_LEFT:
    flagFreeCamera = 1;
    freeAngle += 1;
    break;
  case GLUT_KEY_UP:
    //eyeX -= 1;
    //flagFreeCamera = 1;
    break;
  case GLUT_KEY_DOWN:
    //flagFreeCamera = 1;
    //eyeX += 1;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'w':
  case 'W':
    flagFreeCamera = 0;
    flagRotating = 0;
    moveZ = 0.01;
    break;
  case 'a':
  case 'A':
    flagFreeCamera = 0;
    flagRotating = 1;
    moveZ = -0.01;
    lookingAngle += 1;
    break;
  case 's':
  case 'S':
    flagFreeCamera = 0;
    flagRotating = 0;
    moveZ = -0.01;
    break;
  case 'd':
  case 'D':
    flagFreeCamera = 0;
    flagRotating = 1;
    moveZ = 0.01;
    lookingAngle -= 1;
    break;
  default:
    break;
  }
  if (angMembros >= 12)
    inc = -1.0;
  else if (angMembros <= -12)
    inc = 1.0;
  angMembros += inc;
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Guy");
    glutDisplayFunc(desenha);
    glutSpecialFunc(rotacoes);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}