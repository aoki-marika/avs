#pragma once

#include "WindowBase.hpp"

// an empty class for platform specific functionality
class Window : public WindowBase
{
    protected:
        // internal struct for implementation specific data
        void *Data;

        virtual void *GetDisplay();
        virtual void *GetWindow();

    public:
        Window(std::string title,
               unsigned int width,
               unsigned int height);
        ~Window();
};
