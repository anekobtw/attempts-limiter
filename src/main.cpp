#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>


class $modify(MyLevelInfoLayer, LevelInfoLayer) {
	bool init(GJGameLevel* level, bool challenge) {
		if (!LevelInfoLayer::init(level, challenge)) return false;

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png"),
            this,
            menu_selector(MyLevelInfoLayer::onInfo)
        );

        auto menu = this->getChildByIDRecursive("left-side-menu");
        menu->addChild(button);
        button->setID("my-button-menu"_spr);
        menu->updateLayout();
        this->updateLayout();

        return true;
    }

public:
    void onInfo(CCObject*) {
		int attempts = Mod::get()->template getSettingValue<int64_t>("attempts-slider");
        std::string info;
        if (attempts >= 50) {
            info = "You have <cg>" + std::to_string(attempts) + "</c> attempts left.";
        } else if (attempts >= 15) {
            info = "You have <cy>" + std::to_string(attempts) + "</c> attempts left.";
        } else if (attempts > 0) {
            info = "You have <cr>" + std::to_string(attempts) + "</c> attempts left.";
        } else {
            info = "<cr>You have run out of attempts.</c>";
		}
        FLAlertLayer::create("info", info, "ok")->show();
    }
};


class $modify(PlayLayer) {
public:
    void resetLevel() {
		int attempts = Mod::get()->template getSettingValue<int64_t>("attempts-slider");
        if (attempts <= 0) {
            PlayLayer::onQuit();
        }
        attempts -= 1;
		int a = Mod::get()->template setSettingValue<int64_t>("attempts-slider", attempts);
        PlayLayer::resetLevel();
    }
};


class $modify(ModPauseLayer, PauseLayer) {
public:
    static void onModify(auto& self) {
        self.setHookPriority("PauseLayer::customSetup", INT_MIN);
    }

    void customSetup() override {
        PauseLayer::customSetup();

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
            this,
            menu_selector(MyLevelInfoLayer::onInfo)
        );

        auto menu = this->getChildByIDRecursive("right-button-menu");
        menu->addChild(button);
        button->setID("my-button-pause"_spr);
        menu->updateLayout();
        this->updateLayout();
    }
};
