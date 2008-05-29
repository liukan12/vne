//
// Tutorial 06 - Multitexture (OpenGL 2.1)
// Brandon Jones (Toji)
// Last Updated 10/09/2007 
// Version 1.0
//

#include "GLWindow.h"
#include "TGA.h"
#include <fstream>
#include <gl/glu.h>

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

	// Shader Constant Functions
	PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocationARB;
	PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
	PFNGLUNIFORM1IARBPROC				glUniform1iARB;

	//*** NEW CODE ***
	// Multitexture Extensions
	PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB;
	//****************

	GLuint vertBuffer, texCoordBuffer, indexBuffer;
	GLhandleARB shaderProgram;
	GLuint texture, texture2;

	int xPrev, yPrev;
	float xRot, yRot, zoom;

	public:
		GLApp() {
			vertBuffer		= NULL;
			texCoordBuffer	= NULL;
			indexBuffer		= NULL;
			shaderProgram	= NULL;
			texture			= NULL;
			texture2		= NULL;

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

			glActiveTextureARB			= NULL;
			glClientActiveTextureARB	= NULL;
		}

		virtual void OnStartup( void ) {
			BindExtensions();
			CreateBuffers();
			CreateShaders(); 
			CreateTextures(); 
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

			glGetAttribLocationARB		= (PFNGLGETATTRIBLOCATIONARBPROC) wglGetProcAddress("glGetAttribLocationARB");
			glGetUniformLocationARB		= (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
			glUniform1iARB				= (PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");

			//*** NEW CODE ***
			// These two extensions let us switch which texture level is the active one when
			// binding textures and setting texture coordinates
			if( !ExtensionAvailable("GL_ARB_multitexture") ) {
				throw std::exception("This tutorial requires MultiTexture, which is not supported by the current driver");
			}
			
			glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
			glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");
			//****************
		}

		void CreateBuffers( void ) {
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
		}

		void CreateShaders( void ) {
			GLhandleARB vertShader, fragShader;

			ShaderFromFile("Vertex.glsl", GL_VERTEX_SHADER_ARB, vertShader);
			ShaderFromFile("Fragment.glsl", GL_FRAGMENT_SHADER_ARB, fragShader);
			LinkShaders(vertShader, fragShader, shaderProgram );

			glDeleteObjectARB( vertShader );
			glDeleteObjectARB( fragShader );
		}

		void CreateTextures( void ) {
			TextureFromTGA("Media/Crate.tga", texture);
			//*** NEW CODE ***
			// Load a second texture
			TextureFromTGA("Media/Test.tga", texture2);
			//****************
		}
	
		virtual void OnShutdown( void ) { 
			if(glDeleteBuffersARB != NULL) {
				glDeleteBuffersARB( 1, &vertBuffer );
				glDeleteBuffersARB( 1, &texCoordBuffer );
				glDeleteBuffersARB( 1, &indexBuffer );
			}

			if(glDeleteObjectARB != NULL) {
				glDeleteObjectARB( shaderProgram );
			}

			glDeleteTextures( 1, &texture );
			glDeleteTextures( 1, &texture2 );
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

			// TexCoord Buffer (Bound to TexCoord 0)
			//*** NEW CODE ***
			// This indicates which texture level we are operating on when binding texCoord buffers.
			// It operates identically to glActiveTextureARB, which is demonstrated below.
			// While we are only using a single buffer level, and therefore it's not strictly 
			// nessicary, it's good to see it in action
			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			//****************
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, texCoordBuffer );
			glTexCoordPointer( 2, GL_FLOAT, 0, (char*)NULL );

			// Index Buffer
			glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, indexBuffer );
			glIndexPointer( GL_INT, 0, (char*)NULL );
		
			// Texture 0
			//*** NEW CODE ***
			// Because we are dealing with multiple textures now we need to specify which level 
			// we're binding to for each texture we bind
			glActiveTextureARB(GL_TEXTURE0_ARB);
			//****************
			glBindTexture( GL_TEXTURE_2D, texture );
			glUniform1iARB( glGetUniformLocationARB(shaderProgram, "tex0"), 0 ); // We've condensed the syntax for this call here
			
			// Texture 1
			//*** NEW CODE ***
			// The code for binding the second texture is practically identical to the code for the
			// first texture, but (obviously) using GL_TEXTURE1_ARB and level 1 for the shader sampler
			glActiveTextureARB(GL_TEXTURE1_ARB);
			glBindTexture( GL_TEXTURE_2D, texture2 );
			glUniform1iARB( glGetUniformLocationARB(shaderProgram, "tex1"), 1 );
			//****************
			
			// Draw
			glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

			glDisableClientState( GL_VERTEX_ARRAY );

			//*** NEW CODE ***
			// Just like enabling, we need to disable each level we enabled independantly
			glClientActiveTextureARB(GL_TEXTURE0_ARB);
			//****************
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );

			glFlush();
			SwapBuffers();
		}

		virtual LRESULT CALLBACK OnMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch(uMsg) {
				case WM_KEYDOWN:
					if(wParam == VK_ESCAPE) {
						Exit();
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

		void TextureFromTGA( const char* imagePath, GLuint& handle ) {
			TGAImage imageData;
			try {
				imageData.CreateFromFile(imagePath);
			} catch ( std::exception err ) {
				MessageBox(NULL, err.what(), "OpenGL Tutorial - Error", MB_OK | MB_ICONEXCLAMATION);
				return;
			}

			glGenTextures( 1, &handle );
			glBindTexture( GL_TEXTURE_2D, handle );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

			gluBuild2DMipmaps( GL_TEXTURE_2D, imageData.GetComponents(), 
							imageData.GetWidth(), imageData.GetHeight(), 
							imageData.GetFormat(), imageData.GetType(), 
							imageData.GetImageData() );
		}
};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WindowDisplaySettings wds;
	wds.caption = "Tutorial 06 - Multitexture";
	
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
