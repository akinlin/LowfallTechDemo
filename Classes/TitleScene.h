#ifndef __TITLESCENE_H__
#define __TITLESCENE_H__

#include "cocos2d.h"

class TitleScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuStartCallback(CCObject* pSender);
    void menuExitCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(TitleScene);
};

#endif // __TITLESCENE_H__
