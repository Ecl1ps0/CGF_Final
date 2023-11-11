//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <GL/freeglut.h>
//#include <iostream>
//#include <vector>
//#include <ctime>
//
//const char* vertexShaderCode = R"(
//    #version 330 core
//    in vec3 position;
//    out vec2 uv;
//
//    void main() {
//        gl_Position = vec4(position, 0.4);
//        uv = (position.xy + vec2(1.0, 1.0)) / 2.0;
//    }
//)";
//
//const char* fragmentShaderCode = R"(
//    #version 330 core
//    uniform vec2 resolution;
//    uniform float time;
//    #define timeScale           time * 1.0
//    #define fireMovement        vec2(-0.01, -0.5)
//    #define distortionMovement  vec2(-0.01, -0.3)
//    #define normalStrength      40.0
//    #define distortionStrength  0.1
//
//    vec2 hash(vec2 p) {
//        p = vec2(dot(p, vec2(127.1, 311.7)),
//                 dot(p, vec2(269.5, 183.3)));
//
//        return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
//    }
//
//    float noise(in vec2 p) {
//        const float K1 = 0.366025404; // (sqrt(3)-1)/2;
//        const float K2 = 0.211324865; // (3-sqrt(3))/6;
//
//        vec2 i = floor(p + (p.x + p.y) * K1);
//
//        vec2 a = p - i + (i.x + i.y) * K2;
//        vec2 o = step(a.yx, a.xy);
//        vec2 b = a - o + K2;
//        vec2 c = a - 1.0 + 2.0 * K2;
//
//        vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
//
//        vec3 n = h * h * h * h * vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));
//
//        return dot(n, vec3(70.0));
//    }
//
//    float fbm(in vec2 p) {
//        float f = 0.0;
//        mat2 m = mat2(1.6, 1.2, -1.2, 1.6);
//        f = 0.5000 * noise(p);
//        p = m * p;
//        f += 0.2500 * noise(p);
//        p = m * p;
//        f += 0.1250 * noise(p);
//        p = m * p;
//        f += 0.0625 * noise(p);
//        p = m * p;
//        f = 0.5 + 0.5 * f;
//        return f;
//    }
//
//    vec3 bumpMap(vec2 uv) {
//        vec2 s = 1.0 / resolution.xy;
//        float p = fbm(uv);
//        float h1 = fbm(uv + s * vec2(1., 0));
//        float v1 = fbm(uv + s * vec2(0, 1.));
//
//        vec2 xy = (p - vec2(h1, v1)) * normalStrength;
//        return vec3(xy + .5, 1.);
//    }
//
//    void main() {
//        vec2 uv = gl_FragCoord.xy / resolution.xy;
//        uv.y *= 0.7; // Lower the fire by scaling the y-coordinate
//
//        vec3 normal = bumpMap(uv * vec2(1.0, 0.3) + distortionMovement * timeScale);
//        vec2 displacement = clamp((normal.xy - .5) * distortionStrength, -1., 1.);
//        uv += displacement;
//
//        vec2 uvT = (uv * vec2(1.0, 0.5)) + fireMovement * timeScale;
//        float n = pow(fbm(8.0 * uvT), 1.0);
//
//        float gradient = pow(1.0 - uv.y, 2.0) * 5.;
//        float finalNoise = n * gradient;
//
//        vec3 color = finalNoise * vec3(2. * n, 2. * n * n * n, n * n * n * n);
//        color = vec3(1.0, 0.5, 0.0) + color * 1.5;
//
//        // Make the flame persist by ensuring it doesn't fade to black
//        gl_FragColor = vec4(max(color, vec3(0.0)), 1.0);
//    }
//)";
//
//// Particle structure
//struct Particle {
//    float x, y, z;  // Position
//    float vx, vy, vz;  // Velocity
//    float life;  // Particle life
//    float size;  // Particle size
//};
//
//// Constants
//const int numParticles = 500;
//const float gravity = -0.0005;
//const float particleLife = 100.0;
//
//// Particle system
//std::vector<Particle> particles;
//
//// Shader program ID
//GLuint shaderProgram;
//
//// Uniform locations
//GLuint resolutionLoc, timeLoc;
//
//// GLFW window
//GLFWwindow* window;
//
//// Function to initialize the shader program
//void initShaderProgram() {
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
//    glCompileShader(fragmentShader);
//
//    shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check shader and program compilation/linking status
//    GLint success;
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        GLchar infoLog[512];
//        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
//        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
//    }
//
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        GLchar infoLog[512];
//        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
//        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
//    }
//
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        GLchar infoLog[512];
//        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//}
//
//// Function to initialize particles
//void initParticles() {
//    particles.resize(numParticles);
//    for (int i = 0; i < numParticles; ++i) {
//        particles[i].x = 0.0;
//        particles[i].y = 0.0;
//        particles[i].z = 0.0;
//        particles[i].vx = static_cast<float>(rand()) / RAND_MAX - 0.5;
//        particles[i].vy = static_cast<float>(rand()) / RAND_MAX * 0.5 + 0.5;
//        particles[i].vz = static_cast<float>(rand()) / RAND_MAX - 0.5;
//        particles[i].life = particleLife;
//        particles[i].size = static_cast<float>(rand()) / RAND_MAX * 0.05 + 0.02;
//    }
//}
//
//// Initialize OpenGL
//void init() {
//    glewExperimental = GL_TRUE;
//    GLenum err = glewInit();
//    if (err != GLEW_OK) {
//        std::cerr << "Error: GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
//        exit(EXIT_FAILURE);
//    }
//
//    glClearColor(0.0, 0.0, 0.0, 1.0);
//    glShadeModel(GL_FLAT);
//
//    initShaderProgram();
//    initParticles();
//
//    // Get uniform locations
//    resolutionLoc = glGetUniformLocation(shaderProgram, "resolution");
//    timeLoc = glGetUniformLocation(shaderProgram, "time");
//}
//
//// Update particle positions and velocities
//void updateParticles(float deltaTime) {
//    for (int i = 0; i < numParticles; ++i) {
//        particles[i].x += particles[i].vx;
//        particles[i].y += particles[i].vy;
//        particles[i].z += particles[i].vz;
//
//        particles[i].vy += gravity;
//
//        particles[i].life -= 0.5;
//
//        if (particles[i].life < 0.0) {
//            particles[i].x = static_cast<float>(rand()) / RAND_MAX - 0.5;
//            particles[i].y = -1.0;
//            particles[i].z = static_cast<float>(rand()) / RAND_MAX - 0.5;
//            particles[i].vx = static_cast<float>(rand()) / RAND_MAX - 0.5;
//            particles[i].vy = static_cast<float>(rand()) / RAND_MAX * 0.5 + 0.5;
//            particles[i].vz = static_cast<float>(rand()) / RAND_MAX - 0.5;
//            particles[i].life = particleLife;
//        }
//    }
//}
//
//// Display function
//void display() {
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glUseProgram(shaderProgram);
//
//    glUniform1f(timeLoc, glfwGetTime());
//
//    updateParticles(0.016);
//
//    glUniform2f(resolutionLoc, 800.0, 600.0);
//
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//    for (int i = 0; i < numParticles; ++i) {
//        float x = particles[i].x;
//        float y = particles[i].y;
//        float z = particles[i].z;
//        float size = particles[i].size;
//
//        // Render a plane instead of a sphere
//        glBegin(GL_QUADS);
//        glVertex3f(x - size, y, z - size);
//        glVertex3f(x + size, y, z - size);
//        glVertex3f(x + size, y, z + size);
//        glVertex3f(x - size, y, z + size);
//        glEnd();
//    }
//
//    glDisable(GL_BLEND);
//
//    glfwSwapBuffers(window);
//}
//
//// Render loop
//void renderLoop() {
//    while (!glfwWindowShouldClose(window)) {
//        display();
//        glfwPollEvents();
//    }
//}
//
//int main() {
//    srand(static_cast<unsigned>(time(nullptr)));
//
//    if (!glfwInit()) {
//        std::cerr << "Error: GLFW initialization failed" << std::endl;
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//
//    window = glfwCreateWindow(800, 600, "3D Perlin Noise Flame Simulation", nullptr, nullptr);
//    if (!window) {
//        glfwTerminate();
//        std::cerr << "Error: Failed to create GLFW window" << std::endl;
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//
//    init();
//
//    renderLoop();
//
//    glfwTerminate();
//    return 0;
//}
