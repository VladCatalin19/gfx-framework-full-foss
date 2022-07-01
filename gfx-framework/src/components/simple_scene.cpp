#include "components/simple_scene.h"

#include <cmath>
#include <vector>
#include <iostream>

#include "components/camera_input.h"
#include "components/scene_input.h"
#include "components/transform.h"
#include "utils/glm_utils.h"

#include "transform.h"
#include "transform_wrapper.h"

using namespace gfxc;


SimpleScene::SimpleScene()
{
    InitResources();
}


SimpleScene::~SimpleScene()
{
}


void SimpleScene::glLineWidth(GLfloat width)
{
#ifdef __APPLE__
    ::glLineWidth(1);
#else
    ::glLineWidth(width);
#endif
}


void SimpleScene::InitResources()
{
    // Sets common GL states
    glClearColor(0, 0, 0, 1);

    drawGroundPlane = true;

    objectModel = new Transform();

    camera = new Camera();
    camera->SetPerspective(60, window->props.aspectRatio, 0.01f, 200);
    camera->m_transform->SetMoveSpeed(2);
    camera->m_transform->SetWorldPosition(glm::vec3(0, 1.6f, 2.5));
    camera->m_transform->SetWorldRotation(glm::vec3(-15, 0, 0));
    camera->Update();

    cameraInput = new CameraInput(camera);
    window = Engine::GetWindow();

    SceneInput *SI = new SceneInput(this);
    (void)SI;

    xozPlane = new Mesh("plane");
    xozPlane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");

    {
        std::vector<VertexFormat> vertices =
        {
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 0)),
        };
        std::vector<unsigned int> indices = { 0, 1 };

        simpleLine = new Mesh("line");
        simpleLine->InitFromData(vertices, indices);
        simpleLine->SetDrawMode(GL_LINES);
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("Simple");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Default.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for drawing vertex colors
    {
        Shader *shader = new Shader("Color");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Color.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("VertexNormal");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "Normals.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a shader program for drawing vertex colors
    {
        Shader *shader = new Shader("VertexColor");
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "MVP.Texture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::SHADERS, "VertexColor.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Default rendering mode will use depth buffer
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);



    testTransform = new Transform/*Wrapper*/();
    //std::cout << "------------------------1--------------------------\n";
    Transform *tParent = new Transform/*Wrapper*/();
    //std::cout << "------------------------2--------------------------\n";
    Transform *tChild = new Transform/*Wrapper*/();
    //std::cout << "------------------------3--------------------------\n";

    testTransform->AddChild(tParent);
    //std::cout << "------------------------4--------------------------\n";
    tParent->AddChild(tChild);
    //std::cout << "------------------------5--------------------------\n";

    testTransform->SetWorldPosition(glm::vec3(0.0F));
    //std::cout << "------------------------6--------------------------\n";
    tParent->SetWorldPosition(glm::vec3(1.0F, 0.0F, 1.0F));
    //std::cout << "------------------------7--------------------------\n";
    tParent->SetWorldRotation(glm::vec3(0.0F, 45.0F, 0.0F));
    //std::cout << "------------------------8--------------------------\n";

    tChild->SetLocalPosition(glm::vec3_up);
    //std::cout << "------------------------9--------------------------\n";
    tChild->SetReleativeRotation(glm::vec3(0.0F, 45.0F, 0.0F));
    //std::cout << "-----------------------10--------------------------\n";
    tChild->SetScale(0.25F * glm::vec3(1.0F));
    //std::cout << "-----------------------11--------------------------\n";


    Mesh *box = new Mesh("Box");
    box->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    AddMeshToList(box);

    // std::cout << "Parent model:" << std::endl
    //           << "    " << tParent->GetModel()[0] << std::endl
    //           << "    " << tParent->GetModel()[1] << std::endl
    //           << "    " << tParent->GetModel()[2] << std::endl
    //           << "    " << tParent->GetModel()[3] << std::endl;

    // std::cout << "Child model:" << std::endl
    //           << "    " << tChild->GetModel()[0] << std::endl
    //           << "    " << tChild->GetModel()[1] << std::endl
    //           << "    " << tChild->GetModel()[2] << std::endl
    //           << "    " << tChild->GetModel()[3] << std::endl;
}


void SimpleScene::AddMeshToList(Mesh * mesh)
{
    if (mesh->GetMeshID())
    {
        meshes[mesh->GetMeshID()] = mesh;
    }
}


void SimpleScene::DrawCoordinateSystem()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), camera->GetProjectionMatrix());
}


void SimpleScene::DrawCoordinateSystem(const glm::mat4 & viewMatrix, const glm::mat4 & projectionMaxtix)
{
    glLineWidth(1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render the coordinate system
    {
        Shader *shader = shaders["Color"];
        shader->Use();
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMaxtix));

        if (drawGroundPlane)
        {
            objectModel->SetScale(glm::vec3(1));
            objectModel->SetWorldPosition(glm::vec3(0));
            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
            glUniform3f(shader->GetUniformLocation("color"), 0.5f, 0.5f, 0.5f);
            xozPlane->Render();
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glLineWidth(3);
        objectModel->SetScale(glm::vec3(1, 25, 1));
        objectModel->SetWorldRotation(glm::quat());
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
        glUniform3f(shader->GetUniformLocation("color"), 0, 1, 0);
        simpleLine->Render();

        objectModel->SetWorldRotation(glm::vec3(0, 0, -90));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
        glUniform3f(shader->GetUniformLocation("color"), 1, 0, 0);
        simpleLine->Render();

        objectModel->SetWorldRotation(glm::vec3(90, 0, 0));
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(objectModel->GetModel()));
        glUniform3f(shader->GetUniformLocation("color"), 0, 0, 1);
        simpleLine->Render();

        objectModel->SetWorldRotation(glm::quat());

        glLineWidth(1);
    }
}


void SimpleScene::RenderMesh(Mesh * mesh, Shader * shader, glm::vec3 position, glm::vec3 scale)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

    glm::mat4 model(1);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
}


void SimpleScene::RenderMesh(Mesh * mesh, glm::vec3 position, glm::vec3 scale)
{
    RenderMesh(mesh, shaders["Simple"], position, scale);
}


void SimpleScene::RenderMesh2D(Mesh * mesh, Shader * shader, const glm::mat3 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

    glm::mat3 mm = modelMatrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    mesh->Render();
}


void SimpleScene::RenderMesh2D(Mesh * mesh, const glm::mat3 & modelMatrix, const glm::vec3 & color) const
{
    Shader* shader = shaders.at("Color");

    if (!mesh || !shader || !shader->program)
        return;

    glm::mat3 mm = modelMatrix;
    glm::mat4 model = glm::mat4(
        mm[0][0], mm[0][1], mm[0][2], 0.f,
        mm[1][0], mm[1][1], mm[1][2], 0.f,
        0.f, 0.f, mm[2][2], 0.f,
        mm[2][0], mm[2][1], 0.f, 1.f);

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(shader->GetUniformLocation("color"), color.r, color.g, color.b);

    mesh->Render();
}


void SimpleScene::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void SimpleScene::ReloadShaders() const
{
    std::cout << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Reloading Shaders" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << std::endl;

    for (auto &shader : shaders)
    {
        shader.second->Reload();
    }
}


Camera * SimpleScene::GetSceneCamera() const
{
    return camera;
}


InputController * SimpleScene::GetCameraInput() const
{
    return cameraInput;
}


void SimpleScene::ClearScreen(const glm::vec3 &color)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer
    glClearColor(color.x, color.y, color.z, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


bool SimpleScene::ToggleGroundPlane()
{
    drawGroundPlane = !drawGroundPlane;
    return drawGroundPlane;
}

static unsigned frames = 0U;

void SimpleScene::Update(float deltaTimeSeconds)
{
    ClearScreen();
    DrawCoordinateSystem();

    //camera->m_transform->Move(glm::vec3_down, -deltaTimeSeconds);
    
    //camera->UpdateSpeed(10.0F);
    //camera->m_transform->SetMoveSpeed(10.0F);
    //std::cout << camera->m_transform->GetMoveSpeed() << std::endl;
    
    //std::cout << camera->m_transform->GetLocalOZVector() << std::endl;
    // my camera local oz : [-0.0016882 0.253758 0.967266]

    Transform *parent = *testTransform->m_childNodes.begin();
    Transform *child = *parent->m_childNodes.begin();

    static float totalTime = 0.0F;
    static const glm::vec3 initialParentLocal = parent->GetLocalPosition();
    static const glm::vec3 initialChildLocal = child->GetLocalPosition();

    totalTime += deltaTimeSeconds;

    parent->SetLocalPosition(initialParentLocal + 0.5F * std::sin(totalTime * 5.0F) * glm::vec3_right);
    parent->RotateLocalOX(100.0F * deltaTimeSeconds);
    parent->RotateLocalOY(-50.0F * deltaTimeSeconds);

    child->SetLocalPosition(initialChildLocal + 0.25F * std::sin(totalTime * 2.0F) * glm::vec3_up);
    child->RotateLocalOZ(50.0F * deltaTimeSeconds);

    RenderMesh(meshes.at("Box"), shaders.at("VertexNormal"), parent->GetModel());
    RenderMesh(meshes.at("Box"), shaders.at("VertexNormal"), child->GetModel());

    if (frames == 0U)
    {
        
    }

    ++frames;
}
