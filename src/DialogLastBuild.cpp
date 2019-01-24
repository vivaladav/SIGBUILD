#include "DialogLastBuild.h"

#include <QGridLayout>
#include <QLabel>

namespace Sigbuild
{

DialogLastBuild::DialogLastBuild(const QString & project, const QString & start, const QString & end,
							const QString & buildTime, const QString & status, QWidget * parent) : QDialog(parent)
{
	setWindowTitle(tr("Last build"));

	QGridLayout * layout = new QGridLayout;
	setLayout(layout);

	layout->addWidget(new QLabel(tr("PROJECT:")), 0, 0);
	layout->addWidget(new QLabel(project), 0, 1);

	layout->addWidget(new QLabel(tr("BUILD START:")), 1, 0);
	layout->addWidget(new QLabel(start), 1, 1);

	layout->addWidget(new QLabel(tr("BUILD END:")), 2, 0);
	layout->addWidget(new QLabel(end), 2, 1);

	layout->addWidget(new QLabel(tr("BUILD TIME:")), 3, 0);
	layout->addWidget(new QLabel(buildTime), 3, 1);

	layout->addWidget(new QLabel(tr("BUILD STATUS:")), 4, 0);
	layout->addWidget(new QLabel(status), 4, 1);
}

} // namespace Sigbuild
