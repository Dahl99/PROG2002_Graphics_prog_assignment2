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
#include "pacman.hpp"
#include "ghost.hpp"

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

int main(void)
{

    //-------------- OpenGL initialization --------------

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Enabling blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Initializing music
    static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    static irrklang::ISound* music = soundEngine->play2D(framework::SOUNDTRACKPATH.c_str(), GL_TRUE, GL_FALSE, GL_TRUE);
    music->setVolume(framework::MUSICVOLUME);


    //------------------- Getting and preparing data --------------------

    // Reading and creating the map
    framework::Map map1(framework::LEVELPATH0);
    map1.PrintMap();

    // Getting the map data
    framework::ShaderVertData vertices = map1.retMapVertices();
    std::vector<GLuint> wallIndices = map1.retMapIndices(map1.GetNumWalls());
    std::vector<GLuint> collIndices = map1.retMapIndices(map1.GetNumCollecs());

    static framework::Renderer renderer;

    // Variables used to find delta time
    static GLfloat dt, curTime, lastTime;
    dt = curTime = lastTime = 0.0f;

    static int pelletsCollected = 0; // Keeps track of how many pellets are collected

    //                      Preparing walls

    framework::VertexArray tileVao;               // Create a vertex array
    framework::VertexBuffer tileVbo(vertices.wallVertices);    // Create a vertex buffer

    framework::VertexBufferLayout vbl;            // Create a vertex buffer layout
    vbl.Push<GLfloat>(2);                         // Adding position floats to layout
    vbl.Push<GLfloat>(3);                         // Adding color floats to layout
    vbl.Push<GLfloat>(2);                         // Adding tex coords floats to layout

    tileVao.AddBuffer(tileVbo, vbl);              // Populating the vertex buffer
    framework::IndexBuffer tileIbo(wallIndices);


    //                      Preparing collectibles

    framework::VertexArray collVao;
    framework::VertexBuffer collVbo(vertices.collectibleVertices);
    collVao.AddBuffer(collVbo, vbl);
    framework::IndexBuffer collIbo(collIndices);

    //                      Preparing player and Ghosts

    auto entityData = map1.GetPGPos(); // Fetching player and ghost positions as well as vertices

    // These indices will be used for player and ghosts
    std::vector<GLuint> charIndices = { 0, 1, 2, 2, 3, 1 };

    // Setting texture coordinates for player
    entityData.vertices[0].tex = glm::vec2(0.0f, 0.0f);
    entityData.vertices[1].tex = glm::vec2(1.0f / 6.0f, 0.0f);
    entityData.vertices[2].tex = glm::vec2(0.0f, 1.0f / 4.0f);
    entityData.vertices[3].tex = glm::vec2(1.0f / 6.0f, 1.0f / 4.0f);

    // Creating pacman character
    static Pacman pacman(entityData.positions[0], entityData.vertices, charIndices);

    // Setting texture coordinates for ghosts
    entityData.vertices[0].tex = glm::vec2(2.0f / 3.0f, 0.0f);
    entityData.vertices[1].tex = glm::vec2(5.0f / 6.0f, 0.0f);
    entityData.vertices[2].tex = glm::vec2(2.0f / 3.0f, 1.0f / 4.0f);
    entityData.vertices[3].tex = glm::vec2(5.0f / 6.0f, 1.0f / 4.0f);

    // Creating ghosts and adding them to a vector
    std::vector<std::shared_ptr<Ghost>> ghosts;
    for (int i = 1; i < entityData.positions.size(); i++)
    {
        auto ghost = std::make_shared<Ghost>(entityData.positions[i], entityData.vertices, charIndices);
        ghosts.push_back(ghost);
    }


    //                      Preparing to send data

    // Creating the MVP matrix
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f, 0.8f, 0.0f));
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 0.0f, 36.0f, -1.0f, 1.0f);


    // Initializing shaders, setting projection matrix and texture for entities

    framework::Shader tileShader(framework::TILEVERTSHADERPATH, framework::TILEFRAGSHADERPATH);
    tileShader.Bind();
    tileShader.SetUniformMat4f("u_Projection", projection);

    framework::Shader pacmanShader(framework::CHARVERTGSHADERPATH, framework::CHARFRAGSHADERPATH);
    pacmanShader.Bind();
    pacmanShader.SetUniformMat4f("u_Projection", projection);
    pacmanShader.SetUniform1i("uTexture", 0);
    
    framework::Shader ghostShader(framework::CHARVERTGSHADERPATH, framework::CHARFRAGSHADERPATH);
    ghostShader.Bind();
    ghostShader.SetUniformMat4f("u_Projection", projection);
    ghostShader.SetUniform1i("uTexture", 0);

    framework::Texture texture(framework::PACMANPICTUREPATH);
    texture.Bind(0);    //  Binding to texture slot 0


    // ImGui setup
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430 core");

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;


 
//------------------------------------------------------------------------------------
//                                 Game loop
//------------------------------------------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        //                   Preparation
        updateDeltaTime(dt, curTime, lastTime); // Updating delta time
        glfwPollEvents();

        renderer.Clear();   // Clearing screen


        // Creating ImGui textbox for displaying current score
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Score", NULL, window_flags);
        ImGui::Text("Current score: %d", pelletsCollected * framework::COLLECTIBLESCORE);
        ImGui::End();

        // Score counting
        if (removeCollectible(vertices.collectibleVertices, pacman.GetPos().x + 0.5, pacman.GetPos().y + 0.5))
        {
            pelletsCollected++; // Ups by 1 if pellet is collected
            if (pelletsCollected == map1.GetNumCollecs()) // Freezing game if all pellets are collected
                system("PAUSE");
        }

        // Checking if a ghost collides with pacman
        for (int i = 0; i < 3; i++)
            if (ghosts[i]->CollisionCheck(pacman.GetPos()))
                system("PAUSE");
        


        //                          Player movement and collision calculations

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if (map1.GetArray()[(int)(((int)(pacman.GetPos().y + 1) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 0.1))] != 1 &&
                map1.GetArray()[(int)(((int)(pacman.GetPos().y + 1) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 0.9))] != 1)
            {
                pacman.UpdatePos(dt, framework::Direction::UP);
                pacman.UpdateSprite(pacmanShader, framework::Direction::UP);
            }
        }
        // Move down
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if (map1.GetArray()[(int)(((int)(pacman.GetPos().y) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 0.1))] != 1 &&
                map1.GetArray()[(int)(((int)(pacman.GetPos().y) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 0.9))] != 1)
            {
                pacman.UpdatePos(dt, framework::Direction::DOWN);
                pacman.UpdateSprite(pacmanShader, framework::Direction::DOWN);
            }
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

            // If at right edge and moving right teleport to left edge
            if (map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.5) * map1.GetSizeX()) + (int)(pacman.GetPos().x))] == 2 && 
                pacman.GetPos().x >= map1.GetSizeX() / 2)
            {
                pacman.Teleport(0);
            }
            else if (map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.9) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 1))] != 1 &&
                map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.1) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 1))] != 1)
            {
                pacman.UpdatePos(dt, framework::Direction::RIGHT);
                pacman.UpdateSprite(pacmanShader, framework::Direction::RIGHT);
            }

        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

            // If at left edge and moving left teleport to right edge
            if (map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.5) * map1.GetSizeX()) + (int)(pacman.GetPos().x + 1))] == 2 &&
                pacman.GetPos().x <= map1.GetSizeX() / 2)
            {
                pacman.Teleport(map1.GetSizeX() - 1);
            }
            else if (map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.9) * map1.GetSizeX()) + (int)(pacman.GetPos().x))] != 1 &&
                map1.GetArray()[(int)(((int)(pacman.GetPos().y + 0.1) * map1.GetSizeX()) + (int)(pacman.GetPos().x))] != 1)
            {
                pacman.UpdatePos(dt, framework::Direction::LEFT);
                pacman.UpdateSprite(pacmanShader, framework::Direction::LEFT);
            }
        }


        //                              Draw calls

        renderer.Draw(tileVao, tileIbo, tileShader);    // Drawing map
        collVbo.UpdateData(vertices.collectibleVertices);
        renderer.Draw(collVao, collIbo, tileShader);
        pacman.Draw(pacmanShader);                  // Drawing pacman

        for (auto& ghost : ghosts)                // Drawing all ghosts
        {
            ghost->Behaviour(map1.GetArray(), map1.GetSizeX(), dt, ghostShader);
            ghost->Draw(ghostShader);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        // Exit the loop if escape is pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
    // Goes through every collectible
    for (int i = 0; i < collectibles.size(); i+=4)
    {
        glm::vec2 position = collectibles[i].pos;
        int x = position.x, y = position.y;
        
        // If position is the same as player and not black, paint it black
        if (x == xPos && y == yPos && collectibles[i].col.x != 0)
        {
            collectibles[i].col.x = 0;
            collectibles[i].col.y = 0;
            collectibles[i].col.z = 0;
            
            collectibles[(i + 1)].col.x = 0;
            collectibles[(i + 1)].col.y = 0;
            collectibles[(i + 1)].col.z = 0;
            
            collectibles[(i + 2)].col.x = 0;
            collectibles[(i + 2)].col.y = 0;
            collectibles[(i + 2)].col.z = 0;
            
            collectibles[(i + 3)].col.x = 0;
            collectibles[(i + 3)].col.y = 0;
            collectibles[(i + 3)].col.z = 0;
            return 1;
        }
    }
    return 0;
}

