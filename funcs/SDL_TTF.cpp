#include "SDL_TTF.h"


SDL_TTF::SDL_TTF(int lsize)
{
    size=lsize;
    textColor.r=255;
    textColor.g=0;
    textColor.b=0;
    message = NULL;
}

SDL_TTF::SDL_TTF()
{
    size = 16;
    textColor.r=0;
    textColor.g=0;
    textColor.b=0;
    message = NULL;
}
int SDL_TTF::prep_surface()
{
      font = TTF_OpenFont( "TELETYPE.ttf", size);
         if(!font)
        {
            printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
        }

return 0;
}

int SDL_TTF::rendertext(std::string text,int x,int y)
{
    char* line;
    char ctext[399];
    SDL_Rect offset;
    offset.x = 0;
    offset.y = 0;
    strcpy(ctext,text.c_str());
    line = strtok(ctext,"\n");
    SDL_Surface *background = IMG_Load("mapwin.bmp");

    int i;
    TTF_SetFontStyle(font,TTF_STYLE_BOLD);
    while (line !=NULL)
    {
        cout <<"line = "<<line<<endl;
        message = TTF_RenderText_Blended(font,line,textColor);
        SDL_BlitSurface(message, NULL,background, &offset);
        line = strtok(NULL,"\n");
        cout <<"again: line = "<<line<<endl;
        offset.y+= size;
         SDL_FreeSurface(message);

    }
    prep_texture(background, font_texture);
    cout <<"prepared texture\n";
    glBindTexture(GL_TEXTURE_2D,font_texture);
    glEnable(GL_BLEND);

   cout <<"draw texture\n";
    draw_texture(x, y, 256, 128, 0,0, 256, 128);


    cout <<"end of rendertext\n";




    /*
    char* line;
    char* ctext;

    strcpy(ctext,text.c_str());
    cout <<"begin SDL_TTF::rendertext"<<endl;
    cout <<"ctext = "<<ctext<<endl;
    line = strtok(ctext,"\n");
    cout <<"line = "<<line<<endl;

    int w,h;
    while (line!=NULL)
    {
        cout <<"line = "<<line<<endl;
        message = TTF_RenderText_Blended( font, line, textColor );

        w = message ->w;
        h = message->h;
        //
    }
    SDL_BlitSurface(message, 0,background, 0);
    SDL_FreeSurface(message);

    prep_texture(background, font_texture);
    draw_texture(x, y, size*2/3+1, size, 0,0, 256, 128);
*/
    return 0;
}
void SDL_TTF::free()
{SDL_FreeSurface( message );
TTF_CloseFont( font ); //Quit SDL_ttf TTF_Quit();
}

void SDL_TTF::setcolor(int red,int green,int blue)
{  textColor.r = red;
    textColor.g=green;
    textColor.b=blue;}
