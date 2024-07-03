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

    /* Direction can only be horizontal OR vertical, therefore one of them is set V_NUL (0).
    Here i flip a coin to choose starting direction */
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

    /* I want the snake to move without waiting for user to tell it to move.
    This is assured by making readUserInput non-blocking (see implementation).
    The result is that the snake moves on its own and changes direction when the user
    press specific keys */

    while (isRunning()) {

        readUserInput();
        genFruit();
        updateBodyCoord();
        drawField();
    }
}

bool Snake::isRunning() {
    return m_run;
}

void Snake::readUserInput() {

    /* Call _getch only if a key has been stroke: this assure readUserInput to be non-blocking */
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
    
    /* Evolve head coordinates according to uniform rectilineas motion's law */
    m_x = m_x + m_vx * m_xT;
    m_y = m_y + m_vy * m_yT;

    /* Update again if snake hits edges to make it go out from the other side */
    checkEdges();

    std::pair<int, int> actualCoord(m_x, m_y);

    /* Self-explanatory */
    checkEatItSelf(actualCoord);

    /* I want to keep m_countFruit + 1 (head) coordinates
    Add the new head coordinate and remove tail coordinate
    */
    m_body.push_back(actualCoord);
    while (m_body.size() > (m_countFruit + 1))
        m_body.erase(m_body.begin());
}

void Snake::checkEatItSelf(const std::pair<int, int> &actualCoord) {

    if (find(m_body.begin(), m_body.end(), actualCoord) != m_body.end())
        m_run = false;
}

void Snake::genBody() {
    
    int randomIndex = Utils::genRandomInt(0, m_field.size() - 1);
    m_x = m_field.at(randomIndex).first;
    m_y = m_field.at(randomIndex).second;

    // Start with only head
    m_body.push_back(std::pair<int, int>{m_x, m_y});

    // To add first part?
    // m_body.push_back(std::pair<int, int>{m_x + m_vx, m_y + m_vy});
}

void Snake::genFruit() {

    /* If there is not a fruit in the field m_fruit is false and I have to spawn a new one,
    being in the field but not in the same position of snake's body.
    Therefore I choose a random pair of coordinates among available ones: this subset is obtained
    by subtracting the vector of body coordinates from the vector of field coordinats.
    Then I set m_fruit to true not to generate a new fruit until the current one is not eaten
    and to make drawField draw it. */

    if (!m_fruit) {
    
        // Need to do m_field - m_body and pick a random pair from the result

        /* set_difference requires the vector to be sorted, but I do not want to sort m_body,
        otherwise it is a mess when it comes to updateBodyCoord: so I work on a copy */
        std::vector<std::pair<int, int>> m_bodyCopy = m_body;
        std::sort(m_bodyCopy.begin(), m_bodyCopy.end());

        std::sort(m_field.begin(), m_field.end());

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