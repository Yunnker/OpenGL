#include <GL/glew.h>        //OpenGL Extension Wrangler - OpenGL loader
#include <GLFW/glfw3.h>     //OpenGL

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  //Sets the OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "WE ARE SO BARACK", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); //Turns on the v-sync

    if (glewInit() != GLEW_OK) //Initialize the GLEW library
        std::cout << "GLEW is not okay!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl; //Prints out the OpenGL version
    {
		GLFWimage images[1];
		images[0].pixels = stbi_load("res/textures/clue.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
		glfwSetWindowIcon(window, 1, images);
		stbi_image_free(images[0].pixels);

		float positions[] = {
			 -50.0f,  -50.0f, 0.0f, 0.0f, // 0
              50.0f,  -50.0f, 1.0f, 0.0f, // 1
              50.0f,   50.0f, 1.0f, 1.0f, // 2
			 -50.0f,   50.0f, 0.0f, 1.0f, // 3
		};

        /* Faces */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GlCall(glEnable(GL_BLEND));
        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;                                         //Creates a vertex array
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));      //Creates vertex buffer of size 4 times 2 columns of floats

        VertexBufferLayout layout;                              //Creates a layout
        layout.Push<float>(2);                                  //Adds 2 floats to the layout for faces
		layout.Push<float>(2);                                  //Adds 2 floats to the layout for space for texture
        va.AddBuffer(vb, layout);                               //Adds the vertex buffer with layout to vertex array

        IndexBuffer ib(indices, 6);                             //Creates the Index buffer that stores the faces

        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3( 0, 0, 0));
        
        
        Shader shader("res/shaders/Basic.shader");              //Creates a shader object and loads it with Basic.shader
        shader.Bind();                                          //Selects the shader to set uniform
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f); //Sets the values to uniform
        

        Texture texture("res/textures/logo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        /* Unbinding of everything */
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);


        /* Variables to modify the shader by uniform */
        float r = 0.0f;
        float increment = 0.005f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfw_NewFrame();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            shader.Bind();                                           //Selects the shader we are using
            {
			    glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);

			    glm::mat4 mvp = proj * view * model;

                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);  
            }
            
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);

                glm::mat4 mvp = proj * view * model;

                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }





            /* Incrementing and decrementing the uniform  */
            if (r > 1.0f)
                increment = -0.005f;
            else if (r < 0.0f)
                increment = 0.005f;
            r += increment;

			{
				ImGui::SliderFloat2("Translation A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat2("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			}

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}