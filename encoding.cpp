#include "encoding.h"

Encoding::Encoding()
{
    QTextCodec* codec = QTextCodec::codecForName(DEFAULT_CODEC_NAME);

    // Fall back to locale codec
    if ((QTextCodec*)NULL == codec) {
        codec = QTextCodec::codecForLocale();
    }

    m_codec = codec;
}

Encoding::Encoding(const QString& encoding)
{
    setEncoding(encoding);
}

Encoding::~Encoding()
{
    m_codec = (QTextCodec*)NULL;
}

QString Encoding::decode(const QByteArray& bytes) const
{
    return getCodec()->toUnicode(bytes);
}

QByteArray Encoding::encode(const QString& string) const
{
    return getCodec()->fromUnicode(string);
}

QString Encoding::getName() const
{
    // TODO Is it safe to assume UTF-8 here?
    return QString::fromUtf8(getCodec()->name());
}

void Encoding::setEncoding(const QString& encoding)
{
    if (!encoding.isEmpty()) {
        QTextCodec* codec = QTextCodec::codecForName(encoding.toLatin1());

        if ((QTextCodec*)NULL != codec) {
            m_codec = codec;
        }
    }
}

const Encoding Encoding::UTF8 = Encoding("UTF-8");

// private:
QTextCodec* Encoding::getCodec() const
{
    QTextCodec* codec = m_codec;

    if ((QTextCodec*)NULL == codec) {
        codec = QTextCodec::codecForLocale();
    }

    return codec;
}

const QByteArray Encoding::DEFAULT_CODEC_NAME = "UTF-8";
