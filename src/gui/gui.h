#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "imgui.h"
#include "imgui_impl_sdl_gl3.h"

#include <functional>
#include <map>


typedef int CALLBACK_ID;

using callbackFunc =  std::function<void(void)>;
class GUI
{
	// user-defined callbacks to render GUI
	std::map<CALLBACK_ID, callbackFunc> _callback;
	CALLBACK_ID callbackId;
	SDL_Window* _windows;
	public:
	GUI();
	~GUI();
	void init(SDL_Window* windows);
	void render();
	CALLBACK_ID registerCallback(callbackFunc func);
	void deleteCallback(CALLBACK_ID func);
    void handleInput(SDL_Event& event);

};
