#include "DialogLastBuild.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>

namespace Sigbuild
{

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogLastBuild::DialogLastBuild(const QString & project, const QString & start, const QString & end,
							const QString & buildTime, const QPixmap & statucIcon, QWidget * parent)
	: QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setWindowTitle(tr("Last build"));
	setSizeGripEnabled(false);

	QGridLayout * layout = new QGridLayout;
	setLayout(layout);

	layout->setColumnMinimumWidth(0, 150);

	layout->addWidget(CreateHeaderLabel(tr("PROJECT:")), 0, 0);
	layout->addWidget(new QLabel(project), 0, 1);

	layout->addWidget(CreateHeaderLabel(tr("BUILD START:")), 1, 0);
	layout->addWidget(new QLabel(start), 1, 1);

	layout->addWidget(CreateHeaderLabel(tr("BUILD END:")), 2, 0);
	layout->addWidget(new QLabel(end), 2, 1);

	layout->addWidget(CreateHeaderLabel(tr("BUILD TIME:")), 3, 0);
	layout->addWidget(new QLabel(buildTime), 3, 1);

	layout->addWidget(CreateHeaderLabel(tr("BUILD STATUS:")), 4, 0);
	QLabel * label = new QLabel;
	label->setPixmap(statucIcon);
	layout->addWidget(label, 4, 1);
}

// ==== PRIVATE FUNCTIONS ====

QLabel * DialogLastBuild::CreateHeaderLabel(const QString & str)
{
	QFont font;
	font.setBold(true);

	QLabel * label = new QLabel(str);
	label->setFont(font);

	return label;
}

} // namespace Sigbuild
