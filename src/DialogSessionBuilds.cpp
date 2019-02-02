#include "DialogSessionBuilds.h"

#include "BuildData.h"
#include "SigbuildDefines.h"

#include <QDateTime>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QScrollArea>
#include <QTableWidget>
#include <QVBoxLayout>

#include <QDebug>

namespace Sigbuild
{

enum TableColumns : int
{
	COL_PROJECT = 0,
	COL_START,
	COL_END,
	COL_TIME,
	COL_RESULT,

	NUM_TAB_COLUMNS
};

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogSessionBuilds::DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowTitleHint| Qt::WindowCloseButtonHint)
{
//	QPalette pal(palette());

	// -- properties --
	setWindowTitle(tr("Session builds"));
	setSizeGripEnabled(false);
	setMinimumWidth(850);

	// -- main layout --
	QVBoxLayout * layout = new QVBoxLayout;
	setLayout(layout);

	// -- header --
	QWidget * header = new QWidget;
	header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	layout->addWidget(header);

//	pal.setColor(QPalette::Background, Qt::red);
//	header->setPalette(pal);
//	header->setAutoFillBackground(true);

	int minW[NUM_TAB_COLUMNS] = { 350, 150, 150, 100, 50};

	QFont font;
	font.setBold(true);

	QGridLayout * layoutHeader = new QGridLayout;
	layoutHeader->setContentsMargins(0, 3, 0, 3);
	header->setLayout(layoutHeader);

	QLabel * label;

	label = new QLabel(tr("PROJECT"));
	label->setFont(font);
	layoutHeader->addWidget(label, 0, COL_PROJECT);

	label = new QLabel(tr("BUILD START"));
	label->setFont(font);
	layoutHeader->addWidget(label, 0, COL_START);


	label = new QLabel(tr("BUILD FINISH"));
	label->setFont(font);
	layoutHeader->addWidget(label, 0, COL_END);

	label = new QLabel(tr("BUILD TIME"));
	label->setFont(font);
	layoutHeader->addWidget(label, 0, COL_TIME);

	label = new QLabel(tr("RESULT"));
	label->setFont(font);
	layoutHeader->addWidget(label, 0, COL_RESULT);

//	layoutHeader->setColumnStretch(COL_PROJECT, 40);
//	layoutHeader->setColumnStretch(COL_START, 20);
//	layoutHeader->setColumnStretch(COL_END, 20);
//	layoutHeader->setColumnStretch(COL_TIME, 15);
//	layoutHeader->setColumnStretch(COL_RESULT, 5);

	// SCROLL AREA
	QScrollArea * area = new QScrollArea(this);
	//area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	area->setWidgetResizable(true);
	area->setFrameShape(QFrame::NoFrame);

	layout->addWidget(area);

	qDebug() << "QScrollArea sizeAdjustPolicy:" << area->sizeAdjustPolicy();
	qDebug() << "QScrollArea sizePolicy:" << area->sizePolicy();


//	pal.setColor(QPalette::Background, Qt::blue);
//	area->setPalette(pal);

	QWidget * scrollContent = new QWidget(area);
    scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setWidget(scrollContent);
	//QVBoxLayout * layoutArea = new QVBoxLayout;
	QGridLayout * layoutArea = new QGridLayout;
	//layoutArea->setSpacing(0);
	layoutArea->setContentsMargins(0, 2, 0, 2);
	scrollContent->setLayout(layoutArea);

//	layoutArea->setColumnStretch(COL_PROJECT, 30);
//	layoutArea->setColumnStretch(COL_START, 25);
//	layoutArea->setColumnStretch(COL_END, 25);
//	layoutArea->setColumnStretch(COL_TIME, 15);
//	layoutArea->setColumnStretch(COL_RESULT, 5);

	qDebug() << "scrollContent sizePolicy:" << scrollContent->sizePolicy();

	// -- builds data --
	for(int i = 0; i < data.size(); ++i)
	{
		const BuildData * entry = data[i];

		QLabel * label;

		if(i == 3)
			label = new QLabel(entry->GetProject() + QString(" an even longer long name for a project..."));
		else if(i % 2)
			label = new QLabel(entry->GetProject() + QString(" a very long name for a project..."));
		else
			label = new QLabel(entry->GetProject());

		layoutArea->addWidget(label, i, COL_PROJECT);

		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeStart()).toString("dd-MM-yyyy HH:mm:ss"));
		layoutArea->addWidget(label, i, COL_START);

		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeEnd()).toString("dd-MM-yyyy HH:mm:ss"));

		layoutArea->addWidget(label, i, COL_END);

		qint64 diff = entry->GetTimeEnd() - entry->GetTimeStart();
		QTime buildTime(0, 0, 0, 0);
		buildTime = buildTime.addMSecs(diff);
		label = new QLabel(buildTime.toString("hh:mm:ss"));

		layoutArea->addWidget(label, i, COL_TIME);

		label = new QLabel;
		label->setPixmap(icons[static_cast<int>(entry->GetState())]);
		layoutArea->addWidget(label, i, COL_RESULT);
	}

//	for(int c = 0; c < NUM_TAB_COLUMNS; ++c)
//		layoutHeader->setColumnMinimumWidth(c, layoutArea->columnMinimumWidth(c));

	for(int c = 0; c < NUM_TAB_COLUMNS; ++c)
	{
		layoutArea->setColumnMinimumWidth(c, minW[c]);
		layoutHeader->setColumnMinimumWidth(c, minW[c]);

		qDebug() << c << "]" << minW[c];
	}

	// -- OK BUTTON --
	QPushButton * button = new QPushButton(tr("OK"));
	qDebug() << "button W:" << button->width();

	//button->setMinimumWidth(100);
	//button->setMaximumWidth(150);
	button->setDefault(true);

	qDebug() << "button W:" << button->width();

	layout->addWidget(button, 0, Qt::AlignCenter);

	qDebug() << "button W:" << button->width();

	connect(button, &QPushButton::clicked, this, &QDialog::accept);
}

} // namespace Sigbuild
