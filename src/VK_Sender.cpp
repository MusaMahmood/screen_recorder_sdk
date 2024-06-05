// Include windows?

#include <windows.h>
#include "ScreenRecorder.h"

int VK_Send_Numpad1 (int __INPUT_VK_KEY) {
    // #Source: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput?redirectedfrom=MSDN
    // Create an array of INPUT structures
    INPUT inputs[2] = {};

    // Set up the INPUT structure for the key down event
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = static_cast<WORD>(__INPUT_VK_KEY); // Virtual-Key code for the Num pad '1'
    inputs[0].ki.dwFlags = 0;      // 0 for key press

    // Set up the INPUT structure for the key up event
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = static_cast<WORD>(__INPUT_VK_KEY); // Virtual-Key code for the Num pad '1'
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    
    UINT uSent = SendInput(2, inputs, sizeof(INPUT));
    
    return static_cast<int>(uSent);
}