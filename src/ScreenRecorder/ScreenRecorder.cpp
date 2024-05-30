#include <windows.h>

#include "Recorder.h"
#include "RecorderDDA.h"
#include "RecorderParams.h"
#include "ScreenRecorder.h"

Recorder *recorder = NULL;
HANDLE mutex = NULL;

int InitResources (int pid, int desktopNum)
{
    if (!mutex)
    {
        mutex = CreateMutex (NULL, FALSE, NULL);
        if (!mutex)
        {
            return GENERAL_ERROR;
        }
    }
    if (recorder)
    {
        delete recorder;
        recorder = NULL;
    }

    struct RecorderParams params;
    params.pid = pid;
    params.desktopNum = desktopNum;

    recorder = new RecorderDDA (params);
    WaitForSingleObject (mutex, INFINITE);
    int res = recorder->InitResources ();
    ReleaseMutex (mutex);
    if (res != STATUS_OK)
    {
        delete recorder;
        recorder = NULL;
        CloseHandle (mutex);
        mutex = NULL;
        return res;
    }

    return STATUS_OK;
}

int GetScreenShot (unsigned int maxAttempts, unsigned char *frameBuffer, int *width, int *height)
{
    if (!recorder)
        return SESSION_NOT_CREATED_ERROR;

    WaitForSingleObject (mutex, INFINITE);
    int res = recorder->GetScreenShot (maxAttempts, frameBuffer, width, height);
    ReleaseMutex (mutex);
    return res;
}

int FreeResources ()
{
    if (recorder)
    {
        if (mutex)
        {
            WaitForSingleObject (mutex, INFINITE);
            delete recorder;
            recorder = NULL;
            ReleaseMutex (mutex);
            CloseHandle (mutex);
            mutex = NULL;
        }
        else
        {
            return GENERAL_ERROR;
        }
    }

    return STATUS_OK;
}

int SetLogLevel (int level)
{
    return Recorder::SetLogLevel (level);
}

int SetLogFile (char *logFile)
{
    return Recorder::SetLogFile (logFile);
}

int StartVideoRecording (
    const char *outputFileName, int frameRate, int bitRate, int useHardwareTransform)
{
    if (!recorder)
        return SESSION_NOT_CREATED_ERROR;

    WaitForSingleObject (mutex, INFINITE);
    bool useTransforms = (useHardwareTransform != 0);
    int res = recorder->StartVideoRecording (outputFileName, frameRate, bitRate, useTransforms);
    ReleaseMutex (mutex);
    return res;
}

int StopVideoRecording ()
{
    if (!recorder)
        return SESSION_NOT_CREATED_ERROR;

    WaitForSingleObject (mutex, INFINITE);
    int res = recorder->StopVideoRecording ();
    ReleaseMutex (mutex);
    return res;
}

int GetPID (int *pid)
{
    if (!recorder)
        return SESSION_NOT_CREATED_ERROR;

    *pid = recorder->GetPID ();
    return STATUS_OK;
}
