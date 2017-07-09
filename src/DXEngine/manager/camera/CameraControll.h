#pragma once

/////////////
// INCLUDE
#include "../input/InputManager.h"
#include "Camera.h"


///////////////////////////
// CLASS: CameraControll
class CameraControll
{
private:
    ///////////////////////////////////////
    // Mouse and keyboard event callback
    struct OnKeyDown : InputCore::IEKeyboardCallback
    {
    private:
        /////////////////////
        // STATIC CONSTANT
        static const float DEFAULT_CAMERA_SPEED;

    public:
        OnKeyDown(WPtr<CameraControll> cameraControl);

    private:
        virtual bool OnCall(const InputCore::IEKeyboardEventArgs&) override;

    private:
        WPtr<CameraControll> m_cameraControll;
    };

    struct OnKeyUp : InputCore::IEKeyboardCallback
    {
    public:
        OnKeyUp(WPtr<CameraControll> cameraControl);

    private:
        virtual bool OnCall(const InputCore::IEKeyboardEventArgs&) override;

    private:
        WPtr<CameraControll> m_cameraControll;
    };

public:
    CameraControll();
    CameraControll(const CameraControll&);
    ~CameraControll();

    void Initialize(WPtr<Camera::D2DSideScrollCamera> camera, 
                    InputCore::KMInputManager& inputManager);
    bool Update();

    void HaltControll();
    void ResumeControll();

    void SetCameraX(float x);
    void SetCameraY(float y);

private:
    WPtr<Camera::D2DSideScrollCamera> m_camera;

    SPtr<InputCore::IEKeyboardCallback> m_keyDownCallback;
    SPtr<InputCore::IEKeyboardCallback> m_keyUpCallback;

    hkVector2F m_direction;
};

