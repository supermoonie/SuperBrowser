#ifndef TERMINAL_H
#define TERMINAL_H

#include <QObject>
#include <QString>
#include <ostream>

#include "encoding.h"

class Terminal: public QObject
{
    Q_OBJECT

public:
    static Terminal* instance();

    QString getEncoding() const;
    bool setEncoding(const QString& encoding);

    void cout(const QString& string, const bool newline = true) const;
    void cerr(const QString& string, const bool newline = true) const;

signals:
    void encodingChanged(const QString& encoding);

private:
    void output(std::ostream& out, const QString& string, const bool newline) const;

private:
    Terminal();
    Encoding m_encoding;
};

#endif // TERMINAL_H
