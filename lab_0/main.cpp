#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <cmath>
#include <thread>


const int  WIDTH  = 60;
const int  HEIGHT = 30;
const char PIXEL  = '#';
const char EMPTY  = ' ';


using Canvas = std::vector<std::string>;

Canvas makeCanvas() {
    return Canvas(HEIGHT, std::string(WIDTH, EMPTY));
}

void setPixel(Canvas& c, int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        c[y][x] = PIXEL;
}

void printCanvas(const Canvas& c) {
    std::cout << '+' << std::string(WIDTH, '-') << "+\n";
    for (const auto& row : c)
        std::cout << '|' << row << "|\n";
    std::cout << '+' << std::string(WIDTH, '-') << "+\n";
}

// линия 

void drawLine(Canvas& c, int x0, int y0, int x1, int y1) {
    int dx =  std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    while (true) {
        setPixel(c, x0, y0);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// окружность 

void drawCircle(Canvas& c, int cx, int cy, int r) {
    int x = 0, y = r, err = 3 - 2 * r;

    auto plot8 = [&](int px, int py) {
        setPixel(c, cx+px, cy+py); setPixel(c, cx-px, cy+py);
        setPixel(c, cx+px, cy-py); setPixel(c, cx-px, cy-py);
        setPixel(c, cx+py, cy+px); setPixel(c, cx-py, cy+px);
        setPixel(c, cx+py, cy-px); setPixel(c, cx-py, cy-px);
    };

    while (x <= y) {
        plot8(x, y);
        err += (err > 0) ? 4*(x-y)+10 : 4*x+6;
        if (err > 0) --y;
        ++x;
    }
}

// треугольник  

void drawTriangle(Canvas& c,
                  int x0, int y0,
                  int x1, int y1,
                  int x2, int y2) {
    drawLine(c, x0,y0, x1,y1);
    drawLine(c, x1,y1, x2,y2);
    drawLine(c, x2,y2, x0,y0);
}

// предварительный рендер кадров 

std::vector<Canvas> buildFrames() {
    std::vector<Canvas> frames;

    Canvas c1 = makeCanvas();
    drawLine(c1, 2, 8, WIDTH-3, 8);
    drawLine(c1, 2, HEIGHT-9, WIDTH-3, HEIGHT-9);
    frames.push_back(c1);

    Canvas c2 = makeCanvas();
    drawCircle(c2, WIDTH/2, HEIGHT/2, 11);
    frames.push_back(c2);

    Canvas c3 = makeCanvas();
    drawTriangle(c3, WIDTH/2, 3, 5, HEIGHT-4, WIDTH-6, HEIGHT-4);
    frames.push_back(c3);

    return frames;
}


int main() {
    const auto frames = buildFrames();
    int i = 0;

    while (true) {
        printCanvas(frames[i]);
        i = (i + 1) % 3;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return 0;
}
