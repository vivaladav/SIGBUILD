#include "OptionsPageMain.h"

#include "OptionsPageMainWidget.h"

namespace QtCreatorSysTray
{

OptionsPageMain::OptionsPageMain(QObject * parent)
	: IOptionsPage(parent)
{
	setId("SIGBUILDsettings");
	setDisplayName("SIGBUILD");
	setCategory("SIGBUILD");
	setDisplayCategory("SIGBUILD");
	setCategoryIcon(Utils::Icon(":/img/qtcreator-logo-64.png"));
}

QWidget * OptionsPageMain::widget()
{
	if(nullptr == mWidget)
		mWidget = new OptionsPageMainWidget;

	return mWidget;
}

void OptionsPageMain::apply()
{

}

void OptionsPageMain::finish()
{
	delete mWidget;
	mWidget = nullptr;
}

} // namespace QtCreatorSysTray
