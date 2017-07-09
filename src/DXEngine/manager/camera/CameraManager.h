#pragma once

#include "CameraControll.h"

namespace GameCore
{
    //////////////////////////
    // CLASS: CameraManager
    class CameraManager
    {
    public:
        CameraManager();
        CameraManager(const CameraManager& clone);
        ~CameraManager();

        void Initialize(InputCore::KMInputManager& inputManager,
                        float screenWidth, float screenHeight);

        bool Update();
        hkMatrix3x3 GetViewMatrix();
        WPtr<Camera::D2DSideScrollCamera> GetCamera();

    private:
        SPtr<Camera::D2DSideScrollCamera> m_camera;
        CameraControll m_cameraControl;
    };
}