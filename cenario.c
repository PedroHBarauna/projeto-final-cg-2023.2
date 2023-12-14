#include "utils.h"

float rotacaoMembros = 0.0;
int flagRotating = 0;
int flagFreeCamera = 0;
float moveZ = 0.0, moveX = 0.0;
float angulo;
float characterPosX = 0.0, characterPosY = 0.0, characterPosZ = 0.0;
float cameraPosX = 0.0, cameraPosY = 1.0, cameraPosZ = 0.0;
float anguloDeVista = 0.0;
float anguloAux = 0.0;
float i = 1.0;
float cameraX;
int textura_atual[17];
int flagTextura = 0;
float thetaX = 0.0;
float inc = 1.0;
typedef struct BMPImagem
{
  int width;
  int height;
  char *data;
} BMPImage;

GLuint texture_id[MAX_NO_TEXTURES];

char *filenameArray[MAX_NO_TEXTURES] = {"apple.bmp", "banana.bmp",
                                        "grape.bmp", "cherry.bmp",
                                        "orange.bmp", "watermelon.bmp", "default.bmp"};

int getRandomTexture()
{
  return (rand() % 6) + 1;
}

/*Carrega os arquivos bmps e os converte*/
void getBitmapImageData(char *pFileName, BMPImage *pImage)
{
  FILE *pFile = NULL;
  unsigned short nNumPlanes;
  unsigned short nNumBPP;
  int i;

  if ((pFile = fopen(pFileName, "rb")) == NULL)
    printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

  fseek(pFile, 18, SEEK_CUR);

  if ((i = fread(&pImage->width, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ",
           pFileName);

  if ((i = fread(&pImage->height, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ",
           pFileName);

  if ((fread(&nNumPlanes, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n",
           pFileName);

  if (nNumPlanes != 1)
    printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

  if ((i = fread(&nNumBPP, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ",
           pFileName);

  if (nNumBPP != 24)
    printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);

  fseek(pFile, 24, SEEK_CUR);

  int nTotalImagesize = (pImage->width * pImage->height) * 3;

  pImage->data = (char *)malloc(nTotalImagesize);

  if ((i = fread(pImage->data, nTotalImagesize, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ",
           pFileName);

  char charTemp;
  for (i = 0; i < nTotalImagesize; i += 3)
  {
    charTemp = pImage->data[i];
    pImage->data[i] = pImage->data[i + 2];
    pImage->data[i + 2] = charTemp;
  }
}

/*Função para Carregar uma imagem .BMP */
void CarregaTexturas()
{
  BMPImage textura;

  /* Define quantas texturas serão usadas no programa  */
  glGenTextures(MAX_NO_TEXTURES, texture_id);

  int i;
  for (i = 0; i < MAX_NO_TEXTURES; i++)
  {
    getBitmapImageData(filenameArray[i], &textura);
    glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, textura.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
}

/*Inicia a textura*/
void initTexture(void)
{
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  CarregaTexturas();
}

void chao(int id)
{
  glPushMatrix();
  glTranslatef(0.0, -0.5, 0.0);
  glScalef(3.5, 0.5, 3.5);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3f(-0.25, 0.25, 0.25);
  glVertex3f(0.25, 0.25, 0.25);
  glVertex3f(0.25, -0.25, 0.25);
  glVertex3f(-0.25, -0.25, 0.25);
  glEnd();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3f(-0.25, 0.25, -0.25);
  glVertex3f(0.25, 0.25, -0.25);
  glVertex3f(0.25, -0.25, -0.25);
  glVertex3f(-0.25, -0.25, -0.25);
  glEnd();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3f(0.25, -0.25, 0.25);
  glVertex3f(0.25, 0.25, 0.25);
  glVertex3f(0.25, 0.25, -0.25);
  glVertex3f(0.25, -0.25, -0.25);
  glEnd();
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex3f(-0.25, -0.25, 0.25);
  glVertex3f(-0.25, 0.25, 0.25);
  glVertex3f(-0.25, 0.25, -0.25);
  glVertex3f(-0.25, -0.25, -0.25);
  glEnd();
  /* Face da Textura */
  /* se for modificar a textura colocar flag como 1, se não, pode manter como 0 */
  if (flagTextura == 1)
  {

    glBindTexture(GL_TEXTURE_2D, texture_id[textura_atual[id]]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-0.25, 0.25, 0.25);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(0.25, 0.25, 0.25);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(0.25, 0.25, -0.25);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-0.25, 0.25, -0.25);
    glEnd();
  }
  else
  {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(-0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, 0.25);
    glVertex3f(0.25, -0.25, -0.25);
    glVertex3f(-0.25, -0.25, -0.25);
    glEnd();
    glPopMatrix();
  }
  glColor3f(1.0, 1.0, 1.0);
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

  GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
  glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
  glLightfv(GL_LIGHT1, GL_SPECULAR, white);
  GLfloat direction[] = {1.0f, 0.0f, 0.0f};
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.1f);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
}

void init(void)
{
  glEnable(GL_COLOR_MATERIAL);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
  gluPerspective(80.0, 1.0, 0.1, 800.0); // define uma projeção perspectiva
  // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção
  // perspectiva simétrica glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define
  // uma projeção perspectiva obliqua
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  lightning();
}
/* Início do Corpo teste para o cenário */

void corpo()
{
  glPushMatrix();
  glScalef(0.5, 1.0, 0.41);

  // Esfera esticada para simular o corpo de Fall Guys
  glColor3f(1.0, 0.5, 0.8); // Cor amarela
  glutSolidSphere(0.3, 20, 20);

  glPopMatrix();
}

void cabeca()
{
  glPushMatrix();
  glTranslatef(0.0, 0.10, 0.031);
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
  glColor3f(1.0, 0.5, 0.8);
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
  glColor3f(1.0, 0.5, 0.8);
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
  glColor3f(1.0, 0.5, 0.8);     // Cor
  glScalef(0.04, 0.13, 0.06);   // Ajuste da escala para uma esfera mais alongada
  glutSolidSphere(1.0, 20, 20); // Substituição do cubo por uma esfera
  glPopMatrix();
}

void pernaEsquerda()
{
  glPushMatrix();
  glTranslatef(0.05, -0.25, 0.0); // Ajuste da posição da perna
  glRotatef(thetaX, 1.0, 0.0, 0.0);
  glColor3f(1.0, 0.5, 0.8);     // Cor
  glScalef(0.04, 0.13, 0.06);   // Ajuste da escala para uma esfera mais alongada
  glutSolidSphere(1.0, 20, 20); // Substituição do cubo por uma esfera
  glPopMatrix();
}

void peDireito()
{
  glPushMatrix();
  glTranslatef(-0.05, -0.35, 0.02);
  glRotatef(thetaX, 1.0, 0.0, 0.0);
  glColor3f(1.0, 0.5, 0.8);     // Cor
  glScalef(0.02, 0.03, 0.07);   // Ajuste da escala para uma esfera mais pequena
  glutSolidSphere(1.0, 20, 20); // Desenhar pé direito
  glPopMatrix();
}

void peEsquerdo()
{
  glPushMatrix();
  glTranslatef(0.05, -0.35, 0.02); // Ajuste da posição do pé esquerdo
  glRotatef(thetaX, 1.0, 0.0, 0.0);
  glColor3f(1.0, 0.5, 0.8);     // Cor
  glScalef(0.02, 0.03, 0.07);   // Ajuste da escala para uma esfera mais pequena
  glutSolidSphere(1.0, 20, 20); // Desenhar pé esquerdo
  glPopMatrix();
}

void coroa()
{
  glPushMatrix();
  glTranslatef(0.0, 0.4, 0.0); // Ajuste da posição da coroa em relação à cabeça

  // Adiciona uma rotação de -90 graus em torno do eixo x para deitar a coroa
  glRotatef(-90.0, 1.0, 0.0, 0.0);

  glColor3f(1.0, 0.843, 0.0);         // Cor dourada para a coroa
  glutSolidTorus(0.02, 0.08, 10, 15); // Desenhar uma coroa usando um toro sólido
  glPopMatrix();
}

void fall_guy()
{
  cabeca();
  coroa();
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

void teloes()
{

  flagTextura = 1;
  glPushMatrix();
  glTranslatef(0.0, 1.0, 5.0);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glRotatef(180, 0.0, 1.0, 0.0);
  chao(16);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.0, 1.0, -5.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  chao(16);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(5.0, 1.0, 0.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  glRotatef(90, 0.0, 0.0, 1.0);
  chao(16);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-5.0, 1.0, 0.0);
  glRotatef(-90, 1.0, 0.0, 0.0);
  glRotatef(-90, 0.0, 0.0, 1.0);
  glRotatef(180, 0.0, 1.0, 0.0);
  chao(16);
  glPopMatrix();
}

void plataformas()
{
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.9, 0.0, 0.9);
  flagTextura = 1;
  chao(0);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.9, 0.0, -0.9);
  flagTextura = 1;
  chao(1);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.9, 0.0, 2.7);
  flagTextura = 1;
  chao(2);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(0.9, 0.0, -2.7);
  flagTextura = 1;
  chao(3);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-0.9, 0.0, 0.9);
  flagTextura = 1;
  chao(4);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-0.9, 0.0, -0.9);
  flagTextura = 1;
  chao(5);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-0.9, 0.0, 2.7);
  flagTextura = 1;
  chao(6);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-0.9, 0.0, -2.7);
  flagTextura = 1;
  chao(7);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(2.7, 0.0, 0.9);
  flagTextura = 1;
  chao(8);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(2.7, 0.0, -0.9);
  flagTextura = 1;
  chao(9);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(2.7, 0.0, 2.7);
  flagTextura = 1;
  chao(10);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(2.7, 0.0, -2.7);
  flagTextura = 1;
  chao(11);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-2.7, 0.0, 0.9);
  flagTextura = 1;
  chao(12);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-2.7, 0.0, -0.9);
  flagTextura = 1;
  chao(13);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-2.7, 0.0, 2.7);
  flagTextura = 1;
  chao(14);
  flagTextura = 0;

  glPopMatrix();
  glPushMatrix();
  glRotatef(0.0, 0.0, 1.0, 0.0);
  glTranslatef(-2.7, 0.0, -2.7);
  flagTextura = 1;
  chao(15);
  flagTextura = 0;

  glPopMatrix();
}

void cenario()
{
  teloes();
  plataformas();
}

void desenha()
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  if (!flagRotating)
  {
    characterPosX += moveZ * sin(anguloDeVista * PI / 180);
    characterPosZ += moveZ * cos(anguloDeVista * PI / 180);
  }

  if (flagFreeCamera)
  {
    cameraPosX = characterPosX - 2.8 * sin(anguloAux * PI / 180);
    cameraPosY = 2.2;
    cameraPosZ = characterPosZ - 2.8 * cos(anguloAux * PI / 180);
  }
  else
  {
    cameraPosX = characterPosX - 2.8 * sin(anguloDeVista * PI / 180);
    cameraPosY = 2.2;
    cameraPosZ = characterPosZ - 2.8 * cos(anguloDeVista * PI / 180);
  }

  printf("\nCoordenadas do boneco: %f; %f\n", characterPosX, characterPosZ);
  printf("Coordenadas da camera: %f; %f\n", cameraPosX, cameraPosZ);

  glLoadIdentity();
  gluLookAt(cameraPosX, cameraPosY, cameraPosZ, characterPosX,
            characterPosY * 0.5, characterPosZ, 0.0, 10, 0.0);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glPushMatrix();

  if (flagRotating)
  {
    glTranslatef(characterPosX, characterPosY, characterPosZ);
    glRotatef(anguloDeVista, 0.0, 1.0, 0.0);
    angulo = anguloDeVista;
  }
  else
  {
    glTranslatef(characterPosX, characterPosY, characterPosZ);
    glRotatef(angulo, 0.0, 1.0, 0.0);
  }
  /* Desenha o Corpo teste */
  fall_guy();
  glPopMatrix();

  /* Inicio Criação dos Chãos */
  teloes();
  plataformas();

  /* Fim Criação dos Chãos */

  glFlush();
}

void rotacoes(int key, int x, int y)
{
  if (flagFreeCamera == 0)
  {
    anguloAux = anguloDeVista;
  }

  switch (key)
  {
  case GLUT_KEY_RIGHT:
    flagFreeCamera = 1;
    anguloAux -= 2;
    break;
  case GLUT_KEY_LEFT:
    flagFreeCamera = 1;
    anguloAux += 2;
    break;
  case GLUT_KEY_UP:
    cameraX -= 2;
    flagFreeCamera = 1;
    break;
  case GLUT_KEY_DOWN:
    flagFreeCamera = 1;
    cameraX += 2;
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
    moveZ = 0.1;
    break;
  case 'a':
  case 'A':
    flagFreeCamera = 0;
    flagRotating = 1;
    moveZ = -2.1;
    anguloDeVista += 2.1;
    break;
  case 's':
  case 'S':
    flagFreeCamera = 0;
    flagRotating = 0;
    moveZ = -0.1;
    break;
  case 'd':
  case 'D':
    flagFreeCamera = 0;
    flagRotating = 1;
    moveZ = 2.1;
    anguloDeVista -= 2.1;
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

int main(int argc, char *argv[])
{
  for (int cont = 0; cont < 16; cont++)
  {
    textura_atual[cont] = getRandomTexture();
    printf("textura %d ", textura_atual[cont]);
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Humano");
  glutDisplayFunc(desenha);
  glutSpecialFunc(rotacoes);
  glutKeyboardFunc(keyboard);
  init();
  initTexture();
  glutMainLoop();
  return 0;
}