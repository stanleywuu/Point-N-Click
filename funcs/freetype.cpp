#include "freetype.h"
#include <SDL/SDL.h>

using namespace std;


int FT2::nextp2(int n)
{
	int rval=2;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2;
	while(rval<n) {rval = rval* 2;}
	return rval;
}

int FT2::init(char* fname, int width, int height)
{
    if (FT_Init_FreeType(&ftlib))
    {
          cout <<"Error during lib initialization"<<endl;
          return 1;
    }
    if (FT_New_Face(ftlib,fname,0,&face))
    {
       cout <<"error during face initialization"<<endl;
       return 2;
    }
        FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    	FT_Set_Pixel_Sizes(face,width, height);

    	adjx = width;
    	adjy = height;
   	return 0;
}

void FT2::loadchar(char ch, int x,int y)
{
    FT_GlyphSlot slot = face->glyph;

  if(FT_Load_Char(face, ch,FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT|FT_LOAD_NO_SCALE|
(true ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO) )   )
{
return;
}
   //Draw function
FT_Glyph glyph;

	if(FT_Get_Glyph( face->glyph, &glyph ))
		cout << "FT_Get_Glyph failed";

	// Convert The Glyph To A Bitmap.
    FT_Render_Glyph( face->glyph,    FT_RENDER_MODE_LCD);
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	// This Reference Will Make Accessing The Bitmap Easier.
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;



	//Padding for bitmapformat
	int h = nextp2(bitmap.rows);
	int w = nextp2(bitmap.width);

	char* pBuf = new char[w * h * 4];
//   /*
	for(int j=0; j <h;j++) for(int i=0; i < w; i++) {
		pBuf[2*(i+j*w)] = 255;
        pBuf[2*(i+j*w)+1] = (i>=bitmap.width || j>=bitmap.rows) ?
			0 : bitmap.buffer[i + bitmap.width*j];
    }
 //*/

         GLuint texture;
        glGenTextures( 1, &texture );
        glBindTexture(GL_TEXTURE_2D,texture);
 /*
    		for(int j=0; j  < h ; j++)
		{
			for(int i=0; i < w; i++)
			{
				unsigned char _vl =  (i>=bitmap.width || j>=bitmap.rows) ? 255 : bitmap.buffer[i + bitmap.width*j];
				pBuf[(4*i + (h - j - 1) * w * 4)  ] = 0xff;
				pBuf[(4*i + (h - j - 1) * w * 4)+1] = 0xff;
				pBuf[(4*i + (h - j - 1) * w * 4)+2] = 0xff;
				pBuf[(4*i + (h - j - 1) * w * 4)+3] = _vl;
			}
		}
*/
    FT_Done_Glyph(glyph);



   glTexImage2D( GL_TEXTURE_2D,0,GL_RGBA,w, h,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,pBuf);//GL_LUMINANCE_ALPHA

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


adjx = face->size->metrics.x_ppem;

int ty = face->size->metrics.y_ppem;
//int ty = metrics.height / 64;

y = y + adjy - ty;
//cout <<" y = "<<y<<endl;

     glLoadIdentity();
    		glBegin ( GL_QUADS );
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y,  1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x+adjx, y,  1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x+adjx, y+adjy ,  1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x , y+adjy ,  1.0f);
		}
		glEnd();

    glDeleteTextures(1, &texture);
delete []pBuf;
}

FT2::~FT2()
{
    FT_Done_FreeType(ftlib);
}


int len(char* txt)
{
  int res = 0;
  while (txt[res]!= '\0')
  {
      res++;
  }
  return res;
}

void drawtext(char* txt, int size, int x,int y)
{

     FT2 text;
        //txt = "The quick brown fox jumps over the lazy dog";
    int w = size * 2 /3 +1;

     int a = text.init("TELETYPE.ttf",w,size);

     for (int i=0;i<len(txt);i++)
     {
        text.loadchar(txt[i], x,y);
        x = x+ text.adjx;
     }



//SDL_GL_SwapBuffers();

//glDeleteTextures(1, &texture);
}

void drawtext(string txt, int size,int tx,int ty)
{

     FT2 text;
        //txt = "The quick brown fox jumps over the lazy dog";
    int w = size * 2 /3 +1;

     int a = text.init("TELETYPE.ttf",w,size);

     for (int i=0;i<txt.length();i++)
     {
        text.loadchar(txt[i], tx,ty);
        tx = tx+ text.adjx;
     }


//SDL_GL_SwapBuffers();

//glDeleteTextures(1, &texture);
}
/*
void drawtext(const char* txt, int size, int x,int y)
{

     FT2 text;
        //txt = "The quick brown fox jumps over the lazy dog";
    int w = size * 2 /3 +1;
    char ttxt[100];
    strcpy(ttxt,txt);

     int a = text.init("TELETYPE.ttf",w,size);

     for (int i=0;i<len(ttxt);i++)
     {
        text.loadchar(ttxt[i], x,y);
        x = x+ text.adjx;
     }



//SDL_GL_SwapBuffers();

//glDeleteTextures(1, &texture);
}
*/
