#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include "AntTweakBar.h"

#include "Skeleton.h"
#include "Renderer.h"
#include "GLFWRenderer.h"
#include "Frustum.h"
#include "Camera.h"
#include "Skin.h"
#include "Animation.h"
#include "StereoCamera.h"

// === Global objects ===
Frustum g_frustum;
GLFWRenderer g_renderer;
Skeleton g_skeleton;
Skin g_skin;
Animation g_anim;

float g_zoom = 0.05f;
int g_lastMouseX, g_lastMouseY;
float g_azimuth = 0.0f, g_elevation = 0.0f;
bool g_dragging = false;
bool g_drawWireframe = false;
bool g_drawSkeleton = false;
bool g_playAnimation = true;
bool g_reverseAnimation = false;

void updateCamera(int deltaX, int deltaY)
{
    float distance = 0.7f;
    // Constrain elevation to just under PI/2 to prevent orientation issues
    g_azimuth -= 0.003f * deltaX;
    g_elevation = fmaxf(-0.48f * M_PI, fminf(0.48f * M_PI, g_elevation + 0.003f * deltaY));
    
    float y = sinf(g_elevation);
    float x = sinf(g_azimuth) * cosf(g_elevation);
    float z = cosf(g_azimuth) * cosf(g_elevation);
    
    Camera camera;
    camera.getCenter() = Vector3(x, y, z) * distance;
    Matrix4 viewMatrix = camera.getMatrix();
    g_renderer.setViewMatrix(viewMatrix);
}

// === GLFW callbacks ===
void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW error (" << error << "): " << description << std::endl;
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
    //glViewport(0, 0, width, height);
    g_frustum.setProjection(60.0f, width, height, 0.1f, 100.0f);
    //g_renderer.setProjectionMatrix(g_frustum.getMatrix());
    TwWindowSize(width, height);
}

void windowCallback(GLFWwindow *window, int width, int height)
{
    //TwWindowSize(width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (TwEventKeyGLFW(key, action))
        return;
    
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_W:
            if (action == GLFW_PRESS)
                g_drawWireframe = !g_drawWireframe;
            break;
        case GLFW_KEY_S:
            if (action == GLFW_PRESS)
                g_drawSkeleton = !g_drawSkeleton;
            break;
        case GLFW_KEY_Q:
            if (action == GLFW_PRESS)
            {
                static bool cullToggle = true;
                cullToggle = !cullToggle;
                if (cullToggle)
                    glEnable(GL_CULL_FACE);
                else
                    glDisable(GL_CULL_FACE);
            }
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS)
                g_playAnimation = !g_playAnimation;
            break;
        case GLFW_KEY_R:
            if (action == GLFW_PRESS)
                g_reverseAnimation = !g_reverseAnimation;
            break;
    }
}

void buttonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        g_dragging = false;
    
    if (TwEventMouseButtonGLFW(button, action))
        return;
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        g_dragging = true;
    
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    g_lastMouseX = x;
    g_lastMouseY = y;
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (g_dragging)
    {
        int x = xpos;
        int y = ypos;
        
        int deltaX = x - g_lastMouseX;
        int deltaY = y - g_lastMouseY;
        
        updateCamera(deltaX, deltaY);
        
        g_lastMouseX = x;
        g_lastMouseY = y;
    }
    else
        TwMouseMotion(2*xpos, 2*ypos);
}

void scrollCallback(GLFWwindow *window, double x, double y)
{
    if (TwEventMouseWheelGLFW(floor(2.0 * y)))
        return;
    
    g_zoom = std::min(10.0f, std::max(0.01f, g_zoom += y * 0.01f));
}

// === AntTweakBar callbacks ===
void TW_CALL setZoom(const void *value, void *clientData)
{
    g_zoom = *(const float *)value;
    //updateCamera(0, 0);
}

void TW_CALL getZoom(void *value, void *clientData)
{
    *(float *)value = g_zoom;
}

void TW_CALL setProperty(const void *value, void *clientData)
{
    JointDOF *property = (JointDOF*)clientData;
    property->setVal(*(const float *)value);
    
    g_skeleton.update();
}

void TW_CALL getProperty(void *value, void *clientData)
{
    JointDOF *property = (JointDOF*)clientData;
    *(float *)value = property->getVal();
}

int main(int argc, char *argv[])
{
    std::string skelFile = "wasp.skel";
    std::string skinFile = "wasp.skin";
    std::string animFile = "wasp_walk.anim";
    if (argc > 1)
        skelFile = argv[1];
    if (argc > 2)
        skinFile = argv[2];
    if (argc > 3)
        animFile = argv[3];
    
    GLFWwindow* window = nullptr;
    TwBar *tweakBar = nullptr;
    
    try
    {
        // Init GLFW
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit())
            throw std::runtime_error("Failed to init GLFW");
        
        // Use OpenGL Core 4.1
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        // Create window
        window = glfwCreateWindow(512, 512, "CSE 169 Project 2", NULL, NULL);
        if (window == nullptr)
            throw std::runtime_error("Failed to create window");
        glfwMakeContextCurrent(window);
        
        // Set GLFW callbacks
        glfwSetKeyCallback(window, keyCallback);
        glfwSetCursorPosCallback(window, mouseCallback);
        glfwSetMouseButtonCallback(window, buttonCallback);
        glfwSetWindowSizeCallback(window, windowCallback);
        glfwSetFramebufferSizeCallback(window, resizeCallback);
        glfwSetScrollCallback(window, scrollCallback);
        
        // Initialize AntTweakBar
        TwInit(TW_OPENGL_CORE, NULL);
        
        // Create tweak bar
        tweakBar = TwNewBar("TweakBar");
        TwDefine(" TweakBar size='150 400' color='96 216 224' fontsize=3 ");
        TwAddVarCB(tweakBar, "Zoom", TW_TYPE_FLOAT, setZoom, getZoom, NULL, " min=0.01 max=10 step=0.01 ");
        
        // Initialize the renderer
        g_renderer.initialize();
        
        // Get initial window size
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        resizeCallback(window, width, height);
        glfwGetWindowSize(window, &width, &height);
        windowCallback(window, width, height);

        StereoCamera camera(.33f, .206f, .0065f);
        camera.makeFrustum(.30f, .75f, 100.f);
        
        // Load the skeleton file
        g_skeleton.load(skelFile);
        g_skeleton.update();
        //g_skeleton.print(std::cout);
        
        // Using this for animation for now...
        std::vector<JointDOFPtr> dofList = g_skeleton.getDOFList();
        
        // Create sliders for each DOF
        for (JointDOFPtr dof : dofList)
        {
            std::stringstream ss;
            ss << " min=" << dof->getMin() << " max=" << dof->getMax() << " step=0.02";
            TwAddVarCB(tweakBar, dof->getName().c_str(), TW_TYPE_FLOAT, setProperty, getProperty, dof.get(), ss.str().c_str());
            //std::cout << *dof << std::endl;
        }
        
        // Load the skin file
        g_skin.load(skinFile);
        
        float animationTime = 0.0f;
        g_anim.load(animFile);
        
        updateCamera(0, 0);
        
        while (!glfwWindowShouldClose(window))
        {
            // Get time in seconds since last update
            static double lastTime = glfwGetTime();
            double currentTime = glfwGetTime();
            double elapsedTime = currentTime - lastTime;
            lastTime = currentTime;

            g_renderer.preRender();
            
            // Update animation
            if (g_playAnimation)
            {
                float step = float(g_reverseAnimation ? -elapsedTime : elapsedTime);
                animationTime += step;
                
                g_anim.animate(dofList, animationTime);
                g_skeleton.update();
                TwRefreshBar(tweakBar);
            }

            Matrix4 matrix = Matrix4().scale(g_zoom);

            // Left eye
            g_renderer.setProjectionMatrix(camera.getLeftMatrix());
            glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);

            // Render skeleton or skin
            glPolygonMode(GL_FRONT_AND_BACK, g_drawWireframe ? GL_LINE : GL_FILL);
            g_renderer.setModelMatrix(matrix);
            if (g_drawSkeleton)
                g_renderer.drawSkeleton(g_skeleton);
            else
                g_renderer.drawSkin(g_skin, g_skeleton);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Right eye
            g_renderer.setProjectionMatrix(camera.getRightMatrix());
            glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
            glClear(GL_DEPTH_BUFFER_BIT);

            // Render skeleton or skin
            glPolygonMode(GL_FRONT_AND_BACK, g_drawWireframe ? GL_LINE : GL_FILL);
            g_renderer.setModelMatrix(matrix);
            if (g_drawSkeleton)
                g_renderer.drawSkeleton(g_skeleton);
            else
                g_renderer.drawSkin(g_skin, g_skeleton);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            TwDraw();
            
            g_renderer.postRender();
            
            // Update window and events
            // should be part of postRender?
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    catch (std::exception& err)
    {
        std::cerr << err.what() << std::endl;
    }
    
    if (tweakBar)
        TwTerminate();
    if (window)
        glfwTerminate();
    
    return 0;
}
