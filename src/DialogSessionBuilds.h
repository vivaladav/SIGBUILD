#pragma once

#include <QDialog>
#include <QPixmap>
#include <QVector>

namespace Sigbuild
{

class BuildData;

class DialogSessionBuilds : public QDialog
{
public:
	DialogSessionBuilds(const QVector<BuildData *> & data, const QVector<QPixmap> & icons, QWidget * parent = nullptr);
};

} // namespace Sigbuild
