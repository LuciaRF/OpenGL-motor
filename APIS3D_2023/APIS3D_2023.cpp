// APIS3D_2023.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "common.h"
#include <math.h>
#include <map>
using namespace std;

typedef struct vertex_t {

    glm::vec4 pos;
    glm::vec4 color;

}vertex_t;

class EventState
{
public:
    map<int, bool> keyState;
    int posX, posY;
    int oldPosX, oldPosY;

    EventState()
    {
        for (int i = 0; i < GLFW_KEY_LAST; i++)
        {
            keyState[i] = false;
        }
        posX = posY = oldPosX = oldPosY = 0;
    }
};


EventState events;

class polygon {
public:
    vector<vertex_t> vertices;
    glm::vec4 color;
    glm::vec4 position;
    glm::vec4 rotation;
    glm::vec4 scaling;

    glm::mat4 modelMtx;
    float velocity;


    polygon()
    {
        vertices.resize(3);
        //vector<glm::vec4>::iterator it = vertices.begin();
        vertices[0].pos = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f); //izquierda
        vertices[1].pos = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f); //arriba
        vertices[2].pos = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f); //derecha

        vertices[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); //izquierda
        vertices[1].color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); //arriba
        vertices[2].color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); //derecha

        velocity = 0.05f;
        position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        rotation = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        scaling = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        modelMtx = glm::mat4(1.0f);
        //Cuando use normales sentido antihorario
    }

    void update(float deltaTime) //step en el ejercicio
    {
        if(events.keyState[GLFW_KEY_D])
        {
           position.x += deltaTime * velocity;
        }

        if (events.keyState[GLFW_KEY_A])
        {
            position.x -= deltaTime * velocity;
        }

        if (position.x > 1.5f)
        {
            position.x = -1.5f;
        }

        if (position.x < -1.5f)
        {
            position.x = 1.5f;
        }
    }

    void updateModelMatrix()
    {
        /*
        1 traslacion
        2 rotacion
        3 escalado
        */

        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position));
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); //modelo que rota, angulo por el que rota, rotacion
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scaling));

        this->modelMtx = model;
    }

};

class timeManager {
public:
    float newTime = 0;
    float deltaTime = 0;
    float lastTime = 0;

    void update()
    {
        newTime = static_cast<float>(glfwGetTime());
        deltaTime = newTime - lastTime;
        lastTime = newTime;
    }

};

void keyManager(GLFWwindow* window, int key, int scancode, int action, int
    mods) //me avisa de la tecla que se ha pulsado
{
    switch (action)
    {
    case GLFW_PRESS:
        cout << "Tecla apretada" << endl;
        events.keyState[key] = true;
        break;
    case GLFW_RELEASE:
        cout << "Tecla levantada" << endl;
        events.keyState[key] = false;
        break;
    default:
        break;
    }
}

void mouseManager(GLFWwindow* window, double xpos, double ypos)
{
    events.oldPosX = events.posX;
    events.oldPosY = events.posX;

    events.posX = xpos;
    events.posY = ypos;
    cout << "Posicion " << xpos << " " << ypos << "\n";
}

void mouseKeyManager(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:
        cout << "Boton raton apretado" << endl;
        events.keyState[button] = true;
        break;
    case GLFW_RELEASE:
        cout << "Boton raton levantado" << endl;
        events.keyState[button] = false;
        break;
    default:
        break;
    }
}

void drawPolygon(polygon &p)
{
    p.updateModelMatrix();
    glBegin(GL_TRIANGLES);
    //seteos de vertices y colores
    
    glColor3f(p.color.r, p.color.g, p.color.b);
    for (auto vert : p.vertices)
    {
        vert.pos = p.modelMtx * vert.pos;
        glColor3f(vert.color.r,vert.color.g,vert.color.b); //para setear el color del objeto por vértices se hace aquí, para setear el color en el objeto enetero (todos los vertices a un color, blanco por ejemplo, arriba)
            //EN OPENGL1.0, en OPENGL4.0 en los shaders

        glVertex3f(vert.pos.x, vert.pos.y, vert.pos.z);
    }
    
    glEnd();

}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        cout << "ERROR GLWFINIT" << endl;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Triangulo", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyManager); //control Key
    glfwSetCursorPosCallback(window, mouseManager); //control mov mouse
    glfwSetMouseButtonCallback(window, mouseKeyManager); //control pulsar boton
    gladLoadGL(glfwGetProcAddress);

    timeManager t;
    polygon p;

    while (!glfwWindowShouldClose(window))
    {
        //limpiar
        glClear(GL_COLOR_BUFFER_BIT); //limpia el color para que no deje rastro (limpiar el buffer)

        //actualizar
        t.update();
        p.update(t.deltaTime);
        //dibujar
        drawPolygon(p);
        glfwSwapBuffers(window); //Una vez dibujado intercambiar buffer, back y front buffers

        //gestionar eventos
        glfwPollEvents(); //buscar elementos que haya habido, tipo cierre de ventanas etc o no va cargar las implementaciones que se hagan
    }
    std::cout << "MAIN EXIT!\n";
    glfwTerminate();
    return 0;
}
