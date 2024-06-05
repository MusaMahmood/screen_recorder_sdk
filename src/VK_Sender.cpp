// Include windows?

#include <windows.h>
#include "ScreenRecorder.h"

constexpr int frame_color_depth = 4;

void CropImage(
    char* input_buffer, 
    int left_corner_x,  int left_corner_y, 
    int right_corner_x, int right_corner_y,
    int width, int height, 
    char* output_buffer, 
    int* new_width, int* new_height) {
    
    (*new_width) = right_corner_x - left_corner_x;
    (*new_height) = right_corner_y - left_corner_y;
    int row_pitch = width * frame_color_depth;
    
    for (int i {0}; i < (*new_height); i++) {
        int output_cursor = i * (*new_width) * frame_color_depth;
        int input_cursor = (left_corner_y + i) * row_pitch + (left_corner_x * frame_color_depth);
        memcpy(&output_buffer[output_cursor], &input_buffer[input_cursor], (*new_width) * frame_color_depth);
    }
}

int VK_Send_Keystroke (int __INPUT_VK_KEY) {
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