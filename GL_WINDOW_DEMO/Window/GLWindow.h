//
// OpenGL Window Class (OpenGL 2.1)
// Brandon Jones (Toji)
// Last Updated 12/02/2007 
// Version 1.2
//
// Version History:
// 1.2 - Reordered class members for clarity. Removed GetDisplaySettings function since it's contents were not necessarily accurate
// 1.1 - Moved ChangeDisplaySettings out of CreateGL and ShutdownGL into its own functions (Set/RevertDisplay) so we don't have to 
//		call it twice when using GLWindowEx 

#if !defined(GL21_WINDOW_H_)
#define GL21_WINDOW_H_

#include <windows.h>
#include <string>
#include <gl/gl.h>
#include <gl/glext.h>

namespace OpenGL21 {
	
	//******************
	// WindowDisplaySettings
	//******************

	/// Contains all the information needed for creating
	/// either a window with or without OpenGL. We pass 
	/// it in to the Window class "Create" function.
	struct WindowDisplaySettings {
										WindowDisplaySettings();	/// Initializes all settings to their defaults
										WindowDisplaySettings( const WindowDisplaySettings& wds );	/// Copy constructor

		// Standard window properties
		std::string						caption;		/// The caption to be displayed in the windows title bar
		int								width;			/// The width of the window in pixels
		int								height;			/// The height of the window in pixels
		bool							fullscreen;		/// Wether or not the window is fullscreen (borderless)
		
		// OpenGL Specific properties
		int								colorBits;		/// The color depth of the OpenGL window
		int								alphaBits;		/// The alpha depth of the OpenGL window
		int								depthBits;		/// The z-buffer depth of the OpenGL window
		float							fieldOfView;	/// The field of view (angle) of the OpenGL viewport
	};

	//******************
	// GLWindow
	//******************

	/// Serves as an abstraction of the Win32 code needed to create
	/// an OpenGL compatible window, as well as encapsulating the message pump
	/// in a object-oriented manner. Win32 and OpenGL code are kept as separate
	/// as possible for the sake of clarity
	class GLWindow {
		public:
										GLWindow( void );
			virtual						~GLWindow( void );

		//
		//	Public Interface and Virtual functions
		//
		public:
			void						Create( HINSTANCE hInstance, WindowDisplaySettings& displaySettings );	/// Create a window with the specified attributes
			void						Run( void );			/// Begin the message loop
			void						Exit( void );			/// Exit the message loop 

		//
		// OpenGL Specific Functions and Variables
		//
		public:
			void						SetProjection( int width, int height, float fieldOfView );				/// Set the projection matrix
			void						SwapBuffers( void );								/// Swap the front and back buffers
			bool						ExtensionAvailable( const char *extString );		/// Check to see if an OpenGL extension is avalible

		protected:
			void						CreateGL( void );		/// Initializes the OpenGL portion of the window
			void						ShutdownGL( void );		/// Clean up any OpenGL bindings

			WindowDisplaySettings		m_displaySettings;		/// Stores the display settings used to create this window
			
			HDC							m_hDC;					/// Handle to the windows Device Context
			HGLRC						m_hRC;					/// OpenGL Render Context for this window

		//
		// Win32 Specific Functions and Variables
		//
		public:		
			HWND						GetHWND( void );			/// Get the window handle
			operator					HWND( void );				/// Allows us to treat this object as a window handle
		
		protected:
			void						CreateWin( void );			/// Create a window with the specified attributes
			void						ShutdownWin( void );		/// Close down the window
			void						SetDisplay( WindowDisplaySettings& displaySettings );	/// Allows us to change the resolution and color depth of the screen if we need to (ie: for fullscreen)
			void						RevertDisplay( void );		/// Reverts the display settings back to their normal mode after changing them with SetDisplay

			HWND						m_hWnd;						/// The internal handle to our window
			HINSTANCE					m_hInstance;				/// The HINSTANCE the window was created and registered with
		
		//
		// Virtual Functions
		//
		protected:
			// The following functions are called by Run() and should not be called explicitly. 
			virtual void				OnStartup( void );		/// Called at the beginning of the Run() loop to allow user initialization code
			virtual void				OnFrame( void );		/// Called once per loop iteration, most program drawing and logic will be done here
			virtual void				OnShutdown( void );		/// Called after Exit() has terminated the loop to allow user shutdown code
			virtual LRESULT CALLBACK	OnMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );		/// Called whenever a message is processed for this window
		
		//
		// Message Handling
		//
		private:
			// Defined as static to allow it's use as a callback when registering the window class
			static LRESULT CALLBACK		GlobalMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	/// This function will forward window messages to the appropriate object's OnMessage()
			bool						m_exitFlag;				/// Indicates when we've called Exit()
	};
}

#endif