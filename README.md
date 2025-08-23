# Pixel Physics Project
Simple project with C++ and SFML (2.6.1)

Currently has 35 elements with a couple unique interactions.

<details>
  <summary> linux compile </summary>
  sudo apt-get install libsfml-dev
  
  
  g++ -IHeaders src/UIHandler.cpp src/elemutil.cpp src/Grid.cpp src/States.cpp src/Element.cpp src/InputHandler.cpp src/main.cpp -o sfmlapp `pkg-config --cflags --libs sfml-graphics` && ./sfmlapp && rm sfmlapp
</details>

Water & Heat system

![gif1](https://github.com/user-attachments/assets/027eda8b-3d96-4089-92db-facc22e0d9ea)

Chunk System

![gif2](https://github.com/user-attachments/assets/77b085cf-cf47-48b8-b397-a0aac757ffa7)
