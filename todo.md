# TODO 

## Core Goals for Portolio
- Finish Learn open GL
- PBR : Industry Standard (Require Gamma Correction, FrameBuffer, Cubemaps, HDR)
- ShadowMaps : Require Advenced Lighting
- Deffered Rendering : Requir at least both above I guess. Last fight ? 
- One thing from a reasearch  / siggraph paper : Volumetric Clouds ? Simulation ? You've got time :)


### Main Path (Learn Open GL)
- Faire des classes (Framebuffer, GameObject...) même wip, marre de dupliquer des VAO/BVO
- Pass ImGui Update 
- Importer asset genre WindWaker pour tester le rendu
- Video Update mesh certainement + update readme/features
- Tint les ombres de la diffuse (toon diffuse shader + color ramp ?)
- 2D Games aswell for engine structure can be nice (or side weekend break)

## GPU Instancing (Advanced Fun Goals)
- Grass Shader With Vertex displacement + Color UV + Instancing (voir Acerola)
- (+Particles  + Instancing ?) (Cool veritasium video on complexe plants though) 
- Terrain Generation at runtime + Triplanar Textures (Terrain Shader)

## Later Features
- Dynamic Environment Mapping (6x framebuffer and runtime cubemaps for reflection/refraction but perf ?)
- Todo : Reflection map (Cubemap -- Reflection)
- Todo : Refraction Map (Cubemap) + Light, Refraction, Reflection, Vertex Anim = Water Shader :D

### Bug
- Linear/Quadratic Attenuation  calculation seems weirds / off ? :/
- Bizzare ce truc de quadrillage avec le lighting ?
- Floating Cubes chelou

### Engine
- FaceCulling v2
- Blending v2
- Utility Class
- ResourceLoader Class
- Logger at some point (when engine refactoring I guess)
- Relire la partie sur Pitch/Yaw/Roll et le calcul + reimplémenter le  LookAt
- Settings system config with plain text and hot reload
- Big Refacto 2 Soon pour pouvoir draw ce qu'on veut de maniè-re plus flexible et moins opti mais np
- FrameBuffer Class / Skybox Class / Terrain Class ... 
- ImGuizmo 3D (John Shapman) : https://github.com/john-chapman/im3d
- Render Doc Plugin / Pix ?
- Color Class => Oui ça va etre needed, ou une lib 

## ImGUI
- DockerIMGUI sur le côté
- Bool pour switch entre Toon & Phong

- Camera
    - Bool for switch from ortho to perspective camera
    - Bool pour unlock camera Y
- Support Post Processing Choice (Later Stack?) (Kernel, Blur, Chromatic Abberation, Sharpen... Drop down + Combine or bool flags (the simplest so far)) 
- MeshReload important aussi (via file system ça pourrait être noice)
- Afficher les Draws Calls

# Global Goals
- PostProcessing Effects : presque ok v1
- ToonShader : ok
- View Mode : kinda ok - Normal shader could be cool - (normals, heatmaps...)
- Shadowmaps
- PS1 Shading ?
- Global Illumination ?
- Particle System

## Notes
- Share Progress on Bluesky / Discord / Linkedin ?
- Edit MTL file for the path, texture should be in same folder than the .obj
- Voir vidéo Acerola pour idée de post process
- Voir ça : https://noclip.website/ pour idéees
- Checker the cherno at some point
- Check la playlist thearlesvfx
- Revoir les refs de OpenGL Effects pour des idées de trucs à implémenter at some point

## References
- The Real MGP : https://therealmjp.github.io/posts/gpu-memory-pool/
- Cem Yuksel (Rendereing Courses) : https://www.youtube.com/watch?v=DnkU4_DttGE&list=PLplnkTzzqsZTfYh4UbhLGpI5kGd5oW_Hh&index=10
- Wicked Engine Janos Turansky : https://wickedengine.net/2022/07/game-dev-journey-10-years/
- Tech Artist Org : https://www.tech-artists.org/
- Graphic Opti : https://www.youtube.com/watch?v=YdXi1kcQLDE
- Journal of Computer Graphics, per Year : https://jcgt.org/read.html
## Books 
- 3D Maths Primer for Graphics and Game Dev
- Real Time Rendering Fourth Edition
- GPU gems + Acerola refs
- PBR Games : From Theory to Implementation
