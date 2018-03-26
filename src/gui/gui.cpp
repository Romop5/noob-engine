#include <gui/gui.h>
#include <utils/logger.h>

GUI::GUI(): callbackId(0)
{
}

GUI::~GUI()
{
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();
}

void GUI::init(SDL_Window* windows)
{
	LOG_INFO("Init GUI windows %p\n", windows);
	this->_windows = windows;
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    ImGui_ImplSdlGL3_Init(windows);

    // Setup style
    ImGui::StyleColorsDark();

}

void GUI::render()
{
        ImGui_ImplSdlGL3_NewFrame(this->_windows);
	/* Call all callbacks */
	for(auto &pair: this->_callback)
	{
		pair.second();
	}
	// Render GUI
    ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
}

CALLBACK_ID GUI::registerCallback(callbackFunc func)
{
	this->_callback[++this->callbackId]= func;
	return this->callbackId;
}

void GUI::deleteCallback(CALLBACK_ID func)
{
	this->_callback.erase(this->callbackId);
}
