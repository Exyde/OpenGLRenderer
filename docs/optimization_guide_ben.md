# Global

[Back to menu](index.md)

Analyse First, then optimize : Where are we bottle neck ? Enemies ? Physics ? AI ?
Frame Rate vs Frame Time (much better for details)
- 15 fps : 66.66 ms
- 30 fps : 33.33 ms
- 60 fps : 16.66 ms
- 120 fps : 8.ms 

## Bottleneck
Cpu : Draws Call
Vertex Shader : too many vertices to process from object space to  NDC / Screen projection coordinate
Fragment : too complexe shaders  
**Deffered** : Put things in a GBuffer and then calculate lights in a second pass (Color, rhougness, normal, metallic and other if needed)
Complexity of lighting seppareted from complexity of the geometry ! But large memory (GBUFFER), less shader diversity, no transparent ?

**Forward** : Each object light itself (each object can have a diffrent light model, less VRAM, baack to front rendering (transparency)) + fast
But bad for complex scenes 
Attention : transparent with a lot off light can be expensive

**Opaque** : Deffered, so no lighting data in shader complexity !
**Transparent** : Forward, so lighting calculation are here

## Pick Your Targets
Min CPU? GPU ? Console ? ... Know the hardware -- Fit the audience aswell + TARGET FRAME RATE (60 at least for PC )
Learn the hardware : How many polygons for our target FPS, how many draw call, post-processing cost ? metrics relationships ?
--> Fill the screen with meshes, do test for thoses
Set content budgets :
- Enviro : X ms, Y Drawscall less, Z amount of polygons and K Vram : Each team has a target and a budget 
- Tracks budgets with on screen metric (like Frame, Game, Draw, GPU, CPU time, draws calls, tri-count, memory, etc...) for your engine + color tints
- Recap: Choosing Target Hardawre, Knowing it's capacities, establish a budget and tools to follow it.
- Most Accurate outside the editor in standalone build (a lot of overhead in editor that can skew the results and lead to wrong opti)
- Pick a Fix Camera Position to have consistant measures, eliminate variations !! [Setup fix camera location to get captures)
- Isolation and Stabilization : Turn off anything that can make results vary (if enviro, remove chara, vfx, post process...)
- Disable FrameRate smoothing Unreal + Automate the process
- Track & Graph Results
- Unreal Practicle : stats fps / stats UnitGraph (automate activation)
- Optimize the slowest processor first !!! Thats CPU Bound vs GPU Bound !!
- Game : CPU Game Thread
- DRAW : Cpu Prepare Draw
- GPU Time : Time in GPU

## CPU Bounds with High Draw Call (Probably our case) -- Reducing Draw Calls
- Instancing / Batching : Maybe we could batch things like blobs and so on ? -- Instance the enemy on gpu (was the niagara idea though)
- Occlusion Culling : Try to enable it ? but check perf cost (r.VisualizeOccludePrimitives)
- Level Layout : LD Approach for point of view and things, level streaming 
- Early Distance Culling : %screen size culling 
- HLODS : If not build'in can be a lot of work

## CPU Bound with Low Draw Calls
- Pathfinding ?
- NPC / AI Logic
- Complex Collision or Physics
- Game Logic
- Other cpu intensive tasks ?

## GPU Bounds (Video 6)
https://www.youtube.com/watch?v=hjjodpsZ70Q

## Pix : GPU Profiler (7)

## 8 - Optimize from Analysis with Pix

## 9 - Draw Call Optimization with Pix
 
## 10 - Shader Optimization

## Summary
- Pix for CV
- Custom thing in the engine
- Terrain Shaders at some point :]

