#ifndef __MAPSCENE_H__
#define __MAPSCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class MapScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();

    // menu callbacks
    void menuCloseCallback(CCObject* pSender);
    void menuDieCallback(CCObject* pSender);
    void menuWinCallback(CCObject* pSender);
    
    // touch callbacks
    void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    void handleDrag(CCSet *pTouches, CCEvent *pEvent);
    CCPoint resetBoundries(CCPoint rawPosition, CCSize contentSize);
    void handlePinch(CCSet *pTouches, CCEvent *pEvent);
    
    // Map sprites
    CCParallaxNode *_mapNode;

    // implement the "static node()" method manually
    CREATE_FUNC(MapScene);
    
private:
    void update(float dt);
    float calculateScale(float currentScale, float scaleDiff);
    
    CCPoint distanceFromCenter;
    float pinchDiameter;
    int touchState;
    bool pinchStarted;
    
    CCPointArray *m_pArray1;
    
    float initialCarScale;
};

#endif // __MAPSCENE_H__
