#include <cstdlib>
#include <ios>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <irrKlang.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "framework/globals.hpp"
#include "framework/tilemap.hpp"
#include "framework/vao.hpp"
#include "framework/vbo.hpp"
#include "framework/vbl.hpp"
#include "framework/ibo.hpp"
#include "framework/shader.hpp"
#include "framework/texture.hpp"
#include "framework/renderer.hpp"
#include "framework/entity.hpp"

// Function declarations
GLFWwindow* initWindow();

void updateDeltaTime(GLfloat& dt, GLfloat& ct, GLfloat& lt);
bool removeCollectible(std::vector<framework::Vertex>& collectibles, int xPos, int yPos);


// Error function for GLFW
void GLFWErrorCallback(int code, const char* description);

// Error function for OpenGL
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

//------------------------------------------------------------------------------------
//                                     Main
//------------------------------------------------------------------------------------

int main() 
{
    glfwSetErrorCallback(GLFWErrorCallback);

    auto window = initWindow();
    if (window == nullptr)
    {
        glfwTerminate();
        std::cin.get();
        return EXIT_FAILURE;
    }

    // Enable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // Print OpenGL data
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

    // Clear the background
    glClearColor(0.4f, 0.0f, 0.4f, 1.0f);

    // Initializing music
    /*static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    static irrklang::ISound* music = soundEngine->play2D(framework::SOUNDTRACKPATH.c_str(), GL_TRUE, GL_FALSE, GL_TRUE);
    music->setVolume(framework::MUSICVOLUME);*/


    // Reading and creating the map
    //framework::Map map1(framework::LEVELPATH0);
    //map1.PrintMap();

    static framework::Renderer renderer;
    renderer.EnableBlending();
    renderer.EnableDepthTesting();

    // Variables used to find delta time
    static GLfloat dt, curTime, lastTime;
    dt = curTime = lastTime = 0.0f;

    //framework::VertexArray tileVao;               // Create a vertex array
    //framework::VertexBuffer tileVbo(vertices.wallVertices);    // Create a vertex buffer

    //framework::VertexBufferLayout vbl;            // Create a vertex buffer layout
    //vbl.Push<GLfloat>(3);                         // Adding position floats to layout
    //vbl.Push<GLfloat>(3);                         // Adding color floats to layout
    //vbl.Push<GLfloat>(2);                         // Adding tex coords floats to layout

    //tileVao.AddBuffer(tileVbo, vbl);              // Populating the vertex buffer
    //framework::IndexBuffer tileIbo(wallIndices);


    //                      Preparing collectibles

    /*framework::VertexArray collVao;
    framework::VertexBuffer collVbo(vertices.collectibleVertices);
    collVao.AddBuffer(collVbo, vbl);
    framework::IndexBuffer collIbo(collIndices);*/

    framework::Shader charShader(framework::CHARVERTGSHADERPATH, framework::CHARFRAGSHADERPATH);

    framework::Texture pacTex(framework::PACMANPICTUREPATH);    // Loading texture for pacman
    pacTex.Bind(0);

    framework::Texture ghostTex(framework::GHOSTPICTUREPATH);   // Loading texture for ghost
    ghostTex.Bind(1);

    framework::Entity pacman(glm::vec3(0.0f, 1.0f, 2.0f), framework::PACMANMODELPATH);  // Creating pacman entity with model

    // Creating ghosts using model
    std::vector<std::shared_ptr<framework::Entity>> ghosts;
    for (int i = 0; i < 1; i++)
    {
        auto ghost = std::make_shared<framework::Entity>(glm::vec3(0.0f, 1.0f, -1.0f), framework::GHOSTMODELPATH);
        ghosts.push_back(ghost);
    }

    while (!glfwWindowShouldClose(window))
    {
        //                   Preparation
        updateDeltaTime(dt, curTime, lastTime);
        glfwPollEvents();

        renderer.Clear();   // Clearing screen


        //                              Draw calls

        //renderer.Draw(tileVao, tileIbo, tileShader);    // Drawing map
        //collVbo.UpdateData(vertices.collectibleVertices);
        //renderer.Draw(collVao, collIbo, tileShader);

        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pacman.UpdatePos(dt, framework::Direction::FORWARD);
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pacman.UpdatePos(dt, framework::Direction::BACK);
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pacman.UpdatePos(dt, framework::Direction::RIGHT);
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pacman.UpdatePos(dt, framework::Direction::LEFT);
        }

        auto view = glm::lookAt(glm::vec3(12.f, 1.f, 0.f), { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
        auto proj = glm::perspective(glm::radians(45.f), (float)framework::WINDOWSIZEX / (float)framework::WINDOWSIZEY, 0.01f, 900.f);

        charShader.SetUniform1i("numTex", 0);
        pacTex.Bind(0);
        pacman.Draw(charShader, view, proj);

        ghostTex.Bind(1);
        charShader.SetUniform1i("numTex", 1);
        for (const auto& ghost : ghosts)
            ghost->Draw(charShader, view, proj);

        glfwSwapBuffers(window);

        // Exit the loop if escape is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}


//------------------------------------------------------------------------------------------
//                                  Functions
//------------------------------------------------------------------------------------------

// GLFW and window initialization
GLFWwindow* initWindow()
{
    if (!glfwInit())
    {
        return nullptr;
    }

    // Window hints
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(framework::WINDOWSIZEX, framework::WINDOWSIZEY, "2D PacMan", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);
    return window;
}

void GLFWErrorCallback(int code, const char* description)
{
    std::cerr << "Error " << "0x" << std::hex << code << ':' << description << "\n";
}

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
        "type = 0x" << type <<
        ", severity = 0x" << severity <<
        ", message =" << message << "\n";
}

void updateDeltaTime(GLfloat& dt, GLfloat& ct, GLfloat& lt)
{
    ct = static_cast<GLfloat>(glfwGetTime());
    dt = ct - lt;
    lt = ct;
}

// Recieves player position and the collectibles container, goes through the container until it finds the element with
//  the correct data and removes the 4 vertices from that one;
bool removeCollectible(std::vector<framework::Vertex> &collectibles, int xPos, int yPos)
{
    //// Goes through every collectible
    //for (int i = 0; i < collectibles.size(); i+=4)
    //{
    //    glm::vec2 position = collectibles[i].pos;
    //    int x = position.x, y = position.y;
    //    
    //    // If position is the same as player and not black, paint it black
    //    if (x == xPos && y == yPos && collectibles[i].col.x != 0)
    //    {
    //        collectibles[i].col.x = 0;
    //        collectibles[i].col.y = 0;
    //        collectibles[i].col.z = 0;
    //        
    //        collectibles[(i + 1)].col.x = 0;
    //        collectibles[(i + 1)].col.y = 0;
    //        collectibles[(i + 1)].col.z = 0;
    //        
    //        collectibles[(i + 2)].col.x = 0;
    //        collectibles[(i + 2)].col.y = 0;
    //        collectibles[(i + 2)].col.z = 0;
    //        
    //        collectibles[(i + 3)].col.x = 0;
    //        collectibles[(i + 3)].col.y = 0;
    //        collectibles[(i + 3)].col.z = 0;
    //        return 1;
    //    }
    //}
    return 0;
}

