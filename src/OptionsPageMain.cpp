#include "OptionsPageMain.h"

#include "OptionsPageMainWidget.h"
#include "Settings.h"

namespace Sigbuild
{

OptionsPageMain::OptionsPageMain(Settings * settings, QObject * parent)
    : IOptionsPage(parent)
    , mSettings(settings)
{
    setId("SIGBUILDsettings");
    setDisplayName("General");
    setCategory("SIGBUILD");
    setDisplayCategory("SIGBUILD");
    setCategoryIcon(Utils::Icon(":/img/icon.png"));
}

QWidget * OptionsPageMain::widget()
{
    if(nullptr == mWidget)
        mWidget = new OptionsPageMainWidget(mSettings);

    return mWidget;
}

void OptionsPageMain::apply()
{
    const Settings newSettings = mWidget->GenerateSettings();

    if(newSettings != *mSettings)
    {
        *mSettings = newSettings;

        mSettings->Save();

        emit SettingsChanged();
    }
}

void OptionsPageMain::finish()
{
    delete mWidget;
    mWidget = nullptr;
}

} // namespace Sigbuild
