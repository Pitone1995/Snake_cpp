#include "Snake.h"

#include "Utils.h"
#include <conio.h>

#define H_FIELD 25
#define W_FIELD 50

#define FRUIT '@'
#define BODY ' '
#define H_EDGE '='
#define V_EDGE '|'

#include "Utils.h"

Snake::Snake() {
	
    initSnake();
}

void Snake::initSnake() {

    Utils::showConsoleCursor(false);

    if (!Utils::genRandomInt(0, 1)) {

        m_vx = V_NUL;
        m_vy = Utils::genRandomInt(0, 1) ? V_POS : V_NEG;
    }
    else {

        m_vy = V_NUL;
        m_vx = Utils::genRandomInt(0, 1) ? V_POS : V_NEG;
    }

    m_fruit = {};
    m_countFruit = {};

    m_body.clear();

    // Vector of field, borders are excluded
    for (int y = 1; y < H_FIELD - 1; y++) {
        
        for (int x = 1; x < W_FIELD - 1; x++)
            m_field.push_back({x, y});
    }

    genBody();
    genFruit();
}

void Snake::run() {

    while (isRunning()) {

        readUserInput();
        genFruit();
        updateBodyCoord();
        drawField();

        // if (m_deltaT)
        //     Sleep(m_deltaT);
    }
}

bool Snake::isRunning() {
    return m_run;
}

void Snake::readUserInput() {

    if (_kbhit()) {

        switch(_getch()) {

        case KEY_W:

            setYDirection(V_NEG);
            setXDirection(V_NUL);
            break;

        case KEY_S:

            setYDirection(V_POS);
            setXDirection(V_NUL);
            break;


        case KEY_A:

            setXDirection(V_NEG);
            setYDirection(V_NUL);
            break;

        case KEY_D:
            
            setXDirection(V_POS);
            setYDirection(V_NUL);
            break;
        }
    }
}

void Snake::setXDirection(V vel) {

    m_vx = vel;
}

void Snake::setYDirection(V vel) {

    m_vy = vel;
}

void Snake::drawField() {

    system("cls");

    for (int y = 0; y < H_FIELD; y++) {

        for (int x = 0; x < W_FIELD; x++) {

            if (y == 0 || y == H_FIELD - 1)
                Utils::drawElement(H_EDGE);
            else {

                if (x == 0 || x == W_FIELD - 1)
                    Utils::drawElement(V_EDGE);
                else {

                    // Here i am in the field

                    // Spawn or eat fruit
                    if (checkFruit(x, y)) {

                        // Spawn                   
                        if (!checkHead(x, y)) {
                        
                            if (m_fruit)
                                Utils::drawElement(FRUIT, RED_TXT);
                            else
                                Utils::drawElement(' ');
                        }
                        // Eat
                        else {

                            // Generate new fruit coordinates and spawn at next iteration
                            m_fruit = false;
                            m_countFruit++;

                            // Increase speed
                            // if (m_deltaT)
                            //     m_deltaT - m_countFruit * 5;
    
                            // Draw head
                            Utils::drawElement(BODY, BACKGROUND_GREEN);
                        }

                    }
                    // Color actual snake positions
                    else if (checkBody(x, y)) {      
            
                        if (checkHead(x, y))  
                            Utils::drawElement(BODY, m_run ? BACKGROUND_GREEN : BACKGROUND_RED);
                        else
                            Utils::drawElement(BODY, HIGHLIGHT_TXT);
                    }
                    else
                        Utils::drawElement(' ');
                }
            }
            if (x == W_FIELD - 1)
                std::cout << "" << std::endl;
        }
    }

    std::cout << "" << std::endl;

    // Lose condition       
    if (!m_run) {

        Utils::showConsoleCursor(true);
        std::cout << "Ouch!" << "\n";

        std::string choice = "";
        while (choice != "y" && choice != "n") {

            std::cout << "Do you wanna play again (y/n)? ";
            std::cin >> choice;

            if (choice == "y") {
             
                m_run = true;
                initSnake();
            }
            else if (choice == "n")
                m_run = false;
        }
    }
}

void Snake::checkEdges() {

    if (m_y < 1)
        m_y = H_FIELD - 2;
    else if (m_y > H_FIELD - 2)
        m_y = 1;

    if (m_x < 1)
        m_x = W_FIELD - 2;
    else if (m_x > W_FIELD - 2)
        m_x = 1;
}

bool Snake::checkHead(int x, int y) {
    return (x == m_x && y == m_y);
}

bool Snake::checkFruit(int x, int y) {
    return (x == m_xFruit && y == m_yFruit);
}

bool Snake::checkBody(int x, int y) {

    std::pair<int, int> actualCoord(x, y);
    if (std::find(m_body.begin(), m_body.end(), actualCoord) != m_body.end())
        return true;
    else
        return false;
}

void Snake::updateBodyCoord() {

    m_x = m_x + m_vx * m_xT;
    m_y = m_y + m_vy * m_yT;

    checkEdges();

    std::pair<int, int> actualCoord(m_x, m_y);

    if (find(m_body.begin(), m_body.end(), actualCoord) != m_body.end())
        m_run = false;

    m_body.push_back(actualCoord);

    /* I want to keep m_countFruit + 1 coordinates (head)
    Add the new head coordinate and remove tail coordinate
    */
    while (m_body.size() > (m_countFruit + 1))
        m_body.erase(m_body.begin());
}

void Snake::genBody() {
    
    int randomIndex = Utils::genRandomInt(0, m_field.size() - 1);
    m_x = m_field.at(randomIndex).first;
    m_y = m_field.at(randomIndex).second;

    m_body.push_back(std::pair<int, int>{m_x, m_y});
}

void Snake::genFruit() {

    if (!m_fruit) {
    
        // Need to do m_field - m_body and pick a random pair from the result
        std::sort(m_field.begin(), m_field.end());

        // i do not want the m_body to be sorted
        std::vector<std::pair<int, int>> m_bodyCopy = m_body;
        std::sort(m_bodyCopy.begin(), m_bodyCopy.end());

        std::vector<std::pair<int, int>> availableFruitPos;
        std::set_difference(
            m_field.begin(), m_field.end(), m_bodyCopy.begin(), m_bodyCopy.end(),
            std::back_inserter( availableFruitPos )
            );

        int randomIndex = Utils::genRandomInt(0, availableFruitPos.size() - 1);
        m_xFruit = availableFruitPos.at(randomIndex).first;
        m_yFruit = availableFruitPos.at(randomIndex).second;

        m_fruit = true;
    }
}