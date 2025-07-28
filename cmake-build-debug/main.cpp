
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include <map>
using namespace std;

void revealTiles(Tile& clickedTile, vector<vector<Tile>>& board) {
    if (clickedTile.getypos() < 0 || clickedTile.getxpos() < 0 ||
        clickedTile.getypos() >= board.size() || clickedTile.getxpos() >= board[0].size() ||
        clickedTile.getrevealed()) {
        return;
    }
    clickedTile.reveal();
    if (clickedTile.getsurroundingmines() == 0) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int x = clickedTile.getxpos() + i;
                int y = clickedTile.getypos() + j;
                if (x >= 0 && x < board[0].size() && y >= 0 && y < board.size()) {
                    Tile& neighbor = board[y][x];
                    if (!neighbor.gethasflag() && !neighbor.getrevealed()) {
                        revealTiles(neighbor, board);
                    }
                }
            }
        }
    }
}

int main() {
    int column;
    int row;
    int numMines;
    ifstream cfg("config.cfg");
    if (!cfg.is_open()) {
        cout << "Cannot open config.cfg!" << endl;
        return 1;
    }
    cfg >> column >> row >> numMines;

    sf::RenderWindow window(sf::VideoMode(column * 32, row * 32 + 100), "Welcome Window", sf::Style::Close);
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        return 1;
    }

    sf::Text text("WELCOME TO MINESWEEPER!", font, 24);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setFillColor(sf::Color::White);
    text.setPosition((column * 32) / 2.0f, (row * 32 + 100) / 2.0f - 150.0f);
    text.setOrigin(text.getLocalBounds().width / 2.0f, text.getLocalBounds().height / 2.0f);

    sf::Text nameText("Enter your name:", font, 20);
    nameText.setStyle(sf::Text::Bold);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition((column * 32) / 2.0f, (row * 32 + 100) / 2.0f - 75);
    nameText.setOrigin(nameText.getLocalBounds().width / 2.0f, nameText.getLocalBounds().height / 2.0f);

    sf::Text userText("", font, 18);
    userText.setStyle(sf::Text::Bold);
    userText.setFillColor(sf::Color::Yellow);
    userText.setPosition((column * 32) / 2.0f, (row * 32 + 100) / 2.0f - 45);
    userText.setOrigin(userText.getLocalBounds().width / 2.0f, userText.getLocalBounds().height / 2.0f);

    string username;

    sf::RectangleShape cursor(sf::Vector2f(2.f, 20.f));
    cursor.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            else if (event.type == sf::Event::TextEntered) {
                if (isalpha(event.text.unicode) && userText.getString().getSize() < 10) {
                    if (userText.getString().isEmpty()) {
                        userText.setString(char(toupper(event.text.unicode)));
                        username.push_back(toupper(static_cast<char>(event.text.unicode)));
                    } else {
                        userText.setString(userText.getString() + char(tolower(event.text.unicode)));
                        username.push_back(tolower(static_cast<char>(event.text.unicode)));
                    }
                    sf::FloatRect textRect = userText.getLocalBounds();
                    userText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    userText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45));
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace)) {
                if (!userText.getString().isEmpty()) {
                    userText.setString(userText.getString().substring(0, userText.getString().getSize() - 1));
                }
                sf::FloatRect textRect = userText.getLocalBounds();
                userText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                userText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 45));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && userText.getString().getSize() > 0) {
                window.close();
            }
        }

        cursor.setPosition(userText.findCharacterPos(userText.getString().getSize()));

        window.clear(sf::Color::Blue);
        window.draw(text);
        window.draw(nameText);
        window.draw(cursor);
        window.draw(userText);
        window.display();
    }

    window.close();

    sf::RenderWindow gameWindow(sf::VideoMode(column * 32, (row * 32) + 100), "Minesweeper", sf::Style::Close);
    sf::Clock clock;
    sf::Texture tileHidden, face, debug, paused, play, leaderboard, timer, flag, mines, tileRevealed, tileNum, sad, happy;
    sf::Texture num1,num2,num3,num4,num5,num6,num7,num8;

    num1.loadFromFile("images/number_1.png");
    num2.loadFromFile("images/number_2.png");
    num3.loadFromFile("images/number_3.png");
    num4.loadFromFile("images/number_4.png");
    num5.loadFromFile("images/number_5.png");
    num6.loadFromFile("images/number_6.png");
    num7.loadFromFile("images/number_7.png");
    num8.loadFromFile("images/number_8.png");

    tileHidden.loadFromFile("images/tile_hidden.png");
    face.loadFromFile("images/face_happy.png");
    debug.loadFromFile("images/debug.png");
    paused.loadFromFile("images/pause.png");
    play.loadFromFile("images/play.png");
    leaderboard.loadFromFile("images/leaderboard.png");
    timer.loadFromFile("images/digits.png");
    flag.loadFromFile("images/flag.png");
    mines.loadFromFile("images/mine.png");
    tileRevealed.loadFromFile("images/tile_revealed.png");
    tileNum.loadFromFile("images/number_1.png");

    sad.loadFromFile("images/face_lose.png");
    happy.loadFromFile("images/face_win.png");

    sf::Sprite spriteHidden(tileHidden);
    sf::Sprite spriteFace(face);
    sf::Sprite spriteDebug(debug);
    sf::Sprite spritePaused(paused);
    sf::Sprite spritePlay(play);
    sf::Sprite spriteLeaderboard(leaderboard);

    sf::Sprite spriteTimerM(timer);
    sf::Sprite spriteTimerS(timer);
    sf::Sprite spriteTimerM1(timer);
    sf::Sprite spriteTimerS1(timer);

    sf::Sprite spriteFlag(flag);
    sf::Sprite spriteMines(mines);
    sf::Sprite spriteRevealed(tileRevealed);
    sf::Sprite spriteNum(tileNum);

    sf::Sprite flagCounter1(timer);
    sf::Sprite flagCounter2(timer);
    sf::Sprite flagCounter3(timer);
    sf::Sprite flagCounter4(timer);

    sf::Sprite tile1(num1);
    sf::Sprite tile2(num2);
    sf::Sprite tile3(num3);
    sf::Sprite tile4(num4);
    sf::Sprite tile5(num5);
    sf::Sprite tile6(num6);
    sf::Sprite tile7(num7);
    sf::Sprite tile8(num8);

    sf::Sprite loseFace(sad);
    sf::Sprite winFace(happy);

    bool debugMode = false;
    bool gameWon = false;
    bool leaderboardUpdated = false;
    bool gameLost = false;
    bool pause = false;
    bool allMinesFlagged = false;
    int mineRevealedX;
    int mineRevealedY;

    int seconds = 0;

    int secondsOne = 0;
    int secondsTwo = 0;
    int minutesOne = 0;
    int minutesTwo = 0;

    string contents;

    flagCounter1.setPosition(33, 32 * ((row) + 0.5f) + 16);
    flagCounter2.setPosition(54, 32 * ((row) + 0.5f) + 16);
    flagCounter3.setPosition(75, 32 * ((row) + 0.5f) + 16);
    flagCounter4.setPosition(12, 32 * ((row) + 0.5f) + 16);

    spriteTimerM.setPosition(((column) * 32) - 97, 32 * ((row) + 0.5f) + 16);
    spriteTimerS.setPosition(((column) * 32) - 54, 32 * ((row) + 0.5f) + 16);
    spriteTimerM1.setPosition(((column) * 32) - 76, 32 * ((row) + 0.5f) + 16);
    spriteTimerS1.setPosition(((column) * 32) - 33, 32 * ((row) + 0.5f) + 16);

    spriteDebug.setPosition((column * 32) - 304, 32 * ((row) + 0.5f));
    spriteFace.setPosition((((column) / 2.0) * 32) - 32, 32 * ((row) + 0.5f));
    spritePaused.setPosition(((column) * 32) - 240, 32 * ((row) + 0.5f));
    spriteLeaderboard.setPosition(((column) * 32) - 176, 32 * ((row) + 0.5f));

    spriteTimerM.setTextureRect(sf::IntRect(0, 0, 21, 32));
    spriteTimerS.setTextureRect(sf::IntRect(0, 0, 21, 32));
    spriteTimerM1.setTextureRect(sf::IntRect(0, 0, 21, 32));
    spriteTimerS1.setTextureRect(sf::IntRect(0, 0, 21, 32));

    flagCounter1.setTextureRect(sf::IntRect(21 * (numMines / 100), 0, 21, 32));
    flagCounter2.setTextureRect(sf::IntRect(21 * ((numMines / 10) % 10), 0, 21, 32));
    flagCounter3.setTextureRect(sf::IntRect(21 * (numMines % 10), 0, 21, 32));
    flagCounter4.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));

    int numMinesOnBoard;
    int tilesPlaced;
    int minesPlaced;
    int flagsPlaced = 0;

    bool firstClick = true;
    bool originalGameStateStored = false;

    vector<vector<Tile>> minesweeperBoard(row, vector<Tile>(column));
    vector<vector<Tile>> originalBoard;

    int remainingFlags = numMines - flagsPlaced;
    int remainingFlagsHundreds = abs(remainingFlags / 100) % 10;
    int remainingFlagsTens = abs(remainingFlags / 10) % 10;
    int remainingFlagsOnes = abs(remainingFlags % 10);

    for(int i = 0; i < row; i++) {
        for(int j = 0; j < column; j++) {

            minesweeperBoard[i][j] = Tile(j, i, false, false, false);
        }
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < column; j++) {
            minesweeperBoard[i][j].setneighbors(minesweeperBoard, row, column);
            minesweeperBoard[i][j].countsurroundingmines();
        }
    }

    while (gameWindow.isOpen()) {
        sf::Event event;
        sf::Time elapsed = clock.getElapsedTime();
        if(elapsed.asSeconds() >= 1.0f && !pause && !gameLost && !gameWon) {
            secondsTwo++;
            seconds++;
            if (secondsTwo == 10) {
                secondsTwo = 0;
                secondsOne++;
                if (secondsOne == 6) {
                    secondsOne = 0;
                    minutesTwo++;
                    if (minutesTwo == 10) {
                        minutesTwo = 0;
                        minutesOne++;
                        if (minutesOne == 6) {
                            minutesOne = 0;
                            minutesTwo = 0;
                            secondsOne = 0;
                            secondsTwo = 0;
                        }
                    }
                }
            }

            spriteTimerM.setTextureRect(sf::IntRect(21 * minutesOne, 0, 21, 32));
            spriteTimerM1.setTextureRect(sf::IntRect(21 * minutesTwo, 0, 21, 32));
            spriteTimerS.setTextureRect(sf::IntRect(21 * secondsOne, 0, 21, 32));
            spriteTimerS1.setTextureRect(sf::IntRect(21 * secondsTwo, 0, 21, 32));

            clock.restart();
        }

        bool allRevealed = true;
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                if (!minesweeperBoard[i][j].gethasmine() && !minesweeperBoard[i][j].getrevealed()) {
                    allRevealed = false;
                    break;
                }
            }
            if (!allRevealed) {
                break;
            }
        }

        if (allRevealed && !gameLost && !leaderboardUpdated) {
            cout << "all revealed" << endl;
            gameWon = true;
            spriteFace.setTexture(happy);
            string currentTime = to_string(minutesOne) + to_string(minutesTwo) + ":" + to_string(secondsOne) + to_string(secondsTwo);

            ifstream leaderboardReadFile("leaderboard.txt");
            map<string, pair<string, bool>> entries;

            if (leaderboardReadFile.is_open()) {
                string line;
                while (getline(leaderboardReadFile, line)) {
                    size_t commaPos = line.find(',');
                    if (commaPos != string::npos) {
                        string time = line.substr(0, commaPos);
                        string name = line.substr(commaPos + 1);
                        entries.emplace(time, make_pair(name, false));
                    }
                }
                leaderboardReadFile.close();
            } else {
                cout << "Error opening leaderboard.txt for reading." << endl;
            }

            auto newEntryIt = entries.find(currentTime);
            if (newEntryIt != entries.end()) {
                newEntryIt->second.second = true;
            } else {
                entries.emplace(currentTime, make_pair(username, true));
            }

            ofstream leaderboardWriteFile("leaderboard.txt");
            if (leaderboardWriteFile.is_open()) {
                for (const auto& entry : entries) {
                    leaderboardWriteFile << entry.first << "," << entry.second.first;
                    if (entry.second.second) {
                        leaderboardWriteFile << "*";  // Add an asterisk for new entries
                    }
                    leaderboardWriteFile << "\n";
                }
                leaderboardWriteFile.close();
                leaderboardUpdated = true;
            } else {
                cout << "Error opening leaderboard.txt for writing." << endl;
            }
        }

        while (gameWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    int x = (event.mouseButton.x / 32);
                    int y = (event.mouseButton.y / 32);

                    if (firstClick) {
                        while (minesPlaced < numMines) {
                            int i = rand() % row;
                            int j = rand() % column;
                            if (!minesweeperBoard[i][j].gethasmine() && (i != y || j != x)) {
                                minesweeperBoard[i][j].setasmine();
                                minesPlaced++;
                            }
                        }
                        for (int i = 0; i < row; ++i) {
                            for (int j = 0; j < column; ++j) {
                                minesweeperBoard[i][j].setneighbors(minesweeperBoard, row, column);
                                minesweeperBoard[i][j].countsurroundingmines();
                            }
                        }
                        firstClick = false;
                    }

                    int coordX = event.mouseButton.x;
                    int coordY = event.mouseButton.y;
                    int debugX = (column * 32) - 304;
                    int debugY = 32 * (row + 0.5f);
                    int happyX = ((column / 2.0) * 32) - 32;
                    int happyY = 32 *(row + 0.5f);
                    int pauseX = (column * 32) - 240;
                    int pauseY = 32 * (row + 0.5f);
                    int leaderboardX = (column * 32) - 176;
                    int leaderboardY = 32 * (row + 0.5f);

                    if (!gameLost && !gameWon && ((pauseX < coordX) && (coordX < (pauseX + 64)) && (pauseY < coordY) && (coordY < (pauseY+ 64)))) {
                        pause = !pause;
                        if(pause) {
                            spritePaused.setTexture(play);
                            if (!originalGameStateStored) {
                                originalBoard = minesweeperBoard;
                                originalGameStateStored = true;
                            }
                        } else {
                            spritePaused.setTexture(paused);
                            if (originalGameStateStored) {
                                minesweeperBoard = originalBoard;
                                originalGameStateStored = false;
                            }
                        }

                    }

                    if ((leaderboardX < coordX) && (coordX < (leaderboardX + 64)) && (leaderboardY < coordY) && (coordY < (leaderboardY+ 64))) {
                        pause = !pause;
                        if(!gameWon) {
                            if (pause) {
                                spritePaused.setTexture(play);
                                if (!originalGameStateStored) {
                                    originalBoard = minesweeperBoard;
                                    originalGameStateStored = true;
                                }
                            } else {
                                spritePaused.setTexture(paused);
                                if (originalGameStateStored) {
                                    minesweeperBoard = originalBoard;
                                    originalGameStateStored = false;
                                }
                            }
                        }

                        if (!gameWon && pause) {
                            for (int i = 0; i < row; i++) {
                                for (int j = 0; j < column; j++) {
                                    spriteRevealed.setPosition(j * 32, i * 32);
                                    gameWindow.draw(spriteRevealed);
                                }
                            }
                        }
                        ifstream file("leaderboard.txt");
                        if (!file.is_open()) {
                            cout << "Error opening leaderboard file." << endl;
                            return 1;
                        }

                        sf::RenderWindow leaderboardWindow(sf::VideoMode(column * 16, (row * 16) + 50), "Leaderboards", sf::Style::Close);

                        sf::Text leaderboardText("LEADERBOARD", font, 20);
                        leaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
                        leaderboardText.setFillColor(sf::Color::White);
                        leaderboardText.setPosition((column * 16) / 2.0f, ((row * 16) + 50) / 2.0f - 120);
                        leaderboardText.setOrigin(leaderboardText.getLocalBounds().width / 2.0f, leaderboardText.getLocalBounds().height / 2.0f);

                        sf::Text contentText("", font, 18);
                        contentText.setStyle(sf::Text::Bold);
                        contentText.setFillColor(sf::Color::White);
                        contentText.setPosition((column * 8) / 2.0f, (row * 8) / 2.0f + 20);
                        contentText.setOrigin(contentText.getLocalBounds().width / 2.0f, contentText.getLocalBounds().height / 2.0f);

                        string line;
                        int lineNum = 1;
                        while (getline(file, line) && lineNum <= 5) {
                            string time, name;
                            size_t commaPos = line.find(',');
                            if (commaPos != string::npos) {
                                time = line.substr(0, commaPos);
                                name = line.substr(commaPos + 1);
                                contentText.setString(contentText.getString() + to_string(lineNum) + ". " + time + " " + name + "\n\n");
                                lineNum++;
                            }
                        }
                        file.close();

                        while (leaderboardWindow.isOpen()) {
                            sf::Event leadEvent;
                            while (leaderboardWindow.pollEvent(leadEvent)) {
                                if (leadEvent.type == sf::Event::Closed) {
                                    leaderboardWindow.close();
                                }
                            }
                            leaderboardWindow.clear(sf::Color::Blue);
                            leaderboardWindow.draw(leaderboardText);
                            leaderboardWindow.draw(contentText);
                            leaderboardWindow.display();
                        }

                    }

                    if(!gameLost && !gameWon && ((debugX < coordX) && (coordX < (debugX + 64)) && (debugY < coordY) && (coordY < (debugY + 64)))) {
                        debugMode = !debugMode;
                    }

                    if ((happyX < coordX) && (coordX < (happyX + 64)) && (happyY < coordY) && (coordY < (happyY + 64))) {
                        minesweeperBoard.clear();
                        minesweeperBoard.resize(row, vector<Tile>(column));
                        originalBoard.clear();
                        originalGameStateStored = false;
                        seconds = 0;
                        secondsOne = 0;
                        secondsTwo = 0;
                        minutesOne = 0;
                        minutesTwo = 0;
                        flagsPlaced = 0;
                        minesPlaced = 0;
                        for (int i = 0; i < row; i++) {
                            for (int j = 0; j < column; j++) {
                                minesweeperBoard[i][j] = Tile(j, i, false, false, false);
                            }
                        }
                        for (int i = 0; i < row; ++i) {
                            for (int j = 0; j < column; ++j) {
                                minesweeperBoard[i][j].setneighbors(minesweeperBoard, row, column);
                                minesweeperBoard[i][j].countsurroundingmines();
                            }
                        }
                        firstClick = true;
                        gameLost = false;
                        gameWon = false;
                        debugMode = false;
                        pause = false;
                        leaderboardUpdated = false;
                        flagsPlaced = 0;
                        minesPlaced = 0;
                        spriteFace.setTexture(face);
                        clock.restart();
                    }

                    if (!gameLost && !gameWon && (y >= 0 && y < row && x >= 0 && x < column)) {
                        Tile& clickedTile = minesweeperBoard[y][x];
                        if (!clickedTile.gethasflag() && !clickedTile.getrevealed()) {
                            if (clickedTile.gethasmine()) {
                                cout << "GAME OVER" << endl;
                                gameLost = true;
                                spriteFace.setTexture(sad);
                                for (int i = 0; i < row; ++i) {
                                    for (int j = 0; j < column; ++j) {
                                        if(minesweeperBoard[i][j].gethasmine()) {
                                            spriteMines.setPosition(j * 32, i * 32);
                                            gameWindow.draw(spriteMines);
                                        }
                                    }
                                }
                            } else {
                                revealTiles(clickedTile, minesweeperBoard);
                            }
                        }
                    }

                }

                if(event.mouseButton.button == sf::Mouse::Right) {
                    int x = (event.mouseButton.x / 32);
                    int y = (event.mouseButton.y / 32);
                    int coordX = event.mouseButton.x;
                    int coordY = event.mouseButton.y;
                    if (!gameLost && !gameWon && (y >= 0 && y < row && x >= 0 && x < column)) {
                        minesweeperBoard[y][x].toggleFlag();
                        flagsPlaced += (minesweeperBoard[y][x].gethasflag() ? 1 : -1);
                        remainingFlags = numMines - flagsPlaced;
                        remainingFlagsHundreds = abs(remainingFlags / 100) % 10;
                        remainingFlagsTens = abs(remainingFlags / 10) % 10;
                        remainingFlagsOnes = abs(remainingFlags % 10);

                        flagCounter1.setTextureRect(sf::IntRect(21 * remainingFlagsHundreds, 0, 21, 32));
                        flagCounter2.setTextureRect(sf::IntRect(21 * remainingFlagsTens, 0, 21, 32));
                        flagCounter3.setTextureRect(sf::IntRect(21 * remainingFlagsOnes, 0, 21, 32));
                        flagCounter4.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
                    }
                }

            }
        }

        gameWindow.clear(sf::Color::White);
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                if(gameLost) {
                    for (int i = 0; i < row; ++i) {
                        for (int j = 0; j < column; ++j) {
                            if (minesweeperBoard[i][j].gethasmine()) {
                                spriteMines.setPosition(j * 32, i * 32);
                                gameWindow.draw(spriteMines);
                            }
                        }
                    }
                    debugMode = false;
                }
                if(!pause && debugMode) {
                    for (int i = 0; i < row; ++i) {
                        for (int j = 0; j < column; ++j) {
                            if (minesweeperBoard[i][j].gethasmine()) {
                                spriteMines.setPosition(j * 32, i * 32);
                                gameWindow.draw(spriteMines);
                            }
                        }
                    }
                }
                if (pause && minesweeperBoard[i][j].getrevealed()) {
                    spriteRevealed.setPosition(j * 32, i * 32);
                    gameWindow.draw(spriteRevealed);
                } else {
                    if (!pause && !minesweeperBoard[i][j].getrevealed()) {
                        spriteHidden.setPosition(j * 32, i * 32);
                        gameWindow.draw(spriteHidden);
                        if (!pause && minesweeperBoard[i][j].gethasflag()) {
                            spriteFlag.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteFlag);
                        }
                    } else {
                        if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 2) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile1.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile1);
                        } else if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 4) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile2.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile2);
                        } else if (!pause && minesweeperBoard[i][j].getsurroundingmines() == 6) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile3.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile3);
                        } else if (!pause && minesweeperBoard[i][j].getsurroundingmines() == 8) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile4.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile4);
                        } else if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 10) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile5.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile5);
                        } else if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 12) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile6.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile6);
                        } else if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 14) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile7.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile7);
                        } else if(!pause && minesweeperBoard[i][j].getsurroundingmines() == 16) {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                            tile8.setPosition(j * 32, i * 32);
                            gameWindow.draw(tile8);
                        } else {
                            spriteRevealed.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteRevealed);
                        }

                        if (!pause && debugMode && minesweeperBoard[i][j].gethasmine()) {
                            spriteMines.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteMines);
                        } else if (!pause && !debugMode && minesweeperBoard[i][j].gethasmine()) {
                            spriteHidden.setPosition(j * 32, i * 32);
                            gameWindow.draw(spriteHidden);
                        }
                    }
                }

            }
        }

        gameWindow.draw(flagCounter1);
        gameWindow.draw(flagCounter2);
        gameWindow.draw(flagCounter3);
        if(flagsPlaced > numMines) {
            gameWindow.draw(flagCounter4);
        }

        gameWindow.draw(spriteTimerM);
        gameWindow.draw(spriteTimerS);
        gameWindow.draw(spriteTimerM1);
        gameWindow.draw(spriteTimerS1);

        gameWindow.draw(spriteDebug);
        gameWindow.draw(spriteFace);
        gameWindow.draw(spriteLeaderboard);
        gameWindow.draw(spritePaused);

        gameWindow.display();
    }
    return 0;
}