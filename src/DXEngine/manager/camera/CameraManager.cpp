#include "CameraManager.h"


GameCore::CameraManager::CameraManager()
{

}

GameCore::CameraManager::CameraManager(const CameraManager& clone)
{
    
}

GameCore::CameraManager::~CameraManager()
{

}

void GameCore::CameraManager::Initialize(InputCore::KMInputManager& inputManager,
                                         float screenWidth, float screenHeight)
{
    // Create and initialize camera
    m_camera = new Camera::D2DSideScrollCamera();
    m_camera->Initialize(screenWidth, screenHeight);

    // Initialize camera control
    m_cameraControl.Initialize(m_camera, inputManager);
}

bool GameCore::CameraManager::Update()
{
    return m_cameraControl.Update();
}

hkMatrix3x3 GameCore::CameraManager::GetViewMatrix()
{
    if (m_camera != nullptr)
        return m_camera->GetViewMatrix();

    return hkMatrix3x3();
}

WPtr<Camera::D2DSideScrollCamera> GameCore::CameraManager::GetCamera()
{
    return m_camera;
}