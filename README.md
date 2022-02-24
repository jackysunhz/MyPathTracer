# MyPathTracer
A simple path tracing renderer currently supporting diffuse and opaque microfacet materials.  

The example image below is rendered at 2048 SSP.
![This is an image](/img/TheBox.png)


### Class Descriptions
* #### BoundingBox
    Acts like a typical Axis Aligned Bounding Box, used for accelerating the calculations of ray-triangle intersections.

* #### BVH
    Bounding Volume Hierarchy, used to accelerate ray-triangle intersections. The idea is to distribute all the triangles in a scene into a tree according to their bounding box centroids. In this way, if the ray does not hit any bounding box at some node in the tree, all the triangles in the children nodes can be excluded in intersection checking.

* #### global
    Some global variables and utility functions.

* #### Intersection
    Contains information about a ray-triangle intersection. (position, normal, material of the triangle, etc.)

* #### main
    The main execution flow.

* #### Material
    Material that is associated with a surface. Affects the calculation of light transportation. For example, on DIFFUSE type materials, light is considered to scatter evenly across the whole hemisphere, so the uniform distribution would be used when sampling Monte Carlo integration. While on MICROFACET surfaces, the direction in which an incident light is scattered is affected by the smoothness, so importance sampling needs to be used. 

    Handles most of the mathematics including sampling reflection rays, calculating pdf, brdf, Fresnel terms, geometry shadowing, etc.

* #### Object
    Abstract base class for geometries.
  * #### Triangle
    Implements Object, represents a single triangle (vertices, normal, material, etc.).

  * #### Mesh
    Implements Object, represents a mesh constructed by a group of Triangles.

* #### Ray
    Represents a light ray (origin, direction, etc.).

* #### Renderer
    Responsible for generating rays per pixel, evaluate them using a Trace method defined in Scene class and averaging them.

    Contains renderer settings methods (# of samples per pixel, threading options, etc.).

* #### Scene
    This is where the actual ray-tracing takes place. Scene is responsible for recursively bounce a ray in the scene, evaluate the shading with the help of various mathematical terms defined in the Materials.

    Contains scene settings methods (fov, resolution, Russian Roulette probability, etc.).

Please refer to the header files for more details.


### Main.cpp structure:
All user customizations and specifications can be done inside the main.cpp file. The file contains the following steps:
* Specify a number of materials (Material class), load the models in through a third-party object loader and associate the materials with different models.
* Put all the models into a scene instance (Scene class).
* Generate Bounding Volume Hierarchy (BVH class) as the acceleration structure for the scene (this is provided as a public method in the scene class).
* Specify scene and renderer (Renderer class) settings on their respective instances.
* Render the scene by passing the scene instance to the renderer instance.


### Compile and Run
A directly runnable [*MyPathTracer.exe*](/sampleExe/MyPathTracer.exe) is provided under "sampleExe/". 
  
If you wish to build and test the code yourself, make sure you are using an IDE with cmake support(VS2019 in my case), or you have the [cmake software](https://cmake.org/download/) installed in your system. Then you can build a cmake project via the cmakelists provided.  
  
Additional notes:  
* If you get warnings stating that models are not loaded, you might need to check the relative paths to the model objects in *main.cpp*(line 23). Since *CMakeLists.txt* is not guaranteed to be in the parent folder of the .exe file. If you're not familiar with relative paths you can simply replace the paths in *main.cpp* with absolute paths to the .obj files on your local machine.  
* The program uses multithreading via [OpenMP](https://www.openmp.org/), which is supported by most of the current compilers, but you can opt not to use it by editting *main.cpp*(line 46)


### Future work
* Implement rough transparent materials. An amazing related [article](https://agraphicsguynotes.com/posts/glass_material_simulated_by_microfacet_bxdf/) and the referenced [paper](https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.html).
* Implement bidirectional path tracing with multi importance sampling to boost renderer robustness when direct lighting is hard to sample(e.g. a scene lit mostly by diffused light). See reference No.4.
* Create a GUI so that settings can be tuned more easily rather than having to change *main.cpp*.
* Optimize BVH structure with SAH(Surface Area Heuristic) method.
* More will come up after I crack down current objectives.


### Acknowledgements
Special thanks to Professor Lingqi Yan(University of California, Santa Barbara) on the amazing course [GAMES101 Introduction to Computer Graphics](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html) and everyone who helped make the course publicly available. 


### References
[1] [GAMES101 Lecture materials and slides](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html)  
[2] [Importance Sampling techniques for GGX with Smith Masking-Shadowing](https://schuttejoe.github.io/post/ggximportancesamplingpart1/)  
[3] [PBR Theory](https://learnopengl.com/PBR/Theory)  
[4] [Robust Monte Carlo Methods for Light Transport Simulation](http://graphics.stanford.edu/papers/veach_thesis/)  
[5] [Sampling Microfacet BRDF](https://agraphicsguynotes.com/posts/sample_microfacet_brdf/)  
