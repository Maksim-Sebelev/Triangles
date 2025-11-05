module;

//----------------------------------------------------------------------------------------------------------------------------

#include <GLFW/glfw3.h>

#include "global.hpp"

//----------------------------------------------------------------------------------------------------------------------------

export module window;

//----------------------------------------------------------------------------------------------------------------------------

export namespace OctreeViz
{

//----------------------------------------------------------------------------------------------------------------------------

/*
this is class is like std::unique_ptr
just wrapper on GLFWwindow*
*/
class Window
{
    private:
        GLFWwindow* window_;

    public:
        Window                  (int width, int height, const char* title);
        Window                  (GLFWwindow* window);
       ~Window                  ();

        /* rule of 5 */
        Window           (const Window& ) = delete;
        Window           (const Window&&) = delete;
        Window& operator=(const Window& ) = delete;
        Window& operator=(const Window&&) = delete;

        GLFWwindow* get         () const;

        
        bool        should_close();
        void        swap_buffers();

        bool        fail        () const;
};

//----------------------------------------------------------------------------------------------------------------------------

Window::Window(int width, int height, const char* title) :
window_(glfwCreateWindow(width, height, title, nullptr, nullptr))
{}

//----------------------------------------------------------------------------------------------------------------------------

Window::Window(GLFWwindow* window) :
window_(window) // all ok, this is exactly what I want
{}

//----------------------------------------------------------------------------------------------------------------------------

Window::~Window()
{
    if (!window_) return;

    glfwDestroyWindow(window_);
    msg_assert(window_ == nullptr, "");
}

//----------------------------------------------------------------------------------------------------------------------------

GLFWwindow*
Window::get() const
{
    return window_;
}

//----------------------------------------------------------------------------------------------------------------------------

bool
Window::should_close()
{
    return glfwWindowShouldClose(window_);
}

//----------------------------------------------------------------------------------------------------------------------------

void
Window::swap_buffers()
{
    glfwSwapBuffers(window_);
}

//----------------------------------------------------------------------------------------------------------------------------

bool
Window::fail() const
{
    return !window_;
}

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace OctreeViz */

//----------------------------------------------------------------------------------------------------------------------------
