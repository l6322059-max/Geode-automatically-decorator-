#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoDecoratorEditorUI, EditorUI) {
    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        // A small floating shortcut so the tool is reachable directly
        // while building a level, on both touch and desktop platforms.
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
            sprite,
            this,
            menu_selector(AutoDecoratorEditorUI::onOpenAutoDecorator)
        );
        button->setID("auto-decorator-x-open-button"_spr);
        menu->addChild(button);
        this->addChild(menu, 1000);

        return true;
    }

    void onOpenAutoDecorator(CCObject*) {
        FLAlertLayer::create(
            "Auto Decorator X",
            "EDITOR MENU\n\n"
            "STYLE PRESETS\n"
            "Choose a decoration style (coming soon).\n\n"
            "COLOR THEME\n"
            "Choose a color palette (coming soon).\n\n"
            "GLOW EFFECTS\n"
            "Glow controls (coming soon).\n\n"
            "DECORATE SELECTED\n"
            "Automatic object decoration is still in development. "
            "This preview does not change your level.",
            "Close"
        )->show();
    }
};
