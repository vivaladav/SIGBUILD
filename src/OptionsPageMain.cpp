#include "OptionsPageMain.h"

#include "OptionsPageMainWidget.h"

namespace QtCreatorSysTray
{

OptionsPageMain::OptionsPageMain(QObject * parent)
	: IOptionsPage(parent)
	, mWidget(new OptionsPageMainWidget)
{
	setId("SIGBUILDsettings");
	setDisplayName("SIGBUILD");
	setCategory("SIGBUILD");
	setDisplayCategory("SIGBUILD");
	setCategoryIcon(Utils::Icon(":/img/qtcreator-logo-64.png"));
}

OptionsPageMain::~OptionsPageMain()
{
	delete mWidget;
}

QWidget * OptionsPageMain::widget()
{
	return mWidget;
}

void OptionsPageMain::apply()
{

}

void OptionsPageMain::finish()
{

}

} // namespace QtCreatorSysTray
