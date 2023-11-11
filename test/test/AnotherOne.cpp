//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <GL/freeglut.h>
//#include <iostream>
//#include <cmath>
//
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec2 position;
//    void main()
//    {
//        gl_Position = vec4(position, 0.0, 2.0);
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform float time;
//    float random(vec2 co){
//        return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
//    }
//    float noise(vec2 uv) {
//        vec2 i = floor(uv);
//        vec2 f = fract(uv);
//
//        float a = random(i);
//        float b = random(i + vec2(1.0, 0.0));
//        float c = random(i + vec2(0.0, 1.0));
//        float d = random(i + vec2(1.0, 1.0));
//
//        vec2 u = f * f * (3.0 - 2.0 * f);
//
//        return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
//    }
//
//    void main()
//    {
//        vec2 uv = gl_FragCoord.xy / vec2(800.0, 600.0);
//        float value = noise(uv * 10.0 + vec2(0.0, time));
//        vec3 color = vec3(value * 2.0, value * value * 4.0, 0.0);
//        FragColor = vec4(color, 1.0);
//    }
//)";
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//int main() {
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Fire Animation", NULL, NULL);
//    if (window == NULL) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cout << "Failed to initialize GLEW" << std::endl;
//        return -1;
//    }
//
//    glViewport(0, 0, 800, 600);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    GLuint vertexShader, fragmentShader, shaderProgram;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    GLuint fbo;
//    glGenFramebuffers(1, &fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//
//    GLuint texture;
//    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
//
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//        std::cout << "Framebuffer is not complete!" << std::endl;
//        return -1;
//    }
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    GLuint vertexArray, vertexBuffer;
//    glGenVertexArrays(1, &vertexArray);
//    glGenBuffers(1, &vertexBuffer);
//
//    glBindVertexArray(vertexArray);
//
//    float vertices[] = {
//        -1.0f, -1.0f,
//         1.0f, -1.0f,
//        -1.0f,  1.0f,
//         1.0f,  1.0f,
//    };
//
//    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    while (!glfwWindowShouldClose(window)) {
//        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//        glUseProgram(shaderProgram);
//        glUniform1f(glGetUniformLocation(shaderProgram, "time"), glfwGetTime());
//        glBindVertexArray(vertexArray);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//        glClear(GL_COLOR_BUFFER_BIT);
//        glUseProgram(shaderProgram);
//        glBindVertexArray(vertexArray);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteFramebuffers(1, &fbo);
//    glDeleteTextures(1, &texture);
//    glDeleteVertexArrays(1, &vertexArray);
//    glDeleteBuffers(1, &vertexBuffer);
//
//    glfwTerminate();
//    return 0;
//}
