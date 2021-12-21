# MyPathTracer
A simple path tracing renderer currently supporting diffuse and opaque microfacet materials.  
Many ideas are from assignment 5-7 from [GAMES101 Introduction to Computer Graphics](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html) by Professor Lingqi Yan, University of California, Santa Barbara.

#### Program Structure
xx


#### Compile and Run

A directly runnable *MyPathTracer.exe* is provided under "out/build/x64-release/". 
  
If you wish to build and test the code yourself, make sure you are using an IDE with cmake support(VS2019 in my case), or you have the [cmake software](https://cmake.org/download/) installed in your system. Then you can build a cmake project via the cmakelists provided.  
   
Here's an example on how to build the project through terminal:
* Open up terminal from the root folder(where *CMakeLists.txt* is at)  
* Create build folder  
`mkdir build`  
* Get into build folder  
`cd build`  
* Use cmakelist to generate makefile  
`cmake ..`  
* Generate executable file  
`make`  
* Run the executable  
`./MyPathTracer`  
  
Additional notes:  
* If you get an "Assertion failed" error, you probably built the object with some cmake support tools. You might need to change the relative paths to the model objects in *main.cpp*(line 23), since *CMakeLists.txt* is not guaranteed to be in the parent folder of the .exe file. If you're not familiar with relative paths you can simply replace the paths in *main.cpp* with absolute paths to the .obj files on your local machine.  
* The program uses multithreading via [OpenMP](https://www.openmp.org/), which is supported by most of the current compilers, but you can opt not to use it by editting *main.cpp*(line 46)


#### Future work
* Implement rough transparent materials. An amazing related [article](https://agraphicsguynotes.com/posts/glass_material_simulated_by_microfacet_bxdf/) and the referenced [paper](https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.html).
* Implement bidirectional path tracing with multi importance sampling to boost renderer robustness when direct lighting is hard to sample(e.g. a scene lit mostly by diffused light). See reference No.4.
* Optimize BVH structure with SAH(Surface Area Heuristic) method.
* More will come up after I crack down current objectives.
#### References
[1] [GAMES101 Lecture materials and slides](https://sites.cs.ucsb.edu/~lingqi/teaching/games101.html)  
[2] [Importance Sampling techniques for GGX with Smith Masking-Shadowing](https://schuttejoe.github.io/post/ggximportancesamplingpart1/)  
[3] [PBR Theory](https://learnopengl.com/PBR/Theory)  
[4] [Robust Monte Carlo Methods for Light Transport Simulation](http://graphics.stanford.edu/papers/veach_thesis/)  
[5] [Sampling Microfacet BRDF](https://agraphicsguynotes.com/posts/sample_microfacet_brdf/)  
