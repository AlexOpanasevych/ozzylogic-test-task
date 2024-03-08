#ifndef EDITFORMDIALOG_H
#define EDITFORMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class EditFormDialog : public QDialog
{
    Q_OBJECT
public:
    EditFormDialog(QWidget * parent = nullptr);
    void setOperator(const QImage &newOperator);

    void setCountry(const QImage &newCountry);

    void setEdit(bool newEdit);

    void setMnc(int newMnc);

    void setMcc(int newMcc);

    void setName(const QString &newName);

    bool edit() const;

    QString name() const;

    int mnc() const;

    int mcc() const;

private slots:
    void onRejected();
    void onAccepted();

    void onCountryChanged();
    void onOperatorChanged();
    void onEditChanged();

signals:
    void countryChanged();
    void operatorChanged();

    void mccChanged();
    void mncChanged();
    void editChanged();
    void nameChanged();
private:
    QImage _operator;
    QImage _country;

private:
    QPushButton * saveButton;
    QPushButton * cancelButton;

    QLabel * countryImage;
    QLabel * operatorImage;
    QLineEdit * nameEdit;
    QLineEdit * mccEdit;
    QLineEdit * mncEdit;

    bool _edit = false;


};

#endif // EDITFORMDIALOG_H
