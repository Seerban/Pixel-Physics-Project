# Pixel Physics Project
Pixel-based automata made with C++ and SFML (2.6.1/2.6.2) based on interaction between elements (35 Unique) with different properties.

<details>
  <summary> <h1>Controls</h1> </summary>

  
F1 - Chunk Debug Mode (Performance Heavy)

Nums - Brush Size
  
<b>◈ Static ◈</b>

[W]Wood 
[F]Fuse 
[m]Metal 
[D]Diamond 
  
<b>■ Solids ■</b>

[d]Dirt
[r]Rock
[i]Ice

<b>▲ Dusts ▲ </b>

[s]Sand 
[g]Gravel 

<b>≈ Liquids ≈ </b>

[w]Water 
[o]Oil 
[l]Lava 

<b>● Gases ● </b>

[c]Cloud 
[O]Oxygen 

<b>* Energy *</b>
  
[f]Fire 
[p]Plasma 
[k]Spark 
[e]Explosion 
[n]Nuke 

<b>✧ Life ✧ </b>

[b]Bug 
[G]Grass Seed 
[P]Plant

Note - Not all elements are accessible through keybinds, some are obtainable through interactions.
</details>

**Compilation**

Install SFML (Might break if version is updated past 2.6.2)

`sudo apt install libsfml-dev`


Compile with:

`g++ src/*.cpp -IHeaders -o app -lsfml-graphics -lsfml-window -lsfml-system && ./app`


Water & Temperature System

![gif1](https://github.com/user-attachments/assets/027eda8b-3d96-4089-92db-facc22e0d9ea)

Chunk Optimizations

![gif2](https://github.com/user-attachments/assets/77b085cf-cf47-48b8-b397-a0aac757ffa7)
