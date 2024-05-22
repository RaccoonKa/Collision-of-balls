#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

struct Ball {
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball(float radius, sf::Vector2f pos, sf::Vector2f vel)
        : shape(radius)
    {
        shape.setPosition(pos);
        shape.setFillColor(sf::Color::Yellow);
        velocity = vel;
    }

    void update() {
        shape.move(velocity);
    }

    bool checkCollision(const Ball& other) {
        return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
    }

    void resolveCollision(Ball& other) {
        sf::Vector2f delta = shape.getPosition() - other.shape.getPosition();
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
        float overlap = (shape.getRadius() + other.shape.getRadius()) - distance;
        if (overlap > 0) {
            sf::Vector2f direction = delta / distance;
            sf::Vector2f correction = direction * (overlap * 0.1f);
            shape.move(correction);
            other.shape.move(-correction);
            velocity = velocity - 1.0f * (velocity.x * direction.x + velocity.y * direction.y) * direction;
            other.velocity = other.velocity - 1.0f * (other.velocity.x * direction.x + other.velocity.y * direction.y) * direction;
        }
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bouncing Balls");
    std::vector<Ball> balls;

    // Добавление нескольких шариков
    balls.push_back(Ball(20, sf::Vector2f(100, 100), sf::Vector2f(0.03f, 0.1f)));
    balls.push_back(Ball(20, sf::Vector2f(200, 200), sf::Vector2f(-0.03f, 0.06f)));
    balls.push_back(Ball(20, sf::Vector2f(300, 300), sf::Vector2f(0.02f, -0.01f)));
    balls.push_back(Ball(20, sf::Vector2f(400, 400), sf::Vector2f(-0.03f, 0.03f)));
    balls.push_back(Ball(20, sf::Vector2f(450, 450), sf::Vector2f(-0.04f, 0.01f)));


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Обновление позиций шариков
        for (auto& ball : balls) {
            ball.update();
            for (auto& other : balls) {
                if (&ball != &other) {
                    if (ball.checkCollision(other)) {
                        ball.resolveCollision(other);
                    }
                }
            }
        }

        window.clear();
        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }
        window.display();
    }

    return 0;
}