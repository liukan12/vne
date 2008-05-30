//
// Tutorial 01 - OpenGL Window (OpenGL 2.1) 
// Brandon Jones (Toji)
// Last Updated 10/09/2007 
// Version 1.0
//

#include "GLWindow.h"

using namespace OpenGL21;

// This is our derived class, and where all of our application-specific processing happens
class GLApp : public GLWindow {
	public:
		GLApp() {
			// We should set any initial values needed here
			// Note that when this is called we won't have a valid
			// render context, so OpenGL initialization should NOT
			// be done here, but rather in OnStartup()
		}

	protected:
		virtual void OnStartup( void ) {
			//
			// Any startup code goes here
			//
		} 

		virtual void OnShutdown( void ) {
			//
			// Any shutdown code goes here
			//
		} 

		virtual void OnFrame( void ) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Tell OpenGL to clear the Color and Depth buffers
			
			//
			// Any drawing code goes here
			//

			SwapBuffers(); // Swap the front and back buffers, displaying what we have just drawn
		}
		
		virtual LRESULT CALLBACK OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			//
			// In this function we can catch any windows messages that are raised and react appropriately
			//
			switch(uMsg) {
				case WM_KEYDOWN:						// This message is raised when a key is pressed down
					if(wParam == VK_ESCAPE) {			// If the key is the "Esc" key signal the program to quit
						Exit(); 
					}
					break;
			}
			
			return GLWindow::OnMessage(hWnd, uMsg, wParam, lParam);	// For any message we haven't handled here, pass it on the the base class message handler
		}	
};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowDisplaySettings wds; // We're going to leave this to it's default settings for the most part, with one or two exceptions
	
	//
	// If we wish to change any of the default settings of the display settings, do so here
	//

	wds.caption = "Tutorial 01 - OpenGL Window"; // Set the window caption to something appropriate for the tutorial
	
	// Ask the user if they want to run fullscreen or not, then set the fullscreen mode accordingly
	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", wds.caption.c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES) { 
		wds.fullscreen = true; 
	}

	GLApp glApp;	// Define the application object

	try {
		glApp.Create( hInstance, wds );		// Initialize OpenGL with the previously defined window settings
	} catch( std::exception err ) {			// If an error is thrown at any point during the creation we will catch it here and display it to the user
		MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	try {
		glApp.Run();						// Enter the message pump, this will loop until we indicate that we want to exit 
	} catch ( std::exception err ) {		// If anything goes wrong while running, catch the error and display it to the user, ending the program
		MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	
	return 1;		// And we're done!
}
