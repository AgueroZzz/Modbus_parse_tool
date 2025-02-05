#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QByteArray>
#include <QMessageBox>
#include <QPair>
#include <parse/parser.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    inline QString cleanText(const QString& text){
        QString cleanedText = text.trimmed();
        static QRegularExpression removeexpr("\\s+");
        cleanedText = cleanedText.remove(removeexpr);
        return cleanedText;
    }

public slots:
    void slot_button_clicked();
    void slot_display_data(const QList<QStringList>& data);

signals:
    void sig_display_data(const QList<QStringList>& data);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
