#include <GL/glew.h>        //OpenGL Extension Wrangler - OpenGL loader
#include <GLFW/glfw3.h>     //OpenGL

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "Shader.h"


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
    window = glfwCreateWindow(460, 360, "WE ARE SO BARACK", NULL, NULL);
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
        /* Positions of our triangles */
        float positions[] = {
            -1.0f, -1.0f, // 0
             1.0f, -1.0f, // 1
             1.0f,  1.0f, // 2
            -1.0f,  1.0f, // 3
        };

        /* Faces */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;                                         //Creates a vertex array
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));      //Creates vertex buffer of size 4 times 2 collumns of floats
        VertexBufferLayout layout;                              //Creates a layout
        layout.Push<float>(2);                                  //Sets the layout to 2 floats
        va.AddBuffer(vb, layout);                               //Adds the vertex buffer with layout to vertex array
        IndexBuffer ib(indices, 6);                             //Creates the Index buffer that stores the faces
        
        Shader shader("res/shaders/Basic.shader");              //Creates a shader object and loads it with Basic.shader
        shader.Bind();                                          //Selects the shader to set uniform
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f); //Sets the values to uniform


        /* Unbinding of everything */
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        /* Variables to modify the shader by uniform */
        float r = 0.0f;
        float increment = 0.005f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();                                          //Selects the shader we are using
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);    //Changes the value of the uniform

            va.Bind();                                              //Selects the vertices
            ib.Bind();                                              //Selects the faces


            GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));  //Draws the triangles

            /* Incrementing and decrementing the uniform  */
            if (r > 1.0f)
                increment = -0.005f;
            else if (r < 0.0f)
                increment = 0.005f;
            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}