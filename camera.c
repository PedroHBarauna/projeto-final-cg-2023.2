#include "utils.h"

typedef struct BMPImagem
{
  int width;
  int height;
  char *data;
} BMPImage;

/* Qtd máxima de texturas a serem usadas no programa */
#define MAX_NO_TEXTURES 6

/* vetor com os números das texturas */
GLuint texture_id[MAX_NO_TEXTURES];

char *filenameArray[MAX_NO_TEXTURES] = {
    "earthTexture.bmp",
    "surface1.bmp",
    "surface2.bmp",
    "surface3.bmp",
    "surface4.bmp",
    "surface5.bmp"};

GLUquadricObj *obj;

GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;
float angMembros = 0.0;
int girando = 0;
float moveZ = 0.0, moveX = 0.0;
float angulo;
float characterPosX = 0.0, characterPosY = 0.0, characterPosZ = 0.0;
float cameraPosX = 0.0, cameraPosY = 1.0, cameraPosZ = 0.0;
float lookingAngle = 0.0;
float inc = 1.0;
float eyeX, eyeY, eyeZ;
float X = 0.0, Z = 0.0;


//-----------------------------------------------------------------------------
// Name: getBitmapImageData()
// Desc: Simply image loader for 24 bit BMP files.
//-----------------------------------------------------------------------------
void getBitmapImageData(char *pFileName, BMPImage *pImage)
{
  FILE *pFile = NULL;
  unsigned short nNumPlanes;
  unsigned short nNumBPP;
  int i;

  if ((pFile = fopen(pFileName, "rb")) == NULL)
    printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

  // Seek forward to width and height info
  fseek(pFile, 18, SEEK_CUR);

  if ((i = fread(&pImage->width, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);

  if ((i = fread(&pImage->height, 4, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);

  if ((fread(&nNumPlanes, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);

  if (nNumPlanes != 1)
    printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

  if ((i = fread(&nNumBPP, 2, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);

  if (nNumBPP != 24)
    printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);

  // Seek forward to image data
  fseek(pFile, 24, SEEK_CUR);

  // Calculate the image's total size in bytes. Note how we multiply the
  // result of (width * height) by 3. This is becuase a 24 bit color BMP
  // file will give you 3 bytes per pixel.
  int nTotalImagesize = (pImage->width * pImage->height) * 3;

  pImage->data = (char *)malloc(nTotalImagesize);

  if ((i = fread(pImage->data, nTotalImagesize, 1, pFile)) != 1)
    printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);

  //
  // Finally, rearrange BGR to RGB
  //

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
  glGenTextures(MAX_NO_TEXTURES, texture_id); /* 1 = uma textura; */
                                              /* texture_id = vetor que guardas os números das texturas */

  int i;
  for (i = 0; i < MAX_NO_TEXTURES; i++)
  {
    getBitmapImageData(filenameArray[i], &textura);
    glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  }
}

/* **********************************************************************
  void initTexture(void)
        Define a textura a ser usada

 ********************************************************************** */
void initTexture(void)
{

  /* Habilita o uso de textura bidimensional  */
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  /*Carrega os arquivos de textura */
  //  CarregaTextura("tunnelTexture.bmp");
  // CarregaTextura("tex2.bmp");
  CarregaTexturas();
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

  eyeX = 4.0;
  eyeY = 2.0;
  eyeZ = 1.0;

  glMatrixMode(GL_PROJECTION); // define que a matrix é a de projeção
  glLoadIdentity();            // carrega a matrix de identidade
  // glOrtho(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção ortogonal
  gluPerspective(45.0, 1.0, 2.0, 8.0); // define uma projeção perspectiva
  // glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva simétrica
  // glFrustum(-2.0, 1.0, -1.0, 2.0, 2.0, 8.0); //define uma projeção perspectiva obliqua
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  lightning();
}

void cubo()
{
  float size = 1.0;
  GLfloat n[6][3] =
      {
          {-1.0, 0.0, 0.0},
          {0.0, 1.0, 0.0},
          {1.0, 0.0, 0.0},
          {0.0, -1.0, 0.0},
          {0.0, 0.0, 1.0},
          {0.0, 0.0, -1.0}};
  GLint faces[6][4] =
      {
          {0, 1, 2, 3},
          {3, 2, 6, 7},
          {7, 6, 5, 4},
          {4, 5, 1, 0},
          {5, 6, 2, 1},
          {7, 4, 0, 3}};
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--)
  {
    glBindTexture(GL_TEXTURE_2D, texture_id[i]);
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void chao()
{
  glPushMatrix();
  glTranslatef(0.0, -0.5, 0.0);
  glScalef(4.0, 0.1, 4.0);
  cubo();
  glPopMatrix();
}

void corpo()
{
  glPushMatrix();
  glTranslatef(0.0, 0.25, 0.0);
  glScalef(0.5, 0.6, 0.3);
  cubo();

  glPopMatrix();
}

void cabeca()
{
  glPushMatrix();
  glTranslatef(0.0, 0.8, 0.0);
  glScalef(0.25, 0.25, 0.25);
  cubo();
  glPopMatrix();
}

void pescoco()
{
  glPushMatrix();
  glTranslatef(0.0, 0.5, 0.0);
  glScalef(0.08, 0.30, 0.08);
  cubo();
  glPopMatrix();
}

void bracoDireito()
{
  glPushMatrix();
  glTranslatef(0.3, 0.35, 0.0);
  glRotatef(angMembros, 1.0, 0.0, 0.0);
  glScalef(0.1, 0.4, 0.2);
  cubo();
  glPopMatrix();
}

void bracoEsquerdo()
{
  glPushMatrix();
  glTranslatef(-0.3, 0.35, 0.0);
  glRotatef(-angMembros, 1.0, 0.0, 0.0);
  glScalef(0.1, 0.4, 0.2);
  cubo();
  glPopMatrix();
}

void pernaDireita()
{
  glPushMatrix();
  glTranslatef(-0.1, -0.25, 0.0);
  glRotatef(-angMembros, 1.0, 0.0, 0.0);
  glScalef(0.1, 0.4, 0.2);
  cubo();
  glPopMatrix();
}

void pernaEsquerda()
{
  glPushMatrix();
  glTranslatef(0.1, -0.25, 0.0);
  glRotatef(angMembros, 1.0, 0.0, 0.0);
  glScalef(0.1, 0.4, 0.2);
  cubo();
  glPopMatrix();
}

void robo()
{
  cabeca();
  pescoco();
  corpo();
  bracoDireito();
  bracoEsquerdo();
  pernaDireita();
  pernaEsquerda();
}

void displayFunc()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpa o buffer

  glMatrixMode(GL_MODELVIEW); // define que a matrix é a de modelo

  if(!girando){
    characterPosX += moveZ * sin(lookingAngle * PI / 180);
    characterPosZ += moveZ * cos(lookingAngle * PI / 180);
  }
  
  cameraPosX = characterPosX - 4 * sin(lookingAngle * PI / 180);
  cameraPosY = 1.5;
  cameraPosZ = characterPosZ - 4 * cos(lookingAngle * PI / 180);
  
  printf("\nCoordenadas do boneco: %f; %f\n", characterPosX, characterPosZ);
  printf("Coordenadas da camera: %f; %f\n", cameraPosX, cameraPosZ);
  printf("Angulo: %f\n", lookingAngle);
  

  glLoadIdentity();
  gluLookAt(cameraPosX, cameraPosY, cameraPosZ, 
            characterPosX, characterPosY * 0.5, characterPosZ, 
            0.0, 1.0, 0.0);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glPushMatrix();

  if (girando)
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
  robo();
  glPopMatrix();

  glPushMatrix();
  chao();
  glPopMatrix();

  glFlush(); // força o desenho das primitivas
}

// gluLookAt(eyeX, eyeY, eyeZ,   //posição da câmera
//               0.0, 0.0, 0.0,   //para onde a câmera aponta
//               0.0, 1.0, 0.0);  //vetor view-up

void rotacoes(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_RIGHT:
    //eyeZ -= 1;
    break;
  case GLUT_KEY_LEFT:
    //eyeZ += 1;
    break;
  case GLUT_KEY_UP:
    //eyeX -= 1;
    break;
  case GLUT_KEY_DOWN:
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
    girando = 0;
    moveZ = 0.01;
    break;
  case 'a':
  case 'A':
    girando = 1;
    moveZ = -0.01;
    lookingAngle += 1;
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
  glutCreateWindow("Atividade 1 - P2: Robo com Textura e iluminacao");
  glutDisplayFunc(displayFunc);
  glutSpecialFunc(rotacoes);
  glutKeyboardFunc(keyboard);
  init();
  initTexture();
  glutMainLoop();
  return 0;
}