#include "DialogLastBuild.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace Sigbuild
{

// ==== CONSTRUCTOR / DESTRUCTOR ====

DialogLastBuild::DialogLastBuild(const QString & project, const QString & start, const QString & end,
                            const QString & buildTime, const QPixmap & statusIcon, QWidget * parent)
    : QDialog(parent, Qt::Dialog | Qt::WindowTitleHint| Qt::WindowCloseButtonHint)
{
    // -- properties --
    setWindowTitle(tr("Last build"));
    setSizeGripEnabled(false);

    // -- layout --
    QGridLayout * layout = new QGridLayout;
    setLayout(layout);

    // ROW 0
    layout->addWidget(CreateHeaderLabel(tr("PROJECT:")), 0, 0);
    layout->addWidget(new QLabel(project), 0, 1);

    // ROW 1
    layout->addWidget(CreateHeaderLabel(tr("BUILD START:")), 1, 0);
    layout->addWidget(new QLabel(start), 1, 1);

    // ROW 2
    layout->addWidget(CreateHeaderLabel(tr("BUILD END:")), 2, 0);
    layout->addWidget(new QLabel(end), 2, 1);

    // ROW 3
    layout->addWidget(CreateHeaderLabel(tr("BUILD TIME:")), 3, 0);
    layout->addWidget(new QLabel(buildTime), 3, 1);

    // ROW 4
    layout->addWidget(CreateHeaderLabel(tr("BUILD STATUS:")), 4, 0);
    QLabel * label = new QLabel;
    label->setPixmap(statusIcon);
    layout->addWidget(label, 4, 1);

    // ROW 5
    QPushButton * button = new QPushButton(tr("OK"));
    button->setMinimumWidth(100);
    button->setDefault(true);
    layout->addWidget(button, 5, 0, 1, 2, Qt::AlignCenter);

    connect(button, &QPushButton::clicked, this, &QDialog::accept);

    // minimum sizes
    layout->setColumnMinimumWidth(0, 150);
    layout->setRowMinimumHeight(5, 50);
}

// ==== PRIVATE FUNCTIONS ====

QLabel * DialogLastBuild::CreateHeaderLabel(const QString & str)
{
    QFont font;
    font.setBold(true);

    QLabel * label = new QLabel(str);
    label->setFont(font);

    return label;
}

} // namespace Sigbuild
