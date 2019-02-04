#include "Label.h"

#include <QDebug>
#include <QResizeEvent>

namespace Sigbuild
{

Label::Label(const QString & text, QWidget * parent, Qt::WindowFlags f) : QLabel(text, parent, f)
{
}

Label::Label(QWidget * parent, Qt::WindowFlags f) : QLabel(parent, f)
{
}

void Label::resizeEvent(QResizeEvent * event)
{
	qDebug()	<< "QWidget::resizeEvent - LABEL:" << text()
				<< "NEW SIZE:" << event->size() << "OLD SIZE:" << event->oldSize();

	emit SizeChanged(event->size());
}

} // namespace Sigbuild
