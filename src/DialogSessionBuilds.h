#pragma once

#include <QDialog>
#include <QPixmap>
#include <QVector>

class QHBoxLayout;
class QVBoxLayout;

namespace Sigbuild
{

class BuildData;

class DialogSessionBuilds : public QDialog
{
public:
	DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent = nullptr);

	void UpdateSizes();

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
};

} // namespace Sigbuild
