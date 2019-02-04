#pragma once

#include <QLabel>

namespace Sigbuild
{

class Label : public QLabel
{
	Q_OBJECT

public:		
	Label(const QString & text, QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	Label(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
	void SizeChanged(const QSize & newSize);

protected:
	void resizeEvent(QResizeEvent * event) override;
};

} // namespace Sigbuild
