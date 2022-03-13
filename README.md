# Computer Graphics: Opengl

Project for POGL and POGLA course at [EPITA](https://www.epita.fr/) made by
Dominique Michel et Lea Masselles

## What is it ?

This is a 2-person group project where we implement basic OpenGL features and
then add some more complex afterwards.

The basics features are:
- Sample scene with objects
- Skybox
- CPU particle system campfire
- CPU particle system firework

https://user-images.githubusercontent.com/9299438/137045293-bceb6b31-3912-467c-bcc3-aa5a0cf48d40.mp4

The advanced features are:
- GPU particle system campfire
- SSAO (Screen Space Ambient Occlusion)
- DOP (Depth of field)

https://user-images.githubusercontent.com/9299438/158064456-6c4f653a-ae2f-4281-a5b9-f9d6ba5cfcbb.mp4

## Setup

To be able to compile and run the program you need :
- [CMake](https://cmake.org/) (build system)
- [OpenGL](https://opengl.org/) (library)

Setup the build system and compile the project:
```bash
42sh$ cmake -B build_folder -DCMAKE_BUILD_TYPE=[Debug|Release]
```
## Usage

### Running the program

Run the project with ressources in current directory:
```bash
42sh$ cd build_folder && ln -s ../res/* . && ./myOpenGl
42sh$ # OR
42sh$ cd res && cp ../build_folder/myOpenGl . && ./myOpenGl
```

### Interactions and movements
- *WASD* for first person movements
- *MOUSE* for first person head rotations
- *ESPACE* to fly upwards
- *CTRL* to fly downwards
- *MOUSE Wheel* control flying speed
- *P* print camera position

- *ENTER* toggle fireworks
- *O* toggle SSAO
- *F* togle DOP (settings can be set in dop_pass.glsl)
  focusSettings = vec3(-30, 15, 5) with
  x = position of focal point
  y = half-length of clear zone
  z = length ofthe clear / blur transiation zone

## Known issues
- Code could be better done a structured but this remains my first real opengl
- project

## Contributions
- Dominique MICHEL <dominique.michel@epita.fr>
- Léa MASSELLES <lea.masselles@epita.fr>
