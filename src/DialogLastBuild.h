#pragma once

#include <QDialog>

namespace Sigbuild
{

class DialogLastBuild : public QDialog
{
public:
	DialogLastBuild(const QString & project, const QString & start, const QString & end,
					const QString & buildTime, const QString & status, QWidget * parent = nullptr);
};

} // namespace Sigbuild
