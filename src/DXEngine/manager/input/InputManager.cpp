#include "InputManager.h"

//=============================================================================
#pragma region // Event arguments
//=============================================================================

InputCore::IEMouseEventArgs::IEMouseEventArgs()
{
    position = hkVector2F(0, 0);
    mouseState = MS_NONE;
    moveVector = hkVector2F(0, 0);
    modifierState = MS_NONE;
    delta = 0;
}

InputCore::IEMouseEventArgs::IEMouseEventArgs(const IEMouseEventArgs& clone)
{
    *this = clone;
}

InputCore::IEMouseEventArgs::~IEMouseEventArgs()
{

}

void InputCore::IEMouseEventArgs::operator=(const IEMouseEventArgs& clone)
{
    position = clone.position;
    mouseState = clone.mouseState;
    moveVector = clone.moveVector;
    modifierState = clone.modifierState;
}



InputCore::IEKeyboardEventArgs::IEKeyboardEventArgs()
{
    keyId = 0;
    isPressed = false;
    modifierState = MS_NONE;
}

InputCore::IEKeyboardEventArgs::IEKeyboardEventArgs(const IEKeyboardEventArgs& clone)
{
    *this = clone;
}

InputCore::IEKeyboardEventArgs::~IEKeyboardEventArgs()
{

}

void InputCore::IEKeyboardEventArgs::operator=(const IEKeyboardEventArgs& clone)
{
    keyId = clone.keyId;
    isPressed = clone.isPressed;
    modifierState = clone.modifierState;
}

//=============================================================================
#pragma endregion // Event arguments
//=============================================================================



//=============================================================================
#pragma region // Keboard and mouse manager
//=============================================================================

InputCore::KMInputManager::KMInputManager()
{
}

InputCore::KMInputManager::KMInputManager(const KMInputManager& clone)
{
}

InputCore::KMInputManager::~KMInputManager()
{
    m_keyDownCallbackList.Clear();
    m_keyUpCallbackList.Clear();
    m_mouseDownCallbackList.Clear();
    m_mouseUpCallbackList.Clear();
    m_mouseMoveCallbackList.Clear();
    m_mouseWheelCallbackList.Clear();
}

void InputCore::KMInputManager::Initialize()
{
    ResetKeyState();
}

void InputCore::KMInputManager::Update()
{

}

void InputCore::KMInputManager::TriggerKeyboardEvent(
    LinkedList<IEKeyboardCallback>& list,
    const IEKeyboardEventArgs& args)
{
    SPtr<ListIterator<IEKeyboardCallback>> iterator = list.GetIterator();

    if (iterator->IsValid())
        do
            if (iterator->Get()->IsEnable())
                if (!iterator->Get()->Trigger(args))
                    return;
        while (iterator->Next());
}

void InputCore::KMInputManager::TriggerMouseEvent(
    LinkedList<IEMouseCallback>& list,
    const IEMouseEventArgs& args)
{
    SPtr<ListIterator<IEMouseCallback>> iterator = list.GetIterator();

    if (iterator->IsValid())
        do
            if (iterator->Get()->IsEnable())
                if (!iterator->Get()->Trigger(args))
                    return;
    while (iterator->Next());
}

void InputCore::KMInputManager::UpdateModifier(uint key, bool press)
{
    switch (key)
    {
    case 0x11:  // Control
        press ? m_mouseArgs.modifierState |= MS_CONTROL : 
            m_mouseArgs.modifierState &= !MS_CONTROL;
        break;

    case 0x12:  // Alt
        press ? m_mouseArgs.modifierState |= MS_ALT : 
            m_mouseArgs.modifierState &= !MS_ALT;
        break;

    case 0x10:  // Shift
        press ? m_mouseArgs.modifierState |= MS_SHIFT : 
            m_mouseArgs.modifierState &= !MS_SHIFT;
        break;

    case 0x14: // Caps
        if (press) m_mouseArgs.modifierState ^= MS_CAPSLOCK;
        break;
    }
}

void InputCore::KMInputManager::KeyDown(uint key)
{
    if (key < NUM_KEY)
    {
        m_keys[key] = true;
        UpdateModifier(key, true);

        IEKeyboardEventArgs args;
        args.keyId = key;
        args.isPressed = true;
        args.modifierState = m_mouseArgs.modifierState;

        TriggerKeyboardEvent(m_keyDownCallbackList, args);
    }
}

void InputCore::KMInputManager::KeyUp(uint key)
{
    if (key < NUM_KEY)
    {
        m_keys[key] = false;
        UpdateModifier(key, false);

        IEKeyboardEventArgs args;
        args.keyId = key;
        args.isPressed = false;
        args.modifierState = m_mouseArgs.modifierState;

        TriggerKeyboardEvent(m_keyUpCallbackList, args);
    }
}

void InputCore::KMInputManager::UpdateMousePosition(int x, int y)
{
    m_mouseArgs.moveVector = hkVector2F(x, y) - m_mouseArgs.position;
    m_mouseArgs.position = hkVector2F(x, y);
}

void InputCore::KMInputManager::UpdateMouseDelta(int delta)
{
    m_mouseArgs.delta = delta;
}

void InputCore::KMInputManager::EnableMouseState(uint state)
{
    m_mouseArgs.mouseState |= state;
}

void InputCore::KMInputManager::DisableMouseState(uint state)
{
    m_mouseArgs.mouseState &= !(state);
}

void InputCore::KMInputManager::LMouseDown(int x, int y)
{
    EnableMouseState(MS_LMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseDownCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::RMouseDown(int x, int y)
{
    EnableMouseState(MS_LMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseDownCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::MMouseDown(int x, int y)
{
    EnableMouseState(MS_MMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseDownCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::LMouseUp(int x, int y)
{
    DisableMouseState(MS_LMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseUpCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::RMouseUp(int x, int y)
{
    DisableMouseState(MS_RMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseUpCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::MMouseUp(int x, int y)
{
    DisableMouseState(MS_MMOUSE);
    UpdateMousePosition(x, y);

    TriggerMouseEvent(m_mouseUpCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::MouseMove(int x, int y)
{
    UpdateMousePosition(x, y);
    
    TriggerMouseEvent(m_mouseMoveCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::MouseWheelChange(int delta)
{
    UpdateMouseDelta(delta);

    TriggerMouseEvent(m_mouseWheelCallbackList, m_mouseArgs);
}

void InputCore::KMInputManager::ResetKeyState()
{
    for (int i = 0; i < NUM_KEY; i++)
    {
        m_keys[i] = false;
    }
}

bool InputCore::KMInputManager::IsKeyDown(uint key)
{
    if (key < NUM_KEY)
    {
        return m_keys[key];
    }

    return false;
}

void InputCore::KMInputManager::OnKeyDown(SPtr<IEKeyboardCallback> callback)
{
    m_keyDownCallbackList.Add(callback);
}

void InputCore::KMInputManager::OnKeyUp(SPtr<IEKeyboardCallback> callback)
{
    m_keyUpCallbackList.Add(callback);
}

void InputCore::KMInputManager::OnMouseDown(SPtr<IEMouseCallback> callback)
{
    m_mouseDownCallbackList.Add(callback);
}

void InputCore::KMInputManager::OnMouseUp(SPtr<IEMouseCallback> callback)
{
    m_mouseUpCallbackList.Add(callback);
}

void InputCore::KMInputManager::OnMouseMove(SPtr<IEMouseCallback> callback)
{
    m_mouseMoveCallbackList.Add(callback);
}

void InputCore::KMInputManager::OnMouseWheelChange(SPtr<IEMouseCallback> callback)
{
    m_mouseWheelCallbackList.Add(callback);
}

//=============================================================================
#pragma endregion // Keboard and mouse manager
//=============================================================================