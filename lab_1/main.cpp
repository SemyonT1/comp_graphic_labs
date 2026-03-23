#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>


float angle = 0.0f;
bool isDay = true;
bool isPaused = false;

// цвет неба
float skyR = 0.5f, skyG = 0.8f, skyB = 1.0f;


std::vector<std::pair<float, float>> stars;

// генерация звезд
void initStars() {
    for (int i = 0; i < 100; i++) {
        float x = (rand() % 200 - 100) / 100.0f; // [-1, 1]
        float y = (rand() % 100) / 100.0f;       // [0, 1]
        stars.push_back({x, y});
    }
}

// круг
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++) {
        float a = 2 * M_PI * i / 100;
        glVertex2f(cx + cos(a) * r, cy + sin(a) * r);
    }
    glEnd();
}

// звезды
void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);

    glBegin(GL_POINTS);
    for (auto& s : stars) {
        glVertex2f(s.first, s.second);
    }
    glEnd();
}

// дом
void drawHouse() {
    // стены
    glColor3f(0.8f, 0.5f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.5f);
    glVertex2f(0.2f, -0.5f);
    glVertex2f(0.2f, -0.1f);
    glVertex2f(-0.2f, -0.1f);
    glEnd();
     
    // дверь
    glColor3f(0.3f, 0.15f, 0.05f); 
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, -0.5f);
    glVertex2f(0.05f, -0.5f);
    glVertex2f(0.05f, -0.25f);
    glVertex2f(-0.05f, -0.25f);
    glEnd();

    // дверная ручка
    glColor3f(1.0f, 0.8f, 0.0f); 
    drawCircle(0.03f, -0.38f, 0.01f);

    // крыша
    glColor3f(0.6f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.25f, -0.1f);
    glVertex2f(0.25f, -0.1f);
    glVertex2f(0.0f, 0.2f);
    glEnd();
}

// дерево
void drawTree(float x) {
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02f, -0.5f);
    glVertex2f(x + 0.02f, -0.5f);
    glVertex2f(x + 0.02f, -0.2f);
    glVertex2f(x - 0.02f, -0.2f);
    glEnd();

    glColor3f(0.0f, 0.6f, 0.0f);
    drawCircle(x, -0.1f, 0.1f);
}

// земля
void drawGround() {
    glColor3f(0.2f, 0.8f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(1.0f, -0.5f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}
// обработка нажатий клавиш
void keyboard(unsigned char key, int x, int y) {
    if (key == ' ' || key == 'p' || key == 'P') {  // пробел или p
        isPaused = !isPaused;  
    }
  
    if (key == 27) {  // ESC
        exit(0);
    }
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(skyR, skyG, skyB, 1.0f);

    if (!isDay) {
        drawStars();
    }

    drawGround();
    drawHouse();
    drawTree(-0.6f);
    drawTree(0.6f);

    float x = -cos(angle) * 1.2f;
    float y = sin(angle) * 0.8f;

    if (isDay) {
        glColor3f(1.0f, 1.0f, 0.0f); // солнце
    } else {
        glColor3f(0.9f, 0.9f, 1.0f); // луна
    }

    drawCircle(x, y, 0.1f);

    glutSwapBuffers();
}

void update(int value) {
    if (!isPaused) {  
        angle += 0.01f;

        if (angle > M_PI) {
            angle = 0.0f;
            isDay = !isDay;
        }

        // цвет неба
        if (isDay) {
            skyR = 0.5f;
            skyG = 0.8f;
            skyB = 1.0f;
        } else {
            skyR = 0.0f;
            skyG = 0.0f;
            skyB = 0.15f;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    initStars();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Lab_1");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}