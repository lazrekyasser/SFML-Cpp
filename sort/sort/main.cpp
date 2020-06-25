/*#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include <SFML/window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
using namespace sf;
/////////MIN////GREEN//////
int s = 0;
int nmp = 10;
//////SORT///////
void sort(std::vector<float>& v) {
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size() - i - 1; j++) {
            if (v[j] > v[j+1]) {
                float tmp;
                tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
            }
        }
    }
}
//////////////END
int main()
{
    RenderWindow window(VideoMode(800, 500), "SORT");
    window.setFramerateLimit(30);
    srand(time(NULL));
    std::vector<float> values;
    for (int i = 0; i < 800; i+=nmp) {
        float random = rand() % window.getSize().y;
        values.push_back(random);
    }
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        //update
        float big = 0.f;
        float min = 0.f;
        if (s < values.size()) {
            for (int j = 0; j < values.size() - s - 1; j++) {
                if (values[j] > values[j + 1]) {
                    float tmp;
                    tmp = values[j];
                    values[j] = values[j + 1];
                    values[j + 1] = tmp;
                }
                big = values[j + 1];
                min = values[j];
            }
        }
        s++;
        ////////
        window.clear();
        for (int i = 0; i < values.size(); i++) {
            RectangleShape line(Vector2f(nmp, values[i]));
            line.setPosition(i * nmp, window.getSize().y - values[i]);
            line.setFillColor(Color::White);
            if (values[i] == big)
                line.setFillColor(Color(255, 0, 0, 255));
            else if (values[i] == min)
                line.setFillColor(Color(0, 255, 0, 255));
            window.draw(line);
        }
        window.display();
    }
    return 0;
}*/