#include <GL/freeglut.h>
#include <cmath>

float angle = 0.0f;

float explodeFactor = 0.0f;

float cameraAngle = 1.0f;
float cameraDistance = 6.0f;
float cameraHeight = 3.0f;


void drawExplodingCube(float size)
{
    float half = size / 2.0f;

    glBegin(GL_QUADS);

    // передняя грань
    glNormal3f(0,0,1);
    glVertex3f(-half,-half, half + explodeFactor);
    glVertex3f( half,-half, half + explodeFactor);
    glVertex3f( half, half, half + explodeFactor);
    glVertex3f(-half, half, half + explodeFactor);

    // задняя грань
    glNormal3f(0,0,-1);
    glVertex3f(-half,-half,-half - explodeFactor);
    glVertex3f(-half, half,-half - explodeFactor);
    glVertex3f( half, half,-half - explodeFactor);
    glVertex3f( half,-half,-half - explodeFactor);

    // правая грань
    glNormal3f(1,0,0);
    glVertex3f(half + explodeFactor,-half,-half);
    glVertex3f(half + explodeFactor, half,-half);
    glVertex3f(half + explodeFactor, half, half);
    glVertex3f(half + explodeFactor,-half, half);

    // левая грань
    glNormal3f(-1,0,0);
    glVertex3f(-half - explodeFactor,-half,-half);
    glVertex3f(-half - explodeFactor,-half, half);
    glVertex3f(-half - explodeFactor, half, half);
    glVertex3f(-half - explodeFactor, half,-half);

    // верхняя грань
    glNormal3f(0,1,0);
    glVertex3f(-half, half + explodeFactor,-half);
    glVertex3f(-half, half + explodeFactor, half);
    glVertex3f( half, half + explodeFactor, half);
    glVertex3f( half, half + explodeFactor,-half);

    // нижняя грань
    glNormal3f(0,-1,0);
    glVertex3f(-half,-half - explodeFactor,-half);
    glVertex3f( half,-half - explodeFactor,-half);
    glVertex3f( half,-half - explodeFactor, half);
    glVertex3f(-half,-half - explodeFactor, half);

    glEnd();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // позиция камеры
    float camX = cameraDistance * cos(cameraAngle);
    float camZ = cameraDistance * sin(cameraAngle);

    gluLookAt(camX, cameraHeight, camZ,
              0,0,0,
              0,1,0);


    // координаты источника света
    float lightX = 3 * cos(angle);
    float lightZ = 3 * sin(angle);

    GLfloat lightPos[] = {lightX, 2.0f, lightZ, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


    // Рисуем сферу источника света
    glPushMatrix();

    glTranslatef(lightX, 2.0f, lightZ);

    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);

    glutSolidSphere(0.15, 20, 20);

    glEnable(GL_LIGHTING);

    glPopMatrix();


    // рисуем куб
    glColor3f(1.28f,1.28f,1.28f);
    drawExplodingCube(2.0f);

    glutSwapBuffers();
}


void update(int value)
{
    angle += 0.02f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}


void keyboard(unsigned char key, int x, int y)
{
    if(key=='a' || key=='A')
        cameraAngle -= 0.1f;

    if(key=='d' || key=='D')
        cameraAngle += 0.1f;

    if(key=='w' || key=='W')
        cameraDistance -= 0.3f;

    if(key=='s' || key=='S')
        cameraDistance += 0.3f;

    if(key=='q' || key=='Q')
        cameraHeight += 0.3f;

    if(key=='e' || key=='E')
        cameraHeight -= 0.3f;

    if(key=='z' || key=='Z')
        explodeFactor += 0.1f;

    if(key=='x' || key=='X')
        explodeFactor -= 0.1f;

    if(explodeFactor < 0)
        explodeFactor = 0;
    if(explodeFactor > 15)
	explodeFactor = 15;

    glutPostRedisplay();
}


void init()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightDiffuse[] = {1,1,1,1};

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1.0,1.0,100);

    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,800);

    glutCreateWindow("Lab 3");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
