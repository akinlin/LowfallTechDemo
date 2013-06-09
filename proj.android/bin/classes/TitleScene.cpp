#include "TitleScene.h"
#include "MapScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    TitleScene *layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    /////////////////////////////
    // 2. add a menu items for starting and exiting the game
    
    // create and initialize the menu labels
    CCLabelTTF* pStartMenuLabel = CCLabelTTF::create("Start", "Thonburi", 74);
    CCMenuItemLabel *pStartMenuItem = CCMenuItemLabel::create(pStartMenuLabel, this, menu_selector(TitleScene::menuStartCallback));
    CCSize startMenuItemSize = pStartMenuItem->getContentSize();
    pStartMenuItem->setPosition( ccp(size.width - startMenuItemSize.width - 20, size.height - 100) );
    
    CCLabelTTF* pExitMenuLabel = CCLabelTTF::create("Exit", "Thonburi", 74);
    CCMenuItemLabel *pExitMenuItem = CCMenuItemLabel::create(pExitMenuLabel, this, menu_selector(TitleScene::menuExitCallback));
    CCSize exitMenuItemSize = pExitMenuItem->getContentSize();
    pExitMenuItem->setPosition( ccp(pStartMenuItem->getPositionX(), pStartMenuItem->getPositionY() - startMenuItemSize.height - 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pStartMenuItem, pExitMenuItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pMainTitleLabel = CCLabelTTF::create("LowFall: A Tech Demo", "Thonburi", 50);
    CCSize mainTitleLabelSize = pMainTitleLabel->getContentSize();

    // position the label on the center of the screen
    pMainTitleLabel->setPosition( ccp((mainTitleLabelSize.width / 2) + 20, size.height - mainTitleLabelSize.height - 20) );

    // add the label as a child to this layer
    this->addChild(pMainTitleLabel, 1);

    return true;
}

void TitleScene::menuStartCallback(CCObject* pSender)
{
    // go to MapScene
    CCScene *pScene = MapScene::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void TitleScene::menuExitCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
