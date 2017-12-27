This project shows howto
* using VBOs to display triangels in opengl
  see https://open.gl/drawing
* create opengl textures from several sources using SDL tools
  see http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
* display these textures in opengl
  see https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2   (create SDL_Texture)
  see https://wiki.libsdl.org/SDL_GL_BindTexture   (bind as opengl texture)
* displaying text in opengl using the above technics
  see http://www.sdltutorials.com/sdl-ttf  (rendering text into SDL_Surface/SDL_Texture )
* introduces a class Component to handle io-events as a base class for widgets displayed by opengl
* implement a simple text input widget based on that technics

Call with "-f" option to fullscreen, without opens a window of size 640X480.

While running
* "f"-Key to switch from windowed mode window to full screen an back
* "g"-Key to display monitor parameters on stdout.
* Any other key to exit.
