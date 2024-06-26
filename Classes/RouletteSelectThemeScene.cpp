#include "RouletteSelectThemeScene.h"
#include "AudioManager.h"

#include "PlayerProfile.h"
#include "TopBar.h"
#include "BottomBar.h"

#include "LoadingSplash.h"

#include "RouletteConfig.h"

USING_NS_CC;

Scene * RouletteSelectThemeScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = RouletteSelectThemeScene::create();
    scene->addChild(layer);

    return scene;
}

RouletteSelectThemeScene::RouletteSelectThemeScene()
{
    
}

RouletteSelectThemeScene::~RouletteSelectThemeScene()
{
    
}

bool RouletteSelectThemeScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    CCASSERT(RouletteConfig::Themes.size() == RouletteConfig::ThemeRequirements.size(), "themes array and theme_requirements array should have same sizes");
    CCASSERT(RouletteConfig::Themes.size() + 3 <= RouletteConfig::BetValues.size(), "bet_values array should have more elements than themes array by 3 or more");
    
    auto winSize = Director::getInstance()->getWinSize();
    auto visSize = Director::getInstance()->getVisibleSize();
    auto visOrig = Director::getInstance()->getVisibleOrigin();
    
    //background
    auto background = Sprite::create("ui/MainMenu/main_background.png");
    background->setPosition(winSize / 2);
    addChild(background);
    
    //logo
    auto logo = Sprite::create("ui/Roulette/roulette_logo.png");
    addChild(logo);
    logo->setPosition(Vec2(winSize.width / 2, visOrig.y + visSize.height - 135));
    
    //topbar
    auto topbar = TopBar::create(std::bind(&RouletteSelectThemeScene::onBackButtonPressed, this));
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
    
    for (int i = 0; i < RouletteConfig::Themes.size(); i++)
    {
        std::string themeButtonFile = StringUtils::format("ui/Cities/%s/button.png", RouletteConfig::Themes[i].c_str());
        
        auto theme = ui::Button::create(themeButtonFile, themeButtonFile, themeButtonFile);
        theme->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        theme->setPosition(Vec2(insertPosition, scrollList->getContentSize().height / 2));
        scrollList->getInnerContainer()->addChild(theme);
        insertPosition += theme->getContentSize().width + margin;
        theme->setName(RouletteConfig::Themes[i]);
        theme->addTouchEventListener(std::bind(&RouletteSelectThemeScene::onButtonPressed, this, std::placeholders::_1, std::placeholders::_2));
        theme->setEnabled(PlayerProfile::getInstance().getLevel() >= RouletteConfig::ThemeRequirements[i] );
        
        //hackyhack_publish_levels --> open all levels
//        theme->setEnabled(true);
        
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
            
            auto label = Label::createWithTTF(StringUtils::format("Needs level %d", RouletteConfig::ThemeRequirements[i]), "fonts/Berlin.ttf", 22);
            lock_back->addChild(label);
            label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            label->setPosition(lock_back->getContentSize() / 2);
        }
    }
    
    scrollList->setInnerContainerSize(Size(std::max(insertPosition, (int)scrollList->getContentSize().width), scrollList->getContentSize().height));
    
    return true;
}

void RouletteSelectThemeScene::onEnter()
{
    Layer::onEnter();
    
    AudioManager::getInstance().PlayBackgroundMusic("music/BackgroundMusic/Home");
}

void RouletteSelectThemeScene::onButtonPressed(Ref * sender, ui::Widget::TouchEventType event)
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

void RouletteSelectThemeScene::ProcessButtonAction(cocos2d::ui::Button * button)
{
    for (int i = 0; i < RouletteConfig::Themes.size(); i++)
    {
        if (RouletteConfig::Themes[i] == button->getName())
        {
            printf("Theme selected: %s\n", button->getName().c_str());
            
            auto scene = LoadingSplash::createScene(LoadingSplash::ROULETTE_GAME, button->getName());
            Director::getInstance()->replaceScene(scene);
            
            break;
        }
    }
}

void RouletteSelectThemeScene::onBackButtonPressed()
{
    auto nextScene = LoadingSplash::createScene(LoadingSplash::MAIN_MENU);
    Director::getInstance()->replaceScene(nextScene);
}
