#pragma once
#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define CLOSEWIN() {glfwTerminate();return 0;}
#define HELLOTRIANGLE() {glBegin(GL_TRIANGLES);glVertex2f(-0.5f, -0.5f);\
glColor3f(255.0f, 0, 0);glVertex2f(0.5f, -0.5f);glColor3f(0, 255.0f, 0);\
glVertex2f(0.0f, 0.5f);glColor3f(0.0f, 0.0f, 255.0f);glEnd(); }