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
