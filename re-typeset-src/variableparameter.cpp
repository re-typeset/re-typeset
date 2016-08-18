// This is not The Greatest Code in the World, no! This is just a tribute!
//
// Re-Typeset program for converting scanned documents to smaller size of page
// without changing text size.
//
// Copyright (C) 2013-2016 Piotr Mika (piotr.mt.mika@gmail.com).
//
// This  program is free software:  you can  redistribute it and/or  modify it
// under the terms of the  GNU General Public License as published by the Free
// Software Foundation,  either version 3 of the License,  or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY  WARRANTY;  without  even the implied  warranty  of MERCHANTABILITY  or
// FITNESS FOR  A PARTICULAR  PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the  GNU  General  Public  License along
// with this program. If not, see <http://www.gnu.org/licenses/>.

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
