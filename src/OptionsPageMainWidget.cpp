#include "OptionsPageMainWidget.h"

#include <QCheckBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QSpinBox>
#include <QVBoxLayout>

namespace QtCreatorSysTray
{

OptionsPageMainWidget::OptionsPageMainWidget()
{
	QVBoxLayout * layout = new QVBoxLayout(this);

	// == GENERAL BOX ==
	QGroupBox * box = new QGroupBox(tr("Systray"), this);
	layout->addWidget(box);

	QVBoxLayout * layoutBox = new QVBoxLayout(box);

	QCheckBox * check = new QCheckBox(tr("Enable systray icon"), box);
	layoutBox->addWidget(check);

	check = new QCheckBox(tr("Enable systray notifications"), box);
	layoutBox->addWidget(check);

	check = new QCheckBox(tr("Show systray notifications when Qt Creator is open"), box);
	layoutBox->addWidget(check);

	// == AUDIO BOX ==
	box = new QGroupBox(tr("Audio"), this);
	layout->addWidget(box);

	layoutBox = new QVBoxLayout(box);

	check = new QCheckBox(tr("Enable audio notifications"), box);
	layoutBox->addWidget(check);

	check = new QCheckBox(tr("Play audio notifications when Qt Creator is open"), box);
	layoutBox->addWidget(check);

	QHBoxLayout * layoutRow = new QHBoxLayout;
	layoutBox->addLayout(layoutRow);

	QSpinBox * spin = new QSpinBox(box);
	spin->setMinimum(0);
	spin->setMaximum(100);
	spin->setSingleStep(10);
	spin->setMaximumWidth(50);
	layoutRow->addWidget(spin);

	QLabel * label = new QLabel(tr("Notification volume"), box);
	layoutRow->addWidget(label);

	// == VERTICAL SPACER ==`
	layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
}

} // namespace QtCreatorSysTray
