#include "level2.hpp"
#include "windows.hpp"



using namespace sf;
#define N 12

level2::level2()
{
}

level2::level2(int move, int lfe)
{
	srand((unsigned) time(0));
	moves = move;
	life = lfe;

	destX = rand() % N;
	destY = rand() % N;

	posX = rand() % N;
	posY = rand() % N;

	ride.loadFromFile("sound/textNoise.wav");
	rsound.setBuffer(ride);

	killed.loadFromFile("sound/attacked.wav");
	killsound.setBuffer(killed);

	attacked.loadFromFile("sound/killed.wav");
	attsound.setBuffer(attacked);

	// posX = 0, posY = 5, destX = 11, destY = 11;

	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			grid[x][y] = 1;
		}
	}
}

void level2::valid(int x, int y)

{
	if (x >= 0 && x < N && y >= 0 && y < N)
	{
		tileMap[x][y].setFillColor(sf::Color::Transparent);
		tileMap[x][y].setOutlineColor(sf::Color::Yellow);
		tileMap[x][y].setOutlineThickness(4);
	}
}

void level2::reset(int A, int B)

{
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (x % 2 == 0)
			{
				if (y % 2 == 0)
				{
					tileMap[x][y].setFillColor(sf::Color(194, 175, 161));
				}
				else
					tileMap[x][y].setFillColor(sf::Color(66, 47, 33));
			}
			else
			{
				if (y % 2 == 0)
				{
					tileMap[x][y].setFillColor(sf::Color(66, 47, 33));
				}
				else
					tileMap[x][y].setFillColor(sf::Color(194, 175, 161));
			}
			tileMap[x][y].setSize(sf::Vector2f(gridSizef, gridSizef));
			tileMap[x][y].setOutlineThickness(2.f);
			tileMap[x][y].setOutlineColor(sf::Color::Black);
			tileMap[x][y].setPosition(x * gridSizef, y * gridSizef);
		}
	}
}

void level2::start(sf::RenderWindow &window)
{
	// sf::RenderWindow window(sf::VideoMode(1000, 900), "Arthur- The Gladiator (Level 2)");
	sf::Texture intro;
	intro.loadFromFile("media/level2.png");
	sf::Sprite intro2;
	intro2.setTexture(intro);
	intro2.setScale(1000 / intro2.getGlobalBounds().width, 900 / intro2.getGlobalBounds().height);

	sf::Event event;
	//draw the intro page
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::EventType::Closed)
				window.close();

			int X = event.mouseButton.x;
			int Y = event.mouseButton.y;

			if (X > 700 && X < 950 && Y > 692 && Y < 830)
			{
				startGame(window);
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
			{
				startGame(window);
			}
		}
		window.clear();
		window.draw(intro2);
		window.display();
	}
}

void level2::startGame(sf::RenderWindow &gamewindow)
{

	// sf::RenderWindow gamewindow(sf::VideoMode(1000, 900), "Arthur- The Gladiator (Level 3):Knight Move", sf::Style::Default);
	CircleShape coin(35); //for displaying no of moves
	coin.setFillColor(Color::Black);
	coin.setOutlineThickness(2);

	CircleShape lfe(35); //for displaying the lives
	lfe.setFillColor(Color::Black);
	lfe.setOutlineThickness(2);

	CircleShape hint(35); //USED FOR HINT, DIJKSTRA ALGORITHM
	hint.setFillColor(Color::Black);
	hint.setOutlineThickness(2);

	CircleShape rules(35); //Rules of games
	rules.setFillColor(Color::Black);
	rules.setOutlineThickness(2);

	//Loading the font
	fonts.loadFromFile("fonts/arial.ttf");
	Text move("Moves", fonts, 15);
	Text m(" ", fonts, 15);

	Text lives("Life", fonts, 15);
	Text l(" ", fonts, 15);

	Text rule("Rules", fonts, 15);

	Text h("Hint", fonts, 15);

	//REPRESENT THE ORDER OF THE SHORTEST PATH
	sf::Text order(" ", fonts, 30);

	//rules set
	if (!rulesTexture.loadFromFile("media/rules2.png"))
	{
		//std::cout << "Error loading paddle texture :(" << std::endl;
	}
	rulesSprite.setTexture(rulesTexture);

	//background wallpaper
	if (!texback.loadFromFile("media/wallpaper.png"))
	{
		//std::cout << "Error loading paddle texture :(" << std::endl;
	}
	background.setTexture(texback);

	//knight texture
	if (!texknight.loadFromFile("media/knight.png", sf::IntRect(0, 100, 50, 50)))
		;
	{
		//std::cout << "Error loading paddle texture :(" << std::endl;
	}

	//ememy texture
	if (!emy.loadFromFile("media/enemy.png", sf::IntRect(0, 0, 256, 2030)))
	{
		cout << "Unable to load image!";
	}

	enemy.setTexture(emy);
	enemy.setTextureRect(sf::IntRect(0, 470, 256, 346));
	enemy.setScale(0.3f, 0.3f);

	knight.setTexture(texknight);
	knight.setScale(1.3f, 1.3f);
	knight.setPosition(static_cast<float>(posX * gridSizef), static_cast<float>(posY * gridSizef));
	//knight.setOrigin(0.f , 0.f);

	if (!texprincess.loadFromFile("media/princes.png", sf::IntRect(0, 0, 200, 200)))
	{
		//std::cout << "Error loading paddle texture :(" << std::endl;
	}
	princess.setTexture(texprincess);
	princess.setScale(1.4f, 1.4f);
	princess.setPosition(static_cast<float>(destX * gridSizef), static_cast<float>(destY * gridSizef));

	reset(posX, posY);
	//for hint not implememnted yet
	sf::RectangleShape box;
	box.setFillColor(sf::Color::Yellow);
	box.setOutlineThickness(2.f);
	box.setOutlineColor(sf::Color::White);
	box.setSize(sf::Vector2f(80, 50));
	box.setPosition(930, 300);

	//for hint not implemented yet
	sf::RectangleShape display;
	display.setSize(sf::Vector2f(108, 108));
	display.setFillColor(sf::Color::Yellow);
	display.setOutlineColor(sf::Color::Black);
	display.setOutlineThickness(2.f);
	display.setPosition(1000, 1000);

	//Represent the Shortest path using bfs
	sPath.setRadius(25);
	sPath.setFillColor(sf::Color(181, 101, 29));
	sPath.setOutlineColor(sf::Color::White);
	sPath.setOutlineThickness(2);

	//Creating the enemy
	for (int i = 0; i < 20; i++)
	{
		
		int a = rand() % N, b = rand() % N;
		if (!((a == destX && b == destY)))
		{
			if ((a > 4 || b > 4))
			{
				grid[a][b] = 2;
			}
			if (grid[a][b] == 2)
			{
				if (grid[a + 1][b + 1] == 1)
				{
					grid[a + 1][b + 1] = 3;
				}

				if (grid[a + 1][b - 1] == 1)
				{
					grid[a + 1][b - 1] = 3;
				}

				if (grid[a - 1][b + 1] == 1)
				{
					grid[a - 1][b + 1] = 3;
				}

				if (grid[a - 1][b - 1] == 1)
				{
					grid[a - 1][b - 1] = 3;
				}
			}
		}
	}

	//Creating the immunity
	for (int i = 0; i < 20; i++)
	{
		int a = rand() % N, b = rand() % N;
		if (!((a == posX && b == posY) || (a == destX && b == destY)) && !(grid[a][b] == 2 || grid[a][b] == 3 || grid[a][b] == 0))
		{
			grid[a][b] = 4;
		}
	}

	//window events
	while (gamewindow.isOpen())
	{

		sf::Event event;

		while (gamewindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				gamewindow.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				//tracks the location where mouse button is pressed
				int mouseX = event.mouseButton.x;
				int mouseY = event.mouseButton.y;
				// cout << mouseX << " " << mouseY << endl;
				//track the present location of knight and princess
				posX = knight.getPosition().x;
				posY = knight.getPosition().y;
				destX = princess.getPosition().x;
				destY = princess.getPosition().y;

				//movement of knight inside the board
				if (mouseX < 900 && mouseY < 900)
				{
					rsound.play();
					//top left
					if (mouseX > (posX - 2 * gridSizef) && mouseX < (posX - 1 * gridSizef) && mouseY > (posY - 1 * gridSizef) && mouseY < (posY - 0 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX - 2 * gridSizef), static_cast<float>(posY - 1 * gridSizef));
						posX -= 2;
						posY -= 1;
						moves--;
					}
					//top mid left
					else if (mouseX > (posX - 1 * gridSizef) && mouseX < (posX - 0 * gridSizef) && mouseY > (posY - 2 * gridSizef) && mouseY < (posY - 1 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX - 1 * gridSizef), static_cast<float>(posY - 2 * gridSizef));
						posX -= 1;
						posY -= 2;
						moves--;
					}
					//top mid right
					else if (mouseX > (posX + 1 * gridSizef) && mouseX < (posX + 2 * gridSizef) && mouseY > (posY - 2 * gridSizef) && mouseY < (posY - 1 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX + 1 * gridSizef), static_cast<float>(posY - 2 * gridSizef));
						posX += 1;
						posY -= 2;
						moves--;
					}
					//top right
					else if (mouseX > (posX + 2 * gridSizef) && mouseX < (posX + 3 * gridSizef) && mouseY > (posY - 1 * gridSizef) && mouseY < (posY - 0 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX + 2 * gridSizef), static_cast<float>(posY - 1 * gridSizef));
						posX += 2;
						posY -= 1;
						moves--;
					}
					//bottom left
					else if (mouseX > (posX - 2 * gridSizef) && mouseX < (posX - 1 * gridSizef) && mouseY > (posY + 1 * gridSizef) && mouseY < (posY + 2 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX - 2 * gridSizef), static_cast<float>(posY + 1 * gridSizef));
						posX -= 2;
						posY += 1;
						moves--;
					}
					//bottom mid left
					else if (mouseX > (posX - 1 * gridSizef) && mouseX < (posX - 0 * gridSizef) && mouseY > (posY + 2 * gridSizef) && mouseY < (posY + 3 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX - 1 * gridSizef), static_cast<float>(posY + 2 * gridSizef));
						posX -= 1;
						posY += 2;
						moves--;
					}
					//bottom mid right
					else if (mouseX > (posX + 1 * gridSizef) && mouseX < (posX + 2 * gridSizef) && mouseY > (posY + 2 * gridSizef) && mouseY < (posY + 3 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX + 1 * gridSizef), static_cast<float>(posY + 2 * gridSizef));
						posX += 1;
						posY += 2;
						moves--;
					}
					//bottom right
					else if (mouseX > (posX + 2 * gridSizef) && mouseX < (posX + 3 * gridSizef) && mouseY > (posY + 1 * gridSizef) && mouseY < (posY + 2 * gridSizef))
					{
						reset(posX, posY);
						knight.setPosition(static_cast<float>(posX + 2 * gridSizef), static_cast<float>(posY + 1 * gridSizef));
						posX += 2;
						posY += 1;
						moves--;
					}
					else
					{
						cout << "Knight can't move in the specified position";
					}
				}
			}
		}

		//track the present location of knight and princess
		posX = knight.getPosition().x / gridSizef;
		posY = knight.getPosition().y / gridSizef;
		destX = princess.getPosition().x / gridSizef;
		destY = princess.getPosition().y / gridSizef;

		//moves
		if (moves < 0)
		{

			// gamewindow.close();
			gameWindows g("media/gameover.png", gamewindow);
		}

		//I f the life of player get equals to zero,gameover
		if (life <= 0)
		{
			// window.close();
			gameWindows g("media/gameover.png", gamewindow);
			cout << "You are killed by enemy, try again!!!";
		}

		if (posX == destX && posY == destY)
		{
			nextlevel(gamewindow);
		}

		//implemented hint logic
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int a = posX;
			int b = posY;

			int X = event.mouseButton.x;
			int Y = event.mouseButton.y;

			if (X > 930 && X < 1000 && Y > 280 && Y < 350)
			{
				bf.findpath(destX, destY, posX, posY,grid);
			}

			if (X > 930 && X < 1000 && Y > 40 && Y < 110)
			{

				(valid(a - 2, b + 1));

				(valid(a - 1, b + 2));

				(valid(a + 1, b + 2));

				(valid(a + 2, b + 1));

				(valid(a + 2, b - 1));

				(valid(a + 1, b - 2));

				(valid(a - 1, b - 2));

				(valid(a - 2, b - 1));
			}

			//Display Rules
			if (X > 930 && X < 1000 && Y > 200 && Y < 280)
			{
				seeRules = 1;
			}
		}

		//sleep the window for 3 seconds
		//sf::sleep(sf::milliseconds(5000));
		//gamewindow.clear();

		//clears everything
		gamewindow.clear();
		gamewindow.draw(background);

		for (int x = 0; x < N; x++)
		{
			for (int y = 0; y < N; y++)
			{
				gamewindow.draw(tileMap[x][y]);
			}
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (grid[i][j] == 2)
				{

					//draw the enmy
					enemy.setPosition(i * gridSizef, j * gridSizef);
					gamewindow.draw(enemy);

					// dj.run(posX, posY, i/30, j/30, grid);
				}

				if (grid[i][j] == 3 && posY == j && posX == i)
				{

					cout << "Enemy attacked you!!!";
					attsound.play();
					life--;

					grid[i][j] = 1;
				}

				if (grid[i][j] == 2 && posY == j && posX == i)
				{

					int a = i, b = j;
					cout << "you killed enemy";
					killsound.play();

					grid[i][j] = 1;
					life++;

					if (grid[a + 1][b + 1] == 3)
					{
						grid[a + 1][b + 1] = 1;
					}

					if (grid[a + 1][b - 1] == 3)
					{
						grid[a + 1][b - 1] = 1;
					}

					if (grid[a - 1][b + 1] == 3)
					{
						grid[a - 1][b + 1] = 1;
					}

					if (grid[a - 1][b - 1] == 3)
					{
						grid[a - 1][b - 1] = 1;
					}

					// dj.run(posX, posY, i/30, j/30, grid);
				}
			}
		}
		gamewindow.draw(box);
		gamewindow.draw(display);

		//display the number of moves remaining
		coin.setPosition(930, 40);
		gamewindow.draw(coin);

		//display the number of life
		lfe.setPosition(930, 120);
		gamewindow.draw(lfe);

		//display the rules of game
		rules.setPosition(930, 200);
		gamewindow.draw(rules);

		//Game hint, launch dijkstra algorithm
		hint.setPosition(930, 280);
		gamewindow.draw(hint);

		//number of moves recorded
		move.setPosition(945, 55);
		m.setPosition(955, 75);
		gamewindow.draw(move);

		//number of lives recorded
		lives.setPosition(950, 135);
		l.setPosition(960, 155);
		gamewindow.draw(lives);

		//Rules of the game
		rule.setPosition(950, 225);
		gamewindow.draw(rule);

		//hint of the game
		h.setPosition(950, 305);
		gamewindow.draw(h);

		//to conert int to string
		stringstream smove, slife;

		smove << moves;
		m.setString(smove.str());
		gamewindow.draw(m);

		slife << life;
		l.setString(slife.str());
		gamewindow.draw(l);

		if (!bf.pathD.empty())
		{
			int sequence = 1;
			for (int i = bf.pathD.size() - 1; i >= 0; i--)
			{
				sPath.setPosition(bf.pathD[i].first * gridSizef + 10, bf.pathD[i].second * gridSizef + 10); //Reversed notion of row & column
				gamewindow.draw(sPath);																		//final pathD
																											//to conert int to string
				stringstream seq;
				seq << sequence;
				order.setString(seq.str());
				order.setPosition(bf.pathD[i].first * gridSizef + 28, bf.pathD[i].second * gridSizef + 15);
				gamewindow.draw(order);
				sequence++;
			}
		}

		if (seeRules == 1)
		{
			gamewindow.draw(rulesSprite);
			gamewindow.display();
			sf::sleep(sf::seconds(7));
			seeRules = 0;
		}

		//displaying knight on chessboard
		gamewindow.draw(knight);
		gamewindow.draw(princess);
		//display everything on window
		gamewindow.display();
	}
}

void level2::nextlevel(sf::RenderWindow &window)
{

	// sf::RenderWindow window;

	sf::RectangleShape next, exit;
	GameLevel3 l3(moves);

	sf::Texture texbg;
	sf::Sprite bg;

	// window.create(sf::VideoMode(1000, 900), "Arthur-The Gradiator", sf::Style::Default);

	texbg.loadFromFile("media/win2.png");
	bg.setTexture(texbg);
	bg.setScale(1000 / bg.getGlobalBounds().width, 900 / bg.getGlobalBounds().height);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				int X = event.mouseButton.x;
				int Y = event.mouseButton.y;
				int row = Y / 30; //Reversed notion of row & column
				int col = X / 30;

				if (X > 700 && X < 1000 && Y > 692 && Y < 900)
				{
					// window.close();
					l3.start(window);
				}
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
			{
				// window.close();
				l3.start(window);
			}
		}

		window.clear();
		window.draw(bg);
		window.display();
	}
}