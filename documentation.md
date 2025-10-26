# Documentation per Chapter

## Advanced OpenGL

### Advanced Data

<details>
<summary> Buffer Subdata </summary>

## Buffer Subdata

- Buffer content is target dependant. Binded to GL_ARRAY_BUFFER it means vertices, but something else when binded elsewhere.
- GLBufferSubData can be use to fill parts of a pre-allocated buffer.
```cpp 
glMapBuffer(GL_ARRAYBUFFER, GL_WRITE_ONLY); // return a void* to buffer adress memory. (Usefull to load from file to GPU without stocking in temp memory)
``` 
```cpp 
memcpy(ptr, data, sizeof(data)); // -- copy the data and then
GlUnmapBuffer(); // unmap the pointer.
``` 

#### Batching Vertex Attributes
- Insted of interleaving data in VAO like pos,normal,texcoord, 123123123 etc we can do 11122233 using SubData.
- Remplacer l'implémentation d'assimp par des subdata dans le VAO ? - Well, no, because interleaved it more optimal when reading vertex attributes in vertex shader

#### Copying Buffers
```cpp 
// -- Prototype
void  glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeptr size);
// -- You can bind to two specials buffers : copy and read buffer
glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
// -- Then this read from vbo1 with 0 offset and copy to vbo2 with 0, 8 * sizeof(float) datas !
glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, 8 * sizeof(float));
```
</details>
