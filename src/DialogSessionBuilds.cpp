#include "DialogSessionBuilds.h"

#include "BuildData.h"
#include "SigbuildDefines.h"

#include <QDateTime>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Sigbuild
{

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogSessionBuilds::DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowTitleHint| Qt::WindowCloseButtonHint)
{
	// -- properties --
	setWindowTitle(tr("Session builds"));
	setSizeGripEnabled(false);

	QVBoxLayout * layout = new QVBoxLayout;
	setLayout(layout);

	// SCROLL AREA
	QScrollArea * area = new QScrollArea(this);
	area->setWidgetResizable(true);

	QWidget * scrollContent = new QWidget(area);
	area->setWidget(scrollContent);
	QVBoxLayout * layoutArea = new QVBoxLayout;
	scrollContent->setLayout(layoutArea);

	for(int i = 0; i < data.size(); ++i)
	{
		QWidget * w = new QWidget(area);

		QHBoxLayout * layoutRow = new QHBoxLayout;
		w->setLayout(layoutRow);

		const BuildData * entry = data[i];

		layoutRow->addWidget(new QLabel(entry->GetProject()));
		layoutRow->addWidget(new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeStart()).toString("dd-MM-yyyy HH:mm:ss")));
		layoutRow->addWidget(new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeEnd()).toString("dd-MM-yyyy HH:mm:ss")));

		qint64 diff = entry->GetTimeEnd() - entry->GetTimeStart();
		QTime buildTime(0, 0, 0, 0);
		buildTime = buildTime.addMSecs(diff);
		layoutRow->addWidget(new QLabel(buildTime.toString("hh:mm:ss")));

		QLabel * label = new QLabel;
		label->setPixmap(icons[static_cast<int>(entry->GetState())]);
		layoutRow->addWidget(label);

		layoutArea->addWidget(w);
	}

	layout->addWidget(area);

	// OK BUTTON
	QPushButton * button = new QPushButton(tr("OK"));
	button->setMinimumWidth(100);
	button->setDefault(true);
	layout->addWidget(button);

	connect(button, &QPushButton::clicked, this, &QDialog::accept);
}

} // namespace Sigbuild
