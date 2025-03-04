#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class $modify (LikeLayer, LikeItemLayer) {
    struct Fields {
        SEL_MenuHandler likeCallback;
        SEL_MenuHandler dislikeCallback;
        CCMenuItemSpriteExtra* likeButton;
        CCMenuItemSpriteExtra* dislikeButton;
    };

    bool init(LikeItemType p0, int p1, int p2) {
        if(!LikeItemLayer::init(p0, p1, p2)) return false;
        auto buttonMenu = this->getChildByType<CCLayer>(0)->getChildByType<CCMenu>(0);
        m_fields->likeButton = buttonMenu->getChildByType<CCMenuItemSpriteExtra>(1);
        m_fields->likeCallback = m_fields->likeButton->m_pfnSelector;
        m_fields->likeButton->setTarget(this, menu_selector(LikeLayer::onLikeButton));
        m_fields->dislikeButton = buttonMenu->getChildByType<CCMenuItemSpriteExtra>(2);
        m_fields->dislikeCallback = m_fields->dislikeButton->m_pfnSelector;
        m_fields->dislikeButton->setTarget(this, menu_selector(LikeLayer::onDislikeButton));
        return true;
    }

    void onLikeButton (CCObject* sender) {
        geode::createQuickPopup(
            "Confirm Like", "Are you sure you want to like this level, you cannot undo this.", "Cancel", "Ok",
            [&](auto, bool btn2) {
                if (btn2) {
                    m_fields->likeButton->setTarget(this, m_fields->likeCallback);
                    m_fields->likeButton->activate();
                }
            }
        );
    }

    void onDislikeButton (CCObject* sender) {
        geode::createQuickPopup(
            "Confirm Disike", "Are you sure you want to dislike this level, you cannot undo this.", "Cancel", "Ok",
            [&](auto, bool btn2) {
                if (btn2) {
                    m_fields->dislikeButton->setTarget(this, m_fields->dislikeCallback);
                    m_fields->dislikeButton->activate();
                }
            }
        );
    }
};