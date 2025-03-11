# Drone Delivery

## Description
A game in which you control a delivery drone on Mars.

## Controls

- `WASDQE` to rotate drone
- `X` to attach package
- `SPACE` drone thrust
- `RIGHT CLICK HOLD + MOUSE` camera controls

## Features

- Simple fog shader

    A simple fog effect by blending the color of fragments with the background based on
    their distance from the camera. The blending is done using linear interpolation,
    gradually transitioning from the object’s color to the background color
    as the distance increases.

    <img src = ./gifs/fog.gif>
    
    Two cutoff points define the effect:

    - Near cutoff: Objects closer than this distance appear fully visible, without fog.
    - Far cutoff: Objects beyond this distance are completely covered by fog, matching the background color.

 <p> <br> </p>

- Noise shader for terrain

   This shader generates a dynamic terrain by applying Perlin noise to an initially flat plane. The noise function modifies both the height of the vertices and the color of the fragments, creating a varied landscape.
    
    <img src = ./gifs/terrain.gif>

    - Vertex Displacement: The Perlin noise value at each vertex is used to adjust its height, forming hills and valleys.

    - Color Blending: The fragment color is determined by the noise value, smoothly interpolating between two predefined colors.

 <p> <br> </p>

- Collision detection

    Simple collision detection logic using Axis aligned bounding boxes(AABB) colliders
    for the obstacles and sphere colider for the drone.
    <img src = ./gifs/collision.gif>

 <p> <br> </p>

- Minimap overlay

    Created using an orthographic perspective camera, which provides a top-down view
    of the scene. It replaces the drone with an arrow to indicate direction and
    includes an additional arrow to show the location of the package or checkpoint.

    <img src = ./gifs/minimap.gif>

 <p> <br> </p>

- Shadow mapping

    Using an additional render pass to create a depth map and a light position,
    objects can cast shadows on eachother as well as on the ground.

    <img src = ./gifs/shadow.gif>

 <p> <br> </p>

- Dithering shader

    For a more stylised experience.
    <img src = ./gifs/finished.gif>

## How to run

- Build the gfx-framework from https://github.com/UPB-Graphics/gfx-framework.
- Add the `tema2` folder in `source/lab_m1` to the `lab_m1` folder of the build. (also add them in the visual studio solution)
- Replace the `lab_list.h` from `lab_m1` folder and `main.cpp` with the ones from the `source` folder.
