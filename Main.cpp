//--------------------------------------------------------------------------------------
// File: Tutorial04.cpp
//
// This application displays a 3D cube using Direct3D 11
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <iostream>
#include <boost\python.hpp>
#include <windows.h>

namespace boostPython = boost::python;
typedef boostPython::object pythonObject;

void boostEmbededFunction()
{
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* pystdout = PyFile_FromString("CONOUT$", "wt");
    if (-1 == PyObject_SetAttrString(sys, "stdout", pystdout)) {
        /* raise errors and wail very loud */
    }
    PyObject* pystdin = PyFile_FromString("CONIN$", "rb");
    if (-1 == PyObject_SetAttrString(sys, "stdin", pystdin)) {
        /* raise errors and wail very loud */
    }
    //cout << "no error" << endl;
    Py_DECREF(sys);
    Py_DECREF(pystdout);
    Py_DECREF(pystdin);

    PyObject* filename = PyString_FromString((char*)"Python.Function");
    PyObject* imp = PyImport_Import(filename);
    PyObject* func = PyObject_GetAttrString(imp, (char*)"Multiply");
    PyObject* args = PyTuple_Pack(2, PyLong_FromLong(3), PyLong_FromLong(3));
    PyObject* retured_value = PyObject_CallObject(func, args); // if you have arg
    long result = PyLong_AsLong(retured_value);
    std::cout << result << std::endl;

    pythonObject FileModule = boostPython::import("Python.Function");
    pythonObject Function = FileModule.attr("Function")();
    Function.attr("CheckConnect")("ÇÑ±Û");
}

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               hInst = NULL;
HWND                    hWnd = NULL;
LPWSTR*                 szArgList;
int                     nArgs;


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SetTextAlign(hdc, TA_BOTTOM);
        TextOut(hdc, 200, 60, szArgList[0], 4);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, NULL);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"20170323_ConnectBoostAndPythonUsingWinAPI";
    wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    hWnd = CreateWindow(L"20170323_ConnectBoostAndPythonUsingWinAPI", L"20170323_ConnectBoostAndPythonUsingWinAPI", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
        NULL);
    if (!hWnd)
        return E_FAIL;

    ShowWindow(hWnd, nCmdShow);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    FILE* acStreamOut;
    FILE* acStreamIn;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    AllocConsole();
    freopen_s(&acStreamIn, "CONIN$", "r", stdin);
    freopen_s(&acStreamOut, "CONOUT$", "w", stdout);

    printf("System Start\n");

    if (FAILED(InitWindow(hInstance, nCmdShow)))
        return 0;

    int i;

    szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
    /*if (NULL == szArgList)
    {
    wprintf(L"CommandLineToArgvW failed\n");
    return 0;
    }
    else
    {
    for (i = 0; i < nArgs; i++)
    {
    printf("%d: %ws\n", i, szArgList[i]);
    }
    }*/



    if (Py_Initialize(), Py_IsInitialized())
    {
        try
        {
            boostEmbededFunction();
        }
        catch (boostPython::error_already_set const &)
        {
            PyErr_Print();
        }
        Py_Finalize();
    }

    // Main message loop
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {

        }
    }

    // Free memory allocated for CommandLineToArgvW arguments.
    LocalFree(szArgList);

    printf("System Out\n");

    FreeConsole();

    return (int)msg.wParam;
}