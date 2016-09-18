#include <dart_api.h>

#include <wx/wx.h>

 #include <cstdlib>
#include <iostream> // For debug purposes.
#include <cstring>

//**********************************************************************
// PURPOSE: Native part of wxDart.
//**********************************************************************

static Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

DART_EXPORT Dart_Handle wxdart_Init(Dart_Handle library) {
    std::cout << "In wxdart_Init\n";
    // Input validation:
    if (Dart_IsError(library)) return library;
    std::cout << "A\n";
    Dart_Handle result_code = Dart_SetNativeResolver(library, ResolveName, NULL);
    if (Dart_IsError(result_code)) return result_code;
    std::cout << "B\n";

    return Dart_Null();
}

// Utility
Dart_Handle HandleError(Dart_Handle handle) {
    if (Dart_IsError(handle)) return Dart_PropagateError(handle);
    else return handle;
}

//==================== Functions:
static void quux0(Dart_NativeArguments args) {
    std::cout << "In quux/0.\n";
    Dart_EnterScope();
    Dart_SetReturnValue(args, Dart_Null());
    Dart_ExitScope();
}

static void quux1(Dart_NativeArguments args) {
    std::cout << "In quux/1.\n";
    Dart_EnterScope();
    Dart_SetReturnValue(args, Dart_Null());
    Dart_ExitScope();
}

static void InitializeWx(Dart_NativeArguments args) {
    Dart_EnterScope();
    wxApp* myApp = new wxApp();
    wxApp::SetInstance( myApp );
    int dummy_argc = 1;
    const char* dummy_argv[2] = { "dummyargv", NULL };

    bool res = wxEntryStart(dummy_argc, (char**)dummy_argv);
    std::cout << "Inited: res=" << res << ".\n";

    wxTheApp->CallOnInit();

    Dart_SetReturnValue(args, Dart_NewBoolean(res));
    Dart_ExitScope();
}

static void MainLoop(Dart_NativeArguments args) {
    Dart_EnterScope();
    std::cout << "In MainLoop: " << wxTheApp << "\n";
    bool initialized = (wxTheApp != NULL);
    if (initialized)
        wxTheApp->MainLoop();

    Dart_SetReturnValue(args, Dart_NewBoolean(initialized));
    Dart_ExitScope();
}

static void Frame_Create(Dart_NativeArguments args) {
    std::cout << "In Frame_Create/2.\n";
    Dart_EnterScope();
    // Validate:
    Dart_Handle dartObj = HandleError(Dart_GetNativeArgument(args, 0));
    void* titlePeer;
    Dart_Handle titleH = HandleError(Dart_GetNativeStringArgument(args, 1, &titlePeer));
    const char* titleC;
    HandleError(Dart_StringToCString(titleH, &titleC));

    wxString titleW = wxString::FromUTF8(titleC);
    wxPoint pos(-1,-1);
    wxSize size(-1,-1);
    wxFrame* fr = new wxFrame(NULL, wxID_ANY, titleW, pos, size);
    Dart_SetPeer(dartObj, dynamic_cast<wxWindow*>(fr));
   std::cout << "frame=" << fr << "\n";

    Dart_SetReturnValue(args, Dart_Null());
    Dart_ExitScope();
}

static void Window_Show(Dart_NativeArguments args) {
   Dart_EnterScope();
   Dart_Handle dartObj = HandleError(Dart_GetNativeArgument(args, 0));
   bool visible;
   HandleError(Dart_GetNativeBooleanArgument(args, 1, &visible));

   wxWindow* objPeer;
   HandleError(Dart_GetPeer(dartObj, (void**)&objPeer));
   std::cout << "objPeer=" << objPeer << "\n";

   objPeer->Show(visible);

   Dart_SetReturnValue(args, Dart_Null());
   Dart_ExitScope();
}




typedef struct {
    const char* name;
    int arity;
    Dart_NativeFunction function;
} FunctionItem;

static FunctionItem functions[] = {
    {"quux", 0, quux0},
    {"quux", 1, quux1},
    {"InitializeWx", 0, InitializeWx},
    {"MainLoop", 0, MainLoop},
    {"Frame_Create", 2, Frame_Create},
    {"Window_Show", 2, Window_Show},
    {NULL, 0, NULL} // Sentinel.
};

Dart_NativeFunction ResolveName(Dart_Handle name, int arity, bool* auto_setup_scope) {
    std::cout << "In ResolveName\n";
    Dart_EnterScope();
    // Validate and unpack input 'name':
    if (!Dart_IsString(name)) return NULL;
    const char* cname;
    HandleError(Dart_StringToCString(name, &cname));

    std::cout << "Resolving...\n";
    Dart_NativeFunction result = NULL;
    for (int i=0; functions[i].name; i++) {
        if (0 == strcmp(cname, functions[i].name) &&
            arity == functions[i].arity)
        {
            result = functions[i].function;
            break;
        }
    }

    Dart_ExitScope();
    return result;
}
