#include "Snake.h"

#include "Utils.h"
#include <conio.h>

#define H_FIELD 25
#define W_FIELD 50
#define W_CMD   18

#define FRUIT '@'
#define BODY ' '
#define H_EDGE '='
#define V_EDGE '|'

#include "Utils.h"

Snake::Snake() {
	
    m_timer = std::make_unique<Timer>();

    initSnake();
}

void Snake::initSnake() {

    Utils::clearScreen();
    Utils::showConsoleCursor(false);

    /* Direction can only be horizontal OR vertical, therefore the speed in the other direction
    must be set to V_NUL (0). Here i flip a coin to choose starting direction */
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

    // Generate coordinates of head and fruit
    genBody();
    genFruit();

    // Get starting time point and reset time variables
    m_timer->resetTimer();
}

void Snake::run() {

    /* I want the snake to move without waiting for user to tell it to move.
    This is assured by making readUserInput non-blocking (see implementation).
    The result is that the snake moves on its own and changes direction when the user
    press AWSD upper case or lower case */

    m_timer->resetTimer();

    while (isRunning()) {

        readUserInput();

        /* Call here to pause immediatly after ESC is read */
        if (checkPause())
            break;

        updateBodyCoord();
        genFruit();
        drawField();

        /* I could call checkLose in checkEatItself but I prefer here so i can show all the field
        and the snake with red head */
        if (checkLose())
            break;

        /* If your CPU makes the snake too fast, use a Sleep here */
        Sleep(50);
    }
}

bool Snake::isRunning() {

    return m_run;
}

void Snake::readUserInput() {

    /* Call _getch only if a key has been stroke: this assure readUserInput to be non-blocking */
    if (_kbhit()) {

        switch(toupper(_getch())) {

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

        case KEY_ESC:

            m_pause = true;
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

    Utils::clear();

    drawHeader();
    drawSide();

    Utils::setCursor(0, 2);

    for (int y = 0; y < H_FIELD; y++) {

        if (y == 0 || y == H_FIELD - 1)
            Utils::drawElement(std::string(W_FIELD, H_EDGE));
        else {

            for (int x = 0; x < W_FIELD; x++) {

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
                
                if (x == W_FIELD - 1)
                    std::cout << "" << std::endl;
            }
        }
    }

    std::cout << "" << std::endl;
}

void Snake::drawHeader() {
   
    /*
    0 ==========================================================
    1 |                        SNAKE GAME                      |
    2 ==========================================================
    3 |                               |                        |
    4 |                               | Score:  1              |
    5 |                               | Time:   01:02          |
    6 |                               | Status: RUNNING        |
    7 |                               |                        |
    8 |                               | Left:   A              |
    9 |                               | Up:     W              |
    10|                               | Down:   S              |
    11|                               | Right:  D              |
	12|                               | 					   |
    13|                               | Pause:  ESC            |
	14|                               | 					   |
                                    ...
	  ========================================================== H_FIELD + 1
      |                               |                        |
      0                               W_FIELD - 1              W_FIELD + W_CMD - 1
	  
				  _ 
				_|W|_
			   |A|S|D|
			   
    */

	Utils::setCursor(0, 0);
	
	std::cout << std::string(W_FIELD + W_CMD, H_EDGE) << std::endl;
	std::cout << "|                            SNAKE GAME                            |" << std::endl;
	
    Utils::setCursor(W_FIELD, 2);
    std::cout << std::string(W_CMD, H_EDGE);      
}

void Snake::drawSide() {

    Utils::setCursor(W_FIELD, 3);
    std::cout << "" << std::endl;
    Utils::setCursor(W_FIELD + W_CMD - 1, 3);
    std::cout << V_EDGE << std::endl;

    Utils::setCursor(W_FIELD, 4);
    std::cout << " Score:  " << m_countFruit;
    Utils::setCursor(W_FIELD + W_CMD - 1, 4);
    std::cout << V_EDGE << std::endl;

    Utils::setCursor(W_FIELD, 5);
    std::cout << " Time:   " << m_timer->getElapsedTime();
    Utils::setCursor(W_FIELD + W_CMD - 1, 5);
    std::cout << V_EDGE << std::endl;

    Utils::setCursor(W_FIELD, 6);
    std::cout << " Status: ";
    if (m_run)
        Utils::drawElement("RUNNING\t", GREEN_TXT);
    else if (!m_run)
        Utils::drawElement("END\t\t", RED_TXT);
	Utils::setCursor(W_FIELD + W_CMD - 1, 6);
	std::cout << V_EDGE << std::endl;

	Utils::setCursor(W_FIELD, 7);
    std::cout << "" << std::endl;
    Utils::setCursor(W_FIELD + W_CMD - 1, 7);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 8);
    std::cout << " Left:   A";
    Utils::setCursor(W_FIELD + W_CMD - 1, 8);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 9);
    std::cout << " Up:     W";
    Utils::setCursor(W_FIELD + W_CMD - 1, 9);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 10);
    std::cout << " Down:   S";
    Utils::setCursor(W_FIELD + W_CMD - 1, 10);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 11);
    std::cout << " Right:  D";
    Utils::setCursor(W_FIELD + W_CMD - 1, 11);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 12);
    std::cout << "" << std::endl;
    Utils::setCursor(W_FIELD + W_CMD - 1, 12);
    std::cout << V_EDGE << std::endl;
	
	Utils::setCursor(W_FIELD, 13);
    std::cout << " Pause:  ESC" << std::endl;
    Utils::setCursor(W_FIELD + W_CMD - 1, 13);
    std::cout << V_EDGE << std::endl;

    for (int i = 14; i < H_FIELD + 1; i++) {

        Utils::setCursor(W_FIELD + W_CMD - 1, i);
        std::cout << V_EDGE << std::endl;
    }
    Utils::setCursor(W_FIELD, H_FIELD + 1);
    std::cout << std::string(W_CMD, H_EDGE) << std::endl;
}

bool Snake::checkLose() {

    bool ret = false;

    if (!m_run) {

        if (Utils::pauseRoutine("Ouch!", "Do you wanna play again?", "y", "n")) {
            
            m_run = true;
            initSnake();
        }
        else
            ret = true;
    }
    
    return ret;
}

bool Snake::checkPause() {

    bool ret = false;

    if (m_pause) {

        m_timer->pauseTimer();
        
        Utils::setCursor(W_FIELD, 6);
        std::cout << " Status: ";
        Utils::drawElement("PAUSED\t", YELLOW_TXT);
        Utils::setCursor(W_FIELD + W_CMD - 1, 6);
        std::cout << V_EDGE << std::endl;
        
        Utils::setCursor(0, H_FIELD + 3);
        
        if (Utils::pauseRoutine("Game paused", "Do you wanna resume or quit?", "r", "q")) {
        
            m_pause = false;
            Utils::clearScreen();
            Utils::showConsoleCursor(false);
            
            m_timer->restartWDelay();
        }
        else
            ret = true;
    }

    return ret;
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

    // Check if {x, y} belong to snake's body
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
    Add the new head coordinate and remove tail coordinate */
    m_body.push_back(actualCoord);
    if (m_body.size() > (m_countFruit + 1))
        m_body.erase(m_body.begin());
}

void Snake::checkEatItSelf(const std::pair<int, int> &actualCoord) {

    // Check if head updated coordinates overlap snake's body
    if (find(m_body.begin(), m_body.end(), actualCoord) != m_body.end())
        m_run = false;
}

void Snake::genBody() {
    
    int randomIndex = Utils::genRandomInt(0, m_field.size() - 1);
    m_x = m_field.at(randomIndex).first;
    m_y = m_field.at(randomIndex).second;

    // Start with only head
    m_body.push_back(std::pair<int, int>{m_x, m_y});
}

void Snake::genFruit() {

    /* If there is not a fruit in the field m_fruit is false and I have to spawn a new one
	in the field but not in the same position of snake's body. I compute the subset of available
	coordinates by subtracting the vector of body coordinates from the vector of field coordinates,
	then I pick a random pair of coordinates from it. Eventually I set m_fruit to true not to generate
	a new fruit until the current one is not eaten and to make drawField draw it. */

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