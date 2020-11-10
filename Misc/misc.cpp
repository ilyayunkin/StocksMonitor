#include "misc.h"
#include <QTextDocument>

namespace Misc
{
QString fromEscaped(const QString &in)
{
    return QTextDocument(in).toPlainText();
}
}
