#include "DialogSessionBuilds.h"

#include "BuildData.h"
#include "Label.h"
#include "SigbuildDefines.h"

#include <QDateTime>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QTableWidget>
#include <QVBoxLayout>

#include <QDebug>

namespace Sigbuild
{

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogSessionBuilds::DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent)
	: QDialog(parent, Qt::Dialog | Qt::WindowTitleHint| Qt::WindowCloseButtonHint)
	, mLayoutHeader(nullptr)
	, mLayoutArea(nullptr)
	, mScrollArea(nullptr)
	, mFakeBar(nullptr)
{
//	QPalette pal(palette());

	// -- properties --
	setWindowTitle(tr("Session builds"));
	setSizeGripEnabled(false);

	// -- main layout --
	QVBoxLayout * layout = new QVBoxLayout;
	setLayout(layout);

	// -- header --
	QWidget * header = new QWidget;

	layout->addWidget(header);

	QFont font;
	font.setBold(true);

	const int WIDGET_SPACING = 0;

	const int MARGIN_W = 20;

	const int STRETCH[NUM_TAB_COLUMNS] = { 30, 25, 25, 15, 5 };

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
	mScrollArea = new QScrollArea(this);
	mScrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
	mScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//	mScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	mScrollArea->setWidgetResizable(true);
	mScrollArea->setFrameShape(QFrame::NoFrame);

	layout->addWidget(mScrollArea);

	qDebug() << "QScrollArea sizeAdjustPolicy:" << mScrollArea->sizeAdjustPolicy();
	qDebug() << "QScrollArea sizePolicy:" << mScrollArea->sizePolicy();

	QWidget * scrollContent = new QWidget;
	scrollContent->setContentsMargins(0, 0, 0, 0);
	mScrollArea->setWidget(scrollContent);
	mLayoutArea = new QVBoxLayout;
	mLayoutArea->setSpacing(WIDGET_SPACING);
	mLayoutArea->setContentsMargins(0, 0, 0, 0);
	scrollContent->setLayout(mLayoutArea);

	qDebug() << "scrollContent sizePolicy:" << scrollContent->sizePolicy();

	qDebug() << "---- CREATE DATA ----";

	// -- builds data --
	//for(int i = 0; i < data.size(); ++i)
	for(int i = 0; i < 5; ++i)
	{
		const BuildData * entry = data[0];

		QHBoxLayout * layoutRow = new QHBoxLayout;
		mLayoutArea->addLayout(layoutRow);

		QLabel * label;

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

	QScrollBar * bar = mScrollArea->verticalScrollBar();

	if(bar)
	{
		if(bar->isVisible())
			mFakeBar = new QSpacerItem(bar->width(), bar->height(), QSizePolicy::Fixed, QSizePolicy::Fixed);
		else
			mFakeBar = new QSpacerItem(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

		mLayoutHeader->addSpacerItem(mFakeBar);
	}

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

void DialogSessionBuilds::resizeEvent(QResizeEvent * event)
{
	static bool prevVis = false;

	QScrollBar * bar = mScrollArea->verticalScrollBar();
	const bool visible = bar && bar->isVisible();
	qDebug() << "DialogSessionBuilds::resizeEvent - old size:" << event->oldSize() << "- new size:" << event->size()
			 << "- scrollbar visible:" << (visible ? QString("YES") : QString("NO"));

	if(prevVis != visible)
	{
		if(visible)
			mFakeBar->changeSize(bar->width(), 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
		else
			mFakeBar->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);

		prevVis = visible;
		update();
		//UpdateSizes();
	}
}

void DialogSessionBuilds::UpdateSizes()
{
	qDebug() << "--------------------- 1ST PASS ---------------------";

	const int ROWS = mLayoutArea->count();

	int totMaxW = 0;
	int totMaxWmsh = 0;
	int totMaxWmw = 0;

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

		totMaxWmsh += maxWmsh;

		qDebug() << c << "] max msh w=" << maxWmsh;

		mLayoutHeader->itemAt(c)->widget()->setMinimumWidth(maxWmsh);

		for(int r = 0; r < ROWS; ++r)
			mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->setMinimumWidth(maxWmsh);
	}

	qDebug() << "TOT msh w=" << totMaxWmsh;

	/*
	update();
	mLayoutHeader->update();
	mLayoutArea->update();
	*/

	qDebug() << "--------------------- 2ND PASS ---------------------";

	for(int c = 0; c <NUM_TAB_COLUMNS; ++c)
	{
		int maxW = mLayoutHeader->itemAt(c)->widget()->width();
		int maxWmsh = mLayoutHeader->itemAt(c)->widget()->minimumSizeHint().width();
		int maxWmw = mLayoutHeader->itemAt(c)->widget()->minimumWidth();
		qDebug() << "H" << c << "w=" << maxW << "msh w=" << maxWmsh << "mw w=" << maxWmw;

		totMaxW = maxW;
		totMaxWmsh = maxWmsh;
		totMaxWmw = maxWmw;

		for(int r = 0; r < ROWS; ++r)
		{
			const int DW = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->width();
			const int DWmsh = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumSizeHint().width();
			const int DWmw = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumWidth();

			totMaxW += DW;
			totMaxWmsh += DWmsh;
			totMaxWmw += DWmw;

			qDebug() << "D" << r << "w=" << DW << "msh w=" << DWmsh << "mw w=" << DWmw;

			if(DWmsh > maxWmsh)
				maxWmsh = DWmsh;
		}

		qDebug() << c << "] max msh w=" << maxWmsh << "- TOT w=" << totMaxW << "- TOT msh w=" << totMaxWmsh << "- TOT mw w=" << totMaxWmw;
	}

	qDebug() << "----------------------------------------------------";	
}

} // namespace Sigbuild
