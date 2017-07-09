#pragma once


/////////////
// INCLUDE
#include <hkUtilLib\dataManage\LinkedList.h>
#include <hkUtilLib\core\Vector.h>

#include "KeyInputCode.h"


//////////////////////
// CLASS: InputCore
namespace InputCore
{
    ////////////////////////
    // STRUCT: IECallback
    struct IECallback
    {
    public:
        IECallback() { m_isEnable = true; }
        virtual bool Trigger() { return OnCall(); }

        void Enable() { m_isEnable = true; }
        void Disable() { m_isEnable = false; }
        bool IsEnable() { return m_isEnable; }

    private:
        virtual bool OnCall() { return true; }

    private:
        bool m_isEnable;
        bool m_isHalt;
    };


    ///////////////////////
    // ENUM: MOUSE_STATE
    enum INPUT_MOUSE_STATE
    {
        MS_NONE =       0x0,
        MS_LMOUSE =     0x1,
        MS_RMOUSE =     0x1 << 1,
        MS_MMOUSE =     0x1 << 2
    };


    ////////////////////////////////
    // ENUM: INPUT_MODIFIER_STATE
    enum INPUT_MODIFIER_STATE
    {
        MS_CONTROL =    0x1,
        MS_SHIFT =      0x1 << 1,
        MS_ALT =        0x1 << 2,
        MS_CAPSLOCK =   0x1 << 3,
    };


    //////////////////////////////
    // STRUCT: IEMouseEventArgs
    struct IEMouseEventArgs
    {
    public:
        IEMouseEventArgs();
        IEMouseEventArgs(const IEMouseEventArgs&);
        ~IEMouseEventArgs();

        void operator=(const IEMouseEventArgs&);

    public:
        uint modifierState;
        hkVector2F position;
        hkVector2F moveVector;
        uint mouseState;
        int delta;
    };


    /////////////////////////////
    // STRUCT: IEMouseCallback
    struct IEMouseCallback : public IECallback
    {
    public:
        IEMouseCallback() {}
        bool Trigger(const IEMouseEventArgs& evnt) { return OnCall(evnt); }
        bool operator==(const IEMouseCallback& evnt) { return this == &evnt; }

    private:
        virtual bool OnCall(const IEMouseEventArgs& evnt) { return true; }
    };


    //////////////////////////////
    // STRUCT: IEMouseEventArgs
    struct IEKeyboardEventArgs
    {
    public:
        IEKeyboardEventArgs();
        IEKeyboardEventArgs(const IEKeyboardEventArgs&);
        ~IEKeyboardEventArgs();

        void operator=(const IEKeyboardEventArgs&);

    public:
        uint keyId;
        bool isPressed;
        uint modifierState;
    };


    ////////////////////////////////
    // STRUCT: IEKeyboardCallback
    struct IEKeyboardCallback : public IECallback
    {
    public:
        IEKeyboardCallback() {}
        bool Trigger(const IEKeyboardEventArgs& evnt) { return OnCall(evnt); }
        bool operator==(const IEKeyboardCallback& evnt) { return this == &evnt; }

    private:
        virtual bool OnCall(const IEKeyboardEventArgs& evnt) { return true; }
    };


    //////////////////////////
    // CLASS: KMInputManger
    // KM stand for keyboard and mouse
    class KMInputManager
    {
        ///////////////////
        // STATIC MEMBER
    public:
        static const uint NUM_KEY = 256;


        ///////////////////
        // METHOD MEMBER
    public:
        KMInputManager();
        KMInputManager(const KMInputManager&);
        ~KMInputManager();

        void Initialize();
        void Update();

        void KeyDown(uint key);
        void KeyUp(uint key);
        void ResetKeyState();

        void LMouseDown(int x, int y);
        void RMouseDown(int x, int y);
        void MMouseDown(int x, int y);

        void LMouseUp(int x, int y);
        void RMouseUp(int x, int y);
        void MMouseUp(int x, int y);

        void MouseMove(int x, int y);
        void MouseWheelChange(int delta);

        bool IsKeyDown(uint key);

        //--------------------------------------------------------------------------
        // Event callback
        void OnKeyDown(SPtr<IEKeyboardCallback> callback);
        void OnKeyUp(SPtr<IEKeyboardCallback> callback);

        void OnMouseDown(SPtr<IEMouseCallback> callback);
        void OnMouseUp(SPtr<IEMouseCallback> callback);
        void OnMouseMove(SPtr<IEMouseCallback> callback);
        void OnMouseWheelChange(SPtr<IEMouseCallback> callback);

    private:
        void TriggerKeyboardEvent(LinkedList<IEKeyboardCallback>& list,
            const IEKeyboardEventArgs& args);

        void TriggerMouseEvent(LinkedList<IEMouseCallback>& list,
            const IEMouseEventArgs& args);

        void UpdateModifier(uint key, bool press);

        void UpdateMousePosition(int x, int y);
        inline void UpdateMouseDelta(int delta);
        inline void EnableMouseState(uint state);
        inline void DisableMouseState(uint state);

        //////////////
        // VARIABLE
    private:
        bool m_keys[NUM_KEY];
        IEMouseEventArgs m_mouseArgs;
        
        LinkedList<IEKeyboardCallback> m_keyDownCallbackList;
        LinkedList<IEKeyboardCallback> m_keyUpCallbackList;

        LinkedList<IEMouseCallback> m_mouseDownCallbackList;
        LinkedList<IEMouseCallback> m_mouseUpCallbackList;
        LinkedList<IEMouseCallback> m_mouseMoveCallbackList;
        LinkedList<IEMouseCallback> m_mouseWheelCallbackList;
    };
}

#define IMS_CAPSLOCK InputCore::INPUT_MODIFIER_STATE::MS_CAPSLOCK
#define IMS_CONTROL InputCore::INPUT_MODIFIER_STATE::MS_CONTROL
#define IMS_SHIFT InputCore::INPUT_MODIFIER_STATE::MS_SHIFT
#define IMS_ALT InputCore::INPUT_MODIFIER_STATE::MS_ALT

#define IMS_LMOUSE InputCore::INPUT_MOUSE_STATE::MS_LMOUSE
#define IMS_RMOUSE InputCore::INPUT_MOUSE_STATE::MS_RMOUSE
#define IMS_MMOUSE InputCore::INPUT_MOUSE_STATE::MS_MMOUSE

#define IMS_NONE InputCore::INPUT_MOUSE_STATE::MS_NONE