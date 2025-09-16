#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LikeItemLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

int nodeIDsMajorVersion = -1;
int nodeIDsMinorVersion = -1;
bool calledAlready = false;

class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (calledAlready) return true;
        calledAlready = true;

        auto nodeIDsVersion = Loader::get()->getLoadedMod("geode.node-ids")->getVersion();
        nodeIDsMajorVersion = static_cast<int>(nodeIDsVersion.getMajor());
        nodeIDsMinorVersion = static_cast<int>(nodeIDsVersion.getMinor());

        return true;
    }
};

// the code for this mod is really dumb but it works so idc
class $modify (LikeLayer, LikeItemLayer) {
    struct Fields {
        SEL_MenuHandler likeCallback;
        SEL_MenuHandler dislikeCallback;
        CCMenuItemSpriteExtra* likeButton;
        CCMenuItemSpriteExtra* dislikeButton;
    };

    bool init(LikeItemType p0, int p1, int p2) {
        if(!LikeItemLayer::init(p0, p1, p2)) return false;

        if (!m_mainLayer || !m_buttonMenu) return true;

        auto* likeButton = m_mainLayer->querySelector("action-menu > like-button");
        auto* dislikeButton = m_mainLayer->querySelector("action-menu > dislike-button");

        if (!likeButton || !dislikeButton) {
            if (nodeIDsMinorVersion < 22 && nodeIDsMajorVersion < 2) {
                likeButton = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(1);
                dislikeButton = m_buttonMenu->getChildByType<CCMenuItemSpriteExtra>(2);
            } else return true;
        }

        if (Mod::get()->getSettingValue<bool>("like")) {
            m_fields->likeButton = static_cast<CCMenuItemSpriteExtra*>(likeButton);
            m_fields->likeCallback = m_fields->likeButton->m_pfnSelector;
            m_fields->likeButton->setTarget(this, menu_selector(LikeLayer::onLikeButton));
        } if (Mod::get()->getSettingValue<bool>("dislike")) {
            m_fields->dislikeButton = static_cast<CCMenuItemSpriteExtra*>(dislikeButton);
            m_fields->dislikeCallback = m_fields->dislikeButton->m_pfnSelector;
            m_fields->dislikeButton->setTarget(this, menu_selector(LikeLayer::onDislikeButton));
        }
        return true;
    }

    void onLikeButton (CCObject* sender) {
        std::string text = m_itemType == LikeItemType::Level ? "level" : m_itemType == LikeItemType::LevelList ? "list" : "comment";
        geode::createQuickPopup(
            "Confirm Like", "Are you sure you want to like this " + text + "? You cannot undo this.", "Cancel", "Ok",
            [&](auto, bool btn2) {
                if (btn2) {
                    m_fields->likeButton->setTarget(this, m_fields->likeCallback);
                    m_fields->likeButton->activate();
                }
            }
        );
    }

    void onDislikeButton (CCObject* sender) {
        std::string text = m_itemType == LikeItemType::Level ? "level" : m_itemType == LikeItemType::LevelList ? "list" : "comment";
        geode::createQuickPopup(
            "Confirm Disike", "Are you sure you want to dislike this " + text + "? You cannot undo this.", "Cancel", "Ok",
            [&](auto, bool btn2) {
                if (btn2) {
                    m_fields->dislikeButton->setTarget(this, m_fields->dislikeCallback);
                    m_fields->dislikeButton->activate();
                }
            }
        );
    }
};
