//
// OpenGL Window Class (OpenGL 2.1)
// Brandon Jones (Toji)
// Last Updated 10/29/2007 
// Version 1.2
//

#include "GLWindow.h"
#include <math.h>

namespace OpenGL21 {

	const char* GLWindowClass = "GL21_WINDOW_CLASS"; 

	//******************
	// GLWindow
	//******************
	
	GLWindow::GLWindow() :
	m_hWnd(NULL), m_exitFlag(false), m_hDC(NULL), m_hRC(NULL) {

	}

	GLWindow::~GLWindow() {
		ShutdownGL();		// Make sure OpenGL is shut down before we close the window
		ShutdownWin();		// Close the window
		RevertDisplay();	// Exit Fullscreen if nessicary
	}

	//
	//	Public Interface and Virtual functions
	//

	void GLWindow::Create(HINSTANCE hInstance, WindowDisplaySettings& displaySettings) {
		// Store certain attributes locally for later use and ensure that they're not empty
		m_hInstance = hInstance;
		if(m_hInstance == NULL) {
			throw std::exception("Cannot create window with NULL hInstance.");
		}
		
		m_displaySettings = displaySettings;

		SetDisplay(m_displaySettings);	// Set the display into the correct mode (color depth/size/fullscreen)
		CreateWin();					// Initialize the Win32 portion of the window
		CreateGL();						// Initialize the OpenGL portion of the window
	}

	void GLWindow::Run( void ) {
		
		// Setup some basic OpenGL State
		SetProjection(m_displaySettings.width, m_displaySettings.height, m_displaySettings.fieldOfView);  // Initialize our projection settings

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Clear the Screen to Black
		glClearDepth(1.0f);						// Clear the Depth Buffer completely
		glDepthFunc(GL_LEQUAL);					// The method used to determine if one pixel is behind another
		glEnable(GL_DEPTH_TEST);				// Enable Z-Buffering (Depth Testing)
		glShadeModel(GL_SMOOTH);				// When shading, interpolate values between vertices

		OnStartup(); // Run the app-defined startup code. This takes place before anything is shown

		// Show the window, bring it to the front, and give it focus
		ShowWindow(m_hWnd, SW_SHOW); 
		SetForegroundWindow(m_hWnd);
		SetFocus(m_hWnd);

		// This is the actual message pump. It will loop till the user calls Exit() or the window is closed
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );
		while(msg.message != WM_QUIT) {
			// Grab the nest message in the queue
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
				// Send the message to our GlobalMsgProc()
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			if(!m_exitFlag) {
				OnFrame(); // Call the app-defined OnFrame() routine, which is where all drawing and logic should be done
			}
		}

		OnShutdown(); // Once we've exited the loop, call the app-defined shutdown code
	}

	void GLWindow::Exit( void ) {
		PostQuitMessage(0); // Post a quit message to our message pump, which will generate a WM_QUIT message
		m_exitFlag = true;
	}

	//
	// OpenGL Specific Functions
	//

	void GLWindow::SetProjection( int width, int height, float fieldOfView ) {
		if(height == 0) { height = 1; }		// Must have a height of at least 1 to prevent divide by 0 errors
		
		glViewport (0, 0, width, height);	// Tell OpenGL the size of our window 
		
		glMatrixMode(GL_PROJECTION);		// Notify OpenGL that we are modifying the Projection Matrix																
		glLoadIdentity();					// Set the Projection matrix to the Identiy matrix
		
		// The following code is a fancy bit of math that is eqivilant to calling:
		// gluPerspective( fieldOfView/2.0f, width/height , 0.1f, 255.0f )
		// We do it this way simply to avoid requiring glu.h
		GLfloat zNear = 0.1f;
		GLfloat zFar = 255.0f;
		GLfloat aspect = float(width)/float(height);
		GLfloat fH = tan( float(fieldOfView / 360.0f * 3.14159f) ) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );
							
		glMatrixMode(GL_MODELVIEW);			// Switch the matrix back to ModelView mode 																	
		glLoadIdentity();					// Set the ModelView matrix to the Identity matrix
	}

	void GLWindow::SwapBuffers( void ) {
		::SwapBuffers(m_hDC);				// Swaps the contents of the backbuffer to the front, displaying any drawing we have done
	}

	bool GLWindow::ExtensionAvailable( const char *extString ) {
		char *extList = (char*) glGetString(GL_EXTENSIONS);		// Get the string containing a space-delimited list of all the supported extensions for the current hardware
		size_t extLen = strlen(extString);						// Get the size of our requested extension string for fast comparisons
		size_t nextExtLen = 0;									// Used to store the length of each extension in the list as we process it.

		if(!extString || !extList) { return false; }			// If we don't have either a extension list or requested extension, return false 
		
		while(*extList) {										// Loop through each extention in the list till we find one that matches the requested extension
			nextExtLen = strcspn( extList, " " );				// Get the number of charecters up to the next space (which serves as a delimiter in the extension list)
			if(nextExtLen == extLen && strncmp( extList, extString, extLen ) == 0) {	// We first mach the string length, then do a char-by-char comparison
				return true;									// If the extension matches the requested one, return true
			}
			extList += nextExtLen + 1;							// If this extension did not match, move forward to the next extension string
		}

		return false;											// If we reach here we haven't found the extension in our list, so return false
	}

	void GLWindow::CreateGL() {
		if(m_hWnd == NULL) { throw std::exception("Window handle cannot be NULL"); }	// If we have no window, we cannot continue

		// If any of the following steps fail, we shutdown any OpenGL elements that have been started and exit
		if (!(m_hDC=GetDC(m_hWnd))) {						// Retrieve the Device Context for this window
			ShutdownGL();
			throw std::exception("Could not get window Device Context");
		}

		//Create the PixelFormatDescriptor, which describes to OpenGL the pixel properties such as depth, color, and alpha channels
		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory( &pfd, sizeof(PIXELFORMATDESCRIPTOR) );
		pfd.nVersion = 1;																// The PFD version, always 1
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;		// The properties of the PFD, in this case OpenGL support and double buffering
		pfd.iPixelType = PFD_TYPE_RGBA;													// The type of Pixels we're dealing with
		pfd.cColorBits = m_displaySettings.colorBits;									// The color depth of the window
		pfd.cAlphaBits = m_displaySettings.alphaBits;									// The alpha depth of the window
		pfd.cDepthBits = m_displaySettings.depthBits;									// The number of bits to use for the depth buffer
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);										// The size of this structure
		
		
		int pixelFormat;
		if (!(pixelFormat = ChoosePixelFormat(m_hDC,&pfd))) {
			ShutdownGL();
			throw std::exception("Could not find a suitable Pixel Format");
		}

		if(!SetPixelFormat(m_hDC,pixelFormat,&pfd)) {			// Set the format of the Device Context
			ShutdownGL();
			throw std::exception("Could not set the Pixel Format");
		}
		
		if (!(m_hRC=wglCreateContext(m_hDC))) {				// Create the OpenGL Render Context
			ShutdownGL();
			throw std::exception("Could not create OpenGL Render Context");
		}
		
		if(!wglMakeCurrent(m_hDC, m_hRC)) {					// Bind the render context for drawing
			ShutdownGL();
			throw std::exception("Could not make Render Context current");
		}
	}

	void GLWindow::ShutdownGL( void ) {
		if (m_hDC != NULL) {			
			wglMakeCurrent(m_hDC, NULL);	// If we have a valid device context, unbind any render contexts from it
			if (m_hRC != NULL) {
				wglDeleteContext(m_hRC);	// Delete the render context if we have it
				m_hRC = NULL;
			}
			ReleaseDC(m_hWnd, m_hDC);		// Release the device context
			m_hDC = NULL;
		}
	}
	
	//
	// Win32 Specific Functions
	//

	HWND GLWindow::GetHWND() {
		return m_hWnd;
	}

	GLWindow::operator HWND() {
		return m_hWnd;
	}

	void GLWindow::CreateWin() {
		// Here we will describe the windows class that we're going to be using to create our main window
		WNDCLASSEX wc;
		if(!GetClassInfoEx(m_hInstance, GLWindowClass, &wc)) {			// See if the class is already registered
			memset(&wc,0,sizeof(WNDCLASSEX));
			wc.lpszClassName	= GLWindowClass;						// The windows class name
			wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// The styles for this window
			wc.lpfnWndProc		= (WNDPROC) GLWindow::GlobalMsgProc;		// The message handler for this windows class (which we will redirect later)
			wc.hInstance		= m_hInstance;							// The hInstance to create this class under
			wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// The Mouse Cursor to use with windows of this class
			wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// The Icon to display for windows of this class
			wc.hIconSm			= NULL;									// The Small Icon to use for windows of this class (For taskbar icons, etc) 
			wc.cbSize			= sizeof(WNDCLASSEX);					// The size of this structure
			
			// Attempt to register this class so we create windows with it
			if (!RegisterClassEx(&wc)) {
				throw std::exception("Window Class could not be registered.");
			}
		}
		
		// Next we need to tell the window what window style to use
		// This is as simple as "when windowed we want a border, when fullscreen we don't"
		DWORD dwExStyle;
		DWORD dwStyle;
		
		if ( m_displaySettings.fullscreen ) { 
			dwExStyle = WS_EX_APPWINDOW;	// This style simply ensures that our program will appear on the taskbar
			dwStyle	= WS_POPUP;				// This style indicates that the window should have no border whatsoever 
		} else {
			dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// These two styles combined ensure the program will appear on the taskbar and that it has a nice border
			dwStyle		= WS_OVERLAPPEDWINDOW; // This indicates a combination of styles, which gives our window a title bar, caption, system menu, and minimize/maximize buttons
		}
		
		RECT WinRect;
		WinRect.left=(long)0;
		WinRect.top=(long)0;
		WinRect.right=(long)m_displaySettings.width;
		WinRect.bottom=(long)m_displaySettings.height;
		
		// This function will use the styles we set above and modify WinRect so that it as the appropriate amount of client space.
		AdjustWindowRectEx( &WinRect, dwStyle, false, dwExStyle );
		
		// This is the actual window creation
		m_hWnd = CreateWindowEx(  
					dwExStyle,										// The extended style that we're using, set above	 
					GLWindowClass,									// The name of the class to create this window with. This reffers to the WNDCLASSEX structure above
					m_displaySettings.caption.c_str(),				// The caption for this window
					dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,	// The window style we're using, set above, with two additions that tell windows to properly clip our window
					CW_USEDEFAULT, CW_USEDEFAULT,					// The position the windows top left corner will appear at. In this case we allow windows to decide where best to place us 
					WinRect.right,									// The Width of our window (in pixels)
					WinRect.bottom,									// The Height of our window (in pixles)
					NULL, NULL,										// These two parameters specify our parent window and the menu we want to use, neither of which we have
					m_hInstance,									// The HINSTANCE we're creating the window with
					(void*)this );									// Any user supplied data we need, in this case "this" to allow our OOP framework to pass messages properly
		
		// Check to see if window creation failed
		if (m_hWnd == NULL) {
			throw std::exception("Could not create application window");
		}
	}

	void GLWindow::ShutdownWin() {
		SetWindowLongPtr(m_hWnd, GWL_USERDATA, NULL); // Clear our "this" pointer out of the windows User Data to prevent further call attempts by GlobalMsgProc() 

		if (m_hWnd) { // If we have a valid window handle Destroy and NULL it
			DestroyWindow(m_hWnd);
			m_hWnd=NULL;
		}
	}

	void GLWindow::SetDisplay( WindowDisplaySettings& displaySettings ) {
		if(displaySettings.fullscreen) {												// Check and see if we're in fullscreen mode, and if so signal windows to actually change the display settings
			DEVMODE dmScreenSettings;
			memset(&dmScreenSettings,0,sizeof(DEVMODE));
			dmScreenSettings.dmPelsWidth	= displaySettings.width;					// The width of the screen
			dmScreenSettings.dmPelsHeight	= displaySettings.height;					// The height of the screen 
			dmScreenSettings.dmBitsPerPel	= displaySettings.colorBits;				// How many bits per pixel to use for colors
			dmScreenSettings.dmFields		= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;	// Which fields of the DEVMODE structure we will be paying attention to
			dmScreenSettings.dmSize			= sizeof(DEVMODE);							// The size of this structure
			
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {	// Try to change the display settings to the requested parameters
				// If it didn't work ask the user if they would prefer windowed mode, throw an error if not
				if(MessageBox(NULL, "The requested fullscreen mode is not supported by your video card.\n Would you like to run in windowed mode?", "Run Windowed?", MB_YESNO|MB_ICONEXCLAMATION)==IDYES) {
					displaySettings.fullscreen = false;
				} else {
					throw std::exception("Cannot run fullscreen with the requested settings.");
				}
			}
		}
	}

	void GLWindow::RevertDisplay( void ) {
		ChangeDisplaySettings(NULL, 0);		// If we are in fullscreen mode, switch back to windowed mode 
	}

	//
	// Virtual Functions
	//

	// Default implementations of the app-defined functions that do nothing
	void GLWindow::OnStartup( void ) { }
	void GLWindow::OnFrame( void ) { }
	void GLWindow::OnShutdown( void ) { }
	
	// The default message handler. Any inherited classes should call this in their OnMessage()
	LRESULT CALLBACK GLWindow::OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch( uMsg ) {
			case WM_SIZE:
				SetProjection(LOWORD(lParam), HIWORD(lParam), m_displaySettings.fieldOfView);	// When the window resizes, update the OpenGL viewport with it
				return 0;
			case WM_SYSCOMMAND:
				// The following switch will prevent screensaver and monitor powersave messages from being processed
				// This will prevent the computer from switching to either mode while our application is running
				switch (wParam) {
					case SC_SCREENSAVE:					
					case SC_MONITORPOWER:				
					return 0;							
				}									
				break;
			case WM_DESTROY: // Received when the user clicks the Close button.
				Exit(); // Tell our application to exit
				return 0;
			default:
				break;
		}
		
		// Handle any message which wasn't previously explicitly handled
		return DefWindowProc( hWnd, uMsg, wParam, lParam ); 
	} 

	//
	// Message Handling
	//

	// Receives messages for all windows created with the Window class and forwards them to the correct Window object
	LRESULT CALLBACK GLWindow::GlobalMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {		
		GLWindow* parentWindow = NULL;
		
		// WM_NCCREATE is received immedately after the window is created, before any other messages
		if( uMsg == WM_NCCREATE ) {
			//The lparam in this message contains the user-defined data that was supplied as the last parameter of CreateWindowEX() 
			CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
			// We can extract our "this" pointer from the provided structure now
			parentWindow = reinterpret_cast<GLWindow*>(create->lpCreateParams);
			// Then we set it as a general User-Data element of the window for easy access later
			SetWindowLongPtr(hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(parentWindow));
		} else {
			// For any other message we need to extract the previously set user data and cast it to the correct type
			parentWindow = reinterpret_cast<GLWindow*>(GetWindowLongPtr(hWnd, GWL_USERDATA));
		}
		
		if( parentWindow ) {
			// If we have a valid window now, forward the message to it
			return parentWindow->OnMessage( hWnd, uMsg, wParam, lParam );
		} else {
			//Otherwise handle it with the default behaviour
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}
	}

	//******************
	// WindowDisplaySettings
	//******************

	WindowDisplaySettings::WindowDisplaySettings() : 
		width(800), 
		height(600), 
		fullscreen(false), 
		colorBits(32), 
		alphaBits(8), 
		depthBits(24),
		fieldOfView(90.0f), 
		caption("Untitled Window")
	{ }
	
	WindowDisplaySettings::WindowDisplaySettings( const WindowDisplaySettings& wds ) {
		caption		= wds.caption;
		width		= wds.width;
		height		= wds.height;
		fullscreen	= wds.fullscreen;
		colorBits	= wds.colorBits;
		alphaBits	= wds.alphaBits;
		depthBits	= wds.depthBits;
		fieldOfView	= wds.fieldOfView;
	}
}