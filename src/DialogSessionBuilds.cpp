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

	mLayoutHeader = new QHBoxLayout;
	mLayoutHeader->setContentsMargins(0, 3, 0, 3);
	header->setLayout(mLayoutHeader);

	QLabel * headerLabels[NUM_TAB_COLUMNS];

	headerLabels[COL_PROJECT] = new QLabel(tr("PROJECT"));
	headerLabels[COL_PROJECT]->setFont(font);
	mLayoutHeader->addWidget(headerLabels[COL_PROJECT]);

	headerLabels[COL_START] = new QLabel(tr("BUILD START"));
	headerLabels[COL_START]->setFont(font);
	mLayoutHeader->addWidget(headerLabels[COL_START]);

	headerLabels[COL_END] = new QLabel(tr("BUILD FINISH"));
	headerLabels[COL_END]->setFont(font);
	mLayoutHeader->addWidget(headerLabels[COL_END]);

	headerLabels[COL_TIME] = new QLabel(tr("BUILD TIME"));
	headerLabels[COL_TIME]->setFont(font);
	mLayoutHeader->addWidget(headerLabels[COL_TIME]);

	headerLabels[COL_RESULT] = new QLabel(tr("RESULT"));
	headerLabels[COL_RESULT]->setFont(font);
	mLayoutHeader->addWidget(headerLabels[COL_RESULT]);

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

	QWidget * scrollContent = new QWidget;
	scrollContent->setContentsMargins(0, 0, 0, 0);
	//layout->addWidget(scrollContent);
	//scrollContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setWidget(scrollContent);
	//QVBoxLayout * layoutArea = new QVBoxLayout;
	mLayoutArea = new QVBoxLayout;
	mLayoutArea->setSpacing(0);
	mLayoutArea->setContentsMargins(0, 0, 0, 0);
	scrollContent->setLayout(mLayoutArea);

//	layoutArea->setColumnStretch(COL_PROJECT, 30);
//	layoutArea->setColumnStretch(COL_START, 25);
//	layoutArea->setColumnStretch(COL_END, 25);
//	layoutArea->setColumnStretch(COL_TIME, 15);
//	layoutArea->setColumnStretch(COL_RESULT, 5);

	qDebug() << "scrollContent sizePolicy:" << scrollContent->sizePolicy();

	qDebug() << "---- CREATE DATA ----";

	// -- builds data --
	for(int i = 0; i < data.size(); ++i)
	{
		const BuildData * entry = data[i];

		QHBoxLayout * layoutRow = new QHBoxLayout;
		mLayoutArea->addLayout(layoutRow);

		QLabel * label;

		if(i == 3)
			label = new QLabel(entry->GetProject() + QString(" an even longer long name for a project..."));
		else if(i % 2)
			label = new QLabel(entry->GetProject() + QString(" a very long name for a project..."));
		else
			label = new QLabel(entry->GetProject());

		layoutRow->addWidget(label);
		label->setContentsMargins(0, 0, 0, 0);

//		connect(label, &Label::SizeChanged, headerLabels[COL_PROJECT], [=](const QSize & newSize)
//		{
//			qDebug() << "resize happened...";
//			qDebug() << "OLD headerLabels[COL_PROJECT]->width():" << headerLabels[COL_PROJECT]->width();
//			qDebug() << "newSize.width():" << newSize.width();

//			if(newSize.width() > headerLabels[COL_PROJECT]->width())
//			{
//				headerLabels[COL_PROJECT]->resize(newSize);

//				mLayoutHeader->update();
//			}

//			qDebug() << "NEW headerLabels[COL_PROJECT]->width():" << headerLabels[COL_PROJECT]->width();

//			qDebug() << "";
//		});

		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeStart()).toString("dd-MM-yyyy HH:mm:ss"));
		label->setContentsMargins(0, 0, 0, 0);
		layoutRow->addWidget(label);

//		connect(label, &Label::SizeChanged, headerLabels[COL_START], [=](const QSize & newSize)
//		{
//			if(newSize.width() > headerLabels[COL_START]->width())
//				headerLabels[COL_START]->resize(newSize);
//		});

		label = new QLabel(QDateTime::fromMSecsSinceEpoch(entry->GetTimeEnd()).toString("dd-MM-yyyy HH:mm:ss"));
		label->setContentsMargins(0, 0, 0, 0);
		layoutRow->addWidget(label);

//		connect(label, &Label::SizeChanged, headerLabels[COL_END], [=](const QSize & newSize)
//		{
//			if(newSize.width() > headerLabels[COL_END]->width())
//				headerLabels[COL_END]->resize(newSize);
//		});

		qint64 diff = entry->GetTimeEnd() - entry->GetTimeStart();
		QTime buildTime(0, 0, 0, 0);
		buildTime = buildTime.addMSecs(diff);
		label = new QLabel(buildTime.toString("hh:mm:ss"));
		label->setContentsMargins(0, 0, 0, 0);

		layoutRow->addWidget(label);

//		connect(label, &Label::SizeChanged, headerLabels[COL_TIME], [=](const QSize & newSize)
//		{
//			if(newSize.width() > headerLabels[COL_TIME]->width())
//				headerLabels[COL_TIME]->resize(newSize);
//		});

		label = new QLabel;
		label->setContentsMargins(0, 0, 0, 0);
		label->setPixmap(icons[static_cast<int>(entry->GetState())]);
		layoutRow->addWidget(label);

//		connect(label, &Label::SizeChanged, headerLabels[COL_RESULT], [=](const QSize & newSize)
//		{
//			if(newSize.width() > headerLabels[COL_TIME]->width())
//				headerLabels[COL_RESULT]->resize(newSize);
//		});
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
	for(int c = 0; c <NUM_TAB_COLUMNS; ++c)
	{
		int maxW = mLayoutHeader->itemAt(c)->widget()->minimumSizeHint().width();
		qDebug() << "H" << c << "=" << maxW;

		const int ROWS = mLayoutArea->count();

		for(int r = 0; r < ROWS; ++r)
		{
			const int W = mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->minimumSizeHint().width();

			qDebug() << "D" << r << "=" << W;

			if(W > maxW)
				maxW = W;
		}

		qDebug() << c << "]" << maxW;
		qDebug() << "";

		mLayoutHeader->itemAt(c)->widget()->setMinimumWidth(maxW);

		for(int r = 0; r < ROWS; ++r)
			mLayoutArea->itemAt(r)->layout()->itemAt(c)->widget()->setMinimumWidth(maxW);
	}

	update();

	qDebug() << "---------------------";
/*
	for(int c = 0; c < mLayoutHeader->count(); ++c)
	{
		int maxW = mLayoutHeader->itemAtPosition(0, c)->widget()->width();
		int maxW = 0;
		int maxW2 = 0;
		qDebug() << c << "] maxW H" << maxW;

		for(int r = 0; r <mLayoutArea->rowCount(); ++r)
		{
			const int W = mLayoutArea->itemAtPosition(r, c)->widget()->width();

			if(W > maxW)
				maxW = W;

			const int W2 = mLayoutArea->columnMinimumWidth(c);

			if(W2 > maxW2)
				maxW2 = W2;
		}

		qDebug() << c << "] maxW D" << maxW;
		qDebug() << c << "] maxW D2" << maxW2;
		qDebug() << "";

//		mLayoutHeader->setColumnMinimumWidth(c, maxW);
//		mLayoutArea->setColumnMinimumWidth(c, maxW);
	}
	*/
}

} // namespace Sigbuild
