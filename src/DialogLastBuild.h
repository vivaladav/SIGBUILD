#pragma once

#include <QDialog>

class QLabel;

namespace Sigbuild
{

class DialogLastBuild : public QDialog
{
public:
	DialogLastBuild(const QString & project, const QString & start, const QString & end,
					const QString & buildTime, const QPixmap & statucIcon, QWidget * parent = nullptr);

private:
	QLabel * CreateHeaderLabel(const QString & str);
};

} // namespace Sigbuild
