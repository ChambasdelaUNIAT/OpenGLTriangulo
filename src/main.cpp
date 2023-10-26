#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <iostream>

void processsInput(GLFWwindow *window); 

// Lenguajde GLSL la implementacion del shader de vertices
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Lenguajde GLSL la implementacion del shader de fragmentacion
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0";


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(800, 600, "this is NOT a window!", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Construccion y compilacion de las rutinas o shaders*/

    // Shader de vértices
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);
    // checar que la compilacion haya sido exitosa
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR, TU COMPILACION DEL SHADER DE VERICES FALLO" << infoLog << std::endl;
    }

    // Shader de fragmentación
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);
    // checar que la compilacion haya sido exitosa
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR, TU COMPILACION DEL SHADER DE VERICES FALLO" << infoLog << std::endl;
    }

    //vinculación de los shaders al programa de shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // checar que la vinculación haya sido exitosa
    glGetShaderiv(shaderProgram,GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR, TU COMPILACION DEL SHADER DE VERICES FALLO" << infoLog << std::endl;
    }
    // para posteriormente invocarlo en el main --> glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    /* IMPLEMENTACION DEL TRIANGULO */

    float vertices[]= {
        -0.5f,-0.5f,0.0f, //Vertice inferior izq
         0.5f,-0.5f,0.0f, //Vertice inferior der
         0.0,0.5,0        //Vertice superior
    };

    
    //Hacer un objeto de bufer de vertices que se va a usar como parte del objeto de arreglo de vertices
    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO); 
    
    //0. Se copia nuestro arreglo de vertices en un búfer para que OpenGL lo pueda utilizar
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //1. Configuramos los apuntadores de atributos del vértice
    //Asignación del arreglo genérico de vértices para que OpenGL lo pueda interpretar
    // y lo habilitamos
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0); 

    /* FIN DE LA IMPLEMENTACION */

    while (!glfwWindowShouldClose(window)) {
        
        processsInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);

 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Opcional pero siempre será importante borrar memoria que no usemos luego de nuestro proceso
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
 
    glfwTerminate();
    return 0;
}

void processsInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}