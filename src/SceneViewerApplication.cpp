#include "SceneViewerApplication.h"

#include <ituGL/asset/TextureCubemapLoader.h>
#include <ituGL/asset/ShaderLoader.h>
#include <ituGL/asset/ModelLoader.h>

#include <ituGL/camera/Camera.h>
#include <ituGL/scene/SceneCamera.h>

#include <ituGL/lighting/DirectionalLight.h>
#include <ituGL/lighting/PointLight.h>
#include <ituGL/scene/SceneLight.h>

#include <ituGL/shader/ShaderUniformCollection.h>
#include <ituGL/shader/Material.h>
#include <ituGL/geometry/Model.h>
#include <ituGL/scene/SceneModel.h>

#include <ituGL/renderer/SkyboxRenderPass.h>
#include <ituGL/renderer/ForwardRenderPass.h>
#include <ituGL/scene/RendererSceneVisitor.h>

#include <ituGL/scene/ImGuiSceneVisitor.h>
#include <imgui.h>

#include "nfd.h"

SceneViewerApplication::SceneViewerApplication()
    : Application(1400, 1024, "3D Render Space")
    , m_renderer(GetDevice())
{
}

void SceneViewerApplication::Initialize()
{
    Application::Initialize();

    // Initialize DearImGUI
    m_imGui.Initialize(GetMainWindow());

    InitializeCamera();
    InitializeLights();
    InitializeMaterial();
    InitializeModels();
    InitializeRenderer();
}

void SceneViewerApplication::Update()
{
    Application::Update();

    // Update camera controller
    m_cameraController.Update(GetMainWindow(), GetDeltaTime());

    // Add the scene nodes to the renderer
    RendererSceneVisitor rendererSceneVisitor(m_renderer);
    m_scene.AcceptVisitor(rendererSceneVisitor);
}

void SceneViewerApplication::Render()
{
    Application::Render();

    GetDevice().Clear(true, Color(0.0f, 0.0f, 0.0f, 1.0f), true, 1.0f);

    // if (m_showSkybox)
    // {   
    //     m_SkyboxPassIndex = m_renderer.AddRenderPass(std::make_unique<SkyboxRenderPass>(m_skyboxTexture));
    // }
    // if (!m_showSkybox && m_SkyboxPassIndex != -1)
    // {   
    //     m_renderer.RemoveRenderPass(m_SkyboxPassIndex);
    // }

    // Render the scene
    m_renderer.Render();

    // Render the debug user interface
    RenderGUI();
}

void SceneViewerApplication::Cleanup()
{
    // Cleanup DearImGUI
    m_imGui.Cleanup();

    Application::Cleanup();
}

void SceneViewerApplication::InitializeCamera()
{
    // Create the main camera
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->SetViewMatrix(glm::vec3(-1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->SetPerspectiveProjectionMatrix(1.0f, 1.0f, 0.1f, 100.0f);

    // Create a scene node for the camera
    std::shared_ptr<SceneCamera> sceneCamera = std::make_shared<SceneCamera>("camera", camera);

    // Add the camera node to the scene
    m_scene.AddSceneNode(sceneCamera);

    // Set the camera scene node to be controlled by the camera controller
    m_cameraController.SetCamera(sceneCamera);
}

void SceneViewerApplication::InitializeLights()
{
    // Create a directional light and add it to the scene
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->SetDirection(glm::vec3(-0.3f, -1.0f, -0.3f)); // It will be normalized inside the function
    directionalLight->SetIntensity(3.0f);
    m_scene.AddSceneNode(std::make_shared<SceneLight>("directional light", directionalLight));

    // Create a point light and add it to the scene
    //std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    //pointLight->SetPosition(glm::vec3(0, 0, 0));
    //pointLight->SetDistanceAttenuation(glm::vec2(5.0f, 10.0f));
    //m_scene.AddSceneNode(std::make_shared<SceneLight>("point light", pointLight));
}

void SceneViewerApplication::InitializeMaterial()
{
    // Load and build shader
    std::vector<const char*> vertexShaderPaths;
    vertexShaderPaths.push_back("shaders/version330.glsl");
    vertexShaderPaths.push_back("shaders/default.vert");
    Shader vertexShader = ShaderLoader(Shader::VertexShader).Load(vertexShaderPaths);

    std::vector<const char*> fragmentShaderPaths;
    fragmentShaderPaths.push_back("shaders/version330.glsl");
    fragmentShaderPaths.push_back("shaders/utils.glsl");
    fragmentShaderPaths.push_back("shaders/lambert-ggx.glsl");
    // fragmentShaderPaths.push_back("shaders/blinn-phong.glsl");
    fragmentShaderPaths.push_back("shaders/lighting.glsl");
    fragmentShaderPaths.push_back("shaders/default_pbr.frag");
    // fragmentShaderPaths.push_back("shaders/default.frag");
    Shader fragmentShader = ShaderLoader(Shader::FragmentShader).Load(fragmentShaderPaths);

    std::shared_ptr<ShaderProgram> shaderProgramPtr = std::make_shared<ShaderProgram>();
    shaderProgramPtr->Build(vertexShader, fragmentShader);

    // Get transform related uniform locations
    ShaderProgram::Location cameraPositionLocation = shaderProgramPtr->GetUniformLocation("CameraPosition");
    ShaderProgram::Location worldMatrixLocation = shaderProgramPtr->GetUniformLocation("WorldMatrix");
    ShaderProgram::Location viewProjMatrixLocation = shaderProgramPtr->GetUniformLocation("ViewProjMatrix");

    // Register shader with renderer
    m_renderer.RegisterShaderProgram(shaderProgramPtr,
        [=](const ShaderProgram& shaderProgram, const glm::mat4& worldMatrix, const Camera& camera, bool cameraChanged)
        {
            if (cameraChanged)
            {
                shaderProgram.SetUniform(cameraPositionLocation, camera.ExtractTranslation());
                shaderProgram.SetUniform(viewProjMatrixLocation, camera.GetViewProjectionMatrix());
            }
            shaderProgram.SetUniform(worldMatrixLocation, worldMatrix);
        },
        m_renderer.GetDefaultUpdateLightsFunction(*shaderProgramPtr)
    );

    // Filter out uniforms that are not material properties
    ShaderUniformCollection::NameSet filteredUniforms;
    filteredUniforms.insert("CameraPosition");
    filteredUniforms.insert("WorldMatrix");
    filteredUniforms.insert("ViewProjMatrix");
    filteredUniforms.insert("LightIndirect");
    filteredUniforms.insert("LightColor");
    filteredUniforms.insert("LightPosition");
    filteredUniforms.insert("LightDirection");
    filteredUniforms.insert("LightAttenuation");

    // Create reference material
    assert(shaderProgramPtr);
    m_defaultMaterial = std::make_shared<Material>(shaderProgramPtr, filteredUniforms);
}

float SceneViewerApplication::InitializeSkybox()
{
    m_skyboxTexture = TextureCubemapLoader::LoadTextureShared("models/skybox/room.hdr", TextureObject::FormatRGB, TextureObject::InternalFormatRGB16F);

    m_skyboxTexture->Bind();
    float maxLod;
    m_skyboxTexture->GetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
    TextureCubemapObject::Unbind();

    // m_skyboxTexture = TextureCubemapLoader::LoadTextureShared("models/skybox/room.hdr", TextureObject::FormatRGB, TextureObject::InternalFormatRGB16F);
    // //InternalFormatSRGB8
    // m_skyboxTexture->Bind();
    // float maxLod;
    // m_skyboxTexture->GetParameter(TextureObject::ParameterFloat::MaxLod, maxLod);
    // TextureCubemapObject::Unbind();
    return maxLod;
}

void SceneViewerApplication::InitializeModels()
{
    m_defaultMaterial->SetUniformValue("AmbientColor", glm::vec3(0.25f));

    // Load skybox
    float maxLod = InitializeSkybox();
    m_defaultMaterial->SetUniformValue("EnvironmentTexture", m_skyboxTexture);
    m_defaultMaterial->SetUniformValue("EnvironmentMaxLod", maxLod);
    
    m_defaultMaterial->SetUniformValue("Color", glm::vec3(1.0f));

    // Configure loader
    ModelLoader loader(m_defaultMaterial);

    // Create a new material copy for each submaterial
    loader.SetCreateMaterials(true);

    // Flip vertically textures loaded by the model loader
    loader.GetTexture2DLoader().SetFlipVertical(true);

    // Link vertex properties to attributes
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Position, "VertexPosition");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Normal, "VertexNormal");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Tangent, "VertexTangent");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::Bitangent, "VertexBitangent");
    loader.SetMaterialAttribute(VertexAttribute::Semantic::TexCoord0, "VertexTexCoord");

    // Link material properties to uniforms
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::DiffuseColor, "Color");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::DiffuseTexture, "ColorTexture");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::NormalTexture, "NormalTexture");
    loader.SetMaterialProperty(ModelLoader::MaterialProperty::SpecularTexture, "SpecularTexture");

    // // Load models
    // std::shared_ptr<Model> teapotModel = loader.LoadShared("models/teapot/teapot.obj");
    // m_scene.AddSceneNode(std::make_shared<SceneModel>("Teapot", teapotModel));

    // std::shared_ptr<Model> cameraModel = loader.LoadShared("models/camera/camera.obj");
    // m_scene.AddSceneNode(std::make_shared<SceneModel>("camera model", cameraModel));

    // std::shared_ptr<Model> teaSetModel = loader.LoadShared("models/tea_set/tea_set.obj");
    // m_scene.AddSceneNode(std::make_shared<SceneModel>("tea set", teaSetModel));

    std::shared_ptr<Model> clockModel = loader.LoadShared("models/alarm_clock/alarm_clock.obj");
    m_scene.AddSceneNode(std::make_shared<SceneModel>("alarm clock", clockModel));
}

void SceneViewerApplication::InitializeRenderer()
{
    m_renderer.AddRenderPass(std::make_unique<ForwardRenderPass>());
    m_SkyboxPassIndex = m_renderer.AddRenderPass(std::make_unique<SkyboxRenderPass>(m_skyboxTexture));
}

void SceneViewerApplication::RenderGUI()
{
    m_imGui.BeginFrame();
//    if (ImGui::Button("input file")) {
//      nfdchar_t *outPath = NULL;
//      nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );
//
//      if ( result == NFD_OKAY ) {
//        puts("Success!");
//        puts(outPath);
//        free(outPath);
//      }
//      else if ( result == NFD_CANCEL ) {
//        puts("User pressed cancel.");
//      }
//      else {
//        printf("Error: %s\n", NFD_GetError() );
//      }
//    }
    // Draw GUI for scene nodes, using the visitor pattern
    ImGuiSceneVisitor imGuiVisitor(m_imGui, "Scene");
    m_scene.AcceptVisitor(imGuiVisitor);

    // Draw GUI for camera controller
    m_cameraController.DrawGUI(m_imGui);

    ImGui::Checkbox("Skybox", &m_showSkybox);

    m_imGui.EndFrame();
}
