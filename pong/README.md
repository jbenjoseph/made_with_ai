# Vector Pong

**Vector Pong** is a modern, vector-based Pong game developed in C++ using SDL2 and SDL_ttf. This project showcases real-time graphics rendering without relying on external image assets, featuring dynamic particle effects that transform the ball into a fireball and "fancy" paddles crafted entirely through code.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Game](#running-the-game)
- [Gameplay Controls](#gameplay-controls)
- [Customization and Enhancements](#customization-and-enhancements)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Features

- **Vector-Based Graphics:** All game elements are rendered using SDL2's drawing functions, eliminating the need for external image files.
- **AI-Controlled Paddles:** The AI paddle automatically tracks the ball's position, providing a challenging opponent.
- **Score Tracking:** Real-time display of player and AI scores.
- **Streak Messages:** Dynamic messages like "Double Kill!" and "Holy Shit!" appear when consecutive points are scored.
- **Particle Effects:** The ball emits particles, creating a fireball effect that enhances visual appeal.
- **Responsive Design:** Smooth and consistent gameplay across different systems.

---

## Prerequisites

Before setting up and running **Vector Pong**, ensure your system has the following:

- **Operating System:** macOS (other platforms may require minor adjustments).
- **C++ Compiler:** `g++` (typically included with Xcode Command Line Tools).
- **CMake:** Version 3.10 or later.
- **Homebrew:** Package manager for macOS.
- **SDL2 Library:** For graphics rendering.
- **SDL2_ttf Library:** For text rendering.

---

## Installation

### 1. Install Homebrew (If Not Already Installed)

Homebrew simplifies the installation of software on macOS. If you haven't installed it yet, open Terminal and run:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### 2. Install SDL2 and SDL2_ttf via Homebrew

With Homebrew installed, execute the following commands in Terminal to install SDL2 and SDL2_ttf:

```bash
brew update
brew install sdl2 sdl2_ttf
```

---

## Building the Project

### 1. Create a Build Directory

It's good practice to build in a separate directory to keep the source tree clean.

```bash
mkdir build
cd build
```

### 2. Run CMake to Configure the Project

CMake will generate the necessary build files.

```bash
cmake ..
```

### 3. Compile the Project

Once configuration is successful, build the project using `make`.

```bash
make
```

*This will generate an executable named `VectorPong` in the `build` directory.*

---

## Running the Game

After successfully building the project, run the game using:

```bash
./VectorPong
```

A window titled "Vector Pong with Fireball and Fancy Paddles" will appear, showcasing the Pong game with all its features.

---

## Gameplay Controls

- **Move Paddle Up:** Press the `W` key.
- **Move Paddle Down:** Press the `S` key.
- **Quit Game:** Click the close button on the game window or press `Ctrl + C` in the terminal.

---

## Customization and Enhancements

Feel free to modify and enhance **Vector Pong** to suit your preferences or to learn more about game development. Here are some suggestions:

1. **Adjusting Particle Effects:**
   - **Particle Colors:** Change the `fireColor` in the code to create different visual effects (e.g., blue for ice, red for lava).
   - **Particle Lifetimes:** Modify the `lifetime` of particles for longer or shorter trails.
   - **Emission Rate:** Adjust how frequently particles are emitted from the ball.

2. **Enhancing Paddles:**
   - **Multiple Layers:** Add additional shapes or colors to create layered paddles.
   - **Color Gradients:** Implement color gradients within the paddles for a more dynamic look.
   - **Animated Paddles:** Introduce subtle animations like pulsating sizes or colors.

3. **Improving AI:**
   - **Difficulty Levels:** Introduce varying AI speeds or reaction times based on difficulty settings.
   - **Predictive AI:** Make the AI anticipate the ball's trajectory for a more challenging opponent.

4. **Adding Sound Effects:**
   - **SDL_mixer Integration:** Incorporate sounds for ball collisions, scoring, and streak achievements.
   - **Background Music:** Add looping background music to enhance the gaming experience.

5. **Implementing a Start Menu and Game Over Screen:**
   - **Menus:** Allow players to start the game, view instructions, or quit.
   - **Game Over Conditions:** Define winning conditions and display final scores with options to restart or exit.

6. **Responsive Design:**
   - **Resizable Window:** Allow the game window to be resized and adapt the game elements accordingly.
   - **Full-Screen Mode:** Enable toggling between windowed and full-screen modes.

7. **Multiplayer Support:**
   - **Local Multiplayer:** Allow two players to control separate paddles using different keys (e.g., `W`/`S` for Player 1 and `Up`/`Down` arrows for Player 2).

8. **Visual Enhancements:**
   - **Background Patterns:** Draw background patterns or gradients for added depth.
   - **Shadows and Lighting:** Simulate shadows or lighting effects on game elements.

---

## Troubleshooting

### 1. CMake Cannot Find SDL2 or SDL2_ttf

- **Ensure SDL2 and SDL2_ttf are Installed:**
  - Verify installation via Homebrew:
    ```bash
    brew list | grep sdl2
    ```

- **Check pkg-config Path:**
  - Ensure that `pkg-config` can locate SDL2 and SDL2_ttf:
    ```bash
    pkg-config --cflags sdl2
    pkg-config --libs sdl2
    pkg-config --cflags SDL2_ttf
    pkg-config --libs SDL2_ttf
    ```
  - If these commands return paths and flags, `CMakeLists.txt` should work correctly.

### 2. Compilation Errors Related to SDL2

- **Verify Include Directories:**
  - Ensure that `CMakeLists.txt` correctly includes SDL2 and SDL2_ttf directories.

- **Linking Issues:**
  - Make sure that `CMakeLists.txt` links against the correct SDL2 and SDL2_ttf libraries.

### 3. Runtime Errors or Missing Libraries

- **Dynamic Libraries:**
  - Ensure that SDL2 and SDL2_ttf dynamic libraries are accessible during runtime.
  - On macOS, Homebrew typically handles this, but you can set the `DYLD_LIBRARY_PATH` if necessary.

### 4. Font Loading Issues

- **Font Path:**
  - Verify that the font path in `pong.cpp` is correct. The default path is `/Library/Fonts/Arial.ttf`.
  - If using a different font, update the `fontPath` variable accordingly.

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Acknowledgments

- **SDL2:** Powerful library for handling graphics, input, and more.
- **SDL_ttf:** Extension library for rendering TrueType fonts.
- **OpenGameArt:** Resource for free game assets (initially used for inspiration).
- **CMake:** Cross-platform build system that simplifies the build process.

---

**Happy Coding! 🚀**
