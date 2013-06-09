#include "MapScene.h"
#include "DieScene.h"
#include "WinScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

enum
{
    // Map object tag enums
    kTagMapLayer = 10,
    kTagMapImage,
    kTagMapMarker,
    kTagMapCar,
    
    // touch states
    kTouchStateEmpty = 100, 
    kTouchStateDrag,
    kTouchStatePinch,
};

CCScene* MapScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MapScene *layer = MapScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MapScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // turn on touch
    setTouchEnabled( true );
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // init the scroll delta
    distanceFromCenter = ccp(0,0);
    // init touch state
    touchState = kTouchStateEmpty;
    pinchStarted = false;
    
    /////////////////////////////
    // 2. add a menu items for starting and exiting the game
    
    // create and initialize the menu labels
    CCLabelTTF* pDieMenuLabel = CCLabelTTF::create("Die", "Thonburi", 74);
    CCMenuItemLabel *pDieMenuItem = CCMenuItemLabel::create(pDieMenuLabel, this, menu_selector(MapScene::menuDieCallback));
    CCSize dieMenuItemSize = pDieMenuItem->getContentSize();
    pDieMenuItem->setPosition( ccp(size.width - dieMenuItemSize.width - 20, size.height - 100) );
    
    CCLabelTTF* pWinMenuLabel = CCLabelTTF::create("Win", "Thonburi", 74);
    CCMenuItemLabel *pWinMenuItem = CCMenuItemLabel::create(pWinMenuLabel, this, menu_selector(MapScene::menuWinCallback));
    CCSize winMenuItemSize = pWinMenuItem->getContentSize();
    pWinMenuItem->setPosition( ccp(pDieMenuItem->getPositionX(), pDieMenuItem->getPositionY() - winMenuItemSize.height - 20) );
    
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pDieMenuItem, pWinMenuItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    
    /////////////////////////////
    // 3. add catmull rom spline for road
    
    CCPointArray *array = CCPointArray::create(20);
    
    array->addControlPoint(ccp(0, 0));
    array->addControlPoint(ccp(80, 80));
    array->addControlPoint(ccp(size.width - 80, 80));
    array->addControlPoint(ccp(size.width - 80, size.height - 80));
    array->addControlPoint(ccp(80, size.height - 80));
    array->addControlPoint(ccp(80, 80));
    array->addControlPoint(ccp(size.width / 2, size.height / 2));
    
    // create the catmull rom action
    CCCatmullRomBy *action = CCCatmullRomBy::create(3, array);
    CCActionInterval *reverse = action->reverse();
	
    CCActionInterval *seq = CCSequence::create(action, reverse, NULL);
    
    // create the car sprite
    CCSprite* pCarSprite = CCSprite::create("car.png");
    pCarSprite->setTag(kTagMapCar);
    pCarSprite->setAnchorPoint(ccp(0,0));
    initialCarScale = .25;
    pCarSprite->setScale(initialCarScale);
    pCarSprite->runAction(CCRepeatForever::create(seq));
    
    m_pArray1 = array;
    m_pArray1->retain();
    
    /////////////////////////////
    // 4. add paralax screen

    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Map Scene", "Thonburi", 34);

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("sattellite_lofall.png");
    pSprite->setTag(kTagMapImage);
    pSprite->setAnchorPoint(ccp(0,0));
    
    // position the sprite on the center of the screen
    //pSprite->setPosition( ccp(size.width/2, size.height/2) );
    
    // add the sprite as a child to this layer
    //this->addChild(pSprite, 0, kTagMapLayer);
    
    ///////
    // Paralax map test
    //
    
    // Create the scrollable map node
    _mapNode = CCParallaxNode::create();
    
    // scroll speed
    CCPoint bgSpeed = ccp(1.0, 1.0);
    CCPoint carSpeed = ccp(1.5, 1.5);
    
    // add the map sprint to the scrollable map node
    _mapNode->addChild(pSprite, 0, bgSpeed, CCPointZero);
    _mapNode->addChild(pCarSprite, 1, carSpeed, CCPointZero);
    _mapNode->setTag(kTagMapLayer);
    
    this->addChild(_mapNode, -1);
    
    // update loop
    //this->scheduleUpdate();

    return true;
}

void MapScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MapScene::menuDieCallback(CCObject* pSender)
{
    // go to MapScene
    CCScene *pScene = DieScene::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void MapScene::menuWinCallback(CCObject* pSender)
{
    // go to MapScene
    CCScene *pScene = WinScene::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void MapScene::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	// increment the touch state if less than 2
	if (touchState < kTouchStatePinch)
	{
        // increment the touch state
		touchState++;

		if (touchState == kTouchStatePinch)
		{
			pinchStarted = true;
		}
	}
    
    CCLog("BEGIN TOUCH STATE = %d", touchState);
}

void MapScene::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    if (touchState == kTouchStateDrag)
    {
        handleDrag(pTouches, pEvent);
    }
    else if (touchState == kTouchStatePinch)
    {
        handlePinch(pTouches, pEvent);
    }

    CCLog("MOVE NUM TOUCHES = %d", pTouches->count());
}

CCPoint MapScene::resetBoundries(CCPoint rawPosition, CCSize contentSize)
{
    // get basic information
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // set the x and y positions
    if (rawPosition.x > 0)
    {
        rawPosition.x = 0;
    }
    
    if (rawPosition.x - winSize.width < (-contentSize.width))
    {
        rawPosition.x = -(contentSize.width - winSize.width);
    }
    
    if (rawPosition.y > 0)
    {
        rawPosition.y = 0;
    }
    
    if (rawPosition.y - winSize.height < -(contentSize.height))
    {
        rawPosition.y = -(contentSize.height - winSize.height);
    }
    
    return rawPosition;
}

void MapScene::handleDrag(CCSet *pTouches, CCEvent *pEvent)
{
	// scroll
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint diff = touch->getDelta();
    
    CCNode* node = getChildByTag(kTagMapLayer);
    if (node != NULL)
    {
        CCPoint currentPos = node->getPosition();
        CCSize contentSize = node->getChildByTag(kTagMapImage)->getContentSize();
        
        // adjust for scale
        contentSize.height = contentSize.height * node->getChildByTag(kTagMapImage)->getScale();
        contentSize.width = contentSize.width * node->getChildByTag(kTagMapImage)->getScale();
        
        // make sure the delta doesn't go past the image boundary
        CCPoint newPos = ccpAdd(currentPos, diff);
        
        newPos = resetBoundries(newPos, contentSize);
        
        node->setPosition( newPos );
    }
}

void MapScene::handlePinch(CCSet *pTouches, CCEvent *pEvent)
{
	// pinch zoom
    // kinda stupid code but it just grabs the first two points
    CCSetIterator iter = pTouches->begin();
    CCTouch* pTouch1 = (CCTouch*)(*iter);
    CCPoint point1Location = pTouch1->getLocation();
    iter++;
    CCTouch* pTouch2 = (CCTouch*)(*iter);
    CCPoint point2Location = pTouch2->getLocation();
    
    // real distance
    float distance = ccpAngle(point1Location, point2Location);
    
    // if the pinch has already begun then use the real distance
    if(pinchStarted)
    {
        pinchDiameter = distance;
    }
    
    // get the delta between the real distance and the starting distance
    float distanceDiff = pinchDiameter - distance;
    
    CCNode* node = getChildByTag(kTagMapLayer);
    CCNode* mapImage = node->getChildByTag(kTagMapImage);
    CCNode* carImage = node->getChildByTag(kTagMapCar);
    // update the map scale
    if (mapImage && carImage)
    {
        // get the new map scale
        float currentMapScale = mapImage->getScale();
        float currentCarScale = carImage->getScale();
        float newMapScale = currentMapScale;
        float newCarScale = currentCarScale;
        // update based on current scale
        if (!pinchStarted)
        {
            newMapScale = calculateScale(mapImage->getScale(), distanceDiff);
            newCarScale = calculateScale(carImage->getScale(), distanceDiff);
            if (newMapScale < 1)
            {
                newMapScale = 1;
                newCarScale = initialCarScale;
            }
            else if (newMapScale > 4)
            {
                newMapScale = 4;
                newCarScale = 4 - newCarScale;
            }
        }
        else
        {
            pinchStarted = false;
        }
        
        // change the scale values
        mapImage->setScale(newMapScale);
        carImage->setScale(newCarScale);
        
        // now reset the position based on the new scale
        CCPoint mapPosition = node->getPosition();
        CCSize  contentSize = mapImage->getContentSize();
        // adjust for scale
        contentSize.height = contentSize.height * node->getChildByTag(kTagMapImage)->getScale();
        contentSize.width = contentSize.width * node->getChildByTag(kTagMapImage)->getScale();
        
        CCPoint cleanZoom = resetBoundries(mapPosition, contentSize);
        node->setPosition(cleanZoom);
    }
    else
    {
        CCLOG("*** mapImage or carImage IS NULL ****");
    }
}

float MapScene::calculateScale(float currentScale, float scaleDiff)
{
	// returns current scale if new scale is NaN
    float newScale = currentScale + scaleDiff;
    
    if (newScale != newScale)
    {
        // scale value is NaN
        //CCLOG("*** SCALE IS NaN ****");
        
        return currentScale;
    }

    // otherwise return the new scale value
    return newScale;
}

void MapScene::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	// decrement the touch state if grater than zero
    if (touchState > kTouchStateEmpty)
    {
        touchState--;
    }

    CCLog("END TOUCH STATE = %d", touchState);
}

void MapScene::update(float dt)
{
    // update the car position
//    kmGLPushMatrix();
//	kmGLTranslatef(50, 50, 0);
//	ccDrawCatmullRom(m_pArray1, 50);
//	kmGLPopMatrix();
    
	//ccDrawCatmullRom(m_pArray2,50);
}

void printDebugValues()
{
    CCLOG("");
    CCLOG("*** SCALE VALUES ****");
//    CCLog("currScale: %f", currentScale);
//    CCLog("delta1: (x:%f y:%f)", point1Location.x, point1Location.y);
//    CCLog("delta2: (x:%f y:%f)", point2Location.x, point2Location.y);
//    CCLog("distace: %f", distance);
//    CCLog("newscale:  %f", newScale);
//    
//    CCLog("content size: (width:%f, height:%f)", contentSize.width, contentSize.height);
//    CCLog("currPos: (x:%f, y:%f)", mapPosition.x, mapPosition.y);
//    CCLog("newPos:  (x:%f, y:%f)", cleanZoom.x, cleanZoom.y);
    CCLOG("*** SCALE VALUES END ****");
    
// LOG: Show the position values
//        CCLOG("*** POSITION VALUES ****");
//        CCLog("Scale: (scale:%f)", node->getChildByTag(kTagMapImage)->getScale());
//       // CCLog("winSize: (width:%f, height:%f)", winSize.width, winSize.height);
//        CCLog("content size: (width:%f, height:%f)", contentSize.width, contentSize.height);
//        CCLog("currPos: (x:%f, y:%f)", currentPos.x, currentPos.y);
//        CCLog("delta:   (x:%f, y:%f)", diff.x, diff.y);
//        CCLog("newPos:  (x:%f, y:%f)", newPos.x, newPos.y);
//        CCLOG("*** POSITION VALUES END ****");
}
