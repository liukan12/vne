//
// Tutorial 05 - Texture Mapping (OpenGL 2.1)
// Brandon Jones (Toji)
// Last Updated 10/09/2007 
// Version 1.0
//

#include "GLWindow.h"
#include "TGA.h"
#include <fstream>

//*** NEW CODE ***
// Included so we can use stringstream
#include <sstream>
// Included so we can use gluBuild2DMipmaps 
#include <gl/glu.h>
//****************

using namespace OpenGL21;

class GLApp : public GLWindow {

	// Vertex Buffer Objects
	PFNGLGENBUFFERSARBPROC		glGenBuffersARB;
	PFNGLBINDBUFFERARBPROC		glBindBufferARB;
	PFNGLBUFFERDATAARBPROC		glBufferDataARB;
	PFNGLDELETEBUFFERSARBPROC	glDeleteBuffersARB;
	
	// GLSL Shaders
	PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
	PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
	PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
	PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
	PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
	PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
	PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;
	PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
	PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
	PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;

	//*** NEW CODE ***
	// Shader Constant Functions
	PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB;
	PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
	PFNGLUNIFORM1IARBPROC				glUniform1iARB;
	//****************
	
	//*** NEW CODE ***
	// Replaced color buffer handle with one for texture coords
	GLuint vertBuffer, texCoordBuffer, indexBuffer;
	//****************
	GLhandleARB shaderProgram;
	
	//*** NEW CODE ***
	// New handle for the texture, notice a pattern in the variable type?
	GLuint texture;
	//****************

	int xPrev, yPrev;
	float xRot, yRot, zoom;

	public:
		GLApp() {
			vertBuffer		= NULL;
			texCoordBuffer	= NULL;
			indexBuffer		= NULL;
			shaderProgram	= NULL;
			texture		= NULL;

			xPrev	= -1;
			yPrev	= -1;
			xRot	= 0.0f;
			yRot	= 0.0f; 
			zoom	= -3.0f;

			glGenBuffersARB		= NULL;
			glBindBufferARB		= NULL;
			glBufferDataARB		= NULL;
			glDeleteBuffersARB	= NULL;

			glCreateProgramObjectARB  = NULL;
			glDeleteObjectARB         = NULL;
			glUseProgramObjectARB     = NULL;
			glCreateShaderObjectARB   = NULL;
			glShaderSourceARB         = NULL;
			glCompileShaderARB        = NULL;
			glGetObjectParameterivARB = NULL;
			glAttachObjectARB         = NULL;
			glGetInfoLogARB           = NULL;
			glLinkProgramARB          = NULL;

			glGetAttribLocationARB		= NULL;
			glGetUniformLocationARB		= NULL;
			glUniform1iARB				= NULL;
		}

	protected:
		virtual void OnStartup( void ) {
			BindExtensions();
			CreateBuffers();
			CreateShaders();
			//*** NEW CODE ***
			// Just like CreateBuffers, this will handle texture creation for this app
			CreateTextures(); 
			//****************
			
			//*** NEW CODE ***
			// Make sure our window has an appropriate title to start with
			UpdateWindowTitle();
			//****************
		}

		void BindExtensions( void ) {
			// Vertex Buffer Objects
			if( !ExtensionAvailable("GL_ARB_vertex_buffer_object") ) {
				throw std::exception("This tutorial requires Vertex Buffer Objects, which are not supported by the current driver");
			}

			glGenBuffersARB		= (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");			
			glBindBufferARB		= (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
			glBufferDataARB		= (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
			glDeleteBuffersARB	= (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
			
			// GLSL Shaders
			if( !ExtensionAvailable("GL_ARB_shading_language_100") ) {
				throw std::exception("This tutorial requires GLSL Shaders, which are not supported by the current driver");
			}
			
			glCreateProgramObjectARB	= (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
			glDeleteObjectARB			= (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
			glUseProgramObjectARB		= (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
			glCreateShaderObjectARB		= (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
			glShaderSourceARB			= (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
			glCompileShaderARB			= (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
			glGetObjectParameterivARB	= (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
			glAttachObjectARB			= (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
			glGetInfoLogARB				= (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
			glLinkProgramARB			= (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");

			//*** NEW CODE ***
			// All of the following functions are oriented towards passing constants into shaders from our C++ code 
			glGetAttribLocationARB		= (PFNGLGETATTRIBLOCATIONARBPROC) wglGetProcAddress("glGetAttribLocationARB");
			glGetUniformLocationARB		= (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
			glUniform1iARB				= (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
			//****************
		}

		void CreateBuffers( void ) {
			//*** NEW CODE ***
			// The code here has been updated to define a textured cube, which will likely
			// be the standard geometric form for most future tutorials
			float verts[] = {
				1.0f,  1.0f, 1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  -1.0f, 1.0f,
				1.0f,  -1.0f, -1.0f,
				-1.0f,  1.0f, 1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  -1.0f, 1.0f,
				-1.0f,  -1.0f, -1.0f,

				1.0f,  1.0f, 1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  -1.0f, 1.0f,
				1.0f,  -1.0f, -1.0f,
			};
		
			// Of course we have to set up our texture coords as well
			float texCoord[] = {
				1.f, 1.f,
				1.f, 0.f,
				0.f, 1.f,
				0.f, 0.f,
				
				0.f, 1.f,
				0.f, 0.f,
				1.f, 1.f,
				1.f, 0.f,

				0.f, 0.f,
				0.f, 1.f,
				1.f, 0.f,
				1.f, 1.f,
			};

			int index[] = {
				2, 1, 0,
				1, 2, 3,
				
				4, 5, 6,
				7, 6, 5,
				
				0, 1, 4,
				5, 4, 1,

				6, 3, 2,
				3, 6, 7,

				4, 6, 8,
				10, 8, 6,

				9, 7, 5,
				7, 9, 11,
			};

			glGenBuffersARB( 1, &vertBuffer );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertBuffer );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(verts), verts, GL_STATIC_DRAW_ARB );
			
			glGenBuffersARB( 1, &texCoordBuffer );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, texCoordBuffer );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(texCoord), texCoord, GL_STATIC_DRAW_ARB );

			glGenBuffersARB( 1, &indexBuffer );
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer );
			glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(index), index, GL_STATIC_DRAW_ARB );
			//****************
		}

		void CreateShaders( void ) {
			GLhandleARB vertShader, fragShader;

			ShaderFromFile("Vertex.glsl", GL_VERTEX_SHADER_ARB, vertShader);
			ShaderFromFile("Fragment.glsl", GL_FRAGMENT_SHADER_ARB, fragShader);
			LinkShaders(vertShader, fragShader, shaderProgram );

			glDeleteObjectARB( vertShader );
			glDeleteObjectARB( fragShader );
		}
		
		//*** NEW CODE ***
		// Create any textures we need for this scene
		void CreateTextures( void ) {
			TextureFromTGA("Media/Crate.tga", texture);
		}
		//****************

		virtual void OnShutdown( void ) { 
			if(glDeleteBuffersARB != NULL) {
				glDeleteBuffersARB( 1, &vertBuffer );
				glDeleteBuffersARB( 1, &texCoordBuffer );
				glDeleteBuffersARB( 1, &indexBuffer );
			}

			if(glDeleteObjectARB != NULL) {
				glDeleteObjectARB( shaderProgram );
			}
			
			//*** NEW CODE ***
			// Delete textures
			glDeleteTextures( 1, &texture );
			//****************
		}

		virtual void OnFrame( void ) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 0.0f, zoom);
			glRotatef(yRot, 1.0f, 0.0f, 0.0f);
			glRotatef(xRot, 0.0f, 1.0f, 0.0f);

			// Shader Program
			glUseProgramObjectARB( shaderProgram );

			// Vertex Buffer
			glEnableClientState( GL_VERTEX_ARRAY );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertBuffer );
			glVertexPointer( 3, GL_FLOAT, 0, (char*)NULL );
			
			// TexCoord Buffer
			//*** NEW CODE ***
			// Changed GL_COLOR_ARRAY to GL_TEXTURE_COORD_ARRAY and ColorPointer to TexCoordPointer
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, texCoordBuffer );
			glTexCoordPointer( 2, GL_FLOAT, 0, (char*)NULL );
			//*****************
			
			// Index Buffer
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer );
			glIndexPointer( GL_INT, 0, (char*)NULL );

			//*** NEW CODE ***
			glBindTexture( GL_TEXTURE_2D, texture );	// Set the texture that we want to use
			// We need to let our shader know which texture unit to associate with which name
			int texLoc = glGetUniformLocationARB( shaderProgram, "tex0" );	// Get the location (name) in the shader to bind this texture to 
			glUniform1iARB( texLoc, 0 );				// Tell our shader to use texture 0 with the given name 
			//****************
			
			// Draw
			glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);	// Once again, make sure we have the correct vert count

			glDisableClientState( GL_VERTEX_ARRAY );

			//*** NEW CODE ***
			// Changed GL_COLOR_ARRAY to GL_TEXTURE_COORD_ARRAY
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			//****************

			glFlush();
			SwapBuffers();
		}

		virtual LRESULT CALLBACK OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch(uMsg) {
				case WM_KEYDOWN:
					if(wParam == VK_ESCAPE) {
						Exit();
					}
					if(wParam == '1') {
						glBindTexture( GL_TEXTURE_2D, texture );
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); // Linear filtering
						UpdateWindowTitle();
					}
					if(wParam == '2') {
						glBindTexture( GL_TEXTURE_2D, texture );
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST ); // Bilinear filtering
						UpdateWindowTitle();
					}
					if(wParam == '3') {
						glBindTexture( GL_TEXTURE_2D, texture );
						glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); // Triliniear filtering
						UpdateWindowTitle();
					}
					break;

				case WM_MOUSEMOVE: {
						int xCoord = LOWORD(lParam);
						int yCoord = HIWORD(lParam);

						if(xPrev == -1) { xPrev = xCoord; }
						if(yPrev == -1) { yPrev = yCoord; }

						float xDelta = (float)(xCoord - xPrev);
						float yDelta = (float)(yCoord - yPrev);

						xPrev = xCoord;
						yPrev = yCoord;

						if(wParam & MK_LBUTTON) {
							xRot += xDelta / 5.0f;
							yRot += yDelta / 5.0f;
						} else if(wParam & MK_RBUTTON) {
							zoom += yDelta / 100.0f;
						}
					}
					break;
			}

			return GLWindow::OnMessage(hWnd, uMsg, wParam, lParam);
		}

	private:
		//
		// Utility Code
		//

		void ShaderFromFile( const char* shaderPath, GLenum shaderType, GLhandleARB& handle ) {
			char* shaderSrc = NULL;
			int len = 0;
			int errorFlag = -1;

			if(!FileToString(shaderPath, shaderSrc, len)) {
				std::string err = "Could not read shader:\n\n" + std::string(shaderPath);
				MessageBox(NULL, err.c_str(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
				return;
			}
			
			handle = glCreateShaderObjectARB( shaderType );

			glShaderSourceARB( handle, 1, (const char**)&shaderSrc, &len );
			glCompileShaderARB( handle ); 
			delete[] shaderSrc;

			glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &errorFlag);
			if(!errorFlag) {
				std::string err = "Shader Compile Error:\n\n" + std::string(shaderPath) + "\n\n" + GetShaderInfoLog(handle);
				MessageBox(NULL, err.c_str(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}

		void LinkShaders( GLhandleARB& vertex, GLhandleARB& fragment, GLhandleARB& handle ) {
			int errorFlag = -1;
			
			handle = glCreateProgramObjectARB();
			glAttachObjectARB( handle, vertex );
			glAttachObjectARB( handle, fragment );
			glLinkProgramARB( handle );

			glGetObjectParameterivARB(handle, GL_OBJECT_LINK_STATUS_ARB, &errorFlag);
			if(!errorFlag) {
				std::string err = "Shader Program Link Error:\n\n" + GetShaderInfoLog(handle);
				MessageBox(NULL, err.c_str(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
				return;
			}
		}

		std::string GetShaderInfoLog(GLhandleARB& obj) {
			int logLength = 0;
			int charsWritten  = 0;
			char *tmpLog;
			std::string log;
		
			glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength);
		
			if (logLength > 0) {
				tmpLog = new char[logLength];
				glGetInfoLogARB(obj, logLength, &charsWritten, tmpLog);
				log = tmpLog;
				delete[] tmpLog;
			}

			return log;
		}

		bool FileToString(const char* path, char*& out, int& len) {
			std::ifstream file(path, std::ios::ate | std::ios::binary);
			if(!file.is_open()) {
				return false;
			}
			len = file.tellg();
			out = new char[ len+1 ];
			file.seekg (0, std::ios::beg);
			file.read(out, len);
			file.close();
			out[len] = 0;
			return true;
		}

		//*** NEW CODE ***
		// This function will create a texture from the specified TGA image
		void TextureFromTGA( const char* imagePath, GLuint& handle ) {
			TGAImage imageData;
			try {
				imageData.CreateFromFile(imagePath);	// Use the TGAImage class to wrap away the TGA loading
			} catch ( std::exception err ) {
				MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			glGenTextures( 1, &handle );				// Create a handle to store texture data
			glBindTexture( GL_TEXTURE_2D, handle );	// Set the new texture as the current one
			// The next two functions set the filtering mode. Here we're using linear filtering
			// bi-linear or tri-linear filter would be set here, but that's beyond the scope of this tutorial
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );		// Set the min filtering mode for this texture (Trilinear)
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );		// Set the max filtering mode for this texture

			// Send the texture color data to the currently bound texture
			// This function automatically generates mipmaps of the image for us
			// So that we can use Biliniear and Triliniear filtering
			gluBuild2DMipmaps( GL_TEXTURE_2D, imageData.GetComponents(), 
							imageData.GetWidth(), imageData.GetHeight(), 
							imageData.GetFormat(), imageData.GetType(), 
							imageData.GetImageData() );
		}
		//****************
		
		//*** NEW CODE ***
		// This function will update the title of our window to reflect whatever settings we want to convey
		// It will appear off and on throughout future tutorials
		void UpdateWindowTitle() {
			std::stringstream title;
			GLint filterMode;

			glBindTexture( GL_TEXTURE_2D, texture );
			glGetTexParameteriv( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &filterMode );

			title << m_displaySettings.caption << ": ";

			switch(filterMode) {
				case GL_LINEAR:
					title << "Linear";
					break;
				case GL_LINEAR_MIPMAP_NEAREST:
					title << "Bilinear";
					break;
				case GL_LINEAR_MIPMAP_LINEAR:
					title << "Trilinear";
					break;
			}

			title << " Filtering";

			SetWindowText(GetHWND(), title.str().c_str());
		}
		//****************
};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowDisplaySettings wds;
	wds.caption = "Tutorial 05 - Texture Mapping";
	
	if (MessageBox(NULL, "Would You Like To Run In Fullscreen Mode?", wds.caption.c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES) { 
		wds.fullscreen = true; 
	}

	GLApp glApp;

	try {
		glApp.Create( hInstance, wds );
	} catch( std::exception err ) {
		MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	
	try {
		glApp.Run();
	} catch ( std::exception err ) {
		MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	
	return 1;
}
