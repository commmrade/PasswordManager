#ifndef SETTINGSVALUES_H
#define SETTINGSVALUES_H

struct SettingsNames {
    static inline constexpr const char* const SECURITY_PASSWORD = "security/password";

    static inline constexpr const char* const GUI_LANGUAGE = "gui/language";
    static inline constexpr const char* const GUI_TYPE = "gui/type";
    static inline constexpr const char* const GUI_THEME = "gui/theme";

    static inline constexpr const char* const ACCOUNT_JWTTOKEN = "account/jwtToken";
    static inline constexpr const char* const ACCOUNT_REFRESHTOKEN = "account/refreshToken";
    static inline constexpr const char* const ACCOUNT_EMAIL = "account/email";

    static inline constexpr const char* const FIRST_TIME = "firstTime";
};

struct SettingsValues {
    // static inline constexpr const char* const GUI_LANGUAGE = "English";
    static inline constexpr int GUI_LANGUAGE_ENGLISH = 0;
    static inline constexpr int GUI_LANGUAGE_RUSSIAN = 1;

    static inline constexpr int GUI_TYPE_WIDGETS = 0;
    static inline constexpr int GUI_TYPE_QUICK = 1;

    static inline constexpr int GUI_THEME_DARK = 0;
    static inline constexpr int GUI_THEME_LIGHT = 1;

    static inline constexpr bool FIRST_TIME = true;
};

#endif // SETTINGSVALUES_H
