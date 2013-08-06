#ifndef CCSWF_GAMESWF
#define CCSWF_GAMESWF
#include "cocos2d.h"

class GameSWF : public cocos2d::Node
{
    cocos2d::Array *m_fscommandListeners;

public:
    static GameSWF* sharedInstance();
    void addFscommandResponder( int responder,cocos2d::String* movieName);
    void removeFscommandResponder(int responder, cocos2d::String* movieName);

    bool init();
    void dealloc();
};
#endif


