#include "CameraControll.h"

//==================================================================================================
#pragma region // Camera controller
//==================================================================================================
// 
CameraControll::CameraControll()
{
    m_camera = nullptr;
}

CameraControll::CameraControll(const CameraControll& clone)
{
}

CameraControll::~CameraControll()
{
}

void CameraControll::Initialize(WPtr<Camera::D2DSideScrollCamera> camera,
                                InputCore::KMInputManager& inputManager)
{
    m_camera = camera;

    m_keyDownCallback = new OnKeyDown(this);
    m_keyUpCallback = new OnKeyUp(this);

    inputManager.OnKeyDown(m_keyDownCallback);
    inputManager.OnKeyUp(m_keyUpCallback);
}

bool CameraControll::Update()
{
    if (m_direction != hkVector2F(0, 0) && m_camera != nullptr)
    {
        m_camera->Move(m_direction);
        return true;
    }

    return false;
}

void CameraControll::SetCameraX(float x)
{
    m_direction.x = x;
}

void CameraControll::SetCameraY(float y)
{
    m_direction.y = y;
}

void CameraControll::HaltControll()
{
    m_keyDownCallback->Disable();
    m_keyUpCallback->Disable();
}

void CameraControll::ResumeControll()
{
    m_keyDownCallback->Enable();
    m_keyUpCallback->Enable();
}

//==================================================================================================
#pragma endregion // Camera controller
//==================================================================================================


//==================================================================================================
#pragma region // Event callback
//==================================================================================================

const float CameraControll::OnKeyDown::DEFAULT_CAMERA_SPEED = 10.0f;

CameraControll::OnKeyDown::OnKeyDown(WPtr<CameraControll> cameraControl)
{
    m_cameraControll = cameraControl;
}

bool CameraControll::OnKeyDown::OnCall(const InputCore::IEKeyboardEventArgs& evnt)
{
    switch (evnt.keyId)
    {
    case KI_UP:
        m_cameraControll->SetCameraY(DEFAULT_CAMERA_SPEED);
        break;

    case KI_DOWN:
        m_cameraControll->SetCameraY(-DEFAULT_CAMERA_SPEED);
        break;

    case KI_LEFT:
        m_cameraControll->SetCameraX(-DEFAULT_CAMERA_SPEED);
        break;

    case KI_RIGHT:
        m_cameraControll->SetCameraX(DEFAULT_CAMERA_SPEED);
        break;

    default:
        break;
    }

    return true;
}

CameraControll::OnKeyUp::OnKeyUp(WPtr<CameraControll> cameraControl)
{
    m_cameraControll = cameraControl;
}

bool CameraControll::OnKeyUp::OnCall(const InputCore::IEKeyboardEventArgs& evnt)
{
    if (evnt.keyId == KI_UP || evnt.keyId == KI_DOWN)
    {
        m_cameraControll->SetCameraY(0);
    }
    else if (evnt.keyId == KI_LEFT || evnt.keyId == KI_RIGHT)
    {
        m_cameraControll->SetCameraX(0);
    }

    return true;
}

//==================================================================================================
#pragma endregion // Event callback
//==================================================================================================
