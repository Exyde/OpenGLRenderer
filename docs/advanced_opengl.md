# Advanced OpenGL

[Back to menu](index.md)

- [Advanced OpenGL](#advanced-opengl)
  - [Advanced Datas](#advanced-datas)
    - [Buffer Subdata](#buffer-subdata)
    - [Batching Vertex Attributes](#batching-vertex-attributes)
    - [Copying Buffers](#copying-buffers)
  - [Advanced GLSL](#advanced-glsl)
    - [Built In Variables](#built-in-variables)
    - [Interface Blocks](#interface-blocks)
    - [Uniform Buffer Objects](#uniform-buffer-objects)

## Advanced Datas

### Buffer Subdata

- Buffer content is target dependant. Binded to GL_ARRAY_BUFFER it means vertices, but something else when binded elsewhere.
- GLBufferSubData can be use to fill parts of a pre-allocated buffer.
```cpp 
glMapBuffer(GL_ARRAYBUFFER, GL_WRITE_ONLY); // return a void* to buffer adress memory. (Usefull to load from file to GPU without stocking in temp memory)
``` 
```cpp 
memcpy(ptr, data, sizeof(data)); // -- copy the data and then
GlUnmapBuffer(); // unmap the pointer.
``` 

### Batching Vertex Attributes
- Insted of interleaving data in VAO like pos,normal,texcoord, 123123123 etc we can do 11122233 using SubData.
- Remplacer l'implémentation d'assimp par des subdata dans le VAO ? - Well, no, because interleaved it more optimal when reading vertex attributes in vertex shader

### Copying Buffers
```cpp 
// -- Prototype
void  glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeptr size);
// -- You can bind to two specials buffers : copy and read buffer
glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
// -- Then this read from vbo1 with 0 offset and copy to vbo2 with 0, 8 * sizeof(float) datas !
glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 8 * sizeof(float));
```
---



## Advanced GLSL

- Built-in variables
- News way of organizing shaders inputs/outputs
- Uniform Buffer Objects (UBO)

### Built In Variables
```glsl
// Vertex
gl_Position // output vector of vertex shader
gl_PointSize //influence point size when glEnable(GL_PROGRAM_POINT_SIZE); Fun and usefull for particles
gl_VertexID // Current vertex ID (Was usefull in Veritasium grass video)

// Fragment
gl_FragColor // output color
gl_FragCoord // fragment coord : Z value = depth // x/y = Screen Space Coordinates !
gl_FrontFacing // true or false if facing use - Create Curses
```
### Interface Blocks

- Allows in and out block, similar to structs
- Elements inside the block MUST be in the same order in differents shaders stage.

### Uniform Buffer Objects
- Later. [TODO]
- Advanced GLSL : Finish last part with UBO for shader refactoring !
- Gemotry Shaders : Apres UBO, à l'air cool pour Drawline on the fly though (Visualize Normals)
- Instancing : Ok to do when you want to start particles
- Anti Aliasing : Pas needed dessuite, quand on voudra
