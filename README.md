# Terraria Clone
A small Terraria-style clone I'm working on using Raylib, C++, and Premake.
<img width="1274" height="788" alt="image" src="https://github.com/shikkenzo/TerrariaRaylib/blob/main/RESOURCES/GameScreenshot.png" />

# Technologies 
Raylib Quickstart: https://github.com/raylib-extras/raylib-quickstart

# Features
- Player movement
- Procedural map generation
- AABB continuous collision detection and resolution
- Autotiling system

# Controls
- W, A, S, D: movement keys
- Right Mouse Click: CCD Shapecast Preview
- Left Mouse Click: convert tile to air

# The Process
For the initial setup, as mentioned before, I used the Raylib Quickstart repository as a base. After getting everything ready, I began exploring the basics of Raylib and tried displaying a dirt tile texture on the screen. Once that was working, I moved on to implementing basic procedural map generation, using Raylib’s Perlin noise.

Next, I worked on player movement and a simple AABB collision system. However, after noticing significant issues when running the game at low FPS, I decided to switch to continuous collision detection (CCD) using basic AABB shapecasting.

# What I Learned
- Premake basics
- Implementing simple autotiling
- Implementing AABB shapecasting and CCD

# Next Steps
- Minor code refactoring
- Implementing my own Perlin noise function
- Improving world generation
- Performance optimizations

# Running the Project
- Download the source code
- Run the build-VisualStudio2022.bat file
- Open the project in Visual Studio 2022
- Run the project
