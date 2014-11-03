#include "SDLOGL.h"
#include <SDL_mixer.h>

using namespace std;
int nextp2(int n)
{
	int rval=2;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2;
	while(rval<n) {rval = rval* 2;}
	return rval;
}
int init(int width, int height)
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
    	printf("Unable to initialize SDL: %s\n", SDL_GetError());
    	return 1;
	}

	atexit(SDL_Quit);

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ); // *new*

    SDL_Surface *screen = SDL_SetVideoMode( width, height, 16, SDL_OPENGL); // *changed*

	glClearColor( 0, 0, 0, 0 );
	 glShadeModel(GL_SMOOTH);
	  glEnable ( GL_COLOR_MATERIAL );
	   glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	glEnable( GL_TEXTURE_2D ); // Need this to display a texture
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport( 0, 0, width,height );
    //glViewport(0,640,width, height);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho( 0, width, height, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();


//Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) { return false; }

}

void draw_textureInt(float x, float y, float width, float height, int tx, int ty, int iwidth, int iheight)
{
     float tilex = (float)(tx) / (float)(iwidth);
     float tiley = (float)(ty) / (float)(iheight);
     float fx = tilex + width /iwidth;
     float fy = tiley + height / iheight;
     //cout <<"tilex = "<<tilex<< "; tiley = "<<tiley<<endl;
     //cout <<"fx = "<<fx<< "; fy = "<<fy<<endl;

    glBegin( GL_QUADS );
        // Top-left vertex (corner)
        glTexCoord2f( tilex, tiley );
        glVertex3i( x, y, 0 );
        //glVertex3f( x, y - height, 0 );
        // Top-Right vertex (corner)
        glTexCoord2f( fx,tiley );
        glVertex3i( x+width, y, 0 );
        //glVertex3f( x+width, y - height, 0 );

        // Bottom-right vertex (corner)
        glTexCoord2f( fx, fy );
        glVertex3i( x + width, y + height, 0 );
        //glVertex3f( x + width, y, 0 );

        // Bottom-Left vertex (corner)
        glTexCoord2f( tilex, fy );
        glVertex3i( x, y + height, 0 );
        //glVertex3f( x, y + height, 0 );
  glEnd();
}

void draw_texture(float x, float y, float width, float height, int tx, int ty, int iwidth, int iheight)
{
    //iwidth and iheight are used as the total size of the image, used for calculating percentage of x, and y
    // to use with gltexcoord2f

     //CONVERT tx and ty
     //tx, ty in terms of rows and column
     //tilex, tiley = pixel coordinates percentage in relation with image width and height
     float tilex = tx * width / iwidth;
     float tiley = ty * height / iheight;
     float fx = tilex + width /iwidth;
     float fy = tiley + height / iheight;
     //cout <<"tilex = "<<tilex<< "; tiley = "<<tiley<<endl;
     //cout <<"fx = "<<fx<< "; fy = "<<fy<<endl;

    glBegin( GL_QUADS );
        // Top-left vertex (corner)
        glTexCoord2f( tilex, tiley );
        glVertex3f( x, y, 0 );
        //glVertex3f( x, y - height, 0 );
        // Top-Right vertex (corner)
        glTexCoord2f( fx,tiley );
        glVertex3f( x+width, y, 0 );
        //glVertex3f( x+width, y - height, 0 );

        // Bottom-right vertex (corner)
        glTexCoord2f( fx, fy );
        glVertex3f( x + width, y + height, 0 );
        //glVertex3f( x + width, y, 0 );

        // Bottom-Left vertex (corner)
        glTexCoord2f( tilex, fy );
        glVertex3f( x, y + height, 0 );
        //glVertex3f( x, y + height, 0 );
  glEnd();

}

SDL_Surface *loadimage(std::string filename)
{
            SDL_Surface* image = NULL;
            SDL_Surface* fimage = NULL;
            image = IMG_Load(filename.c_str());
            if (image != NULL)
            {
                            fimage = SDL_DisplayFormatAlpha(image);
                            SDL_FreeSurface(image);
            }
            else printf("Failed ot load image, did you forget to include the dlls?\n");

        return fimage;
}

    void prep_texture(SDL_Surface *surface, GLuint &texture)
    {
         GLint nOfColors;
         GLenum texture_format;
         if (!surface)
         {cout <<"Image reading failed"<<endl;}
            if ( (surface->w & (surface->w - 1)) != 0 ) {
            printf("warning: image.bmp's width is not a power of 2\n");
           }
        // Also check if the height is a power of 2
        if ( (surface->h & (surface->h - 1)) != 0 ) {
            printf("warning: image.bmp's height is not a power of 2\n");
            }

            // get the number of channels in the SDL surface
            nOfColors = surface->format->BytesPerPixel;
            if (nOfColors == 4)     // contains an alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGBA;
                    else
                            texture_format = GL_BGRA;
            } else if (nOfColors == 3)     // no alpha channel
            {
                    if (surface->format->Rmask == 0x000000ff)
                            texture_format = GL_RGB;
                    else
                            texture_format = GL_BGR;
            } else {
                    printf("warning: the image is not truecolor..  this will probably break\n");
                    cout <<"Its number of colors is "<<nOfColors<<endl;
                    // this error should not go unhandled
                    texture_format = GL_RGBA;
            }

        // Have OpenGL generate a texture object handle for us
        glGenTextures( 1, &texture );

        // Bind the texture object
        glBindTexture( GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        int w= nextp2(surface->w);
        int h =  nextp2(surface->h);
        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, w, h, 0,
                          texture_format, GL_UNSIGNED_BYTE, surface->pixels );



    // Free the SDL_Surface only if it was successfully created
        if ( surface )
        {
            //SDL_FreeSurface( surface );
        }

     }
