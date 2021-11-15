#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <vector>
#include <String.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <windows.h>

using namespace std;
using namespace sf;

void Erase();
void ScoreUpdate(char thousand, char hundred, char ten, char unit, int num);
void Automove(int i);
void Checkgap();
void Itemcheck();
void RecordScore();
void ReadScore();

struct Tetris
{
	Sprite cube;
	int cubecolor;
	Vector2f position;
	bool state;
	bool leftsideblock;
	bool rightsideblock;
	bool havegaptoblock;
	bool havegaptoground;
	bool isErase;
};
struct Nameenter 
{
	string name;
	bool getname;
}nameenter;
struct Leader
{
	string name;
	int score;
	string scoretext;
	bool writed=false;
};
struct High
{
	string name;
	int score;
	string scoretext;
};

struct Highscore {
	Text Title;
	String name;
	Text nametitle;
	Text scoretitle;
	Text Firstname;
	Text Firstscore;
	Text Secondname;
	Text Secondscore;
	Text Thirdname;
	Text Thirdscore;
	Text Forthname;
	Text Forthscore;
	Text Fifthname;
	Text Fifthscore;
	bool active;
}Leaderboard;

Sprite color[2][4];
Sprite bomb;
Sprite temp_next;
Sprite nextColor[2][5];
vector<Tetris> blocks;
Leader leader;
Leader read;
High high[5];

int pos_x[] = { 0+10,64+10,128+10,192+10};
int pos_y[] = { 10 };
int temp_x,temp_color_i,temp_color_j;
int next_i, next_j;
int color_spec[2][5] = { {1,2,3,4,9},{5,6,7,8,10} };
char scorecount[5]{'0','0','0','0',NULL};
int totalscore=0;
int itemscore=0;
bool erasecheck = false;
int numerase = 0;
bool gameover = false;
bool item=false;
bool Gamestart = false;
char Selection[3] = {'s','h','e'};
char Nowselect = Selection[0];
float delay_move = 0;
float delay_color = 0;
float delay_over = 0;
float delayMax_move = 0.14;
float deltaTime;
float timepassed;
float speed=225.f;

int main()
{
	srand(time(NULL));
	Clock clock;
	RenderWindow window;

	Texture blockTexture;
	blockTexture.loadFromFile("Sprite/blocks.png");
	blockTexture.setSmooth(true);
	Vector2u blocksize = blockTexture.getSize();

	Texture bombTexture;
	bombTexture.loadFromFile("Sprite/bomb.png");
	bombTexture.setSmooth(true);
	Vector2u bombsize = bombTexture.getSize();

	Image icon;
	icon.loadFromFile("Icon/cube.png");

	Font font;
	font.loadFromFile("Font/BRLNSDB.ttf");
	Text score;
	Text scoreCount;
	Text Gameover;
	Text Gamename;
	Text Myname;
	Text Start;
	Text Highscore;
	Text Exit;
	Text Entername;
	Text player;
	String nameinput;
	score.setFont(font);
	scoreCount.setFont(font);
	Gameover.setFont(font);
	Gamename.setFont(font);
	Myname.setFont(font);
	Start.setFont(font);
	Highscore.setFont(font);
	Exit.setFont(font);
	Entername.setFont(font);
	player.setFont(font);

	window.create(sf::VideoMode(((blocksize.x / 8) * 4 * 2)+200+250, (((blocksize.x / 8) * 10 * 2)+10)), "Not A Tetris", sf::Style::Close);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	//window.setFramerateLimit(200);
	RectangleShape Status_display(Vector2f(170.f, 630.f));
	Status_display.setFillColor(Color::Transparent);
	Status_display.setPosition(Vector2f(276.f, 10.f));
	Status_display.setOutlineThickness(10.f);
	Status_display.setOutlineColor(Color::White);

	RectangleShape Game_display(Vector2f(256.f, 630.f));
	Game_display.setFillColor(Color::Transparent);
	Game_display.setPosition(Vector2f(10.f, 10.f));
	Game_display.setOutlineThickness(10.f);
	Game_display.setOutlineColor(Color::White);

	RectangleShape Name_display(Vector2f(256.f, 150.f));
	Name_display.setFillColor(Color::Transparent);
	Name_display.setPosition(Vector2f(10.f, 235.f));
	Name_display.setOutlineThickness(10.f);
	Name_display.setOutlineColor(Color::White);

	RectangleShape Next_display(Vector2f(170.f, 170.f));
	Next_display.setFillColor(Color::Transparent);
	Next_display.setPosition(Vector2f(276.f, 10.f));
	Next_display.setOutlineThickness(10.f);
	Next_display.setOutlineColor(Color::White);

	score.setCharacterSize(30);
	score.setString("SCORE");
	score.setFillColor(Color::White);
	score.setOrigin(Vector2f(score.getGlobalBounds().width / 2, score.getGlobalBounds().height / 2));
	score.setPosition(Vector2f(Status_display.getPosition().x - 10 + Status_display.getGlobalBounds().width / 2, (Next_display.getPosition().y + Next_display.getGlobalBounds().height / 2) + 200));

	scoreCount.setCharacterSize(25);
	scoreCount.setString(scorecount);
	scoreCount.setFillColor(Color::White);
	scoreCount.setOrigin(Vector2f(scoreCount.getGlobalBounds().width / 2, scoreCount.getGlobalBounds().height / 2));
	scoreCount.setPosition(Vector2f(Status_display.getPosition().x - 10 + Status_display.getGlobalBounds().width / 2
		, (Next_display.getPosition().y + Next_display.getGlobalBounds().height / 2) + 200 + score.getGlobalBounds().height + 10));

	Gameover.setCharacterSize(40);
	Gameover.setString("GAME OVER");
	Gameover.setFillColor(Color::White);
	Gameover.setOrigin(Vector2f(Gameover.getGlobalBounds().width / 2, Gameover.getGlobalBounds().height / 2));
	Gameover.setPosition(Vector2f(Game_display.getPosition().x - 10 + (Game_display.getGlobalBounds().width / 2), Game_display.getPosition().y + (Game_display.getGlobalBounds().height / 2)));

	Gamename.setCharacterSize(50);
	Gamename.setString("  NOT\n    A\nTETRIS");
	Gamename.setFillColor(Color::White);
	Gamename.setOrigin(Vector2f(Gamename.getGlobalBounds().width / 2, Gamename.getGlobalBounds().height / 2));
	Gamename.setPosition(Vector2f(580.f,100.f));

	Myname.setCharacterSize(15);
	Myname.setString("Patipat Koeddang ID:64010475");
	Myname.setFillColor(Color::White);
	Myname.setOrigin(Vector2f(Myname.getGlobalBounds().width / 2, Myname.getGlobalBounds().height / 2));
	Myname.setPosition(Vector2f(580.f, 630.f));

	Start.setCharacterSize(30);
	Start.setString("START");
	Start.setFillColor(Color::Red);
	Start.setOrigin(Vector2f(Start.getGlobalBounds().width / 2, Start.getGlobalBounds().height / 2));
	Start.setPosition(Vector2f(580.f, 350.f));

	Highscore.setCharacterSize(30);
	Highscore.setString("HIGH SCORE");
	Highscore.setFillColor(Color::White);
	Highscore.setOrigin(Vector2f(Highscore.getGlobalBounds().width / 2, Highscore.getGlobalBounds().height / 2));
	Highscore.setPosition(Vector2f(580.f, Start.getPosition().y + 50));

	Exit.setCharacterSize(30);
	Exit.setString("EXIT");
	Exit.setFillColor(Color::White);
	Exit.setOrigin(Vector2f(Exit.getGlobalBounds().width / 2, Exit.getGlobalBounds().height / 2));
	Exit.setPosition(Vector2f(580.f, Highscore.getPosition().y+50));

	Entername.setCharacterSize(30);
	Entername.setString("Enter Name");
	Entername.setFillColor(Color::White);
	Entername.setOrigin(Vector2f(Exit.getGlobalBounds().width / 2, Exit.getGlobalBounds().height / 2));
	Entername.setPosition(Vector2f(45.f, 270.f));

	player.setCharacterSize(25);
	player.setFillColor(Color::White);
	player.setPosition(Vector2f(Entername.getPosition().x-(Exit.getGlobalBounds().width / 2), Entername.getPosition().y+60));

	Leaderboard.Title.setFont(font);
	Leaderboard.Title.setCharacterSize(30);
	Leaderboard.Title.setString("LEADERBOARD");
	Leaderboard.Title.setFillColor(Color::White);
	Leaderboard.Title.setOrigin(Vector2f(Leaderboard.Title.getGlobalBounds().width / 2, Leaderboard.Title.getGlobalBounds().height / 2));
	Leaderboard.Title.setPosition(Vector2f(580.f, 230.f));

	Leaderboard.nametitle.setFont(font);
	Leaderboard.nametitle.setCharacterSize(25);
	Leaderboard.nametitle.setString("Name");
	Leaderboard.nametitle.setFillColor(Color::White);
	Leaderboard.nametitle.setPosition(Vector2f(466.f, Leaderboard.Title.getPosition().y+30));

	Leaderboard.scoretitle.setFont(font);
	Leaderboard.scoretitle.setCharacterSize(25);
	Leaderboard.scoretitle.setString("Score");
	Leaderboard.scoretitle.setFillColor(Color::White);
	Leaderboard.scoretitle.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x+170, Leaderboard.nametitle.getPosition().y));

	Leaderboard.Firstname.setFont(font);
	Leaderboard.Firstname.setCharacterSize(20);
	Leaderboard.Firstname.setFillColor(Color::White);
	Leaderboard.Firstname.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x , Leaderboard.nametitle.getPosition().y+50));

	Leaderboard.Firstscore.setFont(font);
	Leaderboard.Firstscore.setCharacterSize(20);
	Leaderboard.Firstscore.setFillColor(Color::White);
	Leaderboard.Firstscore.setPosition(Vector2f(Leaderboard.scoretitle.getPosition().x, Leaderboard.Firstname.getPosition().y));

	Leaderboard.Secondname.setFont(font);
	Leaderboard.Secondname.setCharacterSize(20);
	Leaderboard.Secondname.setFillColor(Color::White);
	Leaderboard.Secondname.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x, Leaderboard.Firstname.getPosition().y + 50));

	Leaderboard.Secondscore.setFont(font);
	Leaderboard.Secondscore.setCharacterSize(20);
	Leaderboard.Secondscore.setFillColor(Color::White);
	Leaderboard.Secondscore.setPosition(Vector2f(Leaderboard.scoretitle.getPosition().x, Leaderboard.Secondname.getPosition().y));

	Leaderboard.Thirdname.setFont(font);
	Leaderboard.Thirdname.setCharacterSize(20);
	Leaderboard.Thirdname.setFillColor(Color::White);
	Leaderboard.Thirdname.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x, Leaderboard.Secondname.getPosition().y + 50));

	Leaderboard.Thirdscore.setFont(font);
	Leaderboard.Thirdscore.setCharacterSize(20);
	Leaderboard.Thirdscore.setFillColor(Color::White);
	Leaderboard.Thirdscore.setPosition(Vector2f(Leaderboard.scoretitle.getPosition().x, Leaderboard.Thirdname.getPosition().y));

	Leaderboard.Forthname.setFont(font);
	Leaderboard.Forthname.setCharacterSize(20);
	Leaderboard.Forthname.setFillColor(Color::White);
	Leaderboard.Forthname.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x, Leaderboard.Thirdname.getPosition().y + 50));

	Leaderboard.Forthscore.setFont(font);
	Leaderboard.Forthscore.setCharacterSize(20);
	Leaderboard.Forthscore.setFillColor(Color::White);
	Leaderboard.Forthscore.setPosition(Vector2f(Leaderboard.scoretitle.getPosition().x, Leaderboard.Forthname.getPosition().y));

	Leaderboard.Fifthname.setFont(font);
	Leaderboard.Fifthname.setCharacterSize(20);
	Leaderboard.Fifthname.setFillColor(Color::White);
	Leaderboard.Fifthname.setPosition(Vector2f(Leaderboard.nametitle.getPosition().x, Leaderboard.Forthname.getPosition().y + 50));

	Leaderboard.Fifthscore.setFont(font);
	Leaderboard.Fifthscore.setCharacterSize(20);
	Leaderboard.Fifthscore.setFillColor(Color::White);
	Leaderboard.Fifthscore.setPosition(Vector2f(Leaderboard.scoretitle.getPosition().x, Leaderboard.Fifthname.getPosition().y));

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			color[i][j].setTexture(blockTexture);
			color[i][j].setTextureRect(sf::IntRect(j * 32, i * 32, blocksize.x / 8, blocksize.y / 5));
			color[i][j].setScale(2, 2);
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			nextColor[i][j].setTexture(blockTexture);
			nextColor[i][j].setTextureRect(sf::IntRect(j * 32, i * 32, blocksize.x / 8, blocksize.y / 5));
			nextColor[i][j].setScale(2, 2);
		}
	}
	bomb.setTexture(bombTexture);
	bomb.setTextureRect(IntRect(0, 0, bombsize.x, bombsize.y));
	bomb.setScale(0.3,0.3);
	bomb.setOrigin(bomb.getGlobalBounds().width/3, bomb.getGlobalBounds().height);
	bomb.setPosition(score.getPosition().x-30, window.getSize().y - 70);

	

	next_i = rand() % 2;
	next_j = rand() % 4;
	temp_next = nextColor[next_i][next_j];
	temp_next.setOrigin(Vector2f(temp_next.getGlobalBounds().width/3, temp_next.getGlobalBounds().height / 3));
	temp_next.setPosition(Vector2f(Next_display.getPosition().x + Next_display.getGlobalBounds().width/2, Next_display.getPosition().y + Next_display.getGlobalBounds().height / 2));

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		delay_color += deltaTime;
		delay_move += deltaTime;
		delay_over += deltaTime;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if ((int)delay_color == 1.f) {
			Gamename.setFillColor(Color::Red);
		}
		if ((int)delay_color == 2.f) {
			Gamename.setFillColor(Color::Blue);
		}
		if ((int)delay_color == 3.f) {
			Gamename.setFillColor(Color::Yellow);
		}
		if ((int)delay_color == 4.f) {
			Gamename.setFillColor(Color::Green);
		}
		if ((int)delay_color == 5.f) {
			Gamename.setFillColor(Color::Cyan);
		}
		if ((int)delay_color == 6.f) {
			Gamename.setFillColor(Color::Magenta);
			delay_color = 0.f;
		}
		if (!Gamestart && !Leaderboard.active) {
			Gamename.setPosition(Vector2f(580.f, 100.f));
			if (Keyboard::isKeyPressed(Keyboard::Down) && delay_move >= 0.2f && Nowselect == Selection[0]) {
				Start.setFillColor(Color::White);
				Highscore.setFillColor(Color::Red);
				Exit.setFillColor(Color::White);
				Nowselect = Selection[1];
				delay_move = 0.f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && delay_move >= 0.2f && Nowselect == Selection[1]) {
				Start.setFillColor(Color::White);
				Highscore.setFillColor(Color::White);
				Exit.setFillColor(Color::Red);
				Nowselect = Selection[2];
				delay_move = 0.f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) && delay_move >= 0.2f && Nowselect == Selection[2]) {
				Start.setFillColor(Color::White);
				Highscore.setFillColor(Color::Red);
				Exit.setFillColor(Color::White);
				Nowselect = Selection[1];
				delay_move = 0.f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) && delay_move >= 0.2f && Nowselect == Selection[1]) {
				Start.setFillColor(Color::Red);
				Highscore.setFillColor(Color::White);
				Exit.setFillColor(Color::White);
				Nowselect = Selection[0];
				delay_move = 0.f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter) && Nowselect==Selection[2]) {
				window.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter) && Nowselect == Selection[1] && delay_move >= 0.3f) {
				Leaderboard.active = true;
				delay_move = 0.f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter) && Nowselect == Selection[0] && delay_move >= 0.3f) {
				nameenter.getname = true;
				gameover = false;
				delay_move = 0.f;
		}
		if (nameenter.getname) {
			if (event.type == sf::Event::TextEntered && delay_move >= 0.2f) {
				if (event.text.unicode=='\b' && nameenter.name.size() > 0) {
					nameenter.name.erase(nameenter.name.size() - 1, 1);
				}
				else if(event.text.unicode<128 && nameenter.name.size() < 16 && event.text.unicode != 27 && event.text.unicode != '\b') {
					nameenter.name += event.text.unicode;
				}
				delay_move =0.f;
			}
			player.setString(nameenter.name);
			if (Keyboard::isKeyPressed(Keyboard::Enter) && delay_move >= 0.1f && nameenter.name.size() > 0) {
				leader.name = nameenter.name;
				leader.writed = true;
				Gamestart = true;
				nameenter.getname = false;
				for (int i = 0; i < 4; i++) {
					scorecount[i] = '0';
				}
				totalscore = 0;
				itemscore = 0;
				item = false;
				scoreCount.setString(scorecount);
				if (!gameover) {
					temp_x = pos_x[rand() % 4];
					temp_color_i = rand() % 2;
					temp_color_j = rand() % 4;
					blocks.push_back({ color[temp_color_i][temp_color_j],color_spec[temp_color_i][temp_color_j],Vector2f(temp_x,pos_y[0]),true,false,false,false,false,false });
				}
				leader.name = nameenter.name;
				leader.scoretext = scorecount;
				leader.score = totalscore;
				delay_move = 0.f;
			}
		}
		}
		if (Leaderboard.active) {
			ReadScore();
			Leaderboard.Firstname.setString(high[0].name);
			Leaderboard.Firstscore.setString(high[0].scoretext);
			Leaderboard.Secondname.setString(high[1].name);
			Leaderboard.Secondscore.setString(high[1].scoretext);
			Leaderboard.Thirdname.setString(high[2].name);
			Leaderboard.Thirdscore.setString(high[2].scoretext);
			Leaderboard.Forthname.setString(high[3].name);
			Leaderboard.Forthscore.setString(high[3].scoretext);
			Leaderboard.Fifthname.setString(high[4].name);
			Leaderboard.Fifthscore.setString(high[4].scoretext);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && delay_move >= 0.2f) {
				Leaderboard.active = false;
				delay_move = 0.f;

			}
		}
		if (Gamestart && !nameenter.getname) {
			timepassed += deltaTime;
			Gamename.setPosition(Vector2f(580.f, 320.f));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				Gamestart = false;
			}
			if (timepassed < 300) {
				speed = speed;
			}
			else if (timepassed >= 300 && timepassed <= 600) {
				speed = 280;
			}
			else if (timepassed > 600) {
				speed = 330;
			}
			//Checkblocks collision
			for (size_t i = blocks.size() - 1; i < blocks.size(); i++) {
				for (size_t j = 0; j < blocks.size() - 1; j++) {
					if (i != j && (int)blocks[i].cube.getPosition().y + (int)blocks[i].cube.getGlobalBounds().height >= (int)blocks[j].cube.getPosition().y && blocks[i].state
						&& blocks[i].cube.getPosition().x == blocks[j].cube.getPosition().x && !gameover) {
						blocks[i].cube.setPosition(blocks[i].position.x, blocks[j].position.y - 64);
						blocks[i].state = false;
						temp_x = pos_x[rand() % 4];
						temp_color_i = rand() % 2;
						temp_color_j = rand() % 4;
						blocks.push_back({ color[temp_color_i][temp_color_j],color_spec[temp_color_i][temp_color_j],Vector2f(temp_x,pos_y[0]),true,false,false,false,false,false });
						break;
					}
					//fallfornblocktoblock
					for (size_t j = 0; j < blocks.size() - 1; j++) {
						if (i != j && (int)blocks[i].cube.getPosition().y + (int)blocks[i].cube.getGlobalBounds().height <= (int)blocks[j].cube.getPosition().y && blocks[i].state
							&& blocks[i].cube.getPosition().x == blocks[j].cube.getPosition().x && !gameover && blocks[i].havegaptoblock) {
							blocks[i].cube.setPosition(blocks[i].position.x, blocks[j].position.y - 64);
							blocks[i].state = false;
							blocks[i].havegaptoblock = false;
							break;
						}
					}
					//fallfornblocktoground
					for (size_t j = 0; j < blocks.size() - 1; j++) {
						if (i != j && (int)blocks[i].cube.getPosition().y + (int)blocks[i].cube.getGlobalBounds().height >= 576 && !gameover && blocks[i].havegaptoground) {
							blocks[i].cube.setPosition(blocks[i].position.x, 576.f);
							blocks[i].state = false;
							blocks[i].havegaptoground = false;
							break;
						}
					}
					//leftcollision
					if (i != j && blocks[i].cube.getPosition().x == blocks[j].cube.getPosition().x + blocks[j].cube.getGlobalBounds().width
						&& blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height >= blocks[j].cube.getPosition().y && !gameover) {
						blocks[i].leftsideblock = true;
					}
					else {
						blocks[i].leftsideblock = false;
					}
					//rightcollistion
					if (i != j && blocks[i].cube.getPosition().x + blocks[j].cube.getGlobalBounds().width == blocks[j].cube.getPosition().x
						&& blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height >= blocks[j].cube.getPosition().y && !gameover) {
						blocks[i].rightsideblock = true;
					}
					else {
						blocks[i].rightsideblock = false;
					}
				}
				//Keypress A
				if (Keyboard::isKeyPressed(Keyboard::A) && delay_move >= delayMax_move && blocks[i].state && blocks[i].cube.getPosition().x > 10 && !(blocks[i].leftsideblock)) {
					blocks[i].cube.setPosition(blocks[i].position.x -= 64.f, blocks[i].position.y && !gameover);
					delay_move = 0.f;
				}
				//Keypress D
				if (Keyboard::isKeyPressed(Keyboard::D) && delay_move >= delayMax_move && blocks[i].state && blocks[i].cube.getPosition().x + blocks[i].cube.getGlobalBounds().width < 266 && !(blocks[i].rightsideblock)) {
					blocks[i].cube.setPosition(blocks[i].position.x += 64.f, blocks[i].position.y && !gameover);
					delay_move = 0.f;
				}
				//Keypress S
				if (Keyboard::isKeyPressed(Keyboard::S) && blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height < 640 && blocks[i].state && !gameover) {
					blocks[i].cube.setPosition(blocks[i].position.x, blocks[i].position.y += (deltaTime * speed * 2.f));
					delay_move = 0.f;
				}
				//Keypress Spcae
				if (Keyboard::isKeyPressed(Keyboard::Space) && delay_move >= 0.2
					&& blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height < 640 && blocks[i].state && !gameover) {
					blocks[i].cube = color[next_i][next_j];
					blocks[i].cubecolor = color_spec[next_i][next_j];
					blocks[i].cube.setPosition(blocks[i].position.x, blocks[i].position.y);
					next_i = rand() % 2;
					next_j = rand() % 4;
					temp_next = nextColor[next_i][next_j];
					temp_next.setPosition(temp_next.getPosition().x, temp_next.getPosition().y);
					temp_next.setOrigin(Vector2f(temp_next.getGlobalBounds().width / 3, temp_next.getGlobalBounds().height / 3));
					temp_next.setPosition(Vector2f(Next_display.getPosition().x + Next_display.getGlobalBounds().width / 2, Next_display.getPosition().y + Next_display.getGlobalBounds().height / 2));
					delay_move = 0.f;
				}
				//Keypress LShift
				if (Keyboard::isKeyPressed(Keyboard::LShift) && item) {
					for (int i = 0; i < blocks.size() - 1; i++) {
						if ((int)blocks[i].cube.getPosition().y == 576) {
							erasecheck = true;
							blocks[i].isErase = true;
							numerase = 3;
						}
					}
					item = false;
					itemscore = totalscore;
				}
				//Checkground collision
				if (blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height >= 640 && blocks[i].state && !gameover) {
					blocks[i].state = false;
					blocks[i].cube.setPosition(blocks[i].position.x, 576.f);
					temp_x = pos_x[rand() % 4];
					temp_color_i = rand() % 2;
					temp_color_j = rand() % 4;
					blocks.push_back({ color[temp_color_i][temp_color_j],color_spec[temp_color_i][temp_color_j],Vector2f(temp_x,pos_y[0]),true,false,false,false,false,false });
					break;
				}
				//Checkground collision when fall from check color.
				if ((int)blocks[i].cube.getPosition().y + (int)blocks[i].cube.getGlobalBounds().height >= 640 && !gameover && blocks[i].havegaptoground) {
					blocks[i].cube.setPosition(blocks[i].position.x, 576.f);
					blocks[i].state = false;
					blocks[i].havegaptoground = false;
					break;
				}
			}
			Itemcheck();
			Erase();
			Checkgap();
			//scorerun and get value to total score
			if (erasecheck && !gameover) {
				ScoreUpdate(scorecount[0], scorecount[1], scorecount[2], scorecount[3],numerase);
				totalscore = ((scorecount[0] - 48) * 1000) + ((scorecount[1] - 48) * 100) + ((scorecount[2] - 48) * 10) + (scorecount[3] - 48);
				erasecheck = false;
				scoreCount.setString(scorecount);
				leader.scoretext = scorecount;
			}
			//Gameovercheck
			for (int i = 0; i < blocks.size() - 1; i++) {
				if ((int)blocks[i].position.y <= 10) {
						gameover = true;
						Gamestart = false;
						leader.score = totalscore;
						delay_move = 0.f;
						delay_over = 0.f;
						RecordScore();
						nameenter.name.clear();
				}
			}
			//Automove all available locks
			for (int i = 0; i < blocks.size(); i++) {
				Automove(i);
			}
			//fix a bug of falling object
			for (int i = 0; i < blocks.size() - 1; i++) {
				if (blocks[i].position.y >= 576) {
					blocks[i].state = false;
				}
			}
		}
		//Render
		window.clear();
		for (size_t i = 0; i < blocks.size(); i++) {
			window.draw(blocks[i].cube);
		}
		if (!Gamestart && !Leaderboard.active) {
			window.draw(Start);
			window.draw(Highscore);
			window.draw(Exit);
			if (delay_over >= 0.3f) {
				blocks.clear();
				delay_over = 0.f;
			}
		}
		if (nameenter.getname) {
			window.draw(Entername);
			window.draw(player);
			window.draw(Name_display);
		}
		if (Gamestart) {
			window.draw(temp_next);
		}
		if (gameover) {
			if (delay_over >= 0.3f) {
				blocks.clear();
				delay_over = 0.f;
			}
			window.draw(Gameover);
		}
		if (Leaderboard.active) {
			window.draw(Leaderboard.Title);
			window.draw(Leaderboard.nametitle);
			window.draw(Leaderboard.scoretitle);
			window.draw(Leaderboard.Firstname);
			window.draw(Leaderboard.Firstscore);
			window.draw(Leaderboard.Secondname);
			window.draw(Leaderboard.Secondscore);
			window.draw(Leaderboard.Thirdname);
			window.draw(Leaderboard.Thirdscore);
			window.draw(Leaderboard.Forthname);
			window.draw(Leaderboard.Forthscore);
			window.draw(Leaderboard.Fifthname);
			window.draw(Leaderboard.Fifthscore);
		}
		window.draw(Game_display);
		window.draw(Next_display);
		window.draw(Status_display);
		window.draw(score);
		window.draw(Gamename);
		window.draw(Myname);
		window.draw(scoreCount);
		if (item==true) {
			window.draw(bomb);
		}
		window.display();
	}
	return 0;
}
void Erase() {
	if (blocks.size() >= 2) {
		for (size_t i = 0; i < blocks.size(); i++) {
			for (size_t j = i; j < blocks.size(); j++) {
				if (i != j && blocks[i].cube.getPosition().x == blocks[j].cube.getPosition().x
					&& (int)blocks[j].cube.getPosition().y + (int)blocks[j].cube.getGlobalBounds().height >= (int)blocks[i].cube.getPosition().y
					&& blocks[i].cubecolor == blocks[j].cubecolor && blocks[i].state == false && blocks[j].state == false) {
					erasecheck = true;
					numerase += 1;
					blocks[i].isErase = true;
					blocks[j].isErase = true;
				}
			}
		}
	}
	if (blocks.size() >= 2) {
		for (size_t j = 0; j < blocks.size()-1; j++) {
			for (size_t i = j; i < blocks.size()-1; i++) {
				if (i != j && (int)blocks[i].cube.getPosition().y == (int)blocks[j].cube.getPosition().y
					&& blocks[j].cube.getPosition().x + blocks[j].cube.getGlobalBounds().width >= blocks[i].cube.getPosition().x
					&& blocks[j].cube.getPosition().x + blocks[j].cube.getGlobalBounds().width - blocks[i].cube.getPosition().x <= 66
					&& blocks[i].cubecolor == blocks[j].cubecolor && blocks[i].state == false && blocks[j].state == false) {
					erasecheck = true;
					numerase += 1;
					blocks[i].isErase = true;
					blocks[j].isErase = true;
				}
			}
		}
	}
	if (blocks.size() >= 2) {
		for (size_t j = 0; j < blocks.size() - 1; j++) {
			for (size_t i = j; i < blocks.size() - 1; i++) {
				if (i != j && (int)blocks[i].cube.getPosition().y == (int)blocks[j].cube.getPosition().y
					&& blocks[i].cube.getPosition().x + blocks[i].cube.getGlobalBounds().width >= blocks[j].cube.getPosition().x
					&& blocks[i].cube.getPosition().x + blocks[i].cube.getGlobalBounds().width - blocks[j].cube.getPosition().x <= 66
					&& blocks[i].cubecolor == blocks[j].cubecolor && blocks[i].state == false && blocks[j].state == false) {
					erasecheck = true;
					numerase += 1;
					blocks[i].isErase = true;
					blocks[j].isErase = true;
				}
			}
		}
	}
	for (size_t i = 0; i < blocks.size() - 1;i++) {
		if (blocks[i].isErase) {
			for (size_t j = 0; j < blocks.size() - 1; j++) {
				if (blocks[j].isErase ) {
					blocks.erase(blocks.begin() + j);
				}
			}
		}
	}
}
void ScoreUpdate(char thousand, char hundred, char ten, char unit,int num) {
	for (int i = 0; i < num;i++) {
		unit++;
		if (unit > '9') {
			unit = '0';
			ten++;
			if (ten > '9') {
				ten = '0';
				hundred++;
				if (hundred > '9') {
					hundred = '0';
					thousand++;
				}
			}
		}
	}
	scorecount[0] = thousand;
	scorecount[1] = hundred;
	scorecount[2] = ten;
	scorecount[3] = unit;
	numerase=0;
}
void Automove(int i) {
	if ((int)blocks[i].cube.getPosition().y + blocks[i].cube.getGlobalBounds().height < 640 && blocks[i].state && !gameover) {
		blocks[i].cube.setPosition(blocks[i].position.x, blocks[i].position.y += (deltaTime * speed));
	}
}
void Checkgap() {
	for (size_t i = 0; i < blocks.size() - 1; i++) {
		for (size_t j = 0; j < blocks.size() - 1; j++) {
			if (i != j && blocks[i].position.x == blocks[j].position.x && (int)blocks[j].cube.getPosition().y - (int)blocks[i].cube.getPosition().y >= 100) {
				blocks[i].state = true;
				blocks[i].havegaptoblock = true;
			}
		}
	}
	for (size_t i = 0; i < blocks.size() - 1; i++) {
		for (size_t j = 0; j < blocks.size() - 1; j++) {
			if (i != j && blocks[i].position.x == blocks[j].position.x && (int)blocks[j].cube.getPosition().y - (int)blocks[i].cube.getPosition().y == 64) {
				blocks[i].state = false;
				blocks[i].havegaptoblock = false;
			}
		}
	}
	for (size_t i = 0; i < blocks.size() - 1; i++) {
		for (size_t j = 0; j < blocks.size() - 1; j++) {
			if (i != j && blocks[i].position.x != blocks[j].position.x && blocks[i].position.y < 576) {
				blocks[i].state = true;
				blocks[i].havegaptoground = true;
			}
		}
	}
	for (size_t i = 0; i < blocks.size() - 1; i++) {
		for (size_t j = 0; j < blocks.size() - 1; j++) {
			if (i != j && blocks[i].position.x == blocks[j].position.x && (int)blocks[j].cube.getPosition().y - (int)blocks[i].cube.getPosition().y == 64) {
				blocks[i].state = false;
				blocks[i].havegaptoground = false;
			}
		}
	}
	//fix a bug
	if (blocks.size() <= 10 && blocks[0].position.y <= 576) {
		blocks[0].state = true;
		blocks[0].havegaptoground = true;
	}
}
void Itemcheck() {
	if (totalscore - itemscore == 5 && item == false) {
		item = true;
	}
}
void RecordScore() {
	FILE *towrite;
	towrite = fopen("Recordscore/leaderboard.txt", "a");
	if (towrite == NULL) {
		printf("Error to open file.");
		exit(1);
	}
	fwrite(&leader,sizeof(struct Leader),1,towrite);
	fread(&read, sizeof(struct Leader), 1, towrite);
	fclose(towrite);
}
void ReadScore() {
	FILE* toread;
	toread = fopen("Recordscore/leaderboard.txt", "r");
	if (toread == NULL) {
		printf("Error to open file.");
		exit(1);
	}
	while (fread(&read, sizeof(struct Leader), 1, toread)) {
		if (high[0].score<read.score) {
			high[0].score = read.score;
			high[0].name = read.name;
			high[0].scoretext = read.scoretext;
		}
		if (high[1].score < read.score && read.score <= high[0].score && read.name != high[0].name) {
			high[1].score = read.score;
			high[1].name = read.name;
			high[1].scoretext = read.scoretext;
		}
		if (high[2].score < read.score && read.score <= high[1].score && read.name != high[0].name && read.name != high[1].name) {
			high[2].score = read.score;
			high[2].name = read.name;
			high[2].scoretext = read.scoretext;
		}
		if (high[3].score < read.score && read.score <= high[2].score && read.name != high[0].name && read.name != high[1].name && read.name != high[2].name) {
			high[3].score = read.score;
			high[3].name = read.name;
			high[3].scoretext = read.scoretext;
		}
		if (high[4].score < read.score && read.score <= high[3].score && read.name != high[0].name && read.name != high[1].name && read.name != high[2].name && read.name != high[3].name) {
			high[4].score = read.score;
			high[4].name = read.name;
			high[4].scoretext = read.scoretext;
		}
	}
	/*for (int i = 0; i < 5; i++) {
		cout << i << endl;
		cout << high[i].score << endl;
		cout << high[i].name << endl;
		cout << high[i].scoretext << endl;
	}*/
	fclose(toread);
}