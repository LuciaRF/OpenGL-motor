// APIS3D_2023.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "common.h"
#include <math.h>
#include <map>
using namespace std;

class timeManager {
    float newTime = 0;
    float deltaTime = 0;
    float lastTime = 0;

    void update()
    {
        newTime = static_cast<float>(glfwGetTime());
        deltaTime = newTime - lastTime;
        lastTime = newTime;    }
};

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


    while (!glfwWindowShouldClose(window))
    {
        //actualizar

        //dibujar
         glfwSwapBuffers(window); //Una vez dibujado intercambiar buffer, back y front buffers

        //gestionar eventos
        glfwPollEvents(); //buscar elementos que haya habido, tipo cierre de ventanas etc o no va cargar las implementaciones que se hagan
    }
    std::cout << "MAIN EXIT!\n";
    glfwTerminate();
    return 0;
}
