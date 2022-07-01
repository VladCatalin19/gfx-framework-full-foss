#include <ctime>
#include <iostream>

#include "components/camera.h"
#include "components/transform.h"
#include "core/engine.h"
#include "components/simple_scene.h"
#include "core/gpu/mesh.h"

#include "components/transform_wrapper.h"

#if defined(WITH_LAB_M1)
#   include "lab_m1/lab_list.h"
#endif

#if defined(WITH_LAB_M2)
#   include "lab_m2/lab_list.h"
#endif

#if defined(WITH_LAB_EXTRA)
#   include "lab_extra/lab_list.h"
#endif


#ifdef _WIN32
    PREFER_DISCRETE_GPU_NVIDIA;
    PREFER_DISCRETE_GPU_AMD;
#endif


std::string GetParentDir(const std::string &filePath)
{
    size_t pos = filePath.find_last_of("\\/");
    return (std::string::npos == pos) ? "." : filePath.substr(0, pos);
}


#define OFFSETOF(type, field)    ((unsigned long) &(((type *) 0)->field))
#define PRINT_FIELD(type, field) (std::cout << #type << "::" << #field \
                                            << " offset: " << OFFSETOF(type, field) \
                                            << " size: " << sizeof(field) << std::endl)


int main(int argc, char **argv)
{
#if false // print ProjectionAnfo fields offsets and sizes
    std::cout << "ProjectionInfo size: " << sizeof(gfxc::ProjectionInfo) << std::endl;
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::FoVy);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::zNear);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::zFar);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::aspectRatio);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::width);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::height);
    PRINT_FIELD(gfxc::ProjectionInfo, gfxc::ProjectionInfo::isPerspective);
#endif

#if false // print Camera fields offsets and sizes
    std::cout << "Camera size: "<< sizeof(gfxc::Camera) << std::endl;
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_transform);

    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_type);

    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_view);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_projection);

    // Control settings
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_minSpeed);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_maxSpeed);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_sensitivityOX);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_sensitivityOY); // 0xB4

    // Rotation OX constrains
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_limitUp);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_limitDown);

    // Perspective properties
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_zNear);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_zFar);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_FoVy);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_aspectRatio);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_isPerspective);
    PRINT_FIELD(gfxc::Camera, gfxc::Camera::m_ortographicWidth);
#endif

#if false // print Transform fields offsets and sizes 
    std::cout << "Transform size: "<< sizeof(gfxc::Transform) << std::endl;
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_worldModel);

    // Rotations
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_worldRotation);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_relativeRotation);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_invWorldRotation);

    // Positions
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_worldPosition);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_localPosition);

    // Scale relative to the object, not influenced by rotations
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_localScalingFactor);

    // Speeds
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_rotationSpeed);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_translationSpeed);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_scalingSpeed);

    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_isInMotion);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_isModelOutdated);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_updateHierarchy);

    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_parentNode);
    PRINT_FIELD(gfxc::Transform, gfxc::Transform::m_childNodes);
#endif

#if false // miscellaneous
    std::cout << "list: " << sizeof(std::list<void*>) << std::endl;

    gfxc::Transform t;
    std::cout << t.m_isInMotion << t.m_isModelOutdated << t.m_updateHierarchy << std::endl;
    std::cout << t.m_worldRotation << std::endl;

    gfxc::Transform t2;
    t.AddChild(&t2);

    gfxc::Transform t3;
    t.AddChild(&t3);

    std::cout << "t2: " << &t2 << " t3: " << &t3 << std::endl;

    for (gfxc::Transform *child : t.m_childNodes)
    {
        std::cout << child << ' ';
    }
    std::cout << std::endl;
#endif

#if true // original code
    srand((unsigned int)time(NULL));

    // Create a window property structure
    WindowProperties wp;
    wp.resolution = glm::ivec2(1280, 720);
    wp.vSync = true;
    wp.selfDir = GetParentDir(std::string(argv[0]));

    // Init the Engine and create a new window with the defined properties
    (void)Engine::Init(wp);

    // Create a new 3D world and start running it
    gfxc::SimpleScene world;

    world.Init();
    world.Run();

    // Signals to the Engine to release the OpenGL context
    Engine::Exit();

#endif
    return 0;
}
