#pragma once

#include <coreplugin/dialogs/ioptionspage.h>

namespace QtCreatorSysTray
{

class OptionsPageMainWidget;

class OptionsPageMain : public Core::IOptionsPage
{
public:
	OptionsPageMain(QObject * parent = nullptr);
	~OptionsPageMain();

	QWidget * widget() override;
	void apply()  override;
	void finish()  override;

private:
	OptionsPageMainWidget * mWidget;
};

} // namespace QtCreatorSysTray
