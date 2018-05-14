/**
 * @file ./gui/gui.h
 * @brief Class for handling GUI messages, callbacks and rendering
 * @copyright The MIT license 
 */

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

/**
 * @brief GUI Callback has zero parameters and returns void
 */
using callbackFunc =  std::function<void(void)>;

/**
 * @brief GUI handling class
 */
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
    /**
     * @brief Per-frame render of GUI
     */
	void render();
    /**
     * @brief Register user-callback which defines GUI elements
     *
     * @param func
     *
     * @return ID for deleting
     */
	CALLBACK_ID registerCallback(callbackFunc func);
	void deleteCallback(CALLBACK_ID func);
    /**
     * @brief Handle input from engine (SDL)
     *
     * @param event
     */
    void handleInput(SDL_Event& event);

};
