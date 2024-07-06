#ifndef Snake_H
#define Snake_H

#include <algorithm>
#include <utility>
#include <vector>

#include <chrono>
#include <string>

typedef enum {

    V_NEG = -1,
    V_NUL,
    V_POS
} V;

class Snake {

public:

    Snake();

    void run();

private:

    void initSnake();

    // User intervention
    void readUserInput();
    void setXDirection(V vel);
    void setYDirection(V vel);

    bool isRunning();
    void drawField();
    void checkEdges();
    bool checkFruit(int x, int y);
    bool checkHead(int x, int y);
    bool checkBody(int x, int y);
    void updateBodyCoord();
    void checkEatItSelf(const std::pair<int, int> &actualCoord);

    void genFruit();
    void genBody();

    bool checkLose();
    bool checkPause();

    std::string getTime();

    bool m_pause = false;
    bool m_run = true;

    int m_deltaT = 50;

    // Law of motion's components
    int m_x{};
    int m_y{};
    int m_xT = 1;
    int m_yT = 1;
    V m_vx{};
    V m_vy{};

    bool m_fruit{};
    int m_xFruit{};
    int m_yFruit{};
    int m_countFruit{};

    // Snake's body
    std::vector<std::pair<int, int>> m_body;

    std::vector<std::pair<int, int>> m_field;

    int m_min{};
    int m_sec{};
    std::chrono::steady_clock::time_point begin;
};

#endif