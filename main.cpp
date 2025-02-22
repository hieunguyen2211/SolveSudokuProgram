#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace sf;

#define UNASSIGNED 0
#define N 9

struct Sounds
{
  Music main;
  Music button;
};

struct Images
{
  Sprite background;
  Sprite backgroundWithLogo;
  Sprite backgroundInGame;
  Sprite guideScreen;
  Sprite optionScreen;
  Sprite button;
  Sprite buttonActive;
  Sprite buttonItemGenerate;
  Sprite buttonItemSolve;
  Sprite buttonItemBack;
  Sprite buttonItemGenerateActive;
  Sprite buttonItemSolveActive;
  Sprite buttonItemBackActive;
  Sprite buttonMute;
  Sprite buttonUnmute;
  Sprite buttonPrevious;
  Sprite buttonNext;
  Sprite loading;
};

struct inputPoint
{
  int value;
  int x;
  int y;
  bool active;
};

struct Data
{
  Images images;
  Sounds sounds;
  Font font;
  int volumeMusic = 100;
  int volumeButton = 100;
};

void loadImages(Images &images)
{
  Texture *backgroundInGame = new Texture;
  backgroundInGame->loadFromFile("Image/backgroundIngame.jpg");
  images.backgroundInGame.setTexture(*backgroundInGame);

  Texture *backgroundWithLogo = new Texture;
  backgroundWithLogo->loadFromFile("Image/backgroundWithLogo.jpg");
  images.backgroundWithLogo.setTexture(*backgroundWithLogo);

  Texture *guideScreen = new Texture;
  guideScreen->loadFromFile("Image/guideScreen.jpg");
  images.guideScreen.setTexture(*guideScreen);

  Texture *optionScreen = new Texture;
  optionScreen->loadFromFile("Image/optionScreen.jpg");
  images.optionScreen.setTexture(*optionScreen);

  Texture *button = new Texture;
  button->loadFromFile("Image/Buttons/button.png");
  images.button.setTexture(*button);

  Texture *buttonActive = new Texture;
  buttonActive->loadFromFile("Image/Buttons/buttonActive.png");
  images.buttonActive.setTexture(*buttonActive);

  Texture *buttonItemGenerate = new Texture;
  buttonItemGenerate->loadFromFile("Image/Buttons/generate.png");
  images.buttonItemGenerate.setTexture(*buttonItemGenerate);

  Texture *buttonItemSolve = new Texture;
  buttonItemSolve->loadFromFile("Image/Buttons/solve.png");
  images.buttonItemSolve.setTexture(*buttonItemSolve);

  Texture *buttonItemBack = new Texture;
  buttonItemBack->loadFromFile("Image/Buttons/back.png");
  images.buttonItemBack.setTexture(*buttonItemBack);

  Texture *buttonItemGenerateActive = new Texture;
  buttonItemGenerateActive->loadFromFile("Image/Buttons/generateActive.png");
  images.buttonItemGenerateActive.setTexture(*buttonItemGenerateActive);

  Texture *buttonItemSolveActive = new Texture;
  buttonItemSolveActive->loadFromFile("Image/Buttons/solveActive.png");
  images.buttonItemSolveActive.setTexture(*buttonItemSolveActive);

  Texture *buttonItemBackActive = new Texture;
  buttonItemBackActive->loadFromFile("Image/Buttons/backActive.png");
  images.buttonItemBackActive.setTexture(*buttonItemBackActive);

  Texture *buttonMute = new Texture;
  buttonMute->loadFromFile("Image/Buttons/mute.png");
  images.buttonMute.setTexture(*buttonMute);

  Texture *buttonUnmute = new Texture;
  buttonUnmute->loadFromFile("Image/Buttons/unmute.png");
  images.buttonUnmute.setTexture(*buttonUnmute);

  Texture *buttonPrevious = new Texture;
  buttonPrevious->loadFromFile("Image/Buttons/previous.png");
  images.buttonPrevious.setTexture(*buttonPrevious);

  Texture *buttonNext = new Texture;
  buttonNext->loadFromFile("Image/Buttons/next.png");
  images.buttonNext.setTexture(*buttonNext);
}

void loadSounds(Sounds &sounds)
{
  sounds.main.openFromFile("Music/main.wav");
  sounds.button.openFromFile("Music/button-click.wav");
}

void loadFont(Font &font)
{
  font.loadFromFile("Font/main.ttf");
}

void readGridFromInputFile(int **grid)
{

  ifstream ifs;
  ifs.open("InputFile/input.txt");
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      ifs >> *(*(grid + i) + j);
    }
  }
  ifs.close();
}

bool findUnassignedLocation(int **grid, int &row, int &col);

bool isSafe(int **grid, int row, int col, int num);

void displayGameScreen(RenderWindow &window, Data &data);

void displayGuideScreen(RenderWindow &window, Data &data);

void displayOptionScreen(RenderWindow &window, Data &data);

bool findUnassignedLocation(int **grid,
                            int &row, int &col)
{
  for (row = 0; row < N; row++)
    for (col = 0; col < N; col++)
      if (grid[row][col] == UNASSIGNED)
        return true;
  return false;
}

bool usedInRow(int **grid, int row, int num)
{
  for (int col = 0; col < N; col++)
    if (grid[row][col] == num)
      return true;
  return false;
}

bool usedInCol(int **grid, int col, int num)
{
  for (int row = 0; row < N; row++)
    if (grid[row][col] == num)
      return true;
  return false;
}

bool usedInBox(int **grid, int boxStartRow,
               int boxStartCol, int num)
{
  for (int row = 0; row < 3; row++)
    for (int col = 0; col < 3; col++)
      if (grid[row + boxStartRow][col + boxStartCol] == num)
        return true;
  return false;
}

bool isSafe(int **grid, int row,
            int col, int num)
{
  return !usedInRow(grid, row, num) &&
         !usedInCol(grid, col, num) &&
         !usedInBox(grid, row - row % 3,
                    col - col % 3, num) &&
         grid[row][col] == UNASSIGNED;
}

void generateNewGrid(int **grid)
{
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      grid[i][j] = 0;
    }
  }
}

bool solveSudoku(int** grid, vector <int**> vector)
{
	int row = 0, col = 0;
	if (!findUnassignedLocation(grid, row, col))
		return true;
	for (int num = 1; num <= 9; num++)
	{
		if (isSafe(grid, row, col, num))
		{
			bool check = true;
			for (int i = 0; i < vector.size(); i++)
			{
				if (num == vector[i][row][col])
				{
					check = false;
					break;
				}
			}
			if (check)
			{
				grid[row][col] = num;
				if (solveSudoku(grid, vector))
					return true;
				grid[row][col] = UNASSIGNED;
			}
			else
			{
				grid[row][col] = UNASSIGNED;
			}
		}
	}
	return false;
}

void coppyGrid(int** grid, int** backup)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			*(*(backup + i) + j) = *(*(grid + i) + j);
	}
}

void displayTextInButton(RenderWindow &window, int x, int y, Font font, string value)
{
  Text text;
  text.setCharacterSize(20);
  text.setFont(font);
  text.setString(value);
  text.setPosition(x, y);
  text.setColor(Color::White);
  window.draw(text);
}

void displayMenuScreen(RenderWindow &window, Data &data)
{
  bool checkHover[3] = {false, false, false};
  while (window.isOpen())
  {
    Event e;
    while (window.pollEvent(e))
    {
      switch (e.type)
      {
      case Event::Closed:
        window.close();
      case Event::MouseButtonPressed:

        if (e.mouseButton.button == Mouse::Left)
        {
          int x = Mouse::getPosition(window).x;
          int y = Mouse::getPosition(window).y;
          if (x >= 380 && x <= 580 && y >= 348 && y <= 398)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            displayGuideScreen(window, data);
          }
          if (x >= 380 && x <= 580 && y >= 448 && y <= 498)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            displayOptionScreen(window, data);
          }
          if (x >= 380 && x <= 580 && y >= 558 && y <= 598)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            window.close();
          }
        }
        break;
      case Event::MouseMoved:
        int x = e.mouseMove.x;
        int y = e.mouseMove.y;
        if (x >= 380 && x <= 580 && y >= 348 && y <= 398)
        {
          checkHover[0] = true;
        }
        else
        {
          checkHover[0] = false;
        }
        if (x >= 380 && x <= 580 && y >= 448 && y <= 498)
        {
          checkHover[1] = true;
        }
        else
        {
          checkHover[1] = false;
        }
        if (x >= 380 && x <= 580 && y >= 558 && y <= 598)
        {
          checkHover[2] = true;
        }
        else
        {
          checkHover[2] = false;
        }
        break;
      }
    }
    window.clear();
    window.draw(data.images.backgroundWithLogo);
    for (int i = 0; i < 3; i++)
    {
      if (checkHover[i] == false)
      {
        data.images.button.setPosition(380, 300 + i * 100);
        window.draw(data.images.button);
      }
      else
      {
        data.images.buttonActive.setPosition(379, 300 + i * 100);
        window.draw(data.images.buttonActive);
      }
    }
    displayTextInButton(window, 455, 360, data.font, "PLAY");
    displayTextInButton(window, 443, 460, data.font, "OPTION");
    displayTextInButton(window, 460, 560, data.font, "EXIT");
    window.display();
  }
}

void displayGuideScreen(RenderWindow &window, Data &data)
{
  Text text;
  text.setCharacterSize(20);
  text.setFont(data.font);
  text.setString("OK");
  text.setPosition(465, 580);
  text.setColor(Color::White);
  bool checkHover = false;
  while (window.isOpen())
  {
    Event e;
    while (window.pollEvent(e))
    {
      switch (e.type)
      {
      case Event::Closed:
        window.close();
      case Event::MouseButtonPressed:

        if (e.mouseButton.button == Mouse::Left)
        {
          int x = Mouse::getPosition(window).x;
          int y = Mouse::getPosition(window).y;
          if (x >= 380 && x <= 580 && y >= 568 && y <= 618)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            displayGameScreen(window, data);
          }
        }
        break;
      case Event::MouseMoved:
        int x = e.mouseMove.x;
        int y = e.mouseMove.y;
        if (x >= 380 && x <= 580 && y >= 568 && y <= 618)
        {
          checkHover = true;
        }
        else
        {
          checkHover = false;
        }

        break;
      }
    }
    window.clear();
    window.draw(data.images.guideScreen);
    if (checkHover == false)
    {
      data.images.button.setPosition(380, 520);
      window.draw(data.images.button);
    }
    else
    {
      data.images.buttonActive.setPosition(379, 520);
      window.draw(data.images.buttonActive);
    }

    window.draw(text);
    window.display();
  }
}

void setUpInputGrid(inputPoint &p, int &inputX, int &inputY, int valuePX,
                    int valuePY, int valueX, int valueY, RectangleShape rectangles[9][9],
                    int activeInputX, int activeInputY, bool clearAll)
{
  p.active = false;
  if (p.active == false)
  {
    rectangles[activeInputX][activeInputY].setFillColor(Color::Transparent);
    p.active = true;
    p.x = valuePX;
    p.y = valuePY;
    inputX = valueX;
    inputY = valueY;
    if (clearAll == true)
    {
    rectangles[valueX][valueY].setFillColor(Color::Green);
    rectangles[valueX][valueY].setOutlineColor(Color::Black);
    rectangles[valueX][valueY].setOutlineThickness(1);
    }

  }
  else
  {
    p.active = false;
  }
}

void Display(int** grid)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << *(*(grid + i) + j) << " ";
		cout << endl;
	}
}

void displayGameScreen(RenderWindow &window, Data &data)
{
  int **grid;

  vector <int**> listGrid;

  int countSolution;
  int pageIndex;
  grid = new int *[N];
  for (int i = 0; i < N; i++)
  {
    *(grid + i) = new int[N];
  }

  readGridFromInputFile(grid);

  RectangleShape rectangleFull;
  rectangleFull.setSize(Vector2f(360, 360));
  rectangleFull.setFillColor(Color::Transparent);
  rectangleFull.setPosition(300, 180);
  rectangleFull.setOutlineColor(Color::Black);
  rectangleFull.setOutlineThickness(4);

  RectangleShape rectanglesBox[3][3];
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      rectanglesBox[i][j].setSize(Vector2f(120, 120));
      rectanglesBox[i][j].setPosition(300 + j * 120, 180 + 120 * i);
      rectanglesBox[i][j].setOutlineColor(Color::Black);
      rectanglesBox[i][j].setOutlineThickness(2);
      rectanglesBox[i][j].setFillColor(Color::Transparent);
    }
  }

  Text **valuesInGrid;
  valuesInGrid = new Text *[N];
  for (int i = 0; i < N; i++)
  {
    *(valuesInGrid + i) = new Text[N];
  }

  RectangleShape rectangles[N][N];

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      rectangles[i][j].setSize(Vector2f(40, 40));
      rectangles[i][j].setPosition(300 + j * 40, 180 + 40 * i);
      rectangles[i][j].setOutlineColor(Color::Black);
      rectangles[i][j].setOutlineThickness(1);
      rectangles[i][j].setFillColor(Color::Transparent);
      if (grid[i][j] != 0)
      {
        string valueString = to_string(grid[i][j]);
        valuesInGrid[i][j].setFont(data.font);
        valuesInGrid[i][j].setString(valueString);
        valuesInGrid[i][j].setCharacterSize(20);
        valuesInGrid[i][j].setPosition(312 + j * 40, 188 + 40 * i);
        valuesInGrid[i][j].setColor(Color::Black);
      }
    }
  }

  data.images.buttonItemGenerate.setPosition(700, 193);
  data.images.buttonItemSolve.setPosition(700, 313);
  data.images.buttonItemBack.setPosition(700, 433);

  data.images.buttonItemGenerateActive.setPosition(670, 170);
  data.images.buttonItemSolveActive.setPosition(670, 290);
  data.images.buttonItemBackActive.setPosition(670, 410);

  Text annoucement;
  annoucement.setFont(data.font);
  annoucement.setCharacterSize(20);
  annoucement.setPosition(417, 130);
  annoucement.setColor(Color::Red);
  annoucement.setString("");

  inputPoint p;
  int inputX = 0, inputY = 0;
  p.active = false;
  bool checkMultipleSolutions = false;
  bool checkHover[3] = {false, false, false};
  int activeInputX = 0, activeInputY = 0;
  bool clearAll = false;
  while (window.isOpen())
  {
    Event e;
    while (window.pollEvent(e))
    {
      switch (e.type)
      {
      case Event::Closed:
        window.close();
      case Event::MouseButtonPressed:
      {
        if (e.mouseButton.button == Mouse::Left)
        {
          int x = Mouse::getPosition(window).x;
          int y = Mouse::getPosition(window).y;
          if (x >= 730 && x <= 800 && y >= 210 && y <= 280)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            annoucement.setString("");
            generateNewGrid(grid);
            checkMultipleSolutions = false;
            for (int i = 0; i < N; i++)
            {
              for (int j = 0; j < N; j++)
              {
                  valuesInGrid[i][j].setString("");
                  valuesInGrid[i][j].setPosition(312 + j * 40, 188 + 40 * i);
                  window.draw(valuesInGrid[i][j]);
              }
            }
          }
          if (x >= 730 && x <= 800 && y >= 330 && y <= 400)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            countSolution = 0;
            listGrid.clear();

            listGrid.push_back(grid);

            int** mainBackupGrid = new int* [N];
            for (int i = 0; i < N; i++)
            {
                *(mainBackupGrid + i) = new int[N];
            }
            coppyGrid(grid, mainBackupGrid);
            while (true)
            {
                if (solveSudoku(grid, listGrid))
                {
                    countSolution++;
                    int** backupGrid = new int* [N];
                    for (int i = 0; i < N; i++)
                    {
                        *(backupGrid + i) = new int[N];
                    }
                    coppyGrid(grid, backupGrid);
                    listGrid.push_back(backupGrid);
                    coppyGrid(mainBackupGrid, grid);;
                }
                else break;
            }
            int length = listGrid.size();
            if (length == 1)
            {
                annoucement.setString("CANT SOLVE");
                checkMultipleSolutions = false;
            }
            else
            {
                Color color(71, 203, 37);
                annoucement.setColor(color);
                string result;
                if (length == 2)
                {
                    result = "FOUND 1 SOLUTION";
                    checkMultipleSolutions = false;
                }
                else
                {
                    result = "FOUND " + to_string(length - 1) + " SOLUTIONS";
                    checkMultipleSolutions = true;
                    pageIndex = 1;
                }
                annoucement.setString(result);
                annoucement.setPosition(380, 130);

            }


            for (int i = 0; i < N; i++)
              {
                for (int j = 0; j < N; j++)
                {
                  string valueString = to_string(listGrid[1][i][j]);
                  valuesInGrid[i][j].setFont(data.font);
                  valuesInGrid[i][j].setString(valueString);
                  valuesInGrid[i][j].setCharacterSize(20);
                  valuesInGrid[i][j].setPosition(312 + j * 40, 188 + 40 * i);
                  valuesInGrid[i][j].setColor(Color::Black);
                  window.draw(valuesInGrid[i][j]);
                }
              }
          }
          if (x >= 730 && x <= 800 && y >= 450 && y <= 520)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            displayMenuScreen(window, data);
          }

          if (x >= 390 && x <= 440 && y >= 570 && y <= 620 && checkMultipleSolutions)
          {
            if (pageIndex == 1) pageIndex = 1;
            else pageIndex--;
            for (int i = 0; i < N; i++)
              {
                for (int j = 0; j < N; j++)
                {
                  string valueString = to_string(listGrid[pageIndex][i][j]);
                  valuesInGrid[i][j].setFont(data.font);
                  valuesInGrid[i][j].setString(valueString);
                  valuesInGrid[i][j].setCharacterSize(20);
                  valuesInGrid[i][j].setPosition(312 + j * 40, 188 + 40 * i);
                  valuesInGrid[i][j].setColor(Color::Black);
                  window.draw(valuesInGrid[i][j]);
                }
              }

          }

          if (x >= 520 && x <= 570 && y >= 570 && y <= 620 && checkMultipleSolutions)
          {
            if (pageIndex == listGrid.size() - 1) pageIndex = listGrid.size() - 1;
            else pageIndex++;
            for (int i = 0; i < N; i++)
              {
                for (int j = 0; j < N; j++)
                {
                  string valueString = to_string(listGrid[pageIndex][i][j]);
                  valuesInGrid[i][j].setFont(data.font);
                  valuesInGrid[i][j].setString(valueString);
                  valuesInGrid[i][j].setCharacterSize(20);
                  valuesInGrid[i][j].setPosition(312 + j * 40, 188 + 40 * i);
                  valuesInGrid[i][j].setColor(Color::Black);
                  window.draw(valuesInGrid[i][j]);
                }
              }
          }

          for (int i = 0; i < N; i++)
          {
            for (int j = 0; j < N; j++)
            {
              if (x >= 300 + i * 40 && x <= 340 + i * 40 && y >= 180 + j * 40 && y <= 220 + j * 40)
              {
                clearAll = true;
                setUpInputGrid(p, inputX, inputY, 312 + i * 40, 188 + j * 40, j, i, rectangles, activeInputX, activeInputY, clearAll);
                activeInputX = j;
                activeInputY = i;
              }
            }
          }
          if (x < 300 || x > 680 || y < 180 || y > 540)
              {
                clearAll = false;
                setUpInputGrid(p, inputX, inputY, 0, 0, 0, 0, rectangles, activeInputX, activeInputY, clearAll);
                activeInputX = 0;
                activeInputY = 0;
              }


        }
        break;
      }

      case Event::MouseMoved:
      {
        int x = e.mouseMove.x;
        int y = e.mouseMove.y;
        if (x >= 730 && x <= 800 && y >= 210 && y <= 280)
        {
          checkHover[0] = true;
        }
        else
        {
          checkHover[0] = false;
        }
        if (x >= 730 && x <= 800 && y >= 330 && y <= 400)
        {
          checkHover[1] = true;
        }
        else
        {
          checkHover[1] = false;
        }
        if (x >= 730 && x <= 800 && y >= 450 && y <= 520)
        {
          checkHover[2] = true;
        }
        else
        {
          checkHover[2] = false;
        }

        break;
      }
      case Event::KeyPressed:
      {
        if (p.active == true && clearAll == true)
        {
          if (e.key.code == Keyboard::Numpad1)
          {
            grid[inputX][inputY] = 1;
            valuesInGrid[inputX][inputY].setString("1");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad2)
          {
            grid[inputX][inputY] = 2;
            valuesInGrid[inputX][inputY].setString("2");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad3)
          {
            grid[inputX][inputY] = 3;
            valuesInGrid[inputX][inputY].setString("3");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad4)
          {
            grid[inputX][inputY] = 4;
            valuesInGrid[inputX][inputY].setString("4");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad5)
          {
            grid[inputX][inputY] = 5;
            valuesInGrid[inputX][inputY].setString("5");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad6)
          {
            grid[inputX][inputY] = 6;
            valuesInGrid[inputX][inputY].setString("6");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad7)
          {
            grid[inputX][inputY] = 7;
            valuesInGrid[inputX][inputY].setString("7");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad8)
          {
            grid[inputX][inputY] = 8;
            valuesInGrid[inputX][inputY].setString("8");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad9)
          {
            grid[inputX][inputY] = 9;
            valuesInGrid[inputX][inputY].setString("9");
            valuesInGrid[inputX][inputY].setFont(data.font);
            valuesInGrid[inputX][inputY].setCharacterSize(20);
            valuesInGrid[inputX][inputY].setPosition(p.x, p.y);
            valuesInGrid[inputX][inputY].setColor(Color::Black);
            window.draw(valuesInGrid[inputX][inputY]);
          }
          if (e.key.code == Keyboard::Numpad0)
          {
            grid[inputX][inputY] = 0;
            valuesInGrid[inputX][inputY].setString("");
            window.draw(valuesInGrid[inputX][inputY]);
          }
        }
      }
      }
    }
    window.clear();
    window.draw(data.images.backgroundInGame);
    if (checkHover[0])
    {
      window.draw(data.images.buttonItemGenerateActive);
    }
    else
    {
      window.draw(data.images.buttonItemGenerate);
    }
    if (checkHover[1])
    {
      window.draw(data.images.buttonItemSolveActive);
    }
    else
    {
      window.draw(data.images.buttonItemSolve);
    }
    if (checkHover[2])
    {
      window.draw(data.images.buttonItemBackActive);
    }
    else
    {
      window.draw(data.images.buttonItemBack);
    }

    window.draw(rectangleFull);

    for (int i = 0; i < 3; i++)
    {
      for (int j = 0; j < 3; j++)
      {
        window.draw(rectanglesBox[i][j]);
      }
    }
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        window.draw(rectangles[i][j]);
        window.draw(valuesInGrid[i][j]);
      }
    }

    window.draw(annoucement);
    if (checkMultipleSolutions)
    {
        data.images.buttonPrevious.setPosition(390, 570);
        window.draw(data.images.buttonPrevious);
        data.images.buttonNext.setPosition(520, 570);
        window.draw(data.images.buttonNext);
        Text page;
        page.setFont(data.font);
        page.setCharacterSize(30);
        page.setPosition(472, 575);
        page.setColor(Color::Black);
        page.setString(to_string(pageIndex));
        window.draw(page);
    }
    window.display();
  }
}

void displayOptionScreen(RenderWindow &window, Data &data)
{
  Text text;
  text.setCharacterSize(20);
  text.setFont(data.font);
  text.setString("OK");
  text.setPosition(465, 580);
  text.setColor(Color::White);
  int lastVolumeMusic = data.volumeMusic;
  int lastVolumeButton = data.volumeButton;
  bool checkHover = false;

  RectangleShape rectangleVolumeMusic[5];
  RectangleShape rectangleVolumeButton[5];

  for (int i = 0; i < 5; i++)
  {
    rectangleVolumeMusic[i].setPosition(496 + i * 6, 440 - i * 5);
    rectangleVolumeMusic[i].setOutlineColor(Color::Black);
    rectangleVolumeMusic[i].setOutlineThickness(1);
    rectangleVolumeMusic[i].setSize(Vector2f(5, 20 + i * 5));
    if (i < data.volumeMusic / 20)
    {
      rectangleVolumeMusic[i].setFillColor(Color::Red);
    }
    else
    {
      rectangleVolumeMusic[i].setFillColor(Color::Transparent);
    }

    rectangleVolumeButton[i].setPosition(496 + i * 6, 500 - i * 5);
    rectangleVolumeButton[i].setOutlineColor(Color::Black);
    rectangleVolumeButton[i].setOutlineThickness(1);
    rectangleVolumeButton[i].setSize(Vector2f(5, 20 + i * 5));
    if (i < data.volumeButton / 20)
    {
      rectangleVolumeButton[i].setFillColor(Color::Red);
    }
    else
    {
      rectangleVolumeButton[i].setFillColor(Color::Transparent);
    }
  }

  while (window.isOpen())
  {
    Event e;
    while (window.pollEvent(e))
    {
      switch (e.type)
      {
      case Event::Closed:
        window.close();
      case Event::MouseButtonPressed:

        if (e.mouseButton.button == Mouse::Left)
        {
          int x = Mouse::getPosition(window).x;
          int y = Mouse::getPosition(window).y;
          if (x >= 380 && x <= 580 && y >= 568 && y <= 618)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            displayMenuScreen(window, data);
          }
          if (x >= 450 && x <= 480 && y >= 433 && y <= 463)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            data.volumeMusic = data.volumeMusic - 20;

            if (data.volumeMusic < 0)
            {
              data.volumeMusic = 0;
            }
            for (int i = data.volumeMusic / 20; i < 5; i++)
            {
              rectangleVolumeMusic[i].setFillColor(Color::Transparent);
            }
            data.sounds.main.setVolume(data.volumeMusic);
            lastVolumeMusic = data.volumeMusic;
          }
          if (x >= 542 && x <= 572 && y >= 433 && y <= 463)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            data.volumeMusic = data.volumeMusic + 20;
            if (data.volumeMusic > 100)
            {
              data.volumeMusic = 100;
            }
            rectangleVolumeMusic[data.volumeMusic / 20 - 1].setFillColor(Color::Red);
            data.sounds.main.setVolume(data.volumeMusic);
            lastVolumeMusic = data.volumeMusic;
          }
          if (x >= 450 && x <= 480 && y >= 494 && y <= 524)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            data.volumeButton = data.volumeButton - 20;
            if (data.volumeButton < 0)
            {
              data.volumeButton = 0;
            }
            for (int i = data.volumeButton / 20; i < 5; i++)
            {
              rectangleVolumeButton[i].setFillColor(Color::Transparent);
            }
            data.sounds.button.setVolume(data.volumeButton);
            lastVolumeButton = data.volumeButton;
          }
          if (x >= 542 && x <= 572 && y >= 494 && y <= 524)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            data.volumeButton = data.volumeButton + 20;
            if (data.volumeButton > 100)
            {
              data.volumeButton = 100;
            }
            rectangleVolumeButton[data.volumeButton / 20 - 1].setFillColor(Color::Red);
            data.sounds.button.setVolume(data.volumeButton);
            lastVolumeButton = data.volumeButton;
          }
          if (x >= 590 && x <= 620 && y >= 433 && y <= 463)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            if (data.volumeMusic > 0)
            {
              data.volumeMusic = 0;
              for (int i = 0; i < 5; i++)
              {
                rectangleVolumeMusic[i].setFillColor(Color::Transparent);
              }
            }
            else
            {
              data.volumeMusic = lastVolumeMusic;
              for (int i = 0; i < lastVolumeMusic / 20; i++)
              {
                rectangleVolumeMusic[i].setFillColor(Color::Red);
              }
            }
            data.sounds.main.setVolume(data.volumeMusic);
          }
          if (x >= 590 && x <= 620 && y >= 494 && y <= 524)
          {
            data.sounds.button.stop();
            data.sounds.button.play();
            if (data.volumeButton > 0)
            {
              data.volumeButton = 0;
              for (int i = 0; i < 5; i++)
              {
                rectangleVolumeButton[i].setFillColor(Color::Transparent);
              }
            }
            else
            {
              data.volumeButton = lastVolumeButton;
              for (int i = 0; i < lastVolumeButton / 20; i++)
              {
                rectangleVolumeButton[i].setFillColor(Color::Red);
              }
            }
            data.sounds.button.setVolume(data.volumeButton);
          }
        }
        break;
      case Event::MouseMoved:
        int x = e.mouseMove.x;
        int y = e.mouseMove.y;
        if (x >= 380 && x <= 580 && y >= 568 && y <= 618)
        {
          checkHover = true;
        }
        else
        {
          checkHover = false;
        }

        break;
      }
    }
    window.clear();
    window.draw(data.images.optionScreen);
    if (checkHover == false)
    {
      data.images.button.setPosition(380, 520);
      window.draw(data.images.button);
    }
    else
    {
      data.images.buttonActive.setPosition(379, 520);
      window.draw(data.images.buttonActive);
    }
    if (data.volumeMusic == 0)
    {
      data.images.buttonUnmute.setPosition(588, 433);
      window.draw(data.images.buttonUnmute);
    }
    else
    {
      data.images.buttonMute.setPosition(588, 433);
      window.draw(data.images.buttonMute);
    }

    if (data.volumeButton == 0)
    {
      data.images.buttonUnmute.setPosition(588, 493);
      window.draw(data.images.buttonUnmute);
    }
    else
    {
      data.images.buttonMute.setPosition(588, 493);
      window.draw(data.images.buttonMute);
    }
    for (int i = 0; i < 5; i++)
    {
      window.draw(rectangleVolumeMusic[i]);
      window.draw(rectangleVolumeButton[i]);
    }

    window.draw(text);
    window.display();
  }
}

int main()
{
  RenderWindow window(VideoMode(960, 768), "SUDOKU", Style::Default);
  window.setVerticalSyncEnabled(true);

  Data data;

  loadImages(data.images);
  loadSounds(data.sounds);
  loadFont(data.font);

  data.sounds.main.play();
  data.sounds.main.setLoop(true);
  data.sounds.main.setVolume(data.volumeMusic);

  displayMenuScreen(window, data);

  return 0;
}
