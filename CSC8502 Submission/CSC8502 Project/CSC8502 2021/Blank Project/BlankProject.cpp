#include "../NCLGL/window.h"
#include "Renderer.h"
#include <string>
#include <iostream>
using namespace std;
int main()	{
	Window w("Make your own project", 1280, 720, false);

	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow()  && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		float dt = w.GetTimer()->GetTimeDeltaSeconds();
		renderer.UpdateScene(dt);
		renderer.RenderScene();
		renderer.SwapBuffers();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}
		string fps = to_string(1.0 / dt);
		w.SetTitle(fps);
	}


	return 0;
}