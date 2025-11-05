module;

export module octreeviz;


export namespace Geometry
{

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <cmath>

// Структура для вершины
struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

// Класс камеры
class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    
    Camera(glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 5.0f)) 
        : position(startPosition), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), 
          yaw(-90.0f), pitch(0.0f), movementSpeed(2.5f), 
          mouseSensitivity(0.1f), zoom(45.0f) {
        updateCameraVectors();
    }
    
    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }
    
    void processKeyboard(int direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        
        switch (direction) {
            case 0: position += front * velocity; break; // W - вперед
            case 1: position -= front * velocity; break; // S - назад  
            case 2: position -= right * velocity; break; // A - влево
            case 3: position += right * velocity; break; // D - вправо
            case 4: position += worldUp * velocity; break; // Q - вверх
            case 5: position -= worldUp * velocity; break; // E - вниз
        }
    }
    
    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;
        
        yaw += xoffset;
        pitch += yoffset;
        
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        
        updateCameraVectors();
    }
    
    void processMouseScroll(float yoffset) {
        zoom -= yoffset;
        if (zoom < 1.0f) zoom = 1.0f;
        if (zoom > 90.0f) zoom = 90.0f;
    }
    
private:
    void updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

// Глобальные переменные
GLFWwindow* g_window;
Camera g_camera;
float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;
bool g_firstMouse = true;
float g_lastX = 400, g_lastY = 300;

// Исходные данные - массив треугольников
// Каждый треугольник - 9 float: {x1,y1,z1, x2,y2,z2, x3,y3,z3}
std::vector<std::array<float, 9>> g_triangles = {
    // Треугольник 1
    {-1.0f, -1.0f, 0.0f, 
      -1.0f, -3.0f, 1.0f, 
      0.0f,  1.0f, 0.0f},
    
    // Треугольник 2  
    {0.5f, 0.5f, 1.0f,
     1.5f, 0.5f, 1.0f,
     1.0f, 1.5f, 1.0f},
    
    // Треугольник 3
    {-1.5f, 0.5f, -1.0f,
     -0.5f, 0.5f, -1.0f, 
     -1.0f, 1.5f, -1.0f},
     
};

// Callback функции
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (g_firstMouse) {
        g_lastX = xpos;
        g_lastY = ypos;
        g_firstMouse = false;
    }

    float xoffset = xpos - g_lastX;
    float yoffset = g_lastY - ypos;

    g_lastX = xpos;
    g_lastY = ypos;

    g_camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera.processMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera.processKeyboard(0, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera.processKeyboard(1, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera.processKeyboard(2, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera.processKeyboard(3, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        g_camera.processKeyboard(4, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        g_camera.processKeyboard(5, g_deltaTime);
}

// Компиляция шейдера
unsigned int compileShader(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// Создание буфера для треугольников
void setupTrianglesBuffers(unsigned int& VAO, unsigned int& VBO) {
    std::vector<Vertex> vertices;

    // Преобразуем массив треугольников в вершины
    for (const auto& triangle : g_triangles) {
        // Разные цвета для каждого треугольника
        glm::vec3 color = glm::vec3(
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX, 
            static_cast<float>(rand()) / RAND_MAX
        );
        
        // Три вершины треугольника
        for (int i = 0; i < 3; ++i) {
            Vertex vertex;
            vertex.position = glm::vec3(triangle[i*3], triangle[i*3+1], triangle[i*3+2]);
            vertex.color = color;
            vertices.push_back(vertex);
        }
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                 vertices.data(), GL_STATIC_DRAW);

    // Атрибут позиции
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Атрибут цвета
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                         (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
}

// Создание буфера для координатных осей
void setupCoordinateAxes(unsigned int& VAO, unsigned int& VBO) {
    // Вершины для осей (X, Y, Z)
    std::vector<Vertex> axesVertices = {
        // Ось X (красная)
        {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{2.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        
        // Ось Y (зеленая)
        {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.0f, 2.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        
        // Ось Z (синяя)
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 1.0f}}
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, axesVertices.size() * sizeof(Vertex),
                 axesVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
}

int main() {
    // Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    g_window = glfwCreateWindow(1200, 800, "3D Triangles with ImGui", NULL, NULL);

    if (!g_window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(g_window);
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, mouse_callback);
    glfwSetScrollCallback(g_window, scroll_callback);
    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed!" << std::endl;
        return -1;
    }
    
    // Инициализация ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Настройка OpenGL
    glEnable(GL_DEPTH_TEST);
    
    // Шейдеры
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;
        
        out vec3 fragColor;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            fragColor = aColor;
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        in vec3 fragColor;
        out vec4 FragColor;
        
        void main() {
            FragColor = vec4(fragColor, 1.0);
        }
    )";
    
    unsigned int shaderProgram = compileShader(vertexShaderSource, fragmentShaderSource);
    
    // Создание буферов
    unsigned int trianglesVAO, trianglesVBO;
    unsigned int axesVAO, axesVBO;
    
    setupTrianglesBuffers(trianglesVAO, trianglesVBO);
    setupCoordinateAxes(axesVAO, axesVBO);

    int triangleCount = g_triangles.size();

    // Переменные для ImGui
    glm::vec3 sceneScale(1.0f);
    bool showAxes = true;
    
    bool wireframeMode = false;
    float bgColor[3] = {0.1f, 0.1f, 0.1f};

    // Главный цикл
    while (!glfwWindowShouldClose(g_window)) {
        float currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - g_lastFrame;
        g_lastFrame = currentFrame;
        
        processInput(g_window);
        
        // ImGui новый кадр
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Окно управления
        ImGui::Begin("Управление сценой");
        
        ImGui::Text("Масштаб сцены:");
        ImGui::SliderFloat3("##Scale", &sceneScale.x, 0.1f, 3.0f);
        
        ImGui::Checkbox("Показать оси", &showAxes);
        ImGui::Checkbox("Режим каркаса", &wireframeMode);
        
        ImGui::ColorEdit3("Цвет фона", bgColor);
        
        ImGui::Separator();
        ImGui::Text("Камера:");
        ImGui::Text("Позиция: (%.2f, %.2f, %.2f)", 
                   g_camera.position.x, g_camera.position.y, g_camera.position.z);
        ImGui::Text("Угол обзора: %.1f", g_camera.zoom);
        
        ImGui::Separator();
        ImGui::Text("Треугольники: %d", triangleCount);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        
        ImGui::End();
        
        // Рендеринг
        glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Устанавливаем режим отрисовки
        if (wireframeMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
        
        // Матрицы
        glm::mat4 model = glm::scale(glm::mat4(1.0f), sceneScale);
        glm::mat4 view = g_camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(g_camera.zoom), 1200.0f/800.0f, 0.1f, 100.0f);
        
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        // Отрисовка треугольников
        glBindVertexArray(trianglesVAO);
        glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);

        // Отрисовка координатных осей (линиями)
        if (showAxes) {
            glBindVertexArray(axesVAO);
            glDrawArrays(GL_LINES, 0, 6);
        }
        
        // Восстанавливаем режим отрисовки
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        // Рендеринг ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
    
    // Очистка
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glDeleteVertexArrays(1, &trianglesVAO);
    glDeleteBuffers(1, &trianglesVBO);
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}

} /* namspace Geometry */
