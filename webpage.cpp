#include "webpage.h"
#include <QBuffer>
#include "terminal.h"

static WebPage* webPage = NULL;

WebPage::WebPage(QObject* parent): QWebPage(parent)
{
    userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36";
    this->setViewportSize(QSize(980, 900));
    connect(this->currentFrame(), &QWebFrame::loadFinished, [=](){
        Terminal::instance()->cout(this->currentFrame()->url().toString() + " load finished", true);
//        QImage image = renderImage();
//        image.save("C:/yzm/captcha.png", "png");
    });
}

WebPage::~WebPage()
{

}

WebPage* WebPage::instance(QObject* parent) {
    if(webPage == NULL) {
        webPage = new WebPage(parent);
    }
    return webPage;
}

void WebPage::setUserAgent(const QString &ua) {
    this->userAgent = ua;
}

QString WebPage::userAgentForUrl(const QUrl &url) const {
    return userAgent;
}

QImage WebPage::renderImage() {
    QRect frameRect;
    QSize viewportSize = this->viewportSize();
    QSize contentsSize = this->mainFrame()->contentsSize();
    frameRect = QRect(QPoint(0, 0), contentsSize);
    this->setViewportSize(contentsSize);
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    QImage buffer(frameRect.size(), format);
    buffer.fill(Qt::transparent);
    QPainter painter;
    const int tileSize = 4096;
    int htiles = (buffer.width() + tileSize - 1) / tileSize;
    int vtiles = (buffer.height() + tileSize - 1) / tileSize;
    for (int x = 0; x < htiles; ++x) {
        for (int y = 0; y < vtiles; ++y) {

            QImage tileBuffer(tileSize, tileSize, format);
            tileBuffer.fill(Qt::transparent);

            // Render the web page onto the small tile first
            painter.begin(&tileBuffer);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::TextAntialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter.translate(-frameRect.left(), -frameRect.top());
            painter.translate(-x * tileSize, -y * tileSize);
            this->mainFrame()->render(&painter, QRegion(frameRect));
            painter.end();

            // Copy the tile to the main buffer
            painter.begin(&buffer);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.drawImage(x * tileSize, y * tileSize, tileBuffer);
            painter.end();
        }
    }
    return buffer;
}
