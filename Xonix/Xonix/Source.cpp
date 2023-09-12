#include <SFML/Graphics.hpp>
#include <time.h>

const int M = 25;
const int N = 40;

int grid[M][N] = { 0 };
int tileSize = 18;

struct Enemy
{
	int x, y, dx, dy;

	Enemy() 
	{
		x = y = 300;

		dx = 4 - rand() % 8;
		dy = 4 - rand() % 8;
	}

	void move() 
	{
		x += dx;
		if (grid[y / tileSize][x / tileSize] == 1) 
		{
			dx = -dx;
			x += dx;
		}
		y += dy;
		if (grid[y / tileSize][x / tileSize] == 1) 
		{
			dy = -dy;
			y += dy;
		}
	}
};

void drop(int y, int x) 
{
	if (grid[y][x] == 0)
		grid[y][x] = -1;

	if (grid[y - 1][x] == 0)
		drop(y - 1, x);
	if (grid[y + 1][x] == 0)
		drop(y + 1, x);
	if (grid[y][x - 1] == 0)
		drop(y, x - 1);
	if (grid[y][x + 1] == 0)
		drop(y, x + 1);
}

int main() 
{
	srand(time(0));

	sf::RenderWindow window(sf::VideoMode(N * tileSize, M * tileSize), "Xonix!");
	window.setFramerateLimit(60);

	sf::Texture texture;
	texture.loadFromFile("Paint/tiles.png");
	sf::Sprite tile(texture);

	int x = 0, y = 0, dx = 0, dy = 0;
	float timer = 0, delay = 0.07;
	sf::Clock clock;

	for (int i = 0; i < M; i++)
		for (int j = 0; j < N; j++)
			if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
				grid[i][j] = 1;

	sf::Texture go;
	go.loadFromFile("Paint/gameover.png");
	sf::Sprite GameOver(go);
	GameOver.setPosition(100, 100);
	bool Game = true;

	sf::Texture textureEnemy;
	textureEnemy.loadFromFile("Paint/Enemy.png");
	sf::Sprite EnemySprite(textureEnemy);
	EnemySprite.setOrigin(20, 20);

	int EnemyCount = 4;
	Enemy a[10];

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (Game) 
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
			{
				dx = 1;
				dy = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
			{
				dx = -1;
				dy = 0;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
			{
				dx = 0;
				dy = -1;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
			{
				dx = 0;
				dy = 1;
			}

			if (timer > delay) 
			{
				x += dx;
				y += dy;

				if (x < 0)
					x = 0;
				if (x > N - 1)
					x = N - 1;
				if (y < 0)
					y = 0;
				if (y > M - 1)
					y = M - 1;

				if (grid[y][x] == 2)
					Game = false;
				if (grid[y][x] == 0)
					grid[y][x] = 2;

				timer = 0;
			}

			for (int i = 0; i < EnemyCount; i++)
				a[i].move();

			if (grid[y][x] == 1) 
			{
				dx = dy = 0;

				for (int i = 0; i < EnemyCount; i++)
					drop(a[i].y / tileSize, a[i].x / tileSize);

				for (int i = 0; i < M; i++)
					for (int j = 0; j < N; j++)
						if (grid[i][j] == -1)
							grid[i][j] = 0;
						else grid[i][j] = 1;
			}

			for (int i = 0; i < EnemyCount; i++)
				if (grid[a[i].y / tileSize][a[i].x / tileSize] == 2)
					Game = false;
		}

		window.clear();

		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++) 
			{
				if (grid[i][j] == 0)
					continue;
				if (grid[i][j] == 1)
					tile.setTextureRect(sf::IntRect(0, 0, tileSize, tileSize));
				if (grid[i][j] == 2)
					tile.setTextureRect(sf::IntRect(36, 0, tileSize, tileSize));

				tile.setPosition(j * tileSize, i * tileSize);
				window.draw(tile);
			}

		tile.setTextureRect(sf::IntRect(18, 0, tileSize, tileSize));
		tile.setPosition(x * tileSize, y * tileSize);
		window.draw(tile);

		EnemySprite.rotate(10);
		for (int i = 0; i < EnemyCount; i++) 
		{
			EnemySprite.setPosition(a[i].x, a[i].y);
			window.draw(EnemySprite);
		}

		if (!Game)
			window.draw(GameOver);

		window.display();
	}

	return 0;
}