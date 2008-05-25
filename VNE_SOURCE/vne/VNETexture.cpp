#include "VNETexture.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iomanip>

#include "SOIL.h"

using namespace std;



VNETexture::VNETexture(string fileName)
{
	tex_2d = SOIL_load_OGL_texture
	(	
		fileName.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}
void VNETexture::bindTexture()
{
		if( tex_2d > 0 )
		{
			//	enable texturing
			glEnable( GL_TEXTURE_2D );
			//glEnable( 0x84F5 );// enables texture rectangle
			//  bind an OpenGL texture ID
			glBindTexture(GL_TEXTURE_2D, tex_2d );
			//	report
			//std::cout << "the loaded texture ID was " << tex_2d << std::endl;
			//std::cout << "the load time was " << 0.001f * time_me << " seconds (warning: low resolution timer)" << std::endl;
		}
		else
		{
			//	loading of the texture failed...why?
			glDisable( GL_TEXTURE_2D );
			std::cout << "Texture loading failed: '" << SOIL_last_result() << "'" << std::endl;
		}
}
