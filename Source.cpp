

#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1   
#include <GL/glew.h>    

#include <GLFW/glfw3.h> 

#include "glm/glm.hpp"  
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>


using namespace glm;
using namespace std;



const char* getVertexShaderSource()
{
    
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  
        "uniform mat4 projectionMatrix = mat4(1.0);" //passed vec4 instead because  theres no good reason to convert later 
        ""
        "out vec4 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = vec4(aColor, 1.0f);"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec4 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vertexColor;"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    
    // gimme dat vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void buildGrid(GLuint worldMatrixLocation) 
{ //uses the world matrix location and a for loop to construct the x/z grid for the ground 
    //draw x 
    for (float i = -50; i <= 50; i++)
    {
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, i)) * scale(mat4(1.0f), vec3(100.0f, 0.00f, 0.0f));

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);


        glDrawArrays(GL_LINES, 0, 36);
    }

    //draw z gridlines 
    for (float i = -50; i <= 50; i++)
    {
        mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(i, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.00f, 0.0f, 100.0f));

        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);


        glDrawArrays(GL_LINES, 0, 36);

    }
}


void olafBody(GLuint worldMatrixLocation, GLenum renderMode) 
{
    mat4 olafBotBody = translate(mat4(1.0f), vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBotBody[0][0]);
    glDrawArrays(renderMode, 0, 36);

    mat4 olafTopBody = translate(mat4(1.0f), vec3(0.0f, 1.75f, 0.0f)) * scale(mat4(1.0f), vec3(0.75f, 0.75f, .75));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafTopBody[0][0]);
    glDrawArrays(renderMode, 0, 36);

}

void olafHead(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafHead = translate(mat4(1.0f), vec3(0.0f, 2.3f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 0.5f, .5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHead[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void olafNose(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafNose = translate(mat4(1.0f), vec3(0.25f, 2.25f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafNose[0][0]);
    glDrawArrays(renderMode, 180, 36);
}

void olafHand(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafHand = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 2.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHand[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void olafRLeg(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafRLeg = translate(mat4(1.0f), vec3(0.0f, 0.25f, -0.2f)) * scale(mat4(1.0f), vec3(0.15f, .5f, 0.15f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRLeg[0][0]);
    glDrawArrays(renderMode, 0, 36);
}

void olafLLeg(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafLLeg = translate(mat4(1.0f), vec3(0.0f, 0.25f, 0.2f)) * scale(mat4(1.0f), vec3(0.15f, .5f, 0.15f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLLeg[0][0]);
    glDrawArrays(renderMode, 0, 36);

}

void olafREye(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafREye = translate(mat4(1.0f), vec3(0.25f, 2.45f, -0.15f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafREye[0][0]);
    glDrawArrays(renderMode, 216, 36);
}

void olafLEye(GLuint worldMatrixLocation, GLenum renderMode)
{
    mat4 olafLEye = translate(mat4(1.0f), vec3(0.25f, 2.45f, 0.15f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLEye[0][0]);
    glDrawArrays(renderMode, 216, 36);
}

void doYouWantToBuildASnowman(GLuint worldMatrixLocation, GLenum renderMode)
{
    //takes all parameters needed for input and passes them to the appropriate struct for each body pary
    //eventually this will build the whole snowman according to what input calls it 
    olafBody(worldMatrixLocation, renderMode);
    olafHead(worldMatrixLocation, renderMode);
    olafNose(worldMatrixLocation, renderMode);
    olafHand(worldMatrixLocation, renderMode);
    olafREye(worldMatrixLocation, renderMode);
    olafLEye(worldMatrixLocation, renderMode);
    olafRLeg(worldMatrixLocation, renderMode);
    olafLLeg(worldMatrixLocation, renderMode);
}

int createVertexBufferObject()
{
    // Cube model
    vec3 vertexArray[] = {       // position, then all white for easy products                          
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f), //left - 0
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f), // far 6
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // bottom 12
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // near 18
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // right 24
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f), // top 30
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
        //NEW CUBE GREEN COLOR 36-71
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f), //left 36
        vec3(-0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f), // far 42
        vec3(-0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f), // bottom 48
        vec3(-0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(-0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f), // near 54
        vec3(-0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f),  vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // right 60
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // top 66
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
          // new cube red color 72-107
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left -72
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), // far 78
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // bottom 84
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // near 90
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // right 96
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), // top 102
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        //blue cube
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), //left -108
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f), // far 114
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // bottom 120
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // near 126
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // right 132
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f), // top 138
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
        //black cube
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), //left -144
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), // far 150
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // bottom 156
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // near 162
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // right 168
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // top 174
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        //orange cubarino
        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f), //left -180
        vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f), // far 186
        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f), // bottom 192
        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f), // near 198
        vec3(-0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f), // right 204
        vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f), // top 210
        vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.39f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.39f, 0.0f),
        //cube of color
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), //left -216
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f), // far 222
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // bottom 228
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // near 234
        vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // right 240
        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f), // top 246
        vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 0.0f)



    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3),    // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}

int main(int argc, char* argv[])
{//hidden init context
    // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    // Create Window and rendering context using GLFW
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Assignment 1 - Olaf", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here for grid ...
    int shaderProgram = compileAndLinkShaders();
    
    
    glUseProgram(shaderProgram);
    

    // Camera parameters for view transform
    vec3 cameraPosition(15.0f, 15.0f, 10.0f);
    vec3 cameraLookAt(0.0f, -1.0f, 0.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;



    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(89.0f,            // field of view in degrees
        1024.0f / 786.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);


    // Define and upload geometry to the GPU here ...
    int vao = createVertexBufferObject();
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    glEnable(GL_CULL_FACE|GL_DEPTH_TEST);

    bool olafBool = true;
   
    GLenum renderMode = GL_TRIANGLES;

    
    glBindVertexArray(vao);


    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        mat4 viewMatrix = lookAt(cameraPosition, cameraLookAt, cameraUp);

        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        
        // Draw geometry
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        
       

        
        //draw gridlines
        buildGrid(worldMatrixLocation);
       


 
        //draw x axis 
        
        mat4 xAxisWorldMatrix = translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0)) * scale(mat4(1.0f), vec3(5.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &xAxisWorldMatrix[0][0]);
        glDrawArrays(GL_LINES, 72, 36); //LET ME JUST RANT ABOUT THIS IT TOOK ME FOUR HOURS TO REALIZE THIS DIDNT GO FROM THE SECOND PARAMETER TO THE THIRD, IT STARTS AT THE SECOND AND GOES ON FOR THE NUMBER THAT THE THIRD IS AdklfhjSD;GLKHJASD;LFGJKHASDLFGJHASDLFGKJHASDLGKJHASDLGKJHASDG
        //draw y axis

        mat4 yAxisWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.0f, 5.0f, 0.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &yAxisWorldMatrix[0][0]);
        glDrawArrays(GL_LINES, 36, 36);
       
        
        //draw z axis 
        mat4 zAxisWorldMatrix = translate(mat4(1.0f), vec3 (0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.0f, 0.0f, 5.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &zAxisWorldMatrix[0][0]);
        glDrawArrays(GL_LINES, 108, 36);
       
        //test body draw 
        if (olafBool) {
            doYouWantToBuildASnowman(worldMatrixLocation, renderMode);
        }
        
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        //key inputs will go heyyaaaaaahhh
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                    glfwSetWindowShouldClose(window, true);

        //reposition olaf with space
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            olafBool = true;
        }
        
        //scale olaf upwards with U
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) {

        }

        //scale olaf downwards with J 
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_RELEASE) {

        }

        //move olaf left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {

        }

        //move olaf right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {

        }

        //move olaf up
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {

        }

        //move olaf down
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {

        }


        //rotates anti clockwise about positive x axis 
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {

        }

        //rotates anticlockwise about negative x axis 
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {

        }

        //rotates anti clockwise about positive y axis 
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {

        }

        //rotates anti clockwise about negative y axis
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {

        }


        //changes render mode to points
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {

            renderMode = GL_POINTS;

        }

        //changes render mode to lines
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
            renderMode = GL_LINES;
        }

        //changes render mode to triangles
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
            renderMode = GL_TRIANGLES;
        }

        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

        //mouse controls for camera

        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        const float cameraAngularSpeed = 60.0f;

        //while right clicked, pan
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dx = mousePosX - lastMousePosX;     
            lastMousePosX = mousePosX;
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
            glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
        }

        //while middle clicked, tilt
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dy = mousePosY - lastMousePosY;
            lastMousePosY = mousePosY;
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
            glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
        }
        //while left clicked zoom
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            float dz = mousePosY - lastMousePosY;
            lastMousePosY = mousePosY;
            cameraPosition -= cameraLookAt * dz * dt * cameraFastSpeed;
            
        }



        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

       // glm::normalize(cameraSideVector);

        // Set the view matrix 
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

       

    }


    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
