#include <iostream>
#include <random>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
using namespace sf;
///GLOBAL///////
int total = 1;
int size = 30;
int vx = size * size;
int vy = size * 15;
RenderWindow window(VideoMode(vx,vy), "SNAKE");
RectangleShape block;
RectangleShape food;
int verticaleline = size;//size change
int horizontaleline = 15;
/////////////////
void pickLocation(void);
int dist(Vector2f v1, Vector2f v2);
////////CLASS'S/////
class Snake{
public:
	Snake();
	~Snake();
	int get_x() {
		return this->x;
	}
	int get_y() {
		return this->y;
	}
	int get_xs() {
		return this->xspeed;
	}
	int get_ys() {
		return this->yspeed;
	}
	void dir(int x, int y) {
		this->xspeed = x;
		this->yspeed = y;
	}
	void edges() {
		if (this->x   + size > window.getSize().x && this->xspeed==1)
			this->x = -30;
		else if (this->x < 0 && this->xspeed==-1)
			this->x = window.getSize().x ;//
		else if (this->y  + size > window.getSize().y && this->yspeed==1)
			this->y = -30;
		else if (this->y < 0 && this->yspeed==-1)
			this->y = window.getSize().y;//
	}
	void update() {
		if (total == this->sn.size()) {
			for (int i = 0; i < this->sn.size() - 1; i++) {
				int j = i + 1;
				this->sn[i] = this->sn[j];
			}
		}
		int t = total - 1;
		this->sn[t].setPosition(this->x, this->y);
		this->x += (this->xspeed * size);//size
		this->y += (this->yspeed * size);
	}
	void show() {//for every blocksnake
		for (int i = 0; i < this->sn.size(); i++) {
			//this->sn[i].setSize(Vector2f(size, size));
			this->sn[i].setFillColor(Color::Blue);
			this->sn[i].setOutlineThickness(1);
			this->sn[i].setOutlineColor(Color::Black);
			//this->sn[i].setPosition(Vector2f(this->x, this->y));
			window.draw(this->sn[i]);
		}
	}
	void eat(RectangleShape &fd) {
		//int d = dist(fd.getPosition(),this->sn.getPosition());
		if ((this->xspeed==1&&this->yspeed==0&&this->sn[total-1].getPosition().x+30==fd.getPosition().x&&this->sn[total - 1].getPosition().y==fd.getPosition().y)||
			(this->xspeed==-1&&this->yspeed==0&&this->sn[total - 1].getPosition().x-30==fd.getPosition().x&&this->sn[total - 1].getPosition().y==fd.getPosition().y)||
			(this->xspeed==0&&this->yspeed==1&&this->sn[total - 1].getPosition().y+30==fd.getPosition().y&&this->sn[total - 1].getPosition().x==fd.getPosition().x)||
			(this->xspeed==0&&this->yspeed==-1&&this->sn[total - 1].getPosition().y-30==fd.getPosition().y&&this->sn[total - 1].getPosition().x==fd.getPosition().x)) {
			pickLocation();
			this->sn.push_back(RectangleShape(Vector2f(size,size)));
			total++;
		}
	}
	bool death() {
		for (int i = 0; i < this->sn.size() - 1; i++) {
			if ((this->xspeed == 1 && this->yspeed == 0 && this->sn[total - 1].getPosition().x + 30 == this->sn[i].getPosition().x && this->sn[total - 1].getPosition().y == this->sn[i].getPosition().y) ||
				(this->xspeed == -1 && this->yspeed == 0 && this->sn[total - 1].getPosition().x - 30 == this->sn[i].getPosition().x && this->sn[total - 1].getPosition().y == this->sn[i].getPosition().y) ||
				(this->xspeed == 0 && this->yspeed == 1 && this->sn[total - 1].getPosition().y + 30 == this->sn[i].getPosition().y && this->sn[total - 1].getPosition().x == this->sn[i].getPosition().x) ||
				(this->xspeed == 0 && this->yspeed == -1 && this->sn[total - 1].getPosition().y - 30 == this->sn[i].getPosition().y && this->sn[total - 1].getPosition().x == this->sn[i].getPosition().x)) {
				return true;
			}
		}
		return false;
	}
private:
	std::vector<RectangleShape> sn;
	int x, y, xspeed, yspeed;
};
Snake::Snake(){
	this->x = 60;//
	this->y = 60;//
	this->xspeed = 1;
	this->yspeed = 0;
	this->sn.push_back(RectangleShape(Vector2f(size,size)));
	this->sn[0].setPosition(this->x,this->y);
}
Snake::~Snake()
{
}
///////////////FINSNAKE
/////////////globale class
Snake s;
bool isdeath = false;
//////////////////////
void grid() {
	for (int i = 0; i < verticaleline; i++) {
		for (int j = 0; j < horizontaleline; j++) {
			block.setPosition(Vector2f(i * size, j * size));
			window.draw(block);
		}
	}
}
void Setup() {
	window.setFramerateLimit(10);
	block.setSize(Vector2f(size, size));
	block.setFillColor(Color::Black);
	block.setOutlineThickness(1);
	block.setOutlineColor(Color::White);
	pickLocation();
}
void Draw() {
	grid();
	s.show();
	window.draw(food);
	if (isdeath == true) {
		Texture texture;
		if (!texture.loadFromFile("over.jpeg"))
			std::cout << "loadfailed" << std::endl;
		Sprite sprite;
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0,0,vx,vy));
		window.draw(sprite);
	}
}
int main() {
	srand(time(NULL));
	Setup();
	while (window.isOpen()) {
		Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == Event::Closed)
				window.close();
		}
		///UPDATE/////
		s.edges();
		if (Keyboard::isKeyPressed(Keyboard::W))
			s.dir(0, -1);
		else if (Keyboard::isKeyPressed(Keyboard::S))
			s.dir(0, 1);
		else if (Keyboard::isKeyPressed(Keyboard::A))
			s.dir(-1, 0);
		else if (Keyboard::isKeyPressed(Keyboard::D))
			s.dir(1, 0);
		s.update();
		s.eat(food);
		if (s.death() == true)
			isdeath = true;
		/////////////
		window.clear();
		Draw();
		window.display();
	}
	return 0;
}
//////////////Functions///

void pickLocation(void) {
	food.setSize(Vector2f(size, size));
	food.setFillColor(Color::Red);
	food.setOutlineThickness(1);
	food.setOutlineColor(Color::Black);
	int cols = floor(verticaleline);
	int raws = floor(horizontaleline);
	int x = floor(rand() % cols) * size;
	int y = floor(rand() % raws) * size;
	food.setPosition(Vector2f(floor(x),floor(y)));
}
int dist(Vector2f v1, Vector2f v2) {
	int ret;
	ret = sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
	return ret;
}
//////////////////////////