# Ball Pivoting Algorithm (BPA)

![Reconstruction of bunny model](bunny.png)

This is an exemplary C++ implementation of the ball pivoting algorithm for creating triangle meshes from point clouds.
The implementation is intentionally kept simple and is based on:

*The Ball-Pivoting Algorithm for Surface Reconstruction* by Fausto Bernardini, Joshua Mittleman, Holly Rushmeier, Claudio Silva and Gabriel Taubin

The model of the bunny is provided by the [Stanford University Computer Graphics Laboratory](http://graphics.stanford.edu/data/3Dscanrep/).

## Building

1. Please install the following dependencies using your favourit dependency manager (I tried vcpkg):
    - glm
    - gtest

2. Generate build system using CMake
```
git clone https://github.com/bernhardmgruber/bpa.git
mkdir bpa/build
cd bpa/build
cmake ..
```

3. Run `make` on Linux or open and build Visual Studio solution on Windows. Requires a compiler supporting C\++20, but can probably easily ported down to C\++17 or C\++14.

4. To run the program:
    1. go to bpa/test/reconstruct.cpp
    2. Change the TEST case, also could comment out other TESTs
        ```cpp
        TEST(reconstruct, bunny) {
            const auto cloud = loadXYZ("../test/data/bunny.xyz");
            const auto mesh = measuredReconstruct(cloud, 0.002f);
            saveTriangles("bunny_mesh.stl", mesh); 
        }
        ```
    3. Compile the program again
        ```bash
            $ cd bpa/build
            $ make -j
        ```
    4. Run the executable file with
        ```bash
            $ ./tests
        ```