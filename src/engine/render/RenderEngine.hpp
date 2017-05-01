#pragma once

#include <SDL_video.h>
#include "glm/glm.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "impl/GL.hpp"

#include "impl/Export.hpp"
#include "RenderStats.h"
#include "Mesh.hpp"
#include "Shader.hpp"


// forward declaration
class Mesh;
class ParticleMesh;

class Shader;

/// Maintains shared states for rendering.
/// An object of SimpleRenderEngine must be created once after the SDL_Window has been initialized.
/// After initialization this object can be referenced using the static field SimpleRenderEngine::instance;
///
/// SingleRenderEngine has two important states:
/// - An active camera, which defines how meshes are drawn when rendered using the draw method
/// - Light information (point lights, directional lights, ambient lights).
class DllExport RenderEngine {
public:
    /// SimpleRenderEngine constructor
    /// \param window pointer to the SDL window (must be initialized using OpenGL)
    RenderEngine(SDL_Window *window, int screen_width, int screen_height);
    ~RenderEngine();
    static constexpr int maxSceneLights = 4;
    static constexpr int version_major = 0;
    static constexpr int version_minor = 0;
    static constexpr int version_point = 1;
    GLuint frame_buffer;

    /**
     * Set the light state - this affects all draw calls after the state has been set.
     * @sa Light
     * @param lightIndex (must be between 0 and 3)
     * @param light
     */
    void setLight(int lightIndex, Light light);

    /**
     * Returns the state of the given light index
     * @param lightIndex
     * @return
     */
    Light getLight(int lightIndex);

    /**
     * Set the ambient light color. This is used to add some light to the shadow side of objects. (default: 0.2,0.2,0.2)
     * @param ambientLight
     */
    void setAmbientLight(const glm::vec3 &ambientLight);

    /**
     * Returns ambient light
     * @sa setAmbientLight
     * @return ambient light color
     */
    glm::vec3 getAmbientLight() const;

    /**
     * Draws a mesh instance to the current scene. Uses the current camera object to render the mesh in the scene.
     * @param mesh
     * @param modelTransform
     * @param shader
     */
    void draw(Mesh *mesh, glm::mat4 modelTransform, glm::vec4 color);
    void draw(Mesh *mesh, glm::mat4 modelTransform, Shader* shader);

    void bind_framebuffer();

    void display(Shader* shader);

    /**
     * Sets the current camera object.
     * @param camera
     */
    void setCamera(Camera *camera);

    /**
     * Returns the current camera object.
     * @sa getDefaultCamera()
     * @return camera pointer
     */
    Camera* getCamera();

    /**
     * Returns a reference to the default camera. This camera is owned by the object and should never be deleted.
     * @return
     */
    Camera* getDefaultCamera();

    /**
     * Clear the screen with the given color (default behavior is also clearing color and depth buffer)
     * @param color
     * @param clearColorBuffer
     * @param clearDepthBuffer
     */
    void clearScreen(glm::vec4 color, bool clearColorBuffer=true, bool clearDepthBuffer=true);

    /**
     * Update window with OpenGL rendering
     */
    void swapWindow();

    /**
     * GPU command buffer (must be called when profiling GPU time - should not be called when not profiling)
     */
    void finishGPUCommandBuffer();

    // return stats of the last rendered frame
    // only data maintained by SRE is included (imgui is not included)
    const RenderStats& getRenderStats();

    /**
     * Singleton reference to the engine after initialization.
     */
    static RenderEngine* instance;
private:
    //textures
    unsigned int gbuffer_texture;
    unsigned int color_texture;
    unsigned int normal_texture;
    
    unsigned int framebuffer;
    unsigned int renderbuffer;

    //shaders
    Shader* deferred;

    Mesh* quad;

    //general settings
    glm::vec4 ambientLight = glm::vec4(0.2,0.2,0.2,0.2);
    Light sceneLights[maxSceneLights];
    Camera defaultCamera;
    Camera *camera;
    SDL_Window *window;
    SDL_GLContext glcontext;

    friend class Camera;

    void setupShader(const glm::mat4 &modelTransform, Shader *shader);


    RenderStats renderStatsLast;
    RenderStats renderStats;

    friend class Mesh;
    friend class Mesh::MeshBuilder;
    friend class Shader;
    friend class Shader;
    friend class Texture;
};
