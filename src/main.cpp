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


    // Reading and creating the map
    framework::Map map1(framework::LEVELPATH0);
    map1.PrintMap();

    static framework::Renderer renderer;
    renderer.EnableBlending();
    renderer.EnableDepthTesting();
    renderer.SetClearColor(glm::vec4(0.3f, 0.0f, 0.3f, 1.0f));

    // Initializing music
    static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    static irrklang::ISound* music = soundEngine->play2D(framework::SOUNDTRACKPATH.c_str(), GL_TRUE, GL_FALSE, GL_TRUE);
    music->setVolume(framework::MUSICVOLUME);

    // Variables used to find delta time
    static GLfloat dt, curTime, lastTime;
    dt = curTime = lastTime = 0.0f;

    framework::ShaderVertData vertices = map1.retMapVertices();

    framework::VertexArray tileVao;               // Create a vertex array
    framework::VertexBuffer tileVbo(vertices.wallVertices);    // Create a vertex buffer

    framework::VertexBufferLayout vbl;            // Create a vertex buffer layout
    vbl.Push<GLfloat>(3);                         // Adding position floats to layout
    vbl.Push<GLfloat>(3);                         // Adding normal floats to layout
    vbl.Push<GLfloat>(2);                         // Adding tex coords floats to layout

    tileVao.AddBuffer(tileVbo, vbl);              // Populating the vertex buffer
    framework::IndexBuffer tileIbo(map1.retMapIndices(0));

    framework::Texture wallTex(framework::WALLPICTUREPATH);
    wallTex.Bind(0);


    //                      Preparing collectibles

    /*framework::VertexArray collVao;
    framework::VertexBuffer collVbo(vertices.collectibleVertices);
    collVao.AddBuffer(collVbo, vbl);
    framework::IndexBuffer collIbo(collIndices);*/
    auto wallModel = glm::translate(glm::mat4(1.f), glm::vec3(1.f));
    auto view = glm::lookAt(glm::vec3(14.f, 60.f, 24.f), { 14.f, 1.f, 18.f }, { 0.f, 1.f, 0.f });
    auto proj = glm::perspective(glm::radians(45.f), (float)framework::WINDOWSIZEX / (float)framework::WINDOWSIZEY, 0.01f, 900.f);
    framework::Shader shader(framework::VERTGSHADERPATH, framework::FRAGSHADERPATH);

    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Projection", proj);

    framework::Texture pacTex(framework::PACMANPICTUREPATH);        // Loading texture for pacman
    pacTex.Bind(1);

    // Loading textures for ghosts
    framework::Texture redGhostTex(framework::GHOSTREDPICTUREPATH);
    redGhostTex.Bind(2);
    framework::Texture blueGhostTex(framework::GHOSTBLUEPICTUREPATH);
    blueGhostTex.Bind(3);
    framework::Texture orangeGhostTex(framework::GHOSTORANGEPICTUREPATH);
    orangeGhostTex.Bind(4);
    framework::Texture pinkGhostTex(framework::GHOSTPINKPICTUREPATH);
    pinkGhostTex.Bind(5);

    const auto characterPositions = map1.GetPGPos();    // Getting player and ghost positions

    framework::Entity pacman(characterPositions.front(), framework::PACMANMODELPATH);  // Creating pacman entity with model

    // Creating ghosts using model
    std::vector<std::shared_ptr<framework::Entity>> ghosts;
    for (auto it = characterPositions.begin() + 1; it != characterPositions.end(); it++)
    {
        auto ghost = std::make_shared<framework::Entity>(*it, framework::GHOSTMODELPATH);
        ghosts.push_back(ghost);
    }

    while (!glfwWindowShouldClose(window))
    {
        //                   Preparation
        updateDeltaTime(dt, curTime, lastTime);
        glfwPollEvents();

        renderer.Clear();   // Clearing screen


        //                              Draw calls

        //collVbo.UpdateData(vertices.collectibleVertices);
        //renderer.Draw(collVao, collIbo, tileShader);

        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pacman.Move(dt, framework::Direction::FORWARD);
            pacman.SetRotation(90.f);
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pacman.Move(dt, framework::Direction::BACK);
            pacman.SetRotation(270.f);
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pacman.Move(dt, framework::Direction::RIGHT);
            pacman.SetRotation(0.f);
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pacman.Move(dt, framework::Direction::LEFT);
            pacman.SetRotation(180.f);
        }


        shader.SetUniformMat4f("u_Model", wallModel);
        shader.SetUniform1i("numTex", 0);
        wallTex.Bind(0);
        renderer.Draw(tileVao, tileIbo, shader);    // Drawing map


        shader.SetUniform1i("numTex", 1);
        pacTex.Bind(1);
        pacman.Draw(shader, view, proj);

        redGhostTex.Bind(2);
        shader.SetUniform1i("numTex", 2);
        ghosts[0]->Draw(shader, view, proj);

        blueGhostTex.Bind(3);
        shader.SetUniform1i("numTex", 3);
        ghosts[1]->Draw(shader, view, proj);

        orangeGhostTex.Bind(4);
        shader.SetUniform1i("numTex", 4);
        ghosts[2]->Draw(shader, view, proj);

        pinkGhostTex.Bind(5);
        shader.SetUniform1i("numTex", 5);
        ghosts[3]->Draw(shader, view, proj);


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

