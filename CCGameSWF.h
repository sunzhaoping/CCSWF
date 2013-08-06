#include "cocos2d.h"
class CCGameSWF : public cocos2d::Node
{
    cocos2d::Array *m_fscommandListeners;

public:
    static CCGameSWF* sharedInstance();
    void addFscommandResponder( int responder,cocos2d::String* movieName);
    void removeFscommandResponder(int responder, cocos2d::String* movieName);

    bool init();
    void dealloc();
};


