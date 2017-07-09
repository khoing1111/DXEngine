#pragma once

/////////////
// INCLUDE
#include <hkUtilLib\dataManage\LinkedList.h>
#include <hkUtilLib\core\Vector.h>
#include <hkUtilLib\core\Shape.h>


namespace Camera
{
    ////////////////////////////////
    // CLASS: D2DSideScrollCamera
    class D2DSideScrollCamera
    {
        ////////////
        // MEMBER
    public:
        D2DSideScrollCamera();
        D2DSideScrollCamera(const D2DSideScrollCamera&);
        ~D2DSideScrollCamera();

        void Initialize(float screenWidth, float screenHeight);
        void Update();

        void SetPosition(float x, float y);
        void SetPosition(const hkVector2F&);

        void Move(float x, float y);
        void Move(const hkVector2F&);

        void Zoom(float z);
        float GetZoom();

        hkMatrix3x3 GetViewMatrix();

    private:
        void UpdateViewMatrix();

    private:
        hkMatrix3x3 m_viewMatrix;
        hkVector2F m_screenCenter;
        hkVector2F m_position;
        float m_zoomDegree;
    };


    //////////////////////
    // CLASS: D3DCamera
    class D3DCamera
    {
        ///////////////////
        // STATIC MEMBER
    private:
        static WPtr<D3DCamera> s_activeCamera;
        static LinkedList<D3DCamera> s_cameraList;

    public:
        static void CreateCamera(hkString name, float width, float height, float fov, float zNear, float zFar);

        static WPtr<D3DCamera> GetActiveCamera();
        static WPtr<D3DCamera> GetCamera(hkString name);
        static void SetActiveCamera(hkString name);
        static void ReleaseCamera(hkString name);
        static void ReleasseAll();


        ///////////////////
        // OTHER MEMBERS
    public:
        D3DCamera();
        D3DCamera(const D3DCamera&);
        ~D3DCamera();

        void Initialize(hkString name, float width, float height, float fov, float zNear, float zFar);
        void SetLookAt(hkVector3F at, hkVector3F eyes, hkVector3F up);

        void SetOrthoProjection();
        void SetPerspectiveProjection();

        void Move(float x, float y, float z);
        void Move(const hkVector3F&);

        void SetPosition(float x, float y, float z);
        void SetPosition(const hkVector3F&);

        hkMatrix4x4 GetViewMatrix();
        hkMatrix4x4 GetProjectionMatrix();
        hkMatrix4x4 GetViewProjectionMatrix();

        float GetScreenWidth();
        float GetScreenHeight();
        hkVector2F GetScreenSize();
        float GetViewNear();
        float GetViewFar();

        hkString GetName();

        bool operator==(const D3DCamera& other);

    private:
        void InitializeOrthographic(
            float width, float height, float camNear, float camFar);
        void InitializeFOV(
            float aspect, float fov, float camNear, float camFar);
        void ResetViewProjectionMatrix();

    private:
        hkMatrix4x4 m_orthoLHMatrix;
        hkMatrix4x4 m_fovMatrix;
        hkMatrix4x4 m_projectionMatrix;

        hkMatrix4x4 m_viewMatrix;
        hkMatrix4x4 m_viewProjectionMatrix;

        float m_screenWidth;
        float m_screenHeight;
        float m_zNear;
        float m_zFar;

        hkString m_name;
    };
}