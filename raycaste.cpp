#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
#include <random>
#define _USE_MATH_DEFINES
#define INFINITY
#include <math.h>
#include <cmath>
#include <SFML/System/Vector2.hpp>
using namespace sf;
//////////////GLOBAL///
RenderWindow window(VideoMode(800, 400), "raycaste");
std::vector<float> scene;
float head = 0;
Vector2f ref(Vector2f(1.f,0.f));
float turn = 0.f;
////////////////////////
Vector2f vec_ang(float ang);
float dist(Vector2f v1, Vector2f v2);
Vector2f normalise(Vector2f v);
void rot_vec(Vector2f& v, float ang);
float ang_2vec(Vector2f v1, Vector2f v2);
float map(float input, float input_start, float input_end, float output_start, float output_end);
//BOUNDRARY////////////////////////////////////////
class Boundary
{
public:
	Boundary(float x1,float y1,float x2, float y2);
	~Boundary();
	void show(RenderWindow &win) {
		Vertex line[] = {Vertex(Vector2f(this->a.x,this->a.y)), Vertex(Vector2f(this->b.x,this->b.y))};
		win.draw(line, 2, Lines);
	}
	Vector2f get_a(void) {
		return this->a;
	}
	Vector2f get_b(void) {
		return this->b;
	}
private:
	Vector2f a;
	Vector2f b;
};
Boundary::Boundary(float x1, float y1, float x2, float y2)
{
	this->a = Vector2f(x1, y1);
	this->b = Vector2f(x2, y2);
}
Boundary::~Boundary()
{
}
////////////////////////////////////////FinWALL
//RAY////////////////////////////////////////
class Ray
{
public:
	Ray();
	Ray(Vector2f pos, float angle);
	~Ray();
	void show(RenderWindow& win) {
		Transform translation;
		translation.translate(pos);
		Vertex line[] = { Vertex(Vector2f(0.f,0.f)), Vertex(Vector2f(this->dir.x,this->dir.y)) };
		win.draw(line, 2, Lines,translation);
	}
	float heading() {//////////////////////fisheye
		float ret;
		ret = atan(this->dir.y/ this->dir.x);
		//ret = fmod(atan2(this->dir.y,this->dir.x), 2 * M_PI);
		return ret;
	}
	Vector2f cast(Boundary wall) {
		float x1 = wall.get_a().x;
		float y1 = wall.get_a().y;
		float x2 = wall.get_b().x;
		float y2 = wall.get_b().y;
		float x3 = this->pos.x;
		float y3 = this->pos.y;
		float x4 = this->pos.x + this->dir.x;
		float y4 = this->pos.y + this->dir.y;
		float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
		if (den == 0)
			return Vector2f(0.f,0.f);
		float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
		float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
		if (t > 0 && t < 1 && u>0)
		{
			Vector2f pt;
			pt.x = x1 + t * (x2 - x1);
			pt.y = y1 + t * (y2 - y1);
			return pt;
		}
		else
			return Vector2f(0.f,0.f);
	}
	void lookAt(float x, float y) {
		this->dir.x = x - this->pos.x;
		this->dir.y = y - this->pos.y;
	}
	Vector2f get_posr(void) {
		return this->pos;
	}
	float get_ang(void) {
		return ang;
	}
	Vector2f get_dir(void) {
		return this->dir;
	}
protected:
	float ang;
	Vector2f pos;
	Vector2f dir;
};
Ray::Ray() {}
Ray::Ray(Vector2f pos, float angle)
{
	this->pos = pos;
	this->dir = vec_ang(angle);
	this->ang = angle;
}
Ray::~Ray()
{
}
/////////////////////////////////////////FINRAY
//PARTICLE/////////////////////////////////////
class Particle : public Ray
{
public:
	Particle(RenderWindow& win,Vector2f pos, float angle);
	~Particle();
	void showp(RenderWindow &win) {
		CircleShape ellipse(5.f);
		ellipse.setFillColor(Color(255,255,255,255));
		ellipse.setPosition(Vector2f(this->pos_p.x,this->pos_p.y));
		for (int i = 0; i < this->rays.size(); i++) {
			this->rays[i].show(win);
		}
	}
	void look(std::vector<Boundary>& walls, RenderWindow& win) {
		float a=0;
		for (int i = 0; i < this->rays.size(); i++) {
			Vector2f closest(0.f, 0.f);
			float record = 100000;
			for (int j = 0; j < walls.size(); j++) {
				Vector2f pt;
				pt = this->rays[i].cast(walls[j]);
				if (pt != Vector2f(0.f, 0.f)) {
					float d = dist(this->pos_p, pt);
					a = ang_2vec(this->rays[i].get_dir(),ref);
					//a = (a * 2.f * M_PI) / 360.f;
					//std::cout << "a=" << a << std::endl;
					//d *= cos(a);
					if (d < record) {
						record = d;
						closest = pt;
					}
				}
			}
			if (closest != Vector2f(0.f, 0.f)) {
				Vertex line[]={Vertex(Vector2f(this->pos_p.x,this->pos_p.y),Color(255,255,255,255)),Vertex(Vector2f(closest.x,closest.y),Color(255,0,0,100)) };
				win.draw(line, 2, Lines);
			}
			scene.push_back(record);
		}
		for (int i = 0; i < scene.size(); i++) {
			scene.erase(scene.begin());
		}
	}
	void update(float x, float y,RenderWindow &win, float &turn) {
		this->pos_p.x = x;
		this->pos_p.y = y;
		for (float a = turn - 30.f; a < turn + 30.f; a += 1.f) {
			Ray tmp(pos_p, (a * 2.f * M_PI)/360.f);////////convert to randian +loching for down -loking for up 
			this->rays.push_back(tmp);
		}
		for (int i = 0; i < this->rays.size(); i++) {
			this->rays.erase(this->rays.begin());
		}
	}
protected:
	Vector2f pos_p;
	std::vector<Ray> rays;
};
Particle::Particle(RenderWindow& win,Vector2f pos, float angle):Ray(pos,angle){
	this->pos_p = pos;
}
Particle::~Particle()
{
}
///////////////////////////////FinPARTICLE
int main()
{
	window.setFramerateLimit(60);
	srand(time(NULL));
	std::vector<Boundary> walls;
	for (int i = 0; i < 5; i++) {
		float x1 = rand() % window.getSize().x/2;
		float x2 = rand() % window.getSize().x/2;
		float y1 = rand() % window.getSize().y;
		float y2 = rand() % window.getSize().y;
		walls.push_back(Boundary(x1, y1, x2, y2));
	}
	walls.push_back(Boundary(0,0,window.getSize().x/2,0));
	walls.push_back(Boundary(0, 0, 0,window.getSize().y));
	walls.push_back(Boundary(0, window.getSize().y,window.getSize().x/2,window.getSize().y));
	walls.push_back(Boundary(window.getSize().x/2, 0, window.getSize().x/2, window.getSize().y));
	Particle par(window,Vector2f(100.f,100.f),0.f);
	float x = 100.f;
	float y = 100.f;
	while (window.isOpen())
	{
		Event ev;
		while (window.pollEvent(ev))
		{
			if (ev.type == Event::Closed)
				window.close();
		}
		//update
		if (Keyboard::isKeyPressed(Keyboard::W))
			y--;
		else if (Keyboard::isKeyPressed(Keyboard::S))
			y++;
		else if (Keyboard::isKeyPressed(Keyboard::A))
			x--;
		else if (Keyboard::isKeyPressed(Keyboard::D))
			x++;
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			turn--;
			head++;//
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			turn++;
			head--;//
		}
		if (head >= 360 || head <= -360)
			head = 0;
		rot_vec(ref, head);
		par.update(x/*Mouse::getPosition(window).x*/,y/*Mouse::getPosition(window).y*/, window,turn);
		//draw
		window.clear(Color::Black);
		for (int i = 0; i < walls.size(); i++) {
			walls[i].show(window);
		}
		par.showp(window);/////////////////////////////////////////
		par.look(walls, window);
		std::cout << "head=" << head <<"||"<<"ref.x="<<ref.x<<"ref.y="<<ref.y<< std::endl;
		float w = (window.getSize().x/2) / scene.size() + 1;
		float scenew = window.getSize().x / 2;
		float sceneh = window.getSize().y;
		Transform translation;
		translation.translate(window.getSize().x/2,0);
		for (int i = 0; i < scene.size(); i++) {
			float maxdist = (window.getSize().x / sqrt(2));
			float h = map(scene[i],0,maxdist,sceneh,0);
			RectangleShape rect(Vector2f(w,h));
			rect.setPosition(Vector2f(w*i,sceneh/2-h/2/*window.getSize().y-h*/));
			float c = map(scene[i],0,maxdist,255,0);
			rect.setFillColor(Color(c, c, c));
			window.draw(rect,translation);
		}
		//ray.show(window);
		window.display();
	}
	return 0;
}

///vect&&&&ang///////////////////////////////
void rot_vec(Vector2f &v, float ang) {
	ang = (ang * 2.f * M_PI) / 360.f;
	v.x = cos(ang);
	v.y = sin(ang);
}
Vector2f vec_ang(float ang) {
	Vector2f dir;
	//ang = (ang * 2.f * M_PI) / 360.f;
	dir.x = cos(ang);
	dir.y = sin(ang);
	return dir;
}
Vector2f normalise(Vector2f v) {
	float norm = sqrt((v.x * v.x) + (v.y * v.y));
	Vector2f ret;
	ret.x = v.x / norm;
	ret.y = v.y / norm;
	return ret;
}
float ang_2vec(Vector2f v1, Vector2f v2) {
	float ret;
	//Vector2f v1 = normalise(a);
	//Vector2f v2 = normalise(b);
	float dot = (v1.x * v2.x) + (v1.y * v2.y);
	float lenthv1 = sqrt((v1.x * v1.x) + (v1.y * v1.y));
	float lenthv2 = sqrt((v2.x * v2.x) + (v2.y * v2.y));
	ret = acos(dot / (lenthv1 * lenthv2));
	float det = v1.x * v2.y - v1.y * v2.x;
	if (det >= 0)
		return ret;
	else if (det < 0)
		return -ret;
	return ret;
}
float dist(Vector2f v1, Vector2f v2) {
	float ret;
	ret = sqrt((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
	return ret;
}
float map(float input, float input_start, float input_end, float output_start, float output_end) {
	float output;
	output = output_start + ((output_end - output_start) / (input_end - input_start)) * (input - input_start);
	return output;
}
/////////////////////////////finVEC__ANG///////