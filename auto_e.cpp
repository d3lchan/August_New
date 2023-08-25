#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // For simulating keyboard input

void auto_type_from_file(const char *filename, double delay)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        return;
    }

    char line[1024]; // Assuming a maximum line length of 1024 characters
    while (fgets(line, sizeof(line), file))
    {
        // Give some time to switch to the desired input field
        Sleep(5000); // Sleep for 5 seconds

        for (int i = 0; line[i]; i++)
        {
            INPUT input;
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = 0;
            input.ki.wScan = line[i];
            input.ki.dwFlags = KEYEVENTF_UNICODE;
            input.ki.time = 0;
            input.ki.dwExtraInfo = GetMessageExtraInfo();
            SendInput(1, &input, sizeof(INPUT));

            Sleep((int)(delay * 1000));

            input.ki.dwFlags |= KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            Sleep((int)(delay * 1000));
        }

        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = 0x0D; // VK_RETURN for newline
        input.ki.wScan = 0;
        input.ki.dwFlags = KEYEVENTF_UNICODE;
        input.ki.time = 0;
        input.ki.dwExtraInfo = GetMessageExtraInfo();
        SendInput(1, &input, sizeof(INPUT));

        input.ki.dwFlags |= KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <path_to_text_file>\n", argv[0]);
        return 1;
    }

    char *file_path = argv[1];
    auto_type_from_file(file_path, 0.0125); // 10x speed

    return 0;
}
