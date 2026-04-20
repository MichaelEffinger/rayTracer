Some differences between the rasterizer and the ray tracer, is that the ray tracer
has a lot better shading, becaues of the diffuse and mirror shading,
The ray tracer also cannot do stuff in realtime, unlike my nice rasterizer 
currently my spheres are also not that spherical right now in my rasterizer, the rasterizer
also does not have any shadows yet, which id like to implement in the future



NOTE I used some new c++ 23 stuff -- and maybe 26 stuff, I dont remember -- 

so if it is not compiling that could be the issue.



How to run... I stole the starter code from willemse, so its pretty good.


build using cmake presets

cd <path/to/this source>
cmake --preset=default



then build 


cd buildVCPkg


cmake --build 


then run the tests it makes. these will be in buildVCPkg/utests.

these tests are PointN_test, Color_test and VectorN_test


this should work... I hope...
:wq


all of the rasterizer stuff is inside of the OpenGL folder, you will need to run that like your main now instead,

no tests for the rasterizer, so just run the only file inside of OpenGL build on the rasterizer at some point I am going to need to put the shaders somewhere so I can actually push them

