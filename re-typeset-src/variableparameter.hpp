#ifndef VARIABLEPARAMETER_HPP
#define VARIABLEPoARAMETER_HPP

#include "QDoubleSpinBox"
#include "QFormLayout"
#include "QString"


class VariableParameter
{
public:
	VariableParameter();
	operator double();
	void configure(QFormLayout * layout, QString name, double defaultVal, QString suffix=QString(), double minVal=0, double maxVal=99, QString description=QString());

private:
	void addToFormLayout(QFormLayout * layout);

	QDoubleSpinBox * spinBox_;

	QString name_;
	QString description_;
	QString suffix_;
	double defaultVal_;

};

#endif // VARIABLEPARAMETER_HPP
