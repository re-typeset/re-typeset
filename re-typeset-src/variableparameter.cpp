#include "variableparameter.hpp"
#include "QDebug"

VariableParameter::VariableParameter()
	: spinBox_(Q_NULLPTR)
{
	//NOOP
}

void VariableParameter::addToFormLayout(QFormLayout * layout)
{
	QString afterName=":";
    layout->addRow(name_+afterName, spinBox_);
}

void VariableParameter::configure(QFormLayout * layout, QString name, double defaultVal, QString suffix, double minVal, double maxVal, QString description)
{
	QString preSuffix=" × ";

	const int Decimals = 2;
	const double Step = 0.01;

	spinBox_ = new QDoubleSpinBox();
	name_=name;
	defaultVal_=defaultVal;
	spinBox_->setValue(defaultVal_);
	spinBox_->setMinimum(minVal);
	spinBox_->setMaximum(maxVal);
	spinBox_->setSuffix(preSuffix + suffix);
	spinBox_->setDecimals(Decimals);
	spinBox_->setSingleStep(Step);
	description_=description;
	addToFormLayout(layout);
}

VariableParameter::operator double()
{
	return spinBox_->value();
}
