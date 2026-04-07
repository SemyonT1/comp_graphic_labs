#include <GL/freeglut.h>
#include <cmath>

float angle = 0.0f;

float explodeFactor = 0.0f;

bool transparencyEnabled = false;

float cameraAngle = 1.0f;
float cameraDistance = 6.0f;
float cameraHeight = 3.0f;


void drawExplodingCube(float size)
{
    float h = size/2;

    glBegin(GL_QUADS);

    // передняя
    glNormal3f(0,0,1);
    glVertex3f(-h,-h, h+explodeFactor);
    glVertex3f( h,-h, h+explodeFactor);
    glVertex3f( h, h, h+explodeFactor);
    glVertex3f(-h, h, h+explodeFactor);

    // задняя
    glNormal3f(0,0,-1);
    glVertex3f(-h,-h,-h-explodeFactor);
    glVertex3f(-h, h,-h-explodeFactor);
    glVertex3f( h, h,-h-explodeFactor);
    glVertex3f( h,-h,-h-explodeFactor);

    // правая
    glNormal3f(1,0,0);
    glVertex3f(h+explodeFactor,-h,-h);
    glVertex3f(h+explodeFactor, h,-h);
    glVertex3f(h+explodeFactor, h, h);
    glVertex3f(h+explodeFactor,-h, h);

    // левая
    glNormal3f(-1,0,0);
    glVertex3f(-h-explodeFactor,-h,-h);
    glVertex3f(-h-explodeFactor,-h, h);
    glVertex3f(-h-explodeFactor, h, h);
    glVertex3f(-h-explodeFactor, h,-h);

    // верх
    glNormal3f(0,1,0);
    glVertex3f(-h, h+explodeFactor,-h);
    glVertex3f(-h, h+explodeFactor, h);
    glVertex3f( h, h+explodeFactor, h);
    glVertex3f( h, h+explodeFactor,-h);

    // низ
    glNormal3f(0,-1,0);
    glVertex3f(-h,-h-explodeFactor,-h);
    glVertex3f( h,-h-explodeFactor,-h);
    glVertex3f( h,-h-explodeFactor, h);
    glVertex3f(-h,-h-explodeFactor, h);

    glEnd();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = cameraDistance * cos(cameraAngle);
    float camZ = cameraDistance * sin(cameraAngle);

    gluLookAt(camX, cameraHeight, camZ,
              0,0,0,
              0,1,0);

    float lightX = 3*cos(angle);
    float lightZ = 3*sin(angle);

    GLfloat lightPos[] = {lightX,2.0f,lightZ,1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glPushMatrix();

    glTranslatef(lightX,2.0f,lightZ);

    glDisable(GL_LIGHTING);
    glColor3f(1,1,0);

    glutSolidSphere(0.15,20,20);

    glEnable(GL_LIGHTING);

    glPopMatrix();


    if(transparencyEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glColor4f(1,1,1,0.4f);
    }
    else
    {
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);

        glColor3f(1,1,1);
    }

    drawExplodingCube(2.0f);

    glDepthMask(GL_TRUE);

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

    // вращение камеры
    if(key=='a' || key=='A')
        cameraAngle -= 0.1f;

    if(key=='d' || key=='D')
        cameraAngle += 0.1f;

    // приближение
    if(key=='w' || key=='W')
        cameraDistance -= 0.3f;

    if(key=='s' || key=='S')
        cameraDistance += 0.3f;

    // высота камеры
    if(key=='q' || key=='Q')
        cameraHeight += 0.3f;

    if(key=='e' || key=='E')
        cameraHeight -= 0.3f;

    // раздвижение
    if(key=='z' || key=='Z')
        explodeFactor += 0.1f;

    if(key=='x' || key=='X')
        explodeFactor -= 0.1f;

    if(explodeFactor < 0)
        explodeFactor = 0;

    // прозрачность
    if(key=='t' || key=='T')
        transparencyEnabled = !transparencyEnabled;

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
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,1,1,100);

    glMatrixMode(GL_MODELVIEW);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800,800);

    glutCreateWindow("Lab 4");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,update,0);

    glutMainLoop();

    return 0;
}
