#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

// the code for this mod is really dumb but it works so idc
class $modify(LikeItemLayerHook, LikeItemLayer) {
    struct Fields {
        bool likePopup = false;
        bool dislikePopup = false;
    };

    // this is prolly correct i think (beforepre gives errors and geode docs r down so :pray:)
    static void onModify(auto& self) {
        (void)self.setHookPriorityPre("LikeItemLayer::onLike", Priority::First);
        (void)self.setHookPriorityPre("LikeItemLayer::onDislike", Priority::First);
    };

    void onLike(CCObject* sender) {
        if (!Mod::get()->getSettingValue<bool>("like") || m_fields->likePopup) {
            m_fields->likePopup = false;
            // we so silly :3
            return LikeItemLayer::onLike(sender);
        }

        m_fields->likePopup = true;
        geode::createQuickPopup(
            "Confirm Like", 
            "Are you sure you want to like this " + getItemType() + "? You cannot undo this.", 
            "Cancel", "Ok",
            [&] (auto, bool btn2) {
                if (btn2) LikeItemLayer::onLike(sender);
                else if (Mod::get()->getSettingValue<bool>("close")) onClose(sender);
            }
        );
    }
    void onDislike(CCObject* sender) {
        if (!Mod::get()->getSettingValue<bool>("dislike") || m_fields->dislikePopup) {
            m_fields->dislikePopup = false;
            return LikeItemLayer::onDislike(sender);
        }

        m_fields->dislikePopup = true;
        geode::createQuickPopup(
            "Confirm Dislike", 
            "Are you sure you want to dislike this " + getItemType() + "? You cannot undo this.", 
            "Cancel", "Ok",
            [&] (auto, bool btn2) {
                if (btn2) LikeItemLayer::onDislike(sender);
                else if (Mod::get()->getSettingValue<bool>("close")) onClose(sender);
            }
        );
    }

    std::string getItemType() {
        return m_itemType == LikeItemType::Level 
            ? "level" : m_itemType == LikeItemType::LevelList 
                ? "list" : "comment";
    }
};
