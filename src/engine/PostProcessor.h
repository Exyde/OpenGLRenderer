#pragma once

#include "Shader.h"
#include "Texture2D.h"

// -- Todo : support reconstruct buffers when resizing windows + implement this in engine code

class PostProcessor {
   public:
    Shader shader;
    Texture2D sceneTexture;
    unsigned int Width, Height;

    //  -- Effects
    bool Confuse, Chaos, Shake;

    PostProcessor(Shader shader, unsigned int width, unsigned int height);
    void BeginRender();
    void EndRender();
    void RenderPostProcess(float time);

   private:
    unsigned int MSFBO, FBO;  // -- Multi-sampled FBO & Classic FBO.
    unsigned int RBO;
    unsigned int VAO;
    void InitRenderDatas();
};