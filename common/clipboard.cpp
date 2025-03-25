#include "clipboard.h"
#include <QApplication>
#include <QClipboard>

Clipboard::Clipboard(QObject *parent)
    : QObject{parent}
{}

void Clipboard::copyText(const QString &text) {
    QApplication::clipboard()->setText(text);
}
