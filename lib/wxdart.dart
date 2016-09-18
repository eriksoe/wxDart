library wxdart;

import 'dart-ext:wxdart';

// Returns false on initialization error.
bool InitializeWx() native "InitializeWx";

// Returns false when InitializeWx() has not been called.
bool MainLoop() native "MainLoop";

void quux() native "quux";
void quux1(x) native "quux";

// GUIDELINES:
// - Mirror the WX class hierarchy.

class Window {
}

//========== Frame ==================================================
class Frame extends Window {
    object _peer;
    Frame(string title) {
        _peer = Frame_Create(this, title);
        //TODO: Setup finalization: Call Close() (or Destroy()) on peer if not shown.
    }

    Frame Show(bool visible) {
        Window_Show(this, visible);
        return this;
    }

    Frame Add(Window child) {
        //TODO
        return this;
    }
}
void Frame_Create(Frame f, string title) native "Frame_Create";
void Window_Show(Window w, bool visible) native "Window_Show";
