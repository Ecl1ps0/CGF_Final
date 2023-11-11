//#include <iostream>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <GL/freeglut.h>
//#include <vector>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <math.h>
//
//#define M_PI 3.14
//
//const int WIDTH = 800;
//const int HEIGHT = 600;
//
//GLuint shaderProgram;
//GLuint textureID;
//GLFWwindow* window;;
//
//// Function to initialize GLFW and GLEW
//bool Initialize() {
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return false;
//    }
//
//    // Configure GLFW
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    window = glfwCreateWindow(WIDTH, HEIGHT, "3D Fire", nullptr, nullptr);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return false;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    glewExperimental = GL_TRUE;
//    if (glewInit() != GLEW_OK) {
//        std::cerr << "Failed to initialize GLEW" << std::endl;
//        glfwTerminate();
//        return false;
//    }
//
//    return true;
//}
//
//// Function to create and update the 3D fire texture
//void UpdateFireTexture() {
//    // Define the dimensions of the texture
//    const int textureWidth = 512;
//    const int textureHeight = 512;
//    std::vector<float> noiseData(textureWidth * textureHeight);
//
//    float time = glfwGetTime();
//    for (int y = 0; y < textureHeight; ++y) {
//        for (int x = 0; x < textureWidth; ++x) {
//            // Calculate Perlin noise value based on position and time
//            float s = static_cast<float>(x) / textureWidth;
//            float t = static_cast<float>(y) / textureHeight;
//            float noiseValue = 0.5f + 0.5f * sin(2.0f * M_PI * t + 2.0f * M_PI * s + time);
//            noiseData[y * textureWidth + x] = noiseValue;
//        }
//    }
//
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_FLOAT, noiseData.data());
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//// Shader code for rendering the fire texture
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout(location = 0) in vec3 position;
//    layout(location = 1) in vec2 texCoord;
//    out vec2 TexCoords;
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//
//    void main() {
//        gl_Position = projection * view * model * vec4(position, 1.0);
//        TexCoords = texCoord;
//    }
//)";
//
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    in vec2 TexCoords;
//    out vec4 FragColor;
//    uniform sampler2D fireTexture;
//
//    void main() {
//        FragColor = texture(fireTexture, TexCoords);
//    }
//)";
//
//// Main rendering loop (render a cube)
//void RenderLoop() {
//    // Cube vertex data with positions and texture coordinates
//    float vertices[] = {
//        // Positions           // Texture coordinates
//        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//
//        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
//    };
//
//    unsigned int indices[] = {
//        0, 1, 2, 2, 3, 0,
//        4, 5, 6, 6, 7, 4,
//        0, 1, 5, 5, 4, 0,
//        2, 3, 7, 7, 6, 2,
//        0, 3, 7, 7, 4, 0,
//        1, 2, 6, 6, 5, 1
//    };
//
//    // OpenGL setup code for VAO, VBO, EBO, and loading the texture
//    GLuint VAO, VBO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Set up vertex attributes
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    // Load the texture
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // Create and compile shaders
//    GLuint vertexShader, fragmentShader;
//    vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Check for vertex shader compilation errors...
//
//    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Check for fragment shader compilation errors...
//
//    // Create shader program
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for shader program linking errors...
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Main rendering loop (render the cube)
//    while (!glfwWindowShouldClose(window)) {
//        UpdateFireTexture();
//
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Use the shader program
//        glUseProgram(shaderProgram);
//
//        // Set model, view, and projection matrices as needed for your camera
//        glm::mat4 model = glm::mat4(1.0f);
//        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
//
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
//        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
//
//        // Bind the texture
//        glBindTexture(GL_TEXTURE_2D, textureID);
//
//        // Bind the VAO and draw the cube
//        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//        // Swap buffers and poll events
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // Cleanup and release resources (VAO, VBO, EBO, texture, shader program, etc.)
//
//    glfwTerminate();
//}
//
//int notmain() {
//    if (!Initialize()) {
//        return -1;
//    }
//
//    RenderLoop();
//
//    return 0;
//}
