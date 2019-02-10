#include "DialogSessionBuilds.h"

#include "BuildData.h"
#include "Label.h"
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

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogSessionBuilds::DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowTitleHint| Qt::WindowCloseButtonHint)
{
//	QPalette pal(palette());

	// -- properties --
	setWindowTitle(tr("Session builds"));
	setSizeGripEnabled(false);
	setMinimumWidth(900);

	// -- main layout --
	QVBoxLayout * layout = new QVBoxLayout;
	setLayout(layout);

	// -- header --
	QWidget * header = new QWidget;
	//header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	layout->addWidget(header);

//	pal.setColor(QPalette::Background, Qt::red);
//	header->setPalette(pal);
//	header->setAutoFillBackground(true);

	QFont font;
	font.setBold(true);

	const int WIDGET_SPACING = 0;

	const int MARGIN_W = 20;

	//const int STRETCH[NUM_TAB_COLUMNS] = { 30, 25, 25, 15, 5 };
	const int STRETCH[NUM_TAB_COLUMNS] = { 0, 0, 0, 0, 0 };

	mLayoutHeader = new QHBoxLayout;
	mLayoutHeader->setContentsMargins(0, 0, 0, 0);
	mLayoutHeader->setSpacing(WIDGET_SPACING);
	header->setLayout(mLayoutHeader);

	QLabel * headerLabels[NUM_TAB_COLUMNS];

	headerLabels[COL_PROJECT] = new QLabel(tr("PROJECT"));
	headerLabels[COL_PROJECT]->setFont(font);
	headerLabels[COL_PROJECT]->setContentsMargins(0, 0, MARGIN_W, 0);
	mLayoutHeader->addWidget(headerLabels[COL_PROJECT], STRETCH[COL_PROJECT]);

	headerLabels[COL_START] = new QLabel(tr("BUILD START"));
	headerLabels[COL_START]->setFont(font);
	headerLabels[COL_START]->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
	mLayoutHeader->addWidget(headerLabels[COL_START], STRETCH[COL_START]);

	headerLabels[COL_END] = new QLabel(tr("BUILD FINISH"));
	headerLabels[COL_END]->setFont(font);
	headerLabels[COL_END]->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
	mLayoutHeader->addWidget(headerLabels[COL_END], STRETCH[COL_END]);

	headerLabels[COL_TIME] = new QLabel(tr("BUILD TIME"));
	headerLabels[COL_TIME]->setFont(font);
	headerLabels[COL_TIME]->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
	mLayoutHeader->addWidget(headerLabels[COL_TIME], STRETCH[COL_TIME]);

	headerLabels[COL_RESULT] = new QLabel(tr("RESULT"));
	headerLabels[COL_RESULT]->setFont(font);
	headerLabels[COL_RESULT]->setContentsMargins(MARGIN_W, 0, 0, 0);
	mLayoutHeader->addWidget(headerLabels[COL_RESULT], STRETCH[COL_RESULT]);

	// SCROLL AREA
	QScrollArea * area = new QScrollArea(this);
	//area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	area->setWidgetResizable(true);
	area->setFrameShape(QFrame::NoFrame);

	layout->addWidget(area);

	qDebug() << "QScrollArea sizeAdjustPolicy:" << area->sizeAdjustPolicy();
	qDebug() << "QScrollArea sizePolicy:" << area->sizePolicy();

	QWidget * scrollContent = new QWidget;
	scrollContent->setContentsMargins(0, 0, 0, 0);
	//layout->addWidget(scrollContent);
	//scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setWidget(scrollContent);
	//QVBoxLayout * layoutArea = new QVBoxLayout;
	mLayoutArea = new QVBoxLayout;
	mLayoutArea->setSpacing(WIDGET_SPACING);
	mLayoutArea->setContentsMargins(0, 0, 0, 0);
	scrollContent->setLayout(mLayoutArea);

//	layoutArea->setColumnStretch(COL_PROJECT, 35);
//	layoutArea->setColumnStretch(COL_START, 20);
//	layoutArea->setColumnStretch(COL_END, 20);
//	layoutArea->setColumnStretch(COL_TIME, 15);
//	layoutArea->setColumnStretch(COL_RESULT, 10);

	qDebug() << "scrollContent sizePolicy:" << scrollContent->sizePolicy();

	qDebug() << "---- CREATE DATA ----";

	// -- builds data --
	for(int i = 0; i < data.size(); ++i)
	{
		const BuildData * entry = data[i];

		QHBoxLayout * layoutRow = new QHBoxLayout;
		mLayoutArea->addLayout(layoutRow);

		QLabel * label;

		// PROJECT
		if(i == 4)
			label = new QLabel(entry->GetProject() + QString(" an even longer long name for a project..."));
		else if(i % 2)
			label = new QLabel(entry->GetProject() + QString(" a very long name for a project..."));
		else
			label = new QLabel(entry->GetProject());

		label->setContentsMargins(0, 0, MARGIN_W, 0);
		layoutRow->addWidget(label, STRETCH[COL_PROJECT]);

		// START
		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeStart()).toString("dd-MM-yyyy HH:mm:ss"));
		label->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
		layoutRow->addWidget(label, STRETCH[COL_START]);

		// END
		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeEnd()).toString("dd-MM-yyyy HH:mm:ss"));
		label->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
		layoutRow->addWidget(label, STRETCH[COL_END]);

		// TIME
		qint64 diff = entry->GetTimeEnd() - entry->GetTimeStart();
		QTime buildTime(0, 0, 0, 0);
		buildTime = buildTime.addMSecs(diff);
		label = new QLabel(buildTime.toString("hh:mm:ss"));
		label->setContentsMargins(MARGIN_W, 0, MARGIN_W, 0);
		layoutRow->addWidget(label, STRETCH[COL_TIME]);

		// RESULT
		label = new QLabel;
		label->setContentsMargins(MARGIN_W, 0, 0, 0);
		label->setPixmap(icons[static_cast<int>(entry->GetState())]);
		layoutRow->addWidget(label, STRETCH[COL_RESULT]);
	}

	qDebug() << "";

	// -- OK BUTTON --
	QPushButton * button = new QPushButton(tr("OK"));
	qDebug() << "button W:" << button->width();

	button->setDefault(true);

	qDebug() << "button W:" << button->width();

	layout->addWidget(button, 0, Qt::AlignCenter);

	qDebug() << "button W:" << button->width();

	connect(button, &QPushButton::clicked, this, &QDialog::accept);
}

void DialogSessionBuilds::showEvent(QShowEvent *)
{
	UpdateSizes();
}

void DialogSessionBuilds::UpdateSizes()
{
	qDebug() << "--------------------- 1ST PASS ---------------------";

	const int ROWS = mLayoutArea->count();

	for(int c = 0; c <NUM_TAB_COLUMNS; ++c)
	{
		int maxW = mLayoutHeader->itemAt(c)->widget()->width();
		int maxWmsh = mLayoutHeader->itemAt(c)->widget()->minimumSizeHint().width();
		int maxWmw = mLayoutHeader->itemAt(c)->widget()->minimumWidth();
		qDebug() << "H" << c << "w=" << maxW << "msh w=" << maxWmsh << "mw w=" << maxWmw;

		for(int r = 0; r < ROWS; ++r)
		{
			const int DW = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->width();
			const int DWmsh = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumSizeHint().width();
			const int DWmw = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumWidth();

			qDebug() << "D" << r << "w=" << DW << "msh w=" << DWmsh << "mw w=" << DWmw;

			if(DWmsh > maxWmsh)
				maxWmsh = DWmsh;
		}

		qDebug() << c << "]" << maxWmsh;

		mLayoutHeader->itemAt(c)->widget()->setMinimumWidth(maxWmsh);

		for(int r = 0; r < ROWS; ++r)
			mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->setMinimumWidth(maxWmsh);
	}

	update();
	mLayoutHeader->update();
	mLayoutArea->update();

	qDebug() << "--------------------- 2ND PASS ---------------------";

	for(int c = 0; c <NUM_TAB_COLUMNS; ++c)
	{
		int maxW = mLayoutHeader->itemAt(c)->widget()->width();
		int maxWmsh = mLayoutHeader->itemAt(c)->widget()->minimumSizeHint().width();
		int maxWmw = mLayoutHeader->itemAt(c)->widget()->minimumWidth();
		qDebug() << "H" << c << "w=" << maxW << "msh w=" << maxWmsh << "mw w=" << maxWmw;

		for(int r = 0; r < ROWS; ++r)
		{
			const int DW = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->width();
			const int DWmsh = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumSizeHint().width();
			const int DWmw = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumWidth();

			qDebug() << "D" << r << "w=" << DW << "msh w=" << DWmsh << "mw w=" << DWmw;

			if(DWmsh > maxWmsh)
				maxWmsh = DWmsh;
		}

		qDebug() << c << "]" << maxWmsh;
	}

	qDebug() << "----------------------------------------------------";
}

} // namespace Sigbuild
