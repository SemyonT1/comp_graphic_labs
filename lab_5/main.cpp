#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>

float angle = 0.0f;
float explodeFactor = 0.0f;

bool transparencyEnabled = false;
bool textureEnabled = true;
bool lightingEnabled = true;

float cameraAngle = 1.0f;
float cameraDistance = 6.0f;
float cameraHeight = 3.0f;

GLuint texture;



GLuint loadTexture(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if(!file) return 0;

    unsigned char header[54];
    fread(header,1,54,file);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    int size = 3*width*height;
    unsigned char* data = new unsigned char[size];

    fread(data,1,size,file);
    fclose(file);

    for(int i=0;i<size;i+=3)
        std::swap(data[i],data[i+2]);

    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D,tex);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    delete[] data;

    return tex;
}



void drawExplodingCube(float size)
{
    float h = size/2;

    glBegin(GL_QUADS);

    // передняя
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-h,-h,h+explodeFactor);
    glTexCoord2f(1,0); glVertex3f(h,-h,h+explodeFactor);
    glTexCoord2f(1,1); glVertex3f(h,h,h+explodeFactor);
    glTexCoord2f(0,1); glVertex3f(-h,h,h+explodeFactor);

    // задняя
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(-h,-h,-h-explodeFactor);
    glTexCoord2f(1,0); glVertex3f(h,-h,-h-explodeFactor);
    glTexCoord2f(1,1); glVertex3f(h,h,-h-explodeFactor);
    glTexCoord2f(0,1); glVertex3f(-h,h,-h-explodeFactor);

    // правая
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(h+explodeFactor,-h,-h);
    glTexCoord2f(1,0); glVertex3f(h+explodeFactor,-h,h);
    glTexCoord2f(1,1); glVertex3f(h+explodeFactor,h,h);
    glTexCoord2f(0,1); glVertex3f(h+explodeFactor,h,-h);

    // левая
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(-h-explodeFactor,-h,-h);
    glTexCoord2f(1,0); glVertex3f(-h-explodeFactor,-h,h);
    glTexCoord2f(1,1); glVertex3f(-h-explodeFactor,h,h);
    glTexCoord2f(0,1); glVertex3f(-h-explodeFactor,h,-h);

    // верх
    glNormal3f(0,1,0);
    glTexCoord2f(0,0); glVertex3f(-h,h+explodeFactor,-h);
    glTexCoord2f(1,0); glVertex3f(h,h+explodeFactor,-h);
    glTexCoord2f(1,1); glVertex3f(h,h+explodeFactor,h);
    glTexCoord2f(0,1); glVertex3f(-h,h+explodeFactor,h);

    // низ
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(-h,-h-explodeFactor,-h);
    glTexCoord2f(1,0); glVertex3f(h,-h-explodeFactor,-h);
    glTexCoord2f(1,1); glVertex3f(h,-h-explodeFactor,h);
    glTexCoord2f(0,1); glVertex3f(-h,-h-explodeFactor,h);

    glEnd();
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float camX = cameraDistance * cos(cameraAngle);
    float camZ = cameraDistance * sin(cameraAngle);

    gluLookAt(camX,cameraHeight,camZ,0,0,0,0,1,0);

    float lightX = 3*cos(angle);
    float lightZ = 3*sin(angle);

    GLfloat lightPos[] = {lightX,2.0f,lightZ,1};
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);

    // источник света
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glPushMatrix();
    glDisable(GL_TEXTURE_2D); 
    glDisable(GL_LIGHTING); 
    glTranslatef(lightX,2.0f,lightZ);
    glColor3f(1,1,0);
    glutSolidSphere(0.15,20,20);
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
    }
   
    glPopMatrix();
    glPopAttrib();

    if(textureEnabled)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture);
    }
    else
        glDisable(GL_TEXTURE_2D);

    
    if(transparencyEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1,1,1,0.4f);
    }
    else
    {
        glDisable(GL_BLEND);
        glColor3f(1,1,1);
    }

    drawExplodingCube(2);

    glutSwapBuffers();
}


void update(int value)
{
    angle+=0.02f;

    glutPostRedisplay();
    glutTimerFunc(16,update,0);
}



void keyboard(unsigned char key,int x,int y)
{
    if(key=='a'||key=='A') cameraAngle-=0.1f;
    if(key=='d'||key=='D') cameraAngle+=0.1f;

    if(key=='w'||key=='W') cameraDistance-=0.3f;
    if(key=='s'||key=='S') cameraDistance+=0.3f;

    if(key=='q'||key=='Q') cameraHeight+=0.3f;
    if(key=='e'||key=='E') cameraHeight-=0.3f;

    if(key=='z'||key=='Z') explodeFactor+=0.1f;
    if(key=='x'||key=='X') explodeFactor-=0.1f;

    if(key=='t'||key=='T')
        transparencyEnabled=!transparencyEnabled;

    if(key=='r'||key=='R')
        textureEnabled=!textureEnabled;

    if(key=='l'||key=='L')
    {
        lightingEnabled=!lightingEnabled;

        if(lightingEnabled)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
    }

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

    GLfloat lightDiffuse[]={1,1,1,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    texture=loadTexture("caban.bmp");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,1,1,100);

    glMatrixMode(GL_MODELVIEW);
}



int main(int argc,char** argv)
{
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,800);

    glutCreateWindow("Lab 5");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,update,0);

    glutMainLoop();

    return 0;
}
