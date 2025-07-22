#include "Grid.h"
#include "UIHandler.h"
#include "InputHandler.h"

const int SIZE = 120; //multiple of CHUNK size in grid.h
const int SCALE = 6;
const int FPS = 60;

std::string controls = \
"---------------- CONTROLS ----------------\n \
F1 - Chunk Debug Mode (Performance Heavy) \n\
Nums - Brush Size \n\
◈ Static ◈\n\
[W]Wood \t\
[F]Fuse \t\
[m]Metal \t\
[D]Diamond \n\
■ Solids ■ \n\
[d]Dirt \t\
[r]Rock \t\
[i]Ice \n\
▲ Dusts ▲ \n\
[s]Sand \t\
[g]Gravel \n\
≈ Liquids ≈ \n\
[w]Water \t\
[o]Oil \t\t\
[l]Lava \n\
● Gases ● \n\
[c]Cloud \t\
[O]Oxygen \n\
* Energy * \n\
[f]Fire \t\
[p]Plasma \t\
[k]Spark \t\
[e]Explosion \n\
[n]Nuke \n\
✧ Life ✧ \n\
[b]Bug \t\t\
[G]Grass Seed \t\
[P]Plant \n\
";

int main() {
    srand(time(0));
    sf::RenderWindow window;
    sf::Image image; // manipulated inside grid
    sf::Texture texture;
    sf::Sprite sprite;
    image.create(SIZE, SIZE, sf::Color::Black);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setScale(SCALE, SCALE);
    window.create( sf::VideoMode(SIZE*SCALE, SIZE*SCALE), "Grid" );
    window.setFramerateLimit( FPS );

    //show_controls();
    Grid grid(SIZE, SCALE, FPS, image, window);
    UIHandler ui(SIZE, SCALE, window, grid);
    InputHandler input(SIZE, SCALE, grid, window);

    std::cout<<controls<<std::endl;

    while (window.isOpen()) {
            grid.process();
            input.process();
            
            texture.update(image);
            window.clear();
            
            window.draw(sprite);
            ui.draw();

            window.display();
        }
}
