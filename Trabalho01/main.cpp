/*
*******************************************
***** Trabalho 1 - Computação Gráfica *****
***** Autor: Gabriel Viggiano Fonseca *****
*******************************************
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
#include <GL/glut.h>
#endif // __APPLE__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include <math.h>



/* Declaração de váriaveis GLOBAIS*/
int qtd_vertices = 0, incidencia = 0;
double x;
double y;
double z;
double fov = 60;
double cx, cy, cz;

int altura = 600;
int largura = 800;

double Tempo = 0, aux1 = 0, aux2 = 0;

GLfloat* vertices;
GLfloat* normais;
GLuint* indices;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 50.0f };

bool onGlBegin = true;


/* Função para abrir o arquivo */
static FILE* openfile(char *fileName){
    FILE *arquivo;
    arquivo = fopen(fileName, "r");

    return arquivo;
}

/*Função para inserir os vertices contidos no arquivo*/
static void f_insereVertices(FILE* arquivo){
    char linha[BUFSIZ];

    for (int i=0; i < qtd_vertices; i++){
        fscanf(arquivo, "%s", linha);
        int numero = atoi(linha);

        for (int j=0; j < 3; j++ ){
            fscanf(arquivo,"%s",linha);
            float valor = atof(linha);

            vertices[(i*3)+j] = valor;
        }

        for (int j=0; j < 3; j++ ){
            fscanf(arquivo,"%s",linha);
            float valor = atof(linha);

            normais[(i*3)+j] = valor;
        }
    }
}

/*Função para inserir os indices contidos no arquivo*/
static void f_inserirIndices(FILE* arquivo){
    char linha[BUFSIZ];

    for (int i=0; i < incidencia; i++){
        fscanf(arquivo,"%s",linha);
        int numero = atoi(linha);

        for(int j=0; j<3;j++){
            fscanf(arquivo,"%s",linha);
            int valor = atoi(linha);

            indices[(i*3)+j] = valor;
        }
    }
}

/* Função para alocar variaveis gerais que serão utilizadas nos principais processos do programa, como vetores dinamicos e coordenadas. */
static void f_alocaVariaveis (FILE *arquivo){
    char linha[BUFSIZ];
    char teste[BUFSIZ];

    fscanf(arquivo,"%s",linha);
    qtd_vertices = atoi(linha);

    fscanf(arquivo,"%s",linha);
    incidencia = atoi(linha);

    vertices = (GLfloat*)malloc(qtd_vertices * 3 *sizeof(GLfloat));
    normais = (GLfloat*)malloc(qtd_vertices * 3 *sizeof(GLfloat));
    indices = (GLuint*)malloc(incidencia * 3 * sizeof(GLuint));

    f_insereVertices(arquivo);
    f_inserirIndices(arquivo);
    double maior_z = 0, maior_y = 0, maior_x = 0;
    double menor_z = 1000, menor_y = 1000, menor_x = 1000;
    double d;


	for (int i = 0; i < qtd_vertices; i++){
        if(vertices[(indices[i]*3)] > maior_x){
            maior_x = vertices[(indices[i]*3)];
        }

        if(vertices[(indices[i]*3)] < menor_x){
            menor_x = vertices[(indices[i]*3)];
        }

        if(vertices[(indices[i]*3)+1] > maior_y){
            maior_y = vertices[(indices[i]*3)+1];
        }

        if(vertices[(indices[i]*3)+1] < menor_y){
            menor_y = vertices[(indices[i]*3)+1];
        }

        if(vertices[(indices[i]*3)+2] > maior_z){
            maior_z = vertices[(indices[i]*3)+2];
        }

        if(vertices[(indices[i]*3)+2] < menor_z){
            menor_z = vertices[(indices[i]*3)+2];
        }
	}
	cx = (maior_x - menor_x)/2;
	cy = (maior_y - menor_y)/2;
	cz = (maior_z - menor_z)/2;

    d = (cy) * (tan(fov/2));

    x=0;
    y=0;
    z = d -(cz);
}


// x → posição x dada em pixels na qual o texto será escrito
// y → posição y dada em pixels na qual o texto será escrito
// winwidth → largura da tela em pixels
// winheight →altura da tela em pixels
// str → texto que vai ser desenhado na tela
void printtext(int x, int y, int winwidth, int winheight, char *str) {
 //(x,y) is from the bottom left of the window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, winwidth, 0, winheight, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    int len = (int) strlen(str);
    for (int i=0; i<len; i++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
    }
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 1.0, 100.0);
    gluPerspective(fov,ar,0.1,10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Função de desenho no modo GLDRAWELEMENTS */
static void desenhoGlDrawElements(double a){
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, normais);
    glVertexPointer(3, GL_FLOAT, 0, vertices);


    glPushMatrix();
        glTranslated(x,y,z);
        glRotated(a,0,1,0);
        glTranslated(-cx, -cy, -cz);
        glDrawElements(GL_TRIANGLES, incidencia * 3, GL_UNSIGNED_INT, indices);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    printtext(10,10,largura, altura, "Modo ativo: GL Draw Elements");
}

/* Função de desenho no modo GLBEGIN */
static void desenhoGlBegin(double a){
    glPushMatrix();
        glTranslated(x,y,z);
        glRotated(-a,0,1,0);
        glTranslated(-cx,-cy,-cz);

        glBegin(GL_TRIANGLES);
            for(int i=0; i < incidencia * 3; i++){
                glNormal3f(normais[(indices[i]*3)+0], normais[(indices[i]*3)+1], normais[(indices[i]*3)+2]);
                glVertex3f(vertices[(indices[i]*3)+0], vertices[(indices[i]*3)+1], vertices[(indices[i]*3)+2]);
            }
        glEnd();
    glPopMatrix();

    printtext(10,10,largura,altura, "Modo ativo: GL Begin");
}

/* Função de desenho na tela em geral e calculo de FPS */
static void display(void){
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    char fps_char[3];

    aux1 = aux2;
    aux2 = t;
    Tempo = aux2 - aux1;
    double fps = 1/Tempo;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(onGlBegin){
        glColor3d(1,0,0);
        desenhoGlBegin(a);
    }else{
        glColor3d(0,0,1);
        desenhoGlDrawElements(a);
    }

    sprintf(fps_char, "%.1f", fps);
    printtext(650,30, largura, altura, "FPS:");
    printtext(680,30, largura, altura, fps_char);

    printtext(10, 580, largura, altura, "Trabalho 1 - Topicos Especiais em Computacao Grafica");
    printtext(10, 560, largura, altura, "Autor: Gabriel Viggiano Fonseca");
    printtext(10, 30, largura, altura, "Para alterar o modo de display, aperte 'R'!");

    glutSwapBuffers();

}

static void key(unsigned char key, int x1, int y1){
    switch(key){
        case 'q':
            exit(0);
            break;
        case 'r':
            if (onGlBegin){
                onGlBegin = false;
            }else{
                onGlBegin = true;
            }
            break;
    }
}

int main(int argc, char *argv[]){
    char nome_arquivo[255];
    printf("Digite o nome do arquivo a ser aberto!\n");
    printf("Exemplo: bunny.msh\n");
    scanf("%s", nome_arquivo);
    FILE *arquivo = openfile(nome_arquivo);

    if(arquivo == NULL){
        printf("O arquivo não pode ser aberto.");
        exit(1);
    }

    f_alocaVariaveis(arquivo);

    glutInit(&argc, argv);
    glutInitWindowSize(largura,altura);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Trabalho 1");

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutMainLoop();

    return EXIT_SUCCESS;
}
