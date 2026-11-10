#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(AutoDecoratorEditorUI, EditorUI) {
    struct Fields {
        bool panelOpen = false;
        bool glow = true;
        bool pulse = false;
        bool particles = false;
        bool outline = true;
    };

    bool init(LevelEditorLayer* editorLayer) {
        if (!EditorUI::init(editorLayer)) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Small FX shortcut in the level editor.
        auto shortcutMenu = CCMenu::create();
        shortcutMenu->setID("auto-decorator-x-shortcut-menu"_spr);
        shortcutMenu->setPosition({ 42.f, winSize.height * 0.55f });

        auto shortcutLabel = CCLabelBMFont::create("FX", "bigFont.fnt");
        auto shortcut = CCMenuItemSpriteExtra::create(
            shortcutLabel, this,
            menu_selector(AutoDecoratorEditorUI::onToggleEffectsPanel)
        );
        shortcut->setID("auto-decorator-x-fx-button"_spr);
        shortcutMenu->addChild(shortcut);
        this->addChild(shortcutMenu, 1000);

        // Effects panel. The toggles are real interactive controls;
        // applying them to level objects will be added with the decoration engine.
        auto panel = CCLayerColor::create({ 20, 25, 48, 235 }, 250.f, 205.f);
        panel->setID("auto-decorator-x-effects-panel"_spr);
        panel->setPosition({ 72.f, winSize.height * 0.5f - 100.f });
        panel->setVisible(false);
        this->addChild(panel, 1001);

        auto title = CCLabelBMFont::create("AUTO DECORATOR - EFFECTS", "bigFont.fnt");
        title->setScale(0.38f);
        title->setPosition({ 125.f, 180.f });
        panel->addChild(title);

        auto effectsMenu = CCMenu::create();
        effectsMenu->setPosition({ 0.f, 0.f });
        panel->addChild(effectsMenu);

        addEffectRow(effectsMenu, "Glow", 1, 145.f, true);
        addEffectRow(effectsMenu, "Pulse", 2, 108.f, false);
        addEffectRow(effectsMenu, "Particles", 3, 71.f, false);
        addEffectRow(effectsMenu, "Outline", 4, 34.f, true);

        auto closeLabel = CCLabelBMFont::create("CLOSE", "bigFont.fnt");
        closeLabel->setScale(0.42f);
        auto closeButton = CCMenuItemSpriteExtra::create(
            closeLabel, this,
            menu_selector(AutoDecoratorEditorUI::onToggleEffectsPanel)
        );
        closeButton->setPosition({ 210.f, 12.f });
        effectsMenu->addChild(closeButton);

        return true;
    }

    void addEffectRow(CCMenu* menu, char const* name, int tag, float y, bool enabled) {
        auto label = CCLabelBMFont::create(name, "bigFont.fnt");
        label->setScale(0.48f);
        label->setAnchorPoint({ 0.f, 0.5f });
        label->setPosition({ 28.f, y });
        // Labels are attached to the panel separately by using the menu's parent.
        menu->getParent()->addChild(label);

        auto toggler = CCMenuItemToggler::createWithStandardSprites(
            this, menu_selector(AutoDecoratorEditorUI::onEffectToggled), 0.8f
        );
        toggler->setTag(tag);
        toggler->toggle(!enabled);
        toggler->setPosition({ 205.f, y });
        menu->addChild(toggler);
    }

    void onToggleEffectsPanel(CCObject*) {
        auto panel = this->getChildByID("auto-decorator-x-effects-panel"_spr);
        if (panel) {
            panel->setVisible(!panel->isVisible());
        }
    }

    void onEffectToggled(CCObject* sender) {
        auto toggler = static_cast<CCMenuItemToggler*>(sender);
        if (!toggler) return;

        auto enabled = toggler->isToggled();
        switch (toggler->getTag()) {
            case 1: m_fields->glow = enabled; break;
            case 2: m_fields->pulse = enabled; break;
            case 3: m_fields->particles = enabled; break;
            case 4: m_fields->outline = enabled; break;
            default: break;
        }

        log::info("Auto Decorator X effect {}: {}", toggler->getTag(), enabled ? "ON" : "OFF");
    }
};
