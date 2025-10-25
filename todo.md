# TODO 

- Partager moteur Vianney / Stanis pour feedback at some point


### Main Path (Learn Open GL)
- Montage : music de la playlist high bass energy + post linked/simon/outraghe/bluesky/insta/youtube
- Big Water plane genre WindWaker ? :p
- REFACTORING HERE (Voir ~Engine~)
- 2D Games aswell for engine structure can be nice (or side weekend break)

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

## ImGUI
- Afficher les Draws Calls
- UI : MeshReload important aussi (via file system ça pourrait être noice) (pour voir tout les assets Zelda jor <3)


# Global Goals (de départ, remember d'ou tu viens)
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
