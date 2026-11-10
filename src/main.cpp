#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoDecoratorEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setID("auto-decorator-x-editor-menu"_spr);
        menu->setPosition({ 38.f, winSize.height * 0.55f });

        auto sprite = CCSprite::createWithSpriteFrameName("GJ_editBtn_001.png");
        if (!sprite) sprite = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
        if (!sprite) {
            log::warn("Auto Decorator X: could not create editor button sprite.");
            return true;
        }

        auto button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(AutoDecoratorEditorUI::onOpenAutoDecorator)
        );
        button->setID("auto-decorator-x-open-button"_spr);
        menu->addChild(button);
        this->addChild(menu, 1000);
        return true;
    }

    void onOpenAutoDecorator(CCObject*) {
        FLAlertLayer::create(
            "Auto Decorator X",
            "Decoration tools are being developed.\n\n"
            "Style presets, color themes, glow effects, and one-click decoration "
            "will appear here when ready.\n\n"
            "Your level has not been changed.",
            "Close"
        )->show();
    }
};
