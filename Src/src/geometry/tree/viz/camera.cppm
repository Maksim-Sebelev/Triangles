module;

//----------------------------------------------------------------------------------------------------------------------------

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "global.hpp"

//----------------------------------------------------------------------------------------------------------------------------

export module camera;

//----------------------------------------------------------------------------------------------------------------------------

// some default settings

constexpr float     default_mouse_sensetive =  0.1f;
constexpr float     default_zoom            =  45.0f;
constexpr float     default_speed           =  2.5f;
constexpr float     default_pitch           =  0.0f;
constexpr float     default_yaw             = -90.0f;
constexpr glm::vec3 default_world_up        = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr float     min_zoom                = 0.1f;
constexpr float     max_zoom                = 100.0f;

constexpr float    min_pitch                = -89.0f;
constexpr float    max_pitch                =  89.0f;

//----------------------------------------------------------------------------------------------------------------------------

export namespace OctreeViz
{

//----------------------------------------------------------------------------------------------------------------------------

enum class CameraMovement_t
{
    FORWARD,
    RIGHT,
    LEFT,
    BACK,
    UP,
    DOWN,
};

//----------------------------------------------------------------------------------------------------------------------------

class Camera
{
    private:
        glm::vec3 position_;
        glm::vec3 front_;
        glm::vec3 up_;
        glm::vec3 right_;
        glm::vec3 world_up_;

        float speed_;
        float zoom_;
        float yaw_;
        float mouse_sensitivity;
        float pitch_;

        void update_camera();

    public:
        Camera(glm::vec3 start_position = glm::vec3(0.0f, 0.0f, 5.0f));

        void      parse_movement            (CameraMovement_t movement_type, float delta_time);
        void      parse_mouse_movement (float xoffset, float yoffset);
        void      parse_mouse_scroll   (float yoffset);
        
        glm::mat4 get_view_matrix      () const;
    
        float     get_x                () const;
        float     get_y                () const;
        float     get_z                () const;
        float     get_zoom             () const;
};

// public
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

Camera::Camera(glm::vec3 start_position) :
position_         (start_position         ),
world_up_         (default_world_up       ),
speed_            (default_speed          ),
zoom_             (default_zoom           ),
yaw_              (default_yaw            ),
mouse_sensitivity (default_mouse_sensetive),
pitch_            (default_pitch          )
{
    update_camera();
}

//----------------------------------------------------------------------------------------------------------------------------

void
Camera::parse_movement(CameraMovement_t movement_type, float delta_time)
{
    const float velocity = speed_ * delta_time;

    switch (movement_type)
    {
        case CameraMovement_t::FORWARD: position_ += front_    * velocity; return;
        case CameraMovement_t::BACK   : position_ -= front_    * velocity; return;
        case CameraMovement_t::RIGHT  : position_ -= right_    * velocity; return;
        case CameraMovement_t::LEFT   : position_ += right_    * velocity; return;
        case CameraMovement_t::UP     : position_ += world_up_ * velocity; return;
        case CameraMovement_t::DOWN   : position_ -= world_up_ * velocity; return;

        default: builtin_unreachable_wrapper("Undefined camera movement type. maybe you forgot to add new value in switch-case");
    }

    builtin_unreachable_wrapper("we must return in switch-case");
}

//----------------------------------------------------------------------------------------------------------------------------

void
Camera::parse_mouse_movement(float xoffset, float yoffset)
{
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;

    yaw_   += xoffset;
    pitch_ += yoffset;

    if      (pitch_ < min_pitch) pitch_ = min_pitch;
    else if (pitch_ > max_pitch) pitch_ = max_pitch;

    update_camera();
}

//----------------------------------------------------------------------------------------------------------------------------

void
Camera::parse_mouse_scroll(float yoffset)
{
    zoom_ -= yoffset;
    if      (zoom_ < min_zoom) zoom_ = min_zoom;
    else if (zoom_ > max_zoom) zoom_ = max_zoom;
}

//----------------------------------------------------------------------------------------------------------------------------

glm::mat4
Camera::get_view_matrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

//----------------------------------------------------------------------------------------------------------------------------

float
Camera::get_x() const
{
    return position_.x;
}

//----------------------------------------------------------------------------------------------------------------------------

float
Camera::get_y() const
{
    return position_.y;
}

//----------------------------------------------------------------------------------------------------------------------------

float
Camera::get_z() const
{
    return position_.z;
}

//----------------------------------------------------------------------------------------------------------------------------

float
Camera::get_zoom() const
{
    return zoom_;
}

//----------------------------------------------------------------------------------------------------------------------------

// private
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

void
Camera::update_camera()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw_  )) * cos(glm::radians(pitch_));
    new_front.y = sin(glm::radians(pitch_)                            );
    new_front.z = sin(glm::radians(yaw_  )) * cos(glm::radians(pitch_));

    front_ = glm::normalize(new_front);
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_   ));
}

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace OctreeViz */

//----------------------------------------------------------------------------------------------------------------------------
