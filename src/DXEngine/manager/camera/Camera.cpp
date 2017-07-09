#include "Camera.h"

#include <math.h>



//=============================================================================
#pragma region // 2D Side scroll camera
//=============================================================================

Camera::D2DSideScrollCamera::D2DSideScrollCamera()
{
    m_viewMatrix.SetIdentity();
    m_zoomDegree = 0;
}

Camera::D2DSideScrollCamera::D2DSideScrollCamera(const D2DSideScrollCamera& clone)
{
    m_viewMatrix = clone.m_viewMatrix;
    m_position = clone.m_position;
    m_zoomDegree = clone.m_zoomDegree;
    m_screenCenter = clone.m_screenCenter;
}

Camera::D2DSideScrollCamera::~D2DSideScrollCamera()
{

}

void Camera::D2DSideScrollCamera::Initialize(float screenWidth, float screenHeight)
{
    m_screenCenter.x = screenWidth / 2;
    m_screenCenter.y = screenHeight / 2;
}

void Camera::D2DSideScrollCamera::Update()
{
    UpdateViewMatrix();
}

void Camera::D2DSideScrollCamera::SetPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}

void Camera::D2DSideScrollCamera::SetPosition(const hkVector2F& point)
{
    m_position = point;
}

void Camera::D2DSideScrollCamera::Move(float x, float y)
{
    m_position.x -= x;
    m_position.y += y;
}

void Camera::D2DSideScrollCamera::Move(const hkVector2F& point)
{
    m_position += point;
}

void Camera::D2DSideScrollCamera::Zoom(float z)
{
    m_zoomDegree = z;
}

float Camera::D2DSideScrollCamera::GetZoom() 
{
    return m_zoomDegree;
}

hkMatrix3x3 Camera::D2DSideScrollCamera::GetViewMatrix()
{
    UpdateViewMatrix();
    return m_viewMatrix;
}

void Camera::D2DSideScrollCamera::UpdateViewMatrix()
{
    // Reset view matrix
    m_viewMatrix.SetIdentity();

    // Invert view for Y axist to match 2D screen coordinate.
    m_viewMatrix[0][0] = 1;
    m_viewMatrix[1][1] = -1;

    // Scale matrix base on zoom degree.
    if (m_zoomDegree != 0) 
    {
        m_viewMatrix[0][0] *= m_zoomDegree;
        m_viewMatrix[1][1] *= m_zoomDegree;
    }

    // Set tranlation (with invert position)
    m_viewMatrix.Move(-m_position.x + m_screenCenter.x, m_position.y + m_screenCenter.y);
}

//=============================================================================
#pragma endregion // 2D Side scroll camera
//=============================================================================


//=============================================================================
#pragma region // 3D Camera
//=============================================================================

///////////////////
// STATIC METHOD
WPtr<Camera::D3DCamera> Camera::D3DCamera::s_activeCamera;
LinkedList<Camera::D3DCamera> Camera::D3DCamera::s_cameraList;

void Camera::D3DCamera::CreateCamera(hkString name, 
                                            float width,
                                            float height,
                                            float fov,
                                            float zNear,
                                            float zFar)
{
    SPtr<D3DCamera> newD3DCamera(new D3DCamera());
    newD3DCamera->Initialize(name, width, height, fov, zNear, zFar);

    s_cameraList.Add(newD3DCamera);

    if (s_activeCamera == nullptr)
        s_activeCamera = newD3DCamera;
}

WPtr<Camera::D3DCamera> Camera::D3DCamera::GetActiveCamera()
{
    return s_activeCamera;
}

WPtr<Camera::D3DCamera> Camera::D3DCamera::GetCamera(hkString name)
{
    SPtr<ListIterator<Camera::D3DCamera>> iterator = s_cameraList.GetIterator();

    if (iterator->IsValid())
        do 
            if (iterator->Get()->GetName() == name)
                return iterator->Get();
        while (iterator->Next());

    return nullptr;
}

void Camera::D3DCamera::SetActiveCamera(hkString name)
{
    WPtr<D3DCamera> camera = GetCamera(name);
    if (camera != nullptr)
        s_activeCamera = camera;
}

void Camera::D3DCamera::ReleaseCamera(hkString name)
{
    WPtr<D3DCamera> camera = GetCamera(name);

    if (camera != nullptr)
    {
        s_cameraList.Remove(camera);

        if (s_activeCamera == nullptr)
            s_activeCamera = s_cameraList.GetFirst();
    }
}

void Camera::D3DCamera::ReleasseAll()
{
    s_cameraList.Clear();
    s_activeCamera.Release();
}


///////////////////
// MEMBER METHOD
Camera::D3DCamera::D3DCamera()
{
    m_projectionMatrix;
    m_screenWidth = 0;
    m_screenHeight = 0;
    m_zNear = 0;
    m_zFar = 0;
}

Camera::D3DCamera::D3DCamera(const D3DCamera& clone)
{
    m_viewMatrix = clone.m_viewMatrix;
    m_orthoLHMatrix = clone.m_orthoLHMatrix;
    m_fovMatrix = clone.m_fovMatrix;
    m_projectionMatrix = clone.m_projectionMatrix;
    ResetViewProjectionMatrix();

    m_screenHeight = clone.m_screenHeight;
    m_screenWidth = clone.m_screenWidth;
    m_zNear = clone.m_zNear;
    m_zFar = clone.m_zFar;
}

Camera::D3DCamera::~D3DCamera()
{

}

void Camera::D3DCamera::Initialize(
    hkString name, float width, float height, float fov, float zNear, float zFar)
{
    m_screenHeight = height;
    m_screenWidth = width;
    m_zNear = zNear;
    m_zFar = zFar;

    InitializeOrthographic(width, height, zNear, zFar);
    InitializeFOV(width / height, fov, zNear, zFar);

    SetOrthoProjection();

    m_name = name;
}

void Camera::D3DCamera::InitializeOrthographic(
    float width, float height, float camNear, float camFar)
{
    m_orthoLHMatrix.SetIdentity();
    m_orthoLHMatrix[0][0] = 2 / width;
    m_orthoLHMatrix[1][1] = 2 / height;
    m_orthoLHMatrix[2][2] = 1 / (camFar - camNear);
    m_orthoLHMatrix[3][2] = camNear / (camNear - camFar);
}

void Camera::D3DCamera::ResetViewProjectionMatrix()
{
    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::D3DCamera::InitializeFOV(float aspect, float fov, float camNear, float camFar)
{
    float yScale = 1 / tan(fov / 2);
    float xScale = yScale / aspect;

    m_fovMatrix.SetIdentity();
    m_fovMatrix[0][0] = xScale;
    m_fovMatrix[1][1] = yScale;
    m_fovMatrix[2][2] = camFar / (camFar - camNear);
    m_fovMatrix[3][2] = camFar * camNear / (camNear - camFar);
    m_fovMatrix[2][3] = 1;
    m_fovMatrix[3][3] = 0;
}

void Camera::D3DCamera::SetLookAt(hkVector3F at, hkVector3F eyes, hkVector3F up)
{
    hkVector3F zaxis = at - eyes;
    zaxis.Normalize();

    hkVector3F xaxis = up.Cross(zaxis);
    xaxis.Normalize();

    hkVector3F yaxis = zaxis.Cross(xaxis);

    hkMatrix4x4 lookAtMat;
    lookAtMat.SetIdentity();
    lookAtMat[0][0] = xaxis.x;
    lookAtMat[0][1] = yaxis.x;
    lookAtMat[0][2] = zaxis.x;

    lookAtMat[1][0] = xaxis.y;
    lookAtMat[1][1] = yaxis.y;
    lookAtMat[1][2] = zaxis.y;

    lookAtMat[2][0] = xaxis.z;
    lookAtMat[2][1] = yaxis.z;
    lookAtMat[2][2] = zaxis.z;

    lookAtMat[3][0] = -(xaxis * eyes);
    lookAtMat[3][1] = -(yaxis * eyes);
    lookAtMat[3][2] = -(zaxis * eyes);

    m_viewMatrix = lookAtMat;
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::SetOrthoProjection()
{
    m_projectionMatrix = m_orthoLHMatrix;
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::SetPerspectiveProjection()
{
    m_projectionMatrix = m_fovMatrix;
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::Move(float x, float y, float z)
{
    m_viewMatrix.Move(x, y, z);
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::SetPosition(float x, float y, float z)
{
    m_viewMatrix.SetTranslation(x, y, z);
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::Move(const hkVector3F& d)
{
    m_viewMatrix.Move(d);
    ResetViewProjectionMatrix();
}

void Camera::D3DCamera::SetPosition(const hkVector3F& position)
{
    m_viewMatrix.SetTranslation(position);
    ResetViewProjectionMatrix();
}

hkMatrix4x4 Camera::D3DCamera::GetViewMatrix()
{
    return m_viewMatrix;
}

hkMatrix4x4 Camera::D3DCamera::GetProjectionMatrix()
{
    return m_projectionMatrix;
}

hkMatrix4x4 Camera::D3DCamera::GetViewProjectionMatrix()
{
    return m_viewProjectionMatrix;
}


float Camera::D3DCamera::GetScreenWidth()
{
    return m_screenWidth;
}

float Camera::D3DCamera::GetScreenHeight()
{
    return m_screenHeight;
}

hkVector2F Camera::D3DCamera::GetScreenSize()
{
    return hkVector2F(m_screenWidth, m_screenHeight);
}

float Camera::D3DCamera::GetViewNear()
{
    return m_zNear;
}

float Camera::D3DCamera::GetViewFar()
{
    return m_zFar;
}

hkString Camera::D3DCamera::GetName()
{
    return m_name;
}

bool Camera::D3DCamera::operator==(const D3DCamera& other)
{
    return this == &other;
}

//=============================================================================
#pragma endregion // 3D Camera
//=============================================================================