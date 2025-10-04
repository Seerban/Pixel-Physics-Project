# Pixel Physics Project
Simple pixel-based simulation with C++ and SFML (2.6.1) based on interaction between elements (35 Unique) with different properties.


**Compilation**
  
`g++ -IHeaders src/UIHandler.cpp src/elemutil.cpp src/Grid.cpp src/States.cpp src/Element.cpp src/InputHandler.cpp src/main.cpp -o sfmlapp \`pkg-config --cflags --libs sfml-graphics\` && ./sfmlapp`

Download SFML from `https://www.sfml-dev.org/download/`


Water & Temperature System

![gif1](https://github.com/user-attachments/assets/027eda8b-3d96-4089-92db-facc22e0d9ea)

Chunk Optimizations

![gif2](https://github.com/user-attachments/assets/77b085cf-cf47-48b8-b397-a0aac757ffa7)
