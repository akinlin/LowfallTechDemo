#ifndef __DIESCENE_H__
#define __DIESCENE_H__

#include "cocos2d.h"

class DieScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void returnToMainMenuCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(DieScene);
};

#endif // __DIESCENE_H__
