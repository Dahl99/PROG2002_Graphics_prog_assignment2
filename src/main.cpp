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
#include "ghost.hpp"

// Function declarations
GLFWwindow* initWindow(const std::string& title = "Window");

void updateDeltaTime(GLfloat& dt, GLfloat& ct, GLfloat& lt);
bool removeCollectible(std::vector<framework::Vertex>& collectibles, int xPos, int zPos);


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

    auto window = initWindow("3D Pacman");
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

    // Additional level made as example for proof of the code handling different maps. It is not optimal as there was no\
        optimal view pos calculation we could find that would work for all mapsizes. there is currently one for the extra map\
        and the default one for the standard map

    //framework::Map map1(framework::LEVELPATH1);
    map1.PrintMap();

    static framework::Renderer renderer;
    renderer.EnableBlending();
    renderer.EnableDepthTesting();
    renderer.EnableSRGBFramebuffer();
    renderer.SetClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // Initializing music
    static irrklang::ISoundEngine* soundEngine = irrklang::createIrrKlangDevice();
    static irrklang::ISound* music = soundEngine->play2D(framework::SOUNDTRACKPATH.c_str(), GL_TRUE, GL_FALSE, GL_TRUE);
    music->setVolume(framework::MUSICVOLUME);

    // Variables used to find delta time
    static GLfloat dt, curTime, lastTime, pacmanAnimTimer;
    dt = curTime = lastTime = pacmanAnimTimer = 0.0f;

    static int pelletsCollected = 0;

    framework::ShaderVertData vertices = map1.RetMapVertices();
    framework::IndiceData indices = map1.RetMapIndices();

    framework::VertexArray tileVao;               // Create a vertex array
    framework::VertexBuffer tileVbo(vertices.wallVertices);    // Create a vertex buffer

    framework::VertexBufferLayout vbl;            // Create a vertex buffer layout
    vbl.Push<GLfloat>(3);                         // Adding position floats to layout
    vbl.Push<GLfloat>(3);                         // Adding normal floats to layout
    vbl.Push<GLfloat>(2);                         // Adding tex coords floats to layout

    tileVao.AddBuffer(tileVbo, vbl);              // Populating the vertex buffer
    framework::IndexBuffer tileIbo(indices.walls);

    framework::Texture wallTex(framework::WALLPICTUREPATH);

    //                      Preparing collectibles
    framework::VertexArray collVao;
    framework::VertexBuffer collVbo(vertices.collectibleVertices);
    collVao.AddBuffer(collVbo, vbl);

    framework::IndexBuffer collIbo(indices.collectibles);

    /** Creating model matrix for walls and collectibles
     *  As well as view and projection matrices for everything
     */
    auto tileModelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(1.f));
    glm::vec3 viewPos;
    
    if (map1.GetArray().size() == (30 * 40))
        viewPos = glm::vec3(14.f, 44.f, 0.f);
    else viewPos = glm::vec3(14.f, 34.f, 0.f);

    auto view = glm::lookAt(viewPos, { 14.f, 1.f, 18.f }, { 0.f, 1.f, 0.f });
    auto proj = glm::perspective(glm::radians(45.f), (float)framework::WINDOWSIZEX / (float)framework::WINDOWSIZEY, 0.01f, 900.f);

    // Creating shaders and setting most uniforms
    framework::Shader shader(framework::VERTSHADERPATH, framework::FRAGSHADERPATH);
    framework::Shader lightSrcShader(framework::VERTLIGHTSRCSHADERPATH, framework::FRAGLIGHTSRCSHADERPATH);

    shader.Bind();
    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Projection", proj);

    for (int i = 0; i < framework::NUMGHOSTS + 1; i++)
    {
        shader.SetUniform3fv("u_PointLights[" + std::to_string(i) + "].color", glm::vec3(1.0f));
        shader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].linear", 0.14f);
        shader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].quadratic", 0.07f);
    }

    lightSrcShader.Bind();
    lightSrcShader.SetUniformMat4f("u_View", view);
    lightSrcShader.SetUniformMat4f("u_Projection", proj);

    const auto& characterPositions = map1.GetPGPos();    // Getting player and ghost positions

    // Loading both pacman models and their texture
    std::unique_ptr<framework::Texture> pacmanTextures[2];
    std::unique_ptr<framework::Entity> pacmanEntities[2];
    for (int i = 0; i < 2; i++)
    {
        pacmanTextures[i] = std::make_unique<framework::Texture>(framework::PACMANTEXTUREPATHS[i]);
        pacmanEntities[i] = std::make_unique<framework::Entity>(characterPositions.front(), framework::PACMANMODELPATHS[i]);
        pacmanEntities[i]->SetScale(glm::vec3(0.5f));
    }


    // Loading ghosts and their textures
    std::unique_ptr<framework::Texture> ghostTextures[framework::NUMGHOSTS];
    std::unique_ptr<framework::Model> ghostModel = std::make_unique<framework::Model>(framework::GHOSTMODELPATH);
    std::unique_ptr<Ghost> ghosts[framework::NUMGHOSTS];
    for (int i = 0; i < framework::NUMGHOSTS; i++)
    {
        ghostTextures[i] = std::make_unique<framework::Texture>(framework::GHOSTTEXTUREPATHS[i]);
        ghosts[i] = std::make_unique<Ghost>(characterPositions.at(i+1), ghostModel->GetVertices(), ghostModel->GetIndices());
        ghosts[i]->SetScale(glm::vec3(0.5f));
    }

    framework::Texture collTex(framework::COLLECTIBLEPICTUREPATH);


    // ImGui setup
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430 core");

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoDecoration;
    window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

    bool exitFlag = false;
//------------------------------------------------------------------------------------------
//                                  Game loop
//------------------------------------------------------------------------------------------
    
    while (!glfwWindowShouldClose(window) && exitFlag != true)
    {
        //                   Preparation
        updateDeltaTime(dt, curTime, lastTime);
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

        if (removeCollectible(vertices.collectibleVertices, pacmanEntities[0]->GetPosition().x, pacmanEntities[0]->GetPosition().z - 1.1f))
        {
            
            pelletsCollected++;
            collVbo.UpdateData(vertices.collectibleVertices);
            if (pelletsCollected == map1.GetNumCollecs())
                exitFlag = true;
      
        }
 
        for (int i = 0; i < framework::NUMGHOSTS; i++)
            if (ghosts[i]->CollisionCheck(pacmanEntities[0]->GetPosition()))
                exitFlag = true;

        // Move upward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if (pacmanEntities[0]->GetPosition().z < map1.GetSizeY())
            {
                if (map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z) * map1.GetSizeX()) + ((int)pacmanEntities[0]->GetPosition().x + 0.1)] != 1 &&
                    map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z) * map1.GetSizeX()) + ((int)pacmanEntities[0]->GetPosition().x + 0.9)] != 1)
                {
                    pacmanEntities[0]->Move(dt, framework::Direction::FORWARD);
                    pacmanEntities[0]->SetRotation(270.f);
                }
            }
        }
        // Move downward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

            if (pacmanEntities[0]->GetPosition().z > 0)
            if (map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 1) * map1.GetSizeX()) + ((int)pacmanEntities[0]->GetPosition().x)] != 1 &&
                map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 1) * map1.GetSizeX()) + ((int)pacmanEntities[0]->GetPosition().x + 0.9)] != 1)
            {
                pacmanEntities[0]->Move(dt, framework::Direction::BACK);
                pacmanEntities[0]->SetRotation(90.f);
            }
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            if (pacmanEntities[0]->GetPosition().x - 1 > 0) 
            {
                if (map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 0.1) * map1.GetSizeX()) + (int)(pacmanEntities[0]->GetPosition().x - 1)] !=     1 &&
                    map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 0.9) * map1.GetSizeX()) + (int)(pacmanEntities[0]->GetPosition().x - 1)] != 1)
                {
                    pacmanEntities[0]->Move(dt, framework::Direction::RIGHT);
                    pacmanEntities[0]->SetRotation(180.f);
                }

            }
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

            if (map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 0.1) * map1.GetSizeX()) + (int)(pacmanEntities[0]->GetPosition().x + 0.1)]
                != 1 && 
                map1.GetArray()[(int)((int)(pacmanEntities[0]->GetPosition().z - 0.9) * map1.GetSizeX()) + (int)(pacmanEntities[0]->GetPosition().x + 0.1)]
                != 1)
            {
                pacmanEntities[0]->Move(dt, framework::Direction::LEFT);
                pacmanEntities[0]->SetRotation(0.f);
            }   
        }
        pacmanEntities[1]->SetPosition(pacmanEntities[0]->GetPosition());
        pacmanEntities[1]->SetRotation(pacmanEntities[0]->GetRotation());

        shader.Bind();
        shader.SetUniformMat4f("u_Model", tileModelMatrix);
        shader.SetUniform3fv("u_ViewPos", viewPos);

        shader.SetUniform3fv("u_PointLights[0].position", pacmanEntities[0]->GetPosition());
        for (int i = 0; i < framework::NUMGHOSTS; i++)
            shader.SetUniform3fv("u_PointLights[" + std::to_string(i+1) + "].position", ghosts[i]->GetPosition());

        // Pacman animation logic
        if (pacmanAnimTimer < 1.5f)
        {
            pacmanTextures[0]->Bind();
            pacmanEntities[0]->Draw(lightSrcShader, view, proj);
            pacmanAnimTimer += 0.05f;
        }
        else if (pacmanAnimTimer < 3.0f)
        {
            pacmanTextures[1]->Bind();
            pacmanEntities[1]->Draw(lightSrcShader, view, proj);
            pacmanAnimTimer += 0.05f;

            if (pacmanAnimTimer >= 3.0f)
                pacmanAnimTimer = 0.0f;
        }

        // Drawing ghosts
        for (int i = 0; i < framework::NUMGHOSTS; i++)
        {
            ghostTextures[i]->Bind();
            //ghosts[i]->GhostMovement(pacmanEntities[0]->GetPosition(), map1.GetArray(), map1.GetSizeX(), dt);
            ghosts[i]->Behaviour(map1.GetArray(), map1.GetSizeX(), dt);
            ghosts[i]->Draw(shader, view, proj);
        }

        shader.Bind();
        shader.SetUniformMat4f("u_Model", tileModelMatrix);

        // Drawing walls and collectibles
        wallTex.Bind();
        renderer.Draw(tileVao, tileIbo, shader);
        collTex.Bind();
        renderer.Draw(collVao, collIbo, shader);

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
GLFWwindow* initWindow(const std::string& title)
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
    GLFWwindow* window = glfwCreateWindow(framework::WINDOWSIZEX, framework::WINDOWSIZEY, title.c_str(), nullptr, nullptr);
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
bool removeCollectible(std::vector<framework::Vertex>& collectibles, int xPos, int zPos)
{
    for (int i = 0; i < collectibles.size(); i += 24) // Looping through all collectible tiles
    {
        glm::vec3 position = collectibles.at(i).pos;
        int x = position.x, z = position.z;

        if (x == xPos && z == zPos) // If collectible has same position as player
        {
            for (int j = 0; j < 24; j++) // Looping through all vertices related to tile
            {
                // Doing something cheap by moving them elsewhere
                collectibles.at(i + j).pos.x = 500.f;
                collectibles.at(i + j).pos.y = 500.f;
                collectibles.at(i + j).pos.z = 500.f;
            }

            return true;
        }
    }

    return false;
}