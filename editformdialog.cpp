#include "editformdialog.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QIntValidator>

EditFormDialog::EditFormDialog(QWidget *parent) {

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setModal(true);

    operatorImage = new QLabel(this);
    operatorImage->setGeometry(5, 5, 25, 25);
    operatorImage->setScaledContents(true);


    QVBoxLayout * formLayout = new QVBoxLayout(this);

    QLabel * label = new QLabel();
    label->setText("Operator editor");
    label->setAlignment(Qt::AlignHCenter);
    QFont font;
    font.setBold(true);
    label->setFont(font);
    label->setMinimumHeight(30);
    label->setMinimumWidth(30);


    // QHBoxLayout * nameLayout = new QHBoxLayout();
    QGridLayout * formGrid = new QGridLayout();

    QLabel *nameLabel = new QLabel("Name");
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Name");

    formGrid->addWidget(nameLabel, 0, 0);
    formGrid->addWidget(nameEdit, 0, 1);


    QLabel *mccLabel = new QLabel("MCC");

    QHBoxLayout * mccEditLayout = new QHBoxLayout();

    mccEdit = new QLineEdit();
    mccEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>().max(), this));
    mccEdit->setPlaceholderText("0");
    // mccEdit->setEnabled(false);
    countryImage = new QLabel();
    // countryImage->setPixmap(QPixmap::fromImage(_country));

    mccEditLayout->addWidget(mccEdit);
    mccEditLayout->addWidget(countryImage);

    formGrid->addWidget(mccLabel, 1, 0);
    formGrid->addLayout(mccEditLayout, 1, 1);


    QLabel *mncLabel = new QLabel("MNC");
    mncEdit = new QLineEdit();
    mncEdit->setValidator(new QIntValidator(0, std::numeric_limits<int>().max(), this));
    mncEdit->setPlaceholderText("0");

    formGrid->addWidget(mncLabel, 2, 0);
    formGrid->addWidget(mncEdit, 2, 1);

    auto buttonBox = new QDialogButtonBox();

    saveButton = new QPushButton("Save", buttonBox);

    cancelButton = new QPushButton("Cancel", buttonBox);

    buttonBox->addButton(saveButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

    formLayout->addWidget(label);
    // formLayout->addLayout(nameLayout);

    // formLayout->addLayout(mccLayout);
    // formLayout->addLayout(mncLayout);
    formLayout->addLayout(formGrid);
    formLayout->addWidget(buttonBox);

    setLayout(formLayout);

    connect(this, &EditFormDialog::countryChanged, this, &EditFormDialog::onCountryChanged);
    connect(this, &EditFormDialog::operatorChanged, this, &EditFormDialog::onOperatorChanged);

    connect(this, &EditFormDialog::editChanged, this, &EditFormDialog::onEditChanged);
    connect(nameEdit, &QLineEdit::textChanged, this, &EditFormDialog::nameChanged);
    connect(mccEdit, &QLineEdit::textChanged, this, &EditFormDialog::mccChanged);
    connect(mncEdit, &QLineEdit::textChanged, this, &EditFormDialog::mncChanged);

    connect(cancelButton, &QPushButton::clicked, this, &EditFormDialog::onRejected);
    connect(saveButton, &QPushButton::clicked, this, &EditFormDialog::onAccepted);


}

void EditFormDialog::onRejected()
{
    done(QDialog::Rejected);
}

void EditFormDialog::onAccepted()
{
    accept();
}

void EditFormDialog::onCountryChanged()
{
    countryImage->setPixmap(QPixmap::fromImage(_country));
}

void EditFormDialog::onOperatorChanged()
{
    operatorImage->setPixmap(QPixmap::fromImage(_operator));
}

void EditFormDialog::onEditChanged()
{
    mccEdit->setEnabled(!_edit);
    mncEdit->setEnabled(!_edit);
}

// void EditFormDialog::onMccChanged()
// {
//     mccEdit->setText(QString::number(_mcc));
// }

// void EditFormDialog::onMncChanged()
// {
//     mncEdit->setText(QString::number(_mnc));
// }

// void EditFormDialog::onNameChanged()
// {
//     nameEdit->setText(_name);
// }

int EditFormDialog::mcc() const
{
    return mccEdit->text().toInt();
}

int EditFormDialog::mnc() const
{
    return mncEdit->text().toInt();
}

QString EditFormDialog::name() const
{
    return nameEdit->text();
}

bool EditFormDialog::edit() const
{
    return _edit;
}

void EditFormDialog::setName(const QString &newName)
{
    nameEdit->setText(newName);
}

void EditFormDialog::setMcc(int newMcc)
{
    mccEdit->setText(QString::number(newMcc));
}

void EditFormDialog::setMnc(int newMnc)
{
    mncEdit->setText(QString::number(newMnc));
}

void EditFormDialog::setEdit(bool newEdit)
{
    if(_edit == newEdit)
        return;

    _edit = newEdit;
    emit editChanged();
}

void EditFormDialog::setCountry(const QImage &newCountry)
{
    if(_country == newCountry)
        return;

    _country = newCountry;
    emit countryChanged();
}

void EditFormDialog::setOperator(const QImage &newOperator)
{
    if(_operator == newOperator)
        return;

    _operator = newOperator;

    emit operatorChanged();
}
