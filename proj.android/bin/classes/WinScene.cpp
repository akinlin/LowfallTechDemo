#include "WinScene.h"
#include "TitleScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* WinScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    WinScene *layer = WinScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool WinScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    /////////////////////////////
    // 2. add a timed delay before returning to the main menu
    
    CCFiniteTimeAction*  action = CCSequence::create( CCDelayTime::create(3),
                                                     CCCallFuncN::create( this, callfuncN_selector(WinScene::returnToMainMenuCallback) ),
                                                     NULL);
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Win Scene", "Thonburi", 34);
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    pSprite->setPosition( ccp(size.width/2, size.height/2) );
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    this->runAction(action);
    
    return true;
}

void WinScene::returnToMainMenuCallback(CCObject* pSender)
{
    // return to title screen
    CCScene *pScene = TitleScene::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}
