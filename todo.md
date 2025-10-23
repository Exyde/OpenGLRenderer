# TODO 

### Main Open GL Progress
- Blending
- Depth testing + stencil --> Fog + Outline
- FPS Display
- Grass Shader With Vertex displacement + Color UV + Instancing (voir Acerola)
- (+Particles  + Instancing ?) (Cool veritasium video on complexe plants though) 
- FaceCulling
- FrameBuffers
- Cubemaps --> Skybox

- Color Class => Oui ça va etre needed, ou une lib 
- Video Update mesh certainement 
- Tint les ombres de la diffuse (toon diffuse shader + color ramp ?)
- ImGuizmo 3D (John Shapman) : https://github.com/john-chapman/im3d
- Render Doc Plugin / Pix ?

### Bug
- Linear/Quadratic Attenuation  calculation seems weirds / off ? :/

### Engine
- Utility Class
- Logger at some point (when engine refactoring I guess)
- Relire la partie sur Pitch/Yaw/Roll et le calcul + reimplémenter le  LookAt
- Settings system config with plain text and hot reload
- Full refacto l'engine aussi at some point (post build system I guess ?)

## ImGUI
- MeshReload important aussi (via file system ça pourrait être noice)
- Bool for switch from ortho to perspective camera
- Bool pour unlock camera
- DockerIMGUI sur le côté

# Global Goals
- ToonShader : ok
- View Mode : kinda ok - Normal shader could be cool - (normals, heatmaps...)
- Shadowmaps
- PS1 Shading ?
- Global Illumination ?
- Particle System
- PostProcessing Effects...

## Notes
- Edit MTL file for the path, texture should be in same folder than the .obj
- Voir vidéo Acerola pour idée de post process
- Voir ça : https://noclip.website/ pour idéees
- Share Progress on Bluesky / Discord / Linkedin ?
- Checker the cherno at some point
- Check la playlist thearlesvfx
- Revoir les refs de OpenGL Effects pour des idées de trucs à implémenter at some point


## Core Todo for Portolio
- PBR
- ShadowMaps
- Deffered Rendering
- One thing from a reasearch  / siggraph paper

## References
- The Real MGP : https://therealmjp.github.io/posts/gpu-memory-pool/
- Cem Yuksel (Rendereing Courses) : https://www.youtube.com/watch?v=DnkU4_DttGE&list=PLplnkTzzqsZTfYh4UbhLGpI5kGd5oW_Hh&index=10
- Wicked Engine Janos Turansky : https://wickedengine.net/2022/07/game-dev-journey-10-years/
- Tech Artist Org

## Books 
- 3D Maths Primer for Graphics and Game Dev
- Real Time Rendering Fourth Edition
- GPU gems
- PBR Games : From Theory to Implementation

Graphic Opti : https://www.youtube.com/watch?v=YdXi1kcQLDE