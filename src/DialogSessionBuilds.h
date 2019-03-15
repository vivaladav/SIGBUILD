#pragma once

#include <QDialog>
#include <QPixmap>
#include <QVector>

class QHBoxLayout;
class QScrollArea;
class QSpacerItem;
class QVBoxLayout;

namespace Sigbuild
{

class Spacer : public QWidget
{
	Q_OBJECT

protected:
	void paintEvent(QPaintEvent *) override {}
};

class BuildData;

class DialogSessionBuilds : public QDialog
{
public:
	DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent = nullptr);

	void UpdateSizes();

	bool eventFilter(QObject * obj, QEvent * event) override;

protected:
	void showEvent(QShowEvent * event) override;

private:
	enum TableColumns : int
	{
		COL_PROJECT = 0,
		COL_START,
		COL_END,
		COL_TIME,
		COL_RESULT,

		NUM_TAB_COLUMNS
	};

private:
	QHBoxLayout * mLayoutHeader;
	QVBoxLayout * mLayoutArea;
	QScrollArea * mScrollArea;
	Spacer * mSpacerHeader;
	bool mScrollbarVisible;
};

} // namespace Sigbuild
