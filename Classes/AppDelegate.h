#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#include "ServerMessage.h"

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class  AppDelegate : private cocos2d::Application, private cocos2d::Ref
{
private:
    struct GFXSet
    {
        std::string folder;
        std::string description;
        cocos2d::Size resolution;
        
        GFXSet(std::string description, std::string folder, cocos2d::Size resolution) : description(description), folder(folder), resolution(resolution)
        {
            
        }
    };
    
    unsigned int backgroundTime;
    
    void SaveOnServer();
    bool onDataSaved(ServerMessage * message);
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_
