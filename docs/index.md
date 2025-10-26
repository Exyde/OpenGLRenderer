# Syntropy Roadmap

## Navigation
- [Back to repo](https://github.com/Exyde/OpenGLRenderer)
- [Optimization Guide](/docs/optimization_guide_ben.md)
- [Advanced Open GL](/docs/advanced_opengl.md)
- [Advanced Lighting](/docs/advanced_lighting.md)

- [Syntropy Roadmap](#syntropy-roadmap)
    - [Main Path (Learn Open GL) || TodoList](#main-path-learn-open-gl--todolist)
  - [Core Goals for Portolio](#core-goals-for-portolio)
  - [GPU Instancing (Advanced Fun Goals)](#gpu-instancing-advanced-fun-goals)
  - [Later Features](#later-features)
    - [Bug](#bug)
    - [Engine Features](#engine-features)
    - [Engine Struture](#engine-struture)
- [Inital Goals](#inital-goals)
  - [Misc - Todo : Class this](#misc---todo--class-this)
  - [References](#references)
  - [Books](#books)
  - [Radiance Cascades](#radiance-cascades)

### Main Path (Learn Open GL) || TodoList
- Add renter textures for differents maps views (different buffer, could be nice)
- Mesh Reload from IMGUI + Filesystem
- Improve Site/Files system here (clean all links)
- Camera : F to focus current mesh ?
- Next Video : Nouvelle Era - Clozee
- Flag for post process FragCoord influence
- Big Water plane genre WindWaker ? 
- REFACTORING HERE (Voir ~Engine~) - En vrai, pas la peine de GROS Refactoring avant d'avoir fini tout et de savoir un peu mieux ce qu'on fait - Genre la refactoring avec Uniform Buffer Object cool !
- 2D Games aswell for engine structure can be nice (or side weekend break)
- RayLibEngine at some point ? Game or Jam maybe ? 
- Finnish Advanced Lighting
- Finish Advanced GLSL
- Resources loader class avec vector de model textures shader etc + node view?
- Éditeur shader dans IMGUI pour faire comme shadertoy ?
- Class scene object avec Une ref a une transform 3 vecteurs pour les matrices + Hierarchy display in Scene tab
- Implement Frustrum Culling
- Implement Basic Physics



## Core Goals for Portolio
- Finish Learn open GL
- PBR : Industry Standard (Require Gamma Correction, FrameBuffer, Cubemaps, HDR)
- ShadowMaps : Require Advenced Lighting
- Deffered Rendering : Requir at least both above I guess. Last fight ? 
- One thing from a reasearch  / siggraph paper : Volumetric Clouds ? Simulation ? You've got time :)

## GPU Instancing (Advanced Fun Goals)
- Grass Shader With Vertex displacement + Color UV + Instancing (voir Acerola)
- (+Particles  + Instancing ?) (Cool veritasium video on complexe plants though) 
- Terrain Generation at runtime + Triplanar Textures (Terrain Shader)

## Later Features
- Dynamic Environment Mapping (6x framebuffer and runtime cubemaps for reflection/refraction but perf ?)
- Todo : Reflection map (Cubemap -- Reflection)
- Todo : Refraction Map (Cubemap) + Light, Refraction, Reflection, Vertex Anim = Water Shader :D
- Tint les ombres de la diffuse (toon diffuse shader + color ramp ?)

### Bug
- Linear/Quadratic Attenuation  calculation seems weirds / off ? :/
- Fix Orthographic Camera
- DrawCall Bugger parce que pas pris en compte dans Mesh.Draw (todo : Class Renderer qui wrap tout les calls opens GL)
- camere scroll speed pas pris en comtpe 

### Engine Features
- FaceCulling v2
- Blending v2
- Render Doc Plugin / Pix ?
- ImGuizmo 3D (John Shapman) : https://github.com/john-chapman/im3d
- Settings system config with plain text and hot reload

### Engine Struture
- Color Class / Struct (ou lib) 
- Utility Class
- ResourceLoader Class
- Faire des classes (Framebuffer, GameObject...) même wip, marre de dupliquer des VAO/BVO
- Logger at some point (when engine refactoring I guess)

# Inital Goals

Initial goals for this projects. They were maybe kinda naive, and evolved as the project did. Like, in the beggining, I had no idea of what was Global Illumination.

- PostProcessing Effects : v1 working
- ToonShader : v1 working
- View Mode : kinda ok - Normal shader could be cool - (normals, heatmaps...) + Framebuffer on quad on side
- Shadowmaps : not implemented.
- PS1 Shading : not implemented.
- Global Illumination : not implementend.
- Particle System : not implementend.

## Misc - Todo : Class this
- Voir vidéo Acerola pour idée de post process
- Voir ça : https://noclip.website/ pour idéees
- Checker the cherno at some point
- Check la playlist thearlesvfx
- Revoir les refs de OpenGL Effects pour des idées de trucs à implémenter at some point

## References
- [The Real MGP](https://therealmjp.github.io/posts/gpu-memory-pool/)
- [Cem Yuksel (Rendering Courses)](https://www.youtube.com/watch?v=DnkU4_DttGE&list=PLplnkTzzqsZTfYh4UbhLGpI5kGd5oW_Hh&index=10)
- [Wicked Engine by Janos Turansky](https://wickedengine.net/2022/07/game-dev-journey-10-years/)
- [Tech Artist Org](https://www.tech-artists.org/)
- [Graphic Opti Video](https://www.youtube.com/watch?v=YdXi1kcQLDE)
- [ Research Papers : Journal of Computer Graphics, per Year](https://jcgt.org/read.html)

## Books 
Todo : Add Links
- 3D Maths Primer for Graphics and Game Dev
- Real Time Rendering Fourth Edition
- GPU gems + Acerola refs
- PBR Games : From Theory to Implementation

## Radiance Cascades
- [Introduction Video](https://www.youtube.com/watch?v=5Ua-h1pg6yM)
- [Interactive Site](https://radiance-cascades.com/)
- [In Depth Guide](https://mini.gmshaders.com/p/radiance-cascades)