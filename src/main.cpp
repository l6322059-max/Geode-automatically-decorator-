#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(AutoDecoratorMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto menu = this->getChildByID("bottom-menu");
        if (!menu) {
            log::warn("Auto Decorator X: bottom-menu was not found.");
            return true;
        }
        auto sprite = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        if (!sprite) sprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
        if (!sprite) return true;
        auto button = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(AutoDecoratorMenu::onAutoDecorator));
        button->setID("auto-decorator-x-button"_spr);
        menu->addChild(button);
        menu->updateLayout();
        return true;
    }

    void onAutoDecorator(CCObject*) {
        FLAlertLayer::create(
            "Auto Decorator X",
            "Smart decoration assistant\n\n"
            "Project setup is ready. The next development stage is the Geometry Dash editor decoration engine, including object placement, color presets, glow effects, and undo support. No level objects are changed by this preview.",
            "OK"
        )->show();
    }
};
