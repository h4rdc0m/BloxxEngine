#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct Vertex
{
    glm::vec2 Position;
    glm::vec3 Color;
};

constexpr Vertex vertices[] = {
    {glm::vec2(-0.6f, -0.4f), glm::vec3(1.0f, 0.0f, 0.0f)},
    {glm::vec2(0.6f, -0.4f), glm::vec3(0.0f, 1.0f, 0.0f)},
    {glm::vec2(0.0f, 0.6f), glm::vec3(0.0f, 0.0f, 1.0f)},
};

static const std::string vertexShaderSrc = R"(
#version 330

uniform mat4 MVP;

layout(location = 0) in vec2 vPos;
layout(location = 1) in vec3 vCol;

out vec3 color;

void main() {
    gl_Position = MVP * vec4(vPos, 0.0, 1.0);
    color = vCol;
}
)";

static const std::string fragmentShaderSrc = R"(
#version 330

in vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

static void errorCallback(int error, const char *description)
{
    std::cerr << description << std::endl;
}

static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
        return EXIT_FAILURE;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Sandbox", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    glfwSwapInterval(1);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vsCSrc = vertexShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vsCSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(vertexShader);
        std::cerr << "Failed to compile vertex shader" << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return EXIT_FAILURE;
    }

    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto fsCSrc = fragmentShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fsCSrc, nullptr);
    glCompileShader(fragmentShader);

    success = 0;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
        glDeleteShader(fragmentShader);
        std::cerr << "Failed to compile fragment shader" << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return EXIT_FAILURE;
    }

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
        glDeleteProgram(program);
        std::cerr << "Failed to link program" << std::endl;
        std::cerr << infoLog.data() << std::endl;
        return EXIT_FAILURE;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    const GLint mvpLoc = glGetUniformLocation(program, "MVP");
    const GLint vColLoc = 1;
    const GLint vPosLoc = 0;

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glEnableVertexAttribArray(vPosLoc);
    glVertexAttribPointer(vPosLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Position));
    glEnableVertexAttribArray(vColLoc);
    glVertexAttribPointer(vColLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Color));

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = static_cast<float>(width) / static_cast<float>(height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(.3f, .3f, .3f, 1.f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));

        // Compute the View matrix (camera)
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 5.0f), // Camera position in world coordinates
            glm::vec3(0.0f, 0.0f, 0.0f), // Target position (looking at the origin)
            glm::vec3(0.0f, 1.0f, 0.0f)  // Up vector
        );

        // Compute the Projection matrix
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // Field of view in degrees
            ratio,               // Aspect ratio (width / height)
            0.1f,                // Near clipping plane
            100.0f               // Far clipping plane
        );

        // Compute the MVP matrix
        glm::mat4 MVP = projection * view * model;

        glUseProgram(program);
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (const GLfloat *)&MVP[0][0]);
        glBindVertexArray(vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);

    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}