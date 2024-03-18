COMP3015 CW1 Write-up

o Which version Visual Studio and Operating System you used to test/write the code locally?
I am using Visual Studio Community 2022 with version 17.9.2 on Windows 11.

 o How does it work? 
The project applies Blinn-Phong shaders and toon shading to a custom street light model that I created myself using Blender. The street light model is sat on a plane, and both have textures applied to them. There is also a sky box in my scene. There is a spotlight that shines down on my street light which rotates around it.

o How does your code fit together and how should a programmer navigate it (not intended to be an exercise in formally documenting the code)? 
The codebase consists of a SceneBasic C++ and header file, as well as a frag and vertex shader for the basic uniform and the skybox.
The header file contains variable and function declarations.
The C++ file contains all of the functions necessary for OpenGL. These all were included within the Project Template that was provided. This file is where the model, plane, lights and skybox are all rendered into the scene with their textures, as well as where translation and rotations are set to move the meshes in the scene.
The vertex shader files and fragment shader files contain all code for the shaders that are applied to the scene. It is where the BlinnPhong shading is applied.

o A Link to the unlisted YouTube Video
https://youtu.be/dYez-MosEW8
