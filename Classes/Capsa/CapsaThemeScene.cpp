//
//  CapsaThemeScene.cpp
//  DovemobiCasino
//
//  Created by liu on 9/6/17.
//
//

#include "CapsaThemeScene.hpp"

#include "AudioManager.h"

#include "PlayerProfile.h"
#include "TopBar.h"
#include "BottomBar.h"

#include "LoadingSplash.h"

#include "CasinowarConfig.h"

#include "GameHandler.hpp"

USING_NS_CC;

Scene * CapsaThemeScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = CapsaThemeScene::create();
    scene->addChild(layer);
    
    return scene;
}

CapsaThemeScene::CapsaThemeScene()
{
    
}

CapsaThemeScene::~CapsaThemeScene()
{
    
}

bool CapsaThemeScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    CCASSERT(CasinowarConfig::Themes.size() == CasinowarConfig::ThemeRequirements.size(), "themes array and theme_requirements array should have same sizes");
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto background = Sprite::create("ui/MainMenu/main_background.png");
    background->setPosition(winSize / 2);
    addChild(background);
    
    //logo
    auto logo = Sprite::create("ui/CasinoWar/logo.png");
    addChild(logo);
    logo->setPosition(Vec2(winSize.width / 2, visOrig.y + visSize.height - 135));
    
    //topbar
    auto topbar = TopBar::create(std::bind(&CapsaThemeScene::onBackButtonPressed, this));
    topbar->setPosition(Size(winSize.width / 2, winSize.height));
    addChild(topbar, 90);
    
    //bottombar
    auto bottombar = BottomBar::create();
    bottombar->setPosition(Size(winSize.width / 2, 0));
    bottombar->setScale(Director::getInstance()->getVisibleSize().width / bottombar->getContentSize().width);
    addChild(bottombar, 90);
    
    //scroll area
    int margin = 15;
    int insertPosition = margin;
    
    auto scrollList = ui::ScrollView::create();
    scrollList->setAnchorPoint(Vec2::ZERO);
    scrollList->setContentSize(Size(visSize.width, 456));
    scrollList->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollList->setPosition(Vec2(visOrig.x, 140));
    scrollList->setScrollBarEnabled(false);
    addChild(scrollList);
    
    for (int i = 0; i < CasinowarConfig::Themes.size(); i++)
    {
        std::string themeButtonFile = StringUtils::format("ui/Cities/%s/button.png", CasinowarConfig::Themes[i].c_str());
        
        auto theme = ui::Button::create(themeButtonFile, themeButtonFile, themeButtonFile);
        theme->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        theme->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
        scrollList->getInnerContainer()->addChild(theme);
        insertPosition += theme->getContentSize().width + margin;
        theme->setName(CasinowarConfig::Themes[i]);
        theme->addTouchEventListener(std::bind(&CapsaThemeScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        theme->setEnabled(PlayerProfile::getInstance().getLevel() >= CasinowarConfig::ThemeRequirements[i] );
        
        if (theme->isEnabled() == false)
        {
            auto lock = Sprite::create("ui/Cities/lock.png");
            theme->addChild(lock, 1);
            lock->setAnchorPoint(Vec2::ZERO);
            lock->setPosition(Vec2(10, 10));
            
            auto lock_back = Sprite::create("ui/Slots/lock_pop.png");
            theme->addChild(lock_back);
            lock_back->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            lock_back->setPosition(lock->getPosition() + Vec2(lock->getContentSize().width - 10, lock->getContentSize().height / 2));
            
            auto label = Label::createWithTTF(StringUtils::format("Needs level %d", CasinowarConfig::ThemeRequirements[i]), "fonts/Berlin.ttf", 22);
            lock_back->addChild(label);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setPosition(lock_back->getContentSize() / 2);
        }
    }
    
    scrollList->setInnerContainerSize(Size(std::max(insertPosition, (int)scrollList->getContentSize().width), scrollList->getContentSize().height));
    
    return true;
}

void CapsaThemeScene::onEnter()
{
    Layer::onEnter();
    
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Home");
}

void CapsaThemeScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
{
    auto button = dynamic_cast<ui::Button*>(sender);
    
    if (event == ui::Widget::TouchEventType::BEGAN)
    {
        AudioManager::getInstance().PlaySoundEffect("music/SlotsSound/TapBt");
        button->setColor(Color3B(178, 178, 178));
    }
    else if (event == ui::Widget::TouchEventType::ENDED)
    {
        button->setColor(Color3B::WHITE);
        
        ProcessButtonAction(button);
    }
    else if (event == ui::Widget::TouchEventType::CANCELED)
    {
        button->setColor(Color3B::WHITE);
    }
}

void CapsaThemeScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    for (int i = 0; i < CasinowarConfig::Themes.size(); i++)
    {
        if (CasinowarConfig::Themes[i] == button->getName())
        {
            printf("Theme selected: %s\n", button->getName().c_str());
            
            auto scene = LoadingSplash::createScene(LoadingSplash::CAPSA_GAME, button->getName());
            Director::getInstance()->replaceScene(scene);
            
            GameHandler::getInstance()->setSelectedCityIndex(i);
            
            break;
        }
    }
}

void CapsaThemeScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::MAIN_MENU);
    Director::getInstance()->replaceScene(nextScene);
}

