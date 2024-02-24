#pragma once

class IRBMKLoadingScreenManager
{
public:
    virtual void SetTitle   (const char* Title) = 0;
    virtual void Play       (bool IsNewGame,int MaxStatus) = 0;
    virtual void Wait       () = 0;
};
